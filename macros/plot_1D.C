#include "extra_tools.cc"
#include "TStyle.h"
#include "TROOT.h"
#include "THStack.h"
#include <TLegend.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TF1.h>

std::string histOptionsFile="../data/histOptions.txt";


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
sample DATA;
sample SM;
std::vector<sample> SIGNALS;
std::vector<sample> BACKGROUNDS;
parseOptions histOpts;
TCanvas *plotHist(std::string _histName);
void plot_1D();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TCanvas *plotHist(std::string _histName){

	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);

	std::cout<<"Plotting "<<_histName<<std::endl;

	TCanvas *canvas = new TCanvas((_histName + "_canvas").c_str(), "", 2400, 1800);
	canvas->Draw();

	TLegend * legend = new TLegend(0.10, 0.0, 0.99, 1.0);
	legend->SetTextSize(histOpts.getFloat("legendTextSize"));
	legend->SetNColumns(histOpts.getInt("legendNcols"));
	legend->SetFillStyle(1001);
	legend->SetBorderSize(0);

	/////////////////////////////////////////////////Draw data, bg & signal///////////////////////////////////////////////////
	TPad *pad1 = new TPad("pad1", "", histOpts.getFloat("pad1x1"), histOpts.getFloat("pad1y1"), histOpts.getFloat("pad1x2"), histOpts.getFloat("pad1y2"));
	pad1->SetMargin(histOpts.getFloat("pad1marginL"), histOpts.getFloat("pad1marginR"), histOpts.getFloat("pad1marginB"), histOpts.getFloat("pad1marginT"));
	pad1->SetFillStyle(0);
	pad1->SetFillColor(0);
	pad1->SetFrameFillStyle(0);
	canvas->cd();
	pad1->Draw();
	pad1->SetGrid(1,1);
	pad1->SetLogy();

	TH1F *dataHist = (TH1F*) getHistFromFile(_histName, DATA.ntuple);
	DATA.assignAtt(dataHist, histOpts.getInt("markerSize"), histOpts.getFloat("linewidth"));
	legend->AddEntry(dataHist, DATA.legend.c_str(), "PLE");

	std::vector<Double_t> newBins = getGoodBins(dataHist, histOpts.getFloat("rebinStatUnc"));
	dataHist = (TH1F*) rebinHist(dataHist, newBins);

	std::string x_axis_title, y_axis_title;

	THStack *bgHistStack = new THStack((_histName + "_bg_stack").c_str(), "");
	cout<<BACKGROUNDS.size()<<endl;

	for(Int_t i =0; i< BACKGROUNDS.size(); i ++){
		TH1F *bgHist = (TH1F*) mergeBins(BACKGROUNDS[i].ntuple, _histName, "sumGenWeights", histOpts.get("xSectionMap"), histOpts.getInt("colName"), histOpts.getInt("colXsec"),"_merged");
		if(i==0){
			x_axis_title = bgHist->GetXaxis()->GetTitle();
			y_axis_title =  bgHist->GetYaxis()->GetTitle();
		}
		bgHist = (TH1F*) rebinHist(bgHist, newBins);
		bgHist->Scale(histOpts.getFloat("luminosity"));
		BACKGROUNDS[i].assignAtt(bgHist);
		legend->AddEntry(bgHist, BACKGROUNDS[i].legend.c_str(), "FL");
		bgHistStack->Add(bgHist, "HIST ][");
	}


	std::vector<Float_t> xLimits = getXlimits({dataHist, (TH1F*) bgHistStack->GetStack()->Last()}, histOpts.getFloat("binThreshold"));

	pad1->cd();
	bgHistStack->Draw("HIST");
	bgHistStack->GetYaxis()->SetTitle(y_axis_title.c_str());
	bgHistStack->GetXaxis()->SetRangeUser(xLimits[0], xLimits[1]);
	bgHistStack->GetXaxis()->SetLabelSize(0);

	bgHistStack->GetYaxis()->SetLabelSize(histOpts.getFloat("axislabelsize"));
	bgHistStack->GetYaxis()->SetTitleSize(histOpts.getFloat("axistitlesize"));
	bgHistStack->GetYaxis()->SetTitleOffset(histOpts.getFloat("ytitleoffset"));


	dataHist->Draw("PE A SAME");
	dataHist->GetXaxis()->SetRangeUser(xLimits[0], xLimits[1]);

	for(auto & signal : SIGNALS){
		TH1F *signalHist = (TH1F*) mergeBins(signal.ntuple, _histName, "sumGenWeights", histOpts.get("xSectionMap"), 0, 1,"_merged");
		signalHist = (TH1F*) rebinHist(signalHist, newBins);
		signalHist->Scale(histOpts.getFloat("luminosity"));
		signal.assignAtt(signalHist, histOpts.getFloat("linewidth"));
		legend->AddEntry(signalHist, signal.legend.c_str(), "L");
		pad1->cd();
		signalHist->Draw("HIST A ][ SAME");
		signalHist->GetXaxis()->SetRangeUser(xLimits[0], xLimits[1]);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////Ratio plot//////////////////////////////////////////////////////
	TPad * pad2 = new TPad("pad2", "", histOpts.getFloat("pad2x1"), histOpts.getFloat("pad2y1"), histOpts.getFloat("pad2x2"), histOpts.getFloat("pad2y2"));
	pad2->SetMargin(histOpts.getFloat("pad2marginL"), histOpts.getFloat("pad2marginR"), histOpts.getFloat("pad2marginB"), histOpts.getFloat("pad2marginT"));
	pad2->SetFillStyle(0);
	pad2->SetFillColor(0);
	pad2->SetFrameFillStyle(0);
	canvas->cd();
	pad2->Draw();
	pad2->SetGrid(1,1);



	TH1F *dataOverMC = (TH1F*) dataHist->Clone((_histName+(std::string)"_dataOverMC").c_str());
	TH1F *sumBG = (TH1F*) bgHistStack->GetStack()->Last()->Clone((_histName+"_sumBG").c_str());

	dataOverMC->Divide(sumBG);

	sumBG->Delete();

	DATA.assignAtt(dataOverMC, histOpts.getInt("markerSize"));
	dataOverMC->SetFillStyle(histOpts.getInt("errorBandFillStyle"));

	TF1 *unityLine = new TF1("unityLine","1.",xLimits[0],xLimits[1]);
	unityLine->SetLineWidth(histOpts.getFloat("linewidth"));
	unityLine->SetLineColor(TColor::GetColor(histOpts.get("unitylinecolor").c_str()));

	pad2->cd();
	dataOverMC->Draw("HIST P E3");
	dataOverMC->GetXaxis()->SetTitle(x_axis_title.c_str());
	dataOverMC->GetXaxis()->SetTitleSize(histOpts.getFloat("axistitlesize"));
	dataOverMC->GetXaxis()->SetTitleOffset(histOpts.getFloat("xtitleoffset") * histOpts.getFloat("pad2SF"));
	dataOverMC->GetXaxis()->SetLabelSize(histOpts.getFloat("pad2SF")*histOpts.getFloat("axislabelsize"));
	dataOverMC->GetYaxis()->SetTitle(histOpts.get("ratioPlotYtitle").c_str());
	dataOverMC->GetYaxis()->SetTitleSize(histOpts.getFloat("pad2SF") * histOpts.getFloat("axistitlesize"));
	dataOverMC->GetXaxis()->SetTitleOffset(histOpts.getFloat("xtitleoffset") / histOpts.getFloat("pad2SF"));
	dataOverMC->GetYaxis()->SetLabelSize(histOpts.getFloat("pad2SF")*histOpts.getFloat("axislabelsize"));

	unityLine->Draw("SAME A");
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	TPad *pad0 = new TPad("pad0", "", histOpts.getFloat("pad0x1"), histOpts.getFloat("pad0y1"), histOpts.getFloat("pad0x2"), histOpts.getFloat("pad0y2"));
	pad0->SetMargin(0., 0., 0., 0.);
	pad0->SetFillStyle(0);
	pad0->SetFillColor(0);
	pad0->SetFrameFillStyle(0);
	canvas->cd();
	pad0->Draw();

	pad0->cd();
	legend->Draw();

	gPad->RedrawAxis();
	gPad->RedrawAxis("g");
	gPad->Update();

	canvas->RedrawAxis();
	canvas->Update();
	canvas->Modified();

	std::cout<<"\t\tComplete drawing TH1F "<<_histName<<std::endl;

	return canvas;
}


