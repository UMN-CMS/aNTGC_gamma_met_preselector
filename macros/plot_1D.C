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
void plotHist(std::string _histName);
void plot_1D();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void plotHist(std::string _histName){
	std::vector<TObject*> thingsToDelete;

	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);

	std::cout<<"Plotting "<<_histName<<std::endl;

	TCanvas *canvas = new TCanvas((_histName + "_canvas").c_str(), "", 2400, 1800);
	thingsToDelete.push_back(canvas);
	canvas->Draw();

	TPad *pad0 = new TPad("pad0", "", histOpts.getFloat("pad0x1"), histOpts.getFloat("pad0y1"), histOpts.getFloat("pad0x2"), histOpts.getFloat("pad0y2"));
	thingsToDelete.push_back(pad0);
	pad0->SetMargin(0., 0., 0., 0.);
	pad0->SetFillStyle(0);
	pad0->SetFillColor(0);
	pad0->SetFrameFillStyle(0);
	canvas->cd();
	pad0->Draw();

	TLegend * legend = new TLegend(0.10, 0.0, 0.99, 1.0);
	thingsToDelete.push_back(legend);
	legend->SetTextSize(histOpts.getFloat("legendTextSize"));
	legend->SetNColumns(histOpts.getInt("legendNcols"));
	legend->SetFillStyle(1001);
	legend->SetBorderSize(0);
	pad0->cd();
	legend->Draw();

	/////////////////////////////////////////////////Draw data, bg & signal///////////////////////////////////////////////////
	TPad *pad1 = new TPad("pad1", "", histOpts.getFloat("pad1x1"), histOpts.getFloat("pad1y1"), histOpts.getFloat("pad1x2"), histOpts.getFloat("pad1y2"));
	thingsToDelete.push_back(pad1);
	pad1->SetMargin(histOpts.getFloat("pad1marginL"), histOpts.getFloat("pad1marginR"), histOpts.getFloat("pad1marginB"), histOpts.getFloat("pad1marginT"));
	pad1->SetFillStyle(0);
	pad1->SetFillColor(0);
	pad1->SetFrameFillStyle(0);
	canvas->cd();
	pad1->Draw();
	pad1->SetGrid(1,1);
	pad1->SetLogy();

	TH1F *dataHist = (TH1F*) getHistFromFile(_histName, DATA.ntuple);
	thingsToDelete.push_back(dataHist);
	DATA.assignAtt(dataHist, histOpts.getFloat("markerSize"), histOpts.getFloat("dataLineWidth"));
	// dataHist->SetMarkerStyle(DATA.marker);
	// dataHist->SetLineWidth(histOpts.getFloat("dataLineWidth"));
	legend->AddEntry(dataHist, DATA.legend.c_str(), "LPE");

	std::vector<Double_t> newBins;

	if(_histName == "_1D_NoofakT4jetswithpT40GeV") newBins = getXbins(dataHist);
	else  newBins = getGoodBins(dataHist, histOpts.getFloat("rebinStatUnc"));
	dataHist = (TH1F*) rebinHist(dataHist, newBins);

	dataHist->Scale(1., "width");

	std::string x_axis_title, y_axis_title;

	THStack *bgHistStack = new THStack((_histName + "_bg_stack").c_str(), "");
	thingsToDelete.push_back(bgHistStack);

	for(Int_t i =0; i< BACKGROUNDS.size(); i ++){
		TH1F *bgHist = (TH1F*) mergeBins(BACKGROUNDS[i].ntuple, _histName, "sumGenWeights", histOpts.get("xSectionMap"), histOpts.getInt("colName"), histOpts.getInt("colXsec"),"_merged");
		if(i==0){
			x_axis_title = bgHist->GetXaxis()->GetTitle();
			y_axis_title =  bgHist->GetYaxis()->GetTitle();
		}
		bgHist = (TH1F*) rebinHist(bgHist, newBins);
		thingsToDelete.push_back(bgHist);
		bgHist->Scale(histOpts.getFloat("luminosity"), "width");
		BACKGROUNDS[i].assignAtt(bgHist);
		legend->AddEntry(bgHist, BACKGROUNDS[i].legend.c_str(), "FL");
		bgHistStack->Add(bgHist, "HIST ");
	}

	std::string yUnit = getUnit(y_axis_title);
	y_axis_title = eraseUnit(y_axis_title);
	y_axis_title = y_axis_title + ((yUnit.empty()) ? "" : ("/"+yUnit));


	std::vector<Float_t> xLimits = getXlimits({dataHist, (TH1F*) bgHistStack->GetStack()->Last()}, histOpts.getFloat("binThreshold"));

	pad1->cd();
	dataHist->Draw("HIST PE");
	dataHist->GetYaxis()->SetRangeUser(histOpts.getFloat("stackMin"), histOpts.getFloat("maxBinRatio") * dataHist->GetMaximum());
	dataHist->GetXaxis()->SetRangeUser(xLimits[1], xLimits[0]);

	bgHistStack->Draw("HIST A SAME");
	dataHist->GetYaxis()->SetTitle(y_axis_title.c_str());
	dataHist->GetXaxis()->SetRangeUser(xLimits[1], xLimits[0]);
	dataHist->GetXaxis()->SetLabelSize(0);

	dataHist->GetYaxis()->SetLabelSize(histOpts.getFloat("pad1axislabelsize"));
	dataHist->GetYaxis()->SetTitleSize(histOpts.getFloat("pad1axisTitleSize"));
	dataHist->GetYaxis()->SetTitleOffset(histOpts.getFloat("pad1ytitleoffset"));

	for(auto & signal : SIGNALS){
		TH1F *signalHist = (TH1F*) mergeBins(signal.ntuple, _histName, "sumGenWeights", histOpts.get("xSectionMap"), 0, 1,"_merged");
		signalHist = (TH1F*) rebinHist(signalHist, newBins);
		thingsToDelete.push_back(signalHist);
		signalHist->Scale(histOpts.getFloat("luminosity"), "width");
		signal.assignAtt(signalHist, histOpts.getFloat("markerSize"), histOpts.getFloat("linewidth"));
		legend->AddEntry(signalHist, signal.legend.c_str(), "L");
		pad1->cd();
		signalHist->Draw("HIST A  SAME");
		signalHist->GetXaxis()->SetRangeUser(xLimits[1], xLimits[0]);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////Ratio plot//////////////////////////////////////////////////////
	TPad * pad2 = new TPad("pad2", "", histOpts.getFloat("pad2x1"), histOpts.getFloat("pad2y1"), histOpts.getFloat("pad2x2"), histOpts.getFloat("pad2y2"));
	thingsToDelete.push_back(pad2);
	pad2->SetMargin(histOpts.getFloat("pad2marginL"), histOpts.getFloat("pad2marginR"), histOpts.getFloat("pad2marginB"), histOpts.getFloat("pad2marginT"));
	pad2->SetFillStyle(0);
	pad2->SetFillColor(0);
	pad2->SetFrameFillStyle(0);
	canvas->cd();
	pad2->Draw();
	pad2->SetGrid(1,1);



	TH1F *dataOverMC = (TH1F*) dataHist->Clone((_histName+(std::string)"_dataOverMC").c_str());
	TH1F *sumBG = (TH1F*) bgHistStack->GetStack()->Last()->Clone((_histName+"_sumBG").c_str());
	thingsToDelete.push_back(dataOverMC);
	thingsToDelete.push_back(sumBG);
	dataOverMC->Divide(sumBG);

	sumBG->Delete();

	DATA.assignAtt(dataOverMC, histOpts.getFloat("markerSize"), histOpts.getFloat("linewidth"));
	dataOverMC->SetFillStyle(histOpts.getInt("errorBandFillStyle"));
	dataOverMC->SetFillColor(TColor::GetColor(DATA.color.c_str()));

	TF1 *unityLine = new TF1("unityLine","1.",xLimits[0],xLimits[1]);
	thingsToDelete.push_back(unityLine);
	unityLine->SetLineWidth(histOpts.getFloat("linewidth"));
	unityLine->SetLineColor(TColor::GetColor(histOpts.get("unitylinecolor").c_str()));


	pad2->cd();

	dataOverMC->Draw(histOpts.get("dataMCopt1").c_str());
	dataOverMC->Draw(histOpts.get("dataMCopt2").c_str());
	dataOverMC->GetXaxis()->SetTitle(x_axis_title.c_str());
	dataOverMC->GetXaxis()->SetTitleSize(histOpts.getFloat("pad2axisTitleSize"));
	dataOverMC->GetXaxis()->SetTitleOffset(histOpts.getFloat("pad2xtitleoffset"));
	dataOverMC->GetXaxis()->SetLabelSize(histOpts.getFloat("pad2axislabelsize"));
	dataOverMC->GetYaxis()->SetTitle(histOpts.get("ratioPlotYtitle").c_str());
	dataOverMC->GetYaxis()->SetTitleSize(histOpts.getFloat("pad2axisTitleSize"));
	dataOverMC->GetYaxis()->SetTitleOffset(histOpts.getFloat("pad2ytitleoffset"));
	dataOverMC->GetYaxis()->SetLabelSize(histOpts.getFloat("pad2axislabelsize"));
	dataOverMC->GetXaxis()->SetRangeUser(xLimits[1], xLimits[0]);

	// Double_t ratioMin =  (dataOverMC->GetMinimum() > histOpts.getFloat("ratioMin")) ? histOpts.getFloat("ratioAtLeast") : dataOverMC->GetMinimum();
	// Double_t ratioMax =  (dataOverMC->GetMaximum() < histOpts.getFloat("ratioMax")) ? histOpts.getFloat("ratioAtMost") : dataOverMC->GetMaximum();
	// Double_t ratioMean = weightedYmean(dataOverMC);
	// Double_t _tempMax = ratioMean + histOpts.getFloat("ratioDeltaY") * (ratioMean - histOpts.getFloat("ratioMin"));
	// cout<<"ratioMean = "<<ratioMean<<" tempMax = "<<_tempMax<<std::endl;
	// ratioMax = (ratioMax > histOpts.getFloat("ratioMax")) ? _tempMax : ratioMax;

	Double_t _ratioSpread = weightedYspread(dataOverMC);
	Double_t _ratioMean = weightedYmean(dataOverMC);

	Double_t ratioMin = _ratioMean - histOpts.getFloat("ratioNspread") * _ratioSpread;
	Double_t ratioMax = _ratioMean + histOpts.getFloat("ratioNspread") * _ratioSpread;

	if(ratioMin < histOpts.getFloat("ratioMin")) ratioMin = histOpts.getFloat("ratioMin");
	if(ratioMin > histOpts.getFloat("ratioAtLeast")) ratioMin = histOpts.getFloat("ratioAtLeast");

	if(ratioMax > histOpts.getFloat("ratioMax")) ratioMax = histOpts.getFloat("ratioMax");
	if(ratioMax < histOpts.getFloat("ratioAtMost")) ratioMax = histOpts.getFloat("ratioAtMost");

	if(ratioMin != ratioMin) ratioMin = histOpts.getFloat("ratioMin");
	if(ratioMax != ratioMax) ratioMax = histOpts.getFloat("ratioMax");

	cout<<"rMin = "<<ratioMin<<" rMax"<<ratioMax<<endl;
	unityLine->Draw("SAME HIST A");

	dataOverMC->GetYaxis()->SetRangeUser(ratioMin, ratioMax);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	gPad->RedrawAxis();
	gPad->RedrawAxis("g");
	gPad->Update();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	canvas->RedrawAxis();
	canvas->Update();
	canvas->Modified();

	std::cout<<"\t\tComplete drawing TH1F "<<_histName<<std::endl;

	std::string writeDir = histOpts.get("writeDir");
	canvas->SaveAs( (writeDir + "/" + _histName + ".png").c_str());

	canvas->Clear();

	// for(TObject * obj : thingsToDelete){
	// 	if(obj){
	// 		if(obj->InheritsFrom("TH1")){
	// 			obj->Delete();
	// 		}
	// 	}
	// }
	// canvas->Delete();
}


