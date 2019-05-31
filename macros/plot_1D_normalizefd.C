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

std::string histOptionsFile="../data/histOptionsCompareModels.txt";

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// sample REFERENCE;
// sample SM;
// std::vector<sample> SIGNALS;
// std::vector<sample> SAMPLES;

sample REFERENCE;
std::vector<sample> SAMPLES;

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
	// pad1.SetLogy();

	TH1F *refHist = (TH1F*) mergeBins(REFERENCE.ntuple, _histName, "sumGenWeights", histOpts.get("xSectionMap"), histOpts.getInt("colName"), histOpts.getInt("colXsec"),"_merged");
	REFERENCE.assignAtt(refHist, histOpts.getFloat("markerSize"), histOpts.getFloat("dataLineWidth"));
	legend.AddEntry(refHist, REFERENCE.legend.c_str(), "L");

	//////////////////////////////////////
	thingsToDelete.push_back(refHist);
	//////////////////////////////////////

	std::vector<Double_t> newBins;

	// if(_histName == "_1D_NoofakT4jetswithpT40GeV") newBins = getXbins(refHist);
	// else  newBins = getGoodBins(refHist, histOpts.getFloat("rebinStatUnc"));
	// refHist = (TH1F*) rebinHist(refHist, newBins);

	// refHist->Scale(1./refHist->Integral());

	std::string x_axis_title, y_axis_title;

	THStack histStack((_histName + "_hist_stack").c_str(), "");

	THStack ratioStack((_histName + "_ratio_stack").c_str(), "");

	histStack.Add(refHist, "HIST ");

	std::vector<Float_t> _ratioSpreads;
	std::vector<Float_t> _ratioMeans;

	for(Int_t i =0; i< SAMPLES.size(); i ++){
		TH1F *sampleHist = (TH1F*) mergeBins(SAMPLES[i].ntuple, _histName, "sumGenWeights", histOpts.get("xSectionMap"), histOpts.getInt("colName"), histOpts.getInt("colXsec"),"_merged");
		if(i==0){
			x_axis_title = sampleHist->GetXaxis()->GetTitle();
			y_axis_title =  sampleHist->GetYaxis()->GetTitle();
		}
		// sampleHist = (TH1F*) rebinHist(sampleHist, newBins);
		// sampleHist->Scale(1./sampleHist->Integral());
		SAMPLES[i].assignAtt(sampleHist, histOpts.getFloat("markerSize"), histOpts.getFloat("dataLineWidth"));
		legend.AddEntry(sampleHist, SAMPLES[i].legend.c_str(), "FL");
		histStack.Add(sampleHist, "HIST");

		TH1F *sampleHistRatio = (TH1F*) sampleHist->Clone((_histName+(std::string)"_ratio").c_str());
		sampleHistRatio->Divide(refHist);
		ratioStack.Add(sampleHistRatio, "HIST");

		_ratioSpreads.push_back(weightedYspread(sampleHistRatio));
		_ratioMeans.push_back(weightedYmean(sampleHistRatio));
	}

	std::string yUnit = getUnit(y_axis_title);
	y_axis_title = eraseUnit(y_axis_title);
	y_axis_title = y_axis_title + ((yUnit.empty()) ? "" : ("/"+yUnit));
	y_axis_title = findAndReplaceAll(y_axis_title, "\\slash", "#slash");

	std::vector<Float_t> xLimits = getXlimits({refHist, (TH1F*) histStack.GetStack()->Last()}, histOpts.getFloat("binThreshold"));

	pad1.cd();

	histStack.Draw("NOSTACK");
	histStack.GetYaxis()->SetRangeUser(histOpts.getFloat("stackMin"), histOpts.getFloat("maxBinRatio") * refHist->GetMaximum());
	histStack.GetXaxis()->SetRangeUser(xLimits[1], xLimits[0]);
	histStack.GetYaxis()->SetTitle(y_axis_title.c_str());
	histStack.GetXaxis()->SetRangeUser(xLimits[1], xLimits[0]);
	histStack.GetXaxis()->SetLabelSize(0);

	histStack.GetYaxis()->SetLabelSize(histOpts.getFloat("pad1axislabelsize"));
	histStack.GetYaxis()->SetTitleSize(histOpts.getFloat("pad1axisTitleSize"));
	histStack.GetYaxis()->SetTitleOffset(histOpts.getFloat("pad1ytitleoffset"));
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

	TF1 unityLine("unityLine","1.",xLimits[0],xLimits[1]);
	unityLine.SetLineWidth(histOpts.getFloat("linewidth"));
	unityLine.SetLineColor(TColor::GetColor(histOpts.get("unitylinecolor").c_str()));

	pad2.cd();

	ratioStack.Draw("NOSTACK");
	ratioStack.GetXaxis()->SetTitle(x_axis_title.c_str());
	ratioStack.GetXaxis()->SetTitleSize(histOpts.getFloat("pad2axisTitleSize"));
	ratioStack.GetXaxis()->SetTitleOffset(histOpts.getFloat("pad2xtitleoffset"));
	ratioStack.GetXaxis()->SetLabelSize(histOpts.getFloat("pad2axislabelsize"));
	ratioStack.GetYaxis()->SetTitle(histOpts.get("ratioPlotYtitle").c_str());
	ratioStack.GetYaxis()->SetTitleSize(histOpts.getFloat("pad2axisTitleSize"));
	ratioStack.GetYaxis()->SetTitleOffset(histOpts.getFloat("pad2ytitleoffset"));
	ratioStack.GetYaxis()->SetLabelSize(histOpts.getFloat("pad2axislabelsize"));
	ratioStack.GetXaxis()->SetRangeUser(xLimits[1], xLimits[0]);

	Float_t _ratioSpread = *std::max_element(_ratioSpreads.begin(), _ratioSpreads.end());
	Float_t _ratioMean = getMean(_ratioMeans);

	Float_t ratioMin = _ratioMean - histOpts.getFloat("ratioNspread") * _ratioSpread;
	Float_t ratioMax = _ratioMean + histOpts.getFloat("ratioNspread") * _ratioSpread;

	if(ratioMin < histOpts.getFloat("ratioMin")) ratioMin = histOpts.getFloat("ratioMin");
	if(ratioMin > histOpts.getFloat("ratioAtLeast")) ratioMin = histOpts.getFloat("ratioAtLeast");

	if(ratioMax > histOpts.getFloat("ratioMax")) ratioMax = histOpts.getFloat("ratioMax");
	if(ratioMax < histOpts.getFloat("ratioAtMost")) ratioMax = histOpts.getFloat("ratioAtMost");

	if(ratioMin != ratioMin) ratioMin = histOpts.getFloat("ratioMin");
	if(ratioMax != ratioMax) ratioMax = histOpts.getFloat("ratioMax");

	cout<<"\t\t\trMin = "<<ratioMin<<" rMax = "<<ratioMax<<endl;
	unityLine.Draw("SAME HIST A");

	ratioStack.GetYaxis()->SetRangeUser(ratioMin, ratioMax);
	ratioStack.GetYaxis()->SetLimits(ratioMin, ratioMax);
	ratioStack.SetMaximum(ratioMax);
	ratioStack.SetMinimum(ratioMin);

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
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void plot_1D(){
	gROOT->SetBatch();

	std::cout<<"Begin plotting program..."<<std::endl;

	histOpts.parseIt(histOptionsFile);

	std::string datasetList=histOpts.get("datasetList");

	REFERENCE.set(vLookup("aNTGCjjgloh3z0p0008EFT", datasetList, 0, 1), "EFT", 29, "#a65628.");
	SAMPLES.emplace_back(vLookup("aNTGC0p00080p0p0pFlatPt", datasetList, 0, 1), "Flat p_{T} (Vtx)", 21, "#1b7837");
	SAMPLES.emplace_back(vLookup("aNTGC0p00080p0p0pBinnedPt", datasetList, 0, 1), "p_{T} binned (Vtx)", 22, "#2166ac");

	// std::vector<std::string> histList = getNonemptyLines(histOpts.get("histList"));

	cout<<datasetList<<endl;
	cout<<vLookup("aNTGCjjgloh3z0p0008EFT", datasetList, 0, 1)<<endl;
	std::vector<std::string> histList = getObjectList(getNonemptyLines(vLookup("aNTGCjjgloh3z0p0008EFT", datasetList, 0, 1))[0], "TH1F");
	std::string writeDir = histOpts.get("writeDir");
	mkdir(writeDir);
	for(const auto & hist : histList){
		plotHist(hist);
		// clearStack();
	}
	std::cout<<"Complete!"<<std::endl;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////