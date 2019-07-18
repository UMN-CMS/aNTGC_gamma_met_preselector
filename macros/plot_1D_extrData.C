#include "extra_tools.cc"
#include "TStyle.h"
#include "TROOT.h"
#include "THStack.h"
#include <TLegend.h>
#include <TPad.h>
#include <TCanvas.h>
#include <TF1.h>
#include "TText.h"
#include "TLegendEntry.h"




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string histOptionsFile="../data/stackedHistOptions.txt";

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

	TCanvas canvas((_histName + "_canvas").c_str(), "", 2400, 1800);
	canvas.Draw();

	TPad pad0("pad0", "", histOpts.getFloat("pad0x1"), histOpts.getFloat("pad0y1"), histOpts.getFloat("pad0x2"), histOpts.getFloat("pad0y2"));
	pad0.SetMargin(0., 0., 0., 0.);
	pad0.SetFillStyle(0);
	pad0.SetFillColor(0);
	pad0.SetFrameFillStyle(0);
	canvas.cd();
	pad0.Draw();

	TLegend legend(0.10, 0.0, 0.99, 1.0);
	legend.SetTextSize(histOpts.getFloat("legendTextSize"));
	legend.SetNColumns(histOpts.getInt("legendNcols"));
	legend.SetFillStyle(1001);
	legend.SetBorderSize(0);
	pad0.cd();
	legend.Draw();

	/////////////////////////////////////////////////Draw data, bg & signal///////////////////////////////////////////////////
	TPad pad1("pad1", "", histOpts.getFloat("pad1x1"), histOpts.getFloat("pad1y1"), histOpts.getFloat("pad1x2"), histOpts.getFloat("pad1y2"));
	pad1.SetMargin(histOpts.getFloat("pad1marginL"), histOpts.getFloat("pad1marginR"), histOpts.getFloat("pad1marginB"), histOpts.getFloat("pad1marginT"));
	pad1.SetFillStyle(0);
	pad1.SetFillColor(0);
	pad1.SetFrameFillStyle(0);
	canvas.cd();
	pad1.Draw();
	pad1.SetGrid(1,1);
	pad1.SetLogy();

	TH1F *dataHist = (TH1F*) getHistFromFile(_histName, DATA.ntuple);
	thingsToDelete.push_back(dataHist);
	DATA.assignAtt(dataHist, histOpts.getFloat("markerSize"), histOpts.getFloat("dataLineWidth"));
	dataHist->SetMarkerStyle(DATA.marker);
	dataHist->SetLineWidth(histOpts.getFloat("dataLineWidth"));
	legend.AddEntry(dataHist, DATA.legend.c_str(), "LPE");

	std::vector<Double_t> newBins;
	newBins = getGoodBins(dataHist, histOpts.getFloat("rebinStatUnc"));

	dataHist = (TH1F*) rebinHist(dataHist, newBins);

	dataHist->Scale(1., "width");

	std::string x_axis_title, y_axis_title;

	THStack bgHistStack((_histName + "_bg_stack").c_str(), "");

	Float_t SM_significance = 0.;

	for(Int_t i =0; i< BACKGROUNDS.size(); i ++){
		TH1F *bgHist = (TH1F*) getHistFromFile(_histName, BACKGROUNDS[i].ntuple);

		TH1F *sumGenWeights = (TH1F*) getHistFromFile("sumGenWeights", BACKGROUNDS[i].ntuple);
		sumGenWeights->SetName((BACKGROUNDS[i].ntuple + "_sumGenWeights").c_str());
		bgHist->Scale(1./sumGenWeights->GetBinContent(1));
		delete sumGenWeights;
		sumGenWeights = nullptr;

		bgHist = (TH1F*) rebinHist(bgHist, newBins);
		bgHist->Scale(histOpts.getFloat("luminosity"));

		bgHist->Scale(1., "width");
		BACKGROUNDS[i].assignAtt(bgHist);

		thingsToDelete.push_back(bgHist);

		if(!BACKGROUNDS[i].legend.empty()){
			legend.AddEntry(bgHist, BACKGROUNDS[i].legend.c_str(), "FL");
		} else{
			bgHist->SetLineWidth(0.);
		}

		bgHist->SetFillStyle(3001);
		bgHistStack.Add(bgHist, "HIST ");

		if(i == 0) SM_significance = bgHist->Integral(0, bgHist->GetNbinsX()+1, "width");

		std::cout<<"\t\t"<<BACKGROUNDS[i].ntuple<<","<<bgHist->Integral(0, bgHist->GetNbinsX()+1, "width")<<std::endl;

		x_axis_title = bgHist->GetXaxis()->GetTitle();
		y_axis_title =  bgHist->GetYaxis()->GetTitle();
	}

	std::string yUnit = getUnit(y_axis_title);
	y_axis_title = eraseUnit(y_axis_title);
	y_axis_title = y_axis_title + ((yUnit.empty()) ? "" : ("/"+yUnit));
	y_axis_title = findAndReplaceAll(y_axis_title, "\\", "#");

	std::vector<Float_t> xLimits = getXlimits({dataHist, (TH1F*) bgHistStack.GetStack()->Last()}, histOpts.getFloat("binThreshold"));

	pad1.cd();
	dataHist->Draw("HIST PE");
	bgHistStack.Draw("HIST A SAME");
	dataHist->GetYaxis()->SetRangeUser(histOpts.getFloat("stackMin"), histOpts.getFloat("maxBinRatio") * dataHist->GetMaximum());
	dataHist->GetXaxis()->SetRangeUser(xLimits[1], xLimits[0]);
	dataHist->GetYaxis()->SetTitle(y_axis_title.c_str());
	dataHist->GetXaxis()->SetRangeUser(xLimits[1], xLimits[0]);
	dataHist->GetXaxis()->SetLabelSize(0);
	dataHist->GetYaxis()->SetLabelSize(histOpts.getFloat("pad1axislabelsize"));
	dataHist->GetYaxis()->SetTitleSize(histOpts.getFloat("pad1axisTitleSize"));
	dataHist->GetYaxis()->SetTitleOffset(histOpts.getFloat("pad1ytitleoffset"));

	for(auto & signal : SIGNALS){
		TH1F *signalHist = (TH1F*) getHistFromFile(_histName, signal.ntuple);

		TH1F *sumGenWeights = (TH1F*) getHistFromFile("sumGenWeights", signal.ntuple);
		sumGenWeights->SetName((signal.ntuple + "_sumGenWeights").c_str());
		signalHist->Scale(1./sumGenWeights->GetBinContent(1));
		delete sumGenWeights;
		sumGenWeights = nullptr;

		signalHist = (TH1F*) rebinHist(signalHist, newBins);
		signalHist->Scale(histOpts.getFloat("luminosity"), "width");

		thingsToDelete.push_back(signalHist);

		signal.assignAtt(signalHist, histOpts.getFloat("markerSize"), histOpts.getFloat("linewidth"));
		legend.AddEntry(signalHist, signal.legend.c_str(), "L");
		pad1.cd();
		signalHist->Draw("HIST A  SAME");
		signalHist->GetXaxis()->SetRangeUser(xLimits[1], xLimits[0]);
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////Ratio plot//////////////////////////////////////////////////////
	TPad pad2("pad2", "", histOpts.getFloat("pad2x1"), histOpts.getFloat("pad2y1"), histOpts.getFloat("pad2x2"), histOpts.getFloat("pad2y2"));
	// thingsToDelete.push_back(pad2);
	pad2.SetMargin(histOpts.getFloat("pad2marginL"), histOpts.getFloat("pad2marginR"), histOpts.getFloat("pad2marginB"), histOpts.getFloat("pad2marginT"));
	pad2.SetFillStyle(0);
	pad2.SetFillColor(0);
	pad2.SetFrameFillStyle(0);
	canvas.cd();
	pad2.Draw();
	pad2.SetGrid(1,1);


	TH1F *dataOverMC = (TH1F*) dataHist->Clone((_histName+(std::string)"_dataOverMC").c_str());
	TH1F *sumBG = (TH1F*) bgHistStack.GetStack()->Last()->Clone((_histName+"_sumBG").c_str());

	// SM_significance = SM_significance / std::sqrt(sumBG->Integral(0, sumBG->GetNbinsX()+1, "width"));
	// std::string sigString = "S/\\sqrt{S+B}=" + to_string_with_precision(SM_significance,1);
	// TLegendEntry* sigLeg = legend.AddEntry(&pad2, sigString.c_str(), "");
	// sigLeg->SetTextColor(kRed);

	cout<<"\t\t kFactor with data "<<dataOverMC->Integral(0, dataOverMC->GetNbinsX()+1, "width")/sumBG->Integral(0, sumBG->GetNbinsX()+1, "width");

	dataOverMC->Divide(sumBG);

	thingsToDelete.push_back(dataOverMC);
	thingsToDelete.push_back(sumBG);

	DATA.assignAtt(dataOverMC, histOpts.getFloat("markerSize"), histOpts.getFloat("linewidth"));
	dataOverMC->SetFillStyle(histOpts.getInt("errorBandFillStyle"));
	dataOverMC->SetFillColor(TColor::GetColor(DATA.color.c_str()));

	TF1 unityLine("unityLine","1.",xLimits[1],xLimits[0]);
	unityLine.SetLineWidth(histOpts.getFloat("linewidth"));
	unityLine.SetLineColor(TColor::GetColor(histOpts.get("unitylinecolor").c_str()));

	pad2.cd();

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

	cout<<"\t\t rMin = "<<ratioMin<<" rMax"<<ratioMax<<endl;

	unityLine.Draw("SAME HIST A");

	dataOverMC->GetYaxis()->SetRangeUser(ratioMin, ratioMax);

	// pad1->cd();
	// TText *t = new TText(.8,.85,sigString.c_str());
	// t->SetTextColor(kBlack);
	// t->SetTextFont(43);
	// t->SetTextSize(histOpts.getFloat("pad2axisTitleSize"));
	// t->Draw();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	gPad->RedrawAxis();
	gPad->RedrawAxis("g");
	gPad->Update();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	canvas.RedrawAxis();
	canvas.Update();
	canvas.Modified();

	std::cout<<"\t\tComplete drawing TH1F "<<_histName<<std::endl;

	std::string writeDir = histOpts.get("writeDir");
	canvas.SaveAs( (writeDir + "/" + _histName + ".png").c_str());

	for(TObject * obj : thingsToDelete){
		if(obj){
			// delete obj;
			// obj->Delete();
			// if(obj->InheritsFrom("TH1")){
			// }
		}
	}
	// canvas->Delete();
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void plot_1D(){
	gROOT->SetBatch();

	std::cout<<"Begin plotting program..."<<std::endl;

	histOpts.parseIt(histOptionsFile);

	std::string inDir = histOpts.get("inDir") + "/";


	DATA.set(inDir+"SinglePhotonRun2017.root", "2017 Data", 20, "#000000");

	SM.set(inDir+"ZNuNuGJetsMonoPhotonPtG130TuneCUETP8M113TeVmadgraph.root", "SM Z(#rightarrow #nu#bar{#nu})+#gamma", -3022, "#d73027");

	// SIGNALS.emplace_back(inDir+"aNTGC_0p0003_0p_0p_0p.root", "h_{3}^{Z}=0.0003", 29, "#01665e");

	BACKGROUNDS.emplace_back(inDir+"ZZTuneCP513TeVpythia8.root", "ZZ", -3004, "#762a83");
	BACKGROUNDS.emplace_back(inDir+"TTGJetsTuneCP513TeVamcatnloFXFXmadspinpythia8.root", "t#bar{t}+#gamma+jets", -3004, "#b15928");
	BACKGROUNDS.emplace_back(inDir+"TGJetsTuneCP513TeVamcatnlomadspinpythia8.root", "t+#gamma+jets", -3004, "#6a3d9a");
	BACKGROUNDS.emplace_back(inDir+"WToMuNuM100TuneCP513TeVpythia8.root", "W #rightarrow #mu #nu", -3004, "#ff7f00");
	BACKGROUNDS.emplace_back(inDir+"WZTuneCP513TeVpythia8.root", "W+Z", -3004, "#e31a1c");
	BACKGROUNDS.emplace_back(inDir+"WWTuneCP513TeVpythia8.root", "WW", -3004, "#33a02c");
	BACKGROUNDS.emplace_back(inDir+"WToTauNuM100TuneCP513TeVpythia8tauola.root", "W #rightarrow #tau #nu", -3004, "#1f78b4");
	BACKGROUNDS.emplace_back(inDir+"ZLLGJetsMonoPhotonPtG130TuneCUETP8M113TeVmadgraph.root", "Z(#rightarrow l#bar{l})+#gamma+jets", -3004, "#ffff99");
	BACKGROUNDS.emplace_back(inDir+"DiPhotonJetsMGG80toInf13TeVamcatnloFXFXpythia8.root", "#gamma#gamma+jets", -3004, "#cab2d6");
	BACKGROUNDS.emplace_back(inDir+"DiPhotonJetsBoxMGG80toInf13TeVSherpa.root", "#gamma#gamma+jets (box)", -3004, "#fdbf6f");

	BACKGROUNDS.emplace_back(inDir+"WToENuM100TuneCP513TeVpythia8.root", "W #rightarrow e #nu", -3004, "#fb9a99");

	BACKGROUNDS.emplace_back(inDir+"GJetsHT40To100TuneCP513TeVmadgraphMLMpythia8.root", "#gamma+jets", -3004, "#b2df8a");
	BACKGROUNDS.emplace_back(inDir+"GJetsHT100To200TuneCP513TeVmadgraphMLMpythia8.root", "", -3004, "#b2df8a");
	BACKGROUNDS.emplace_back(inDir+"GJetsHT200To400TuneCP513TeVmadgraphMLMpythia8.root", "", -3004, "#b2df8a");
	BACKGROUNDS.emplace_back(inDir+"GJetsHT400To600TuneCP513TeVmadgraphMLMpythia8.root", "", -3004, "#b2df8a");
	BACKGROUNDS.emplace_back(inDir+"GJetsHT600ToInfTuneCP513TeVmadgraphMLMpythia8.root", "", -3004, "#b2df8a");


	BACKGROUNDS.emplace_back(inDir+"WGJetsMonoPhotonPtG130TuneCUETP8M113TeVmadgraph.root", "W+#gamma+jets" , -3004, "#a6cee3");

	BACKGROUNDS.push_back(SM);


	std::vector<std::string> histList = getNonemptyLines(histOpts.get("histList"));

	std::string writeDir = histOpts.get("writeDir");
	mkdir(writeDir);

	for(const auto & hist : histList){
		plotHist(hist);
	}

	std::cout<<"Complete!"<<std::endl;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////