void plot_1D(){
	gROOT->SetBatch();

	std::cout<<"Begin plotting program..."<<std::endl;

	histOpts.parseIt(histOptionsFile);

	std::string datasetList=histOpts.get("datasetList");

	DATA.set(vLookup("SinglePhoton2017", datasetList, 0, 1), "2017 Data", 20, "#4d4d4d");
	SM.set(vLookup("aNTGCjjgloh3z0sm", datasetList, 0, 1), "SM Z(#rightarrow JJ)+#gamma", -3022, "#d73027");
	SIGNALS.emplace_back(vLookup("aNTGCjjgloh3z0p0003", datasetList, 0, 1), "h_{3}^{Z}=0.0003", 29, "#01665e");
	SIGNALS.emplace_back(vLookup("aNTGCjjgloh3z0p0005", datasetList, 0, 1), "h_{3}^{Z}=0.0005", 29, "#542788");
	SIGNALS.emplace_back(vLookup("aNTGCjjgloh3z0p0008", datasetList, 0, 1), "h_{3}^{Z}=0.0008", 29, "#a65628.");
	SIGNALS.emplace_back(vLookup("aNTGCjjgloh3z0p0015", datasetList, 0, 1), "h_{3}^{Z}=0.0015", 29, "#b2182b");
	SIGNALS.emplace_back(vLookup("aNTGCjjgloh3z0p0029", datasetList, 0, 1), "h_{3}^{Z}=0.0029", 29, "#e08214");
	SIGNALS.emplace_back(vLookup("aNTGCjjgloh3z0p0038", datasetList, 0, 1), "h_{3}^{Z}=0.0038", 29, "#c51b7d");
	BACKGROUNDS.push_back(SM);
	BACKGROUNDS.emplace_back(vLookup(histOpts.get("ttSample"), datasetList, 0, 1), "tt+Jets", -3004, "#762a83");
	BACKGROUNDS.emplace_back(vLookup("QCD", datasetList, 0, 1), "QCD", -3005, "#1b7837");
	BACKGROUNDS.emplace_back(vLookup("GJets", datasetList, 0, 1), "#gamma+Jets", -3007, "#2166ac");

	std::vector<std::string> histList = getNonemptyLines(histOpts.get("histList"));

	std::string writeDir = histOpts.get("writeDir");
	mkdir(writeDir);

	for(const auto & hist : histList){
		plotHist(hist);
		// clearStack();
	}

	std::cout<<"Complete!"<<std::endl;

};