void plot_1D(){
	// gROOT->SetBatch();

	std::cout<<"Begin plotting program..."<<std::endl;

	histOpts.parseIt(histOptionsFile);

	std::string datasetList=histOpts.get("datasetList");

	DATA.set(vLookup("SinglePhoton2017", datasetList, 0, 1), "2017", 21, "#762a83");
	SM.set(vLookup("aNTGCjjgloh3z0sm", datasetList, 0, 1), "SM Z(#rightarrow JJ)+#gamma", -3022, "#ffeda0");
	SIGNALS.emplace_back(vLookup("aNTGCjjgloh3z0p0003", datasetList, 0, 1), "h_{3}^{Z}=0.0003", 29, "#de77ae");
	SIGNALS.emplace_back(vLookup("aNTGCjjgloh3z0p0005", datasetList, 0, 1), "h_{3}^{Z}=0.0005", 29, "#8c510a");
	SIGNALS.emplace_back(vLookup("aNTGCjjgloh3z0p0008", datasetList, 0, 1), "h_{3}^{Z}=0.0008", 29, "#ec7014");
	SIGNALS.emplace_back(vLookup("aNTGCjjgloh3z0p0015", datasetList, 0, 1), "h_{3}^{Z}=0.0015", 29, "#dfc27d");
	SIGNALS.emplace_back(vLookup("aNTGCjjgloh3z0p0029", datasetList, 0, 1), "h_{3}^{Z}=0.0029", 29, "#80cdc1");
	SIGNALS.emplace_back(vLookup("aNTGCjjgloh3z0p0038", datasetList, 0, 1), "h_{3}^{Z}=0.0038", 29, "#01665e");
	BACKGROUNDS.push_back(SM);
	BACKGROUNDS.emplace_back(vLookup(histOpts.get("ttSample"), datasetList, 0, 1), "tt+Jets", -3004, "#66c2a4");
	BACKGROUNDS.emplace_back(vLookup("QCD", datasetList, 0, 1), "QCD", -3005, "#a6bddb");
	BACKGROUNDS.emplace_back(vLookup("GJets", datasetList, 0, 1), "#gamma+Jets", -3007, "#fc8d59");

	std::vector<std::string> histList = getNonemptyLines(histOpts.get("histList"));
	std::string writeDir = histOpts.get("writeDir");

	mkdir(writeDir);

	for(const auto & hist : histList){
		TCanvas *canvas = plotHist(hist);
		canvas->SaveAs( (writeDir + "/" + hist + ".png").c_str());
	}

	std::cout<<"Complete!"<<std::endl;

};