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
#include "limits"



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::string histOptionsFile="../data/dataHistOptions.txt";
std::vector<sample> CUTS;
parseOptions histOpts;
void plotHist(std::string _histName);
void plot_1D();
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



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
	pad0.SetMargin(histOpts.getFloat("pad0marginL"), histOpts.getFloat("pad0marginR"), histOpts.getFloat("pad0marginB"), histOpts.getFloat("pad0marginT"));
	pad0.SetFillStyle(0);
	pad0.SetFillColor(0);
	pad0.SetFrameFillStyle(0);
	canvas.cd();
	pad0.Draw();

	TLegend legend(0., 0.0, 1., 1.0);
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
	if(histOpts.getInt("logy") == 1)pad1.SetLogy();

	THStack cutsStack((_histName + "_cuts_stack").c_str(), "");

	std::string x_axis_title ="";
	std::string y_axis_title = "";
	std::vector<Double_t> goodBins;
	Double_t _yMin = std::numeric_limits<Double_t>::max(), _yMax = std::numeric_limits<Double_t>::min();

	for(Int_t i =0; i< CUTS.size(); i ++){
		TH1F *cutHist = (TH1F*) getHistFromFile(_histName, CUTS[i].ntuple, 1);
		if(i == 0) goodBins = getGoodBins(cutHist, histOpts.getFloat("rebinStatUnc"));
		cutHist = (TH1F*) rebinHist(cutHist, goodBins);
		CUTS[i].assignAtt(cutHist, histOpts.getFloat("markerSize"), histOpts.getFloat("lineWidth"));

		std::string newHistName = findAndReplaceAll(getFileName(CUTS[i].ntuple), ".root", "") + "_" + cutHist->GetName();
		cutHist->SetName(newHistName.c_str());

		if(histOpts.getInt("normalize")==1) cutHist->Scale(histOpts.getFloat("normIntegral") / cutHist->Integral(0,cutHist->GetNbinsX()+1));
		cutHist->Scale(1.,"width");

		thingsToDelete.push_back(cutHist);

		cutHist->SetLineStyle(histOpts.getInt("data" + std::to_string(i+1) + "linestyle"));

		cutsStack.Add(cutHist, histOpts.get("histDrawOpt").c_str());

		TLegendEntry* cutLegend = legend.AddEntry(cutHist, CUTS[i].legend.c_str(), "LPE");
		cutLegend->SetTextColor(TColor::GetColor(CUTS[i].color.c_str()));

		x_axis_title = cutHist->GetXaxis()->GetTitle();
		y_axis_title =  cutHist->GetYaxis()->GetTitle();

		if(_yMin > cutHist->GetMinimum(0.000000001)) _yMin = cutHist->GetMinimum(0.0000000001);
		if(_yMax < cutHist->GetMaximum()) _yMax = cutHist->GetMaximum();
	}

	std::string yUnit = getUnit(x_axis_title);
	y_axis_title = eraseUnit(y_axis_title);
	y_axis_title = y_axis_title + "/" + (yUnit.empty() ? "1" : (yUnit));
	y_axis_title = findAndReplaceAll(y_axis_title, "\\", "#");

	std::vector<Float_t> xLimits = getXlimits({(TH1F*) cutsStack.GetStack()->Last()}, histOpts.getFloat("binThreshold"));

	_yMin = histOpts.getFloat("extraYdn") * _yMin;
	_yMax = histOpts.getFloat("extraYup") * _yMax;

	pad1.cd();
	cutsStack.Draw("NOSTACK");
	cutsStack.GetYaxis()->SetTitle(y_axis_title.c_str());
	cutsStack.GetXaxis()->SetRangeUser(xLimits[1], xLimits[0]);
	cutsStack.GetYaxis()->SetRangeUser(_yMin, _yMax);
	((TGaxis*)cutsStack.GetYaxis())->SetMaxDigits(histOpts.getInt("pad1yMaxDigits"));
	cutsStack.GetXaxis()->SetTitle(x_axis_title.c_str());
	cutsStack.GetXaxis()->SetTitleSize(histOpts.getFloat("pad1axisTitleSize"));
	cutsStack.GetYaxis()->SetLabelSize(histOpts.getFloat("pad1axislabelsize"));
	cutsStack.GetXaxis()->SetLabelSize(histOpts.getFloat("pad1axislabelsize"));
	cutsStack.GetYaxis()->SetTitleSize(histOpts.getFloat("pad1axisTitleSize"));
	cutsStack.GetYaxis()->SetTitleOffset(histOpts.getFloat("pad1ytitleoffset"));
	cutsStack.GetXaxis()->SetTitleOffset(histOpts.getFloat("pad1xtitleoffset"));
	((TGaxis*)cutsStack.GetXaxis())->SetMaxDigits(histOpts.getInt("pad1xMaxDigits"));
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	gPad->RedrawAxis();
	gPad->RedrawAxis("g");
	gPad->Update();
	canvas.RedrawAxis();
	canvas.Update();
	canvas.Modified();

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	canvas.SaveAs( (histOpts.get("writeDir") + "/" + _histName + ".png").c_str());
	for(auto & toDelete: thingsToDelete){
		toDelete->Delete();
	}
	std::cout<<"\t\tComplete drawing TH1F "<<_histName<<std::endl;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void plot_1D(){
	CUTS.clear();
	gROOT->SetBatch();
	std::cout<<"Begin plotting program..."<<std::endl;
	histOpts.parseIt(histOptionsFile);

	UInt_t i = 0;
	while(histOpts.keyExists("data" + std::to_string(i) + "ntuple") || i < histOpts.getInt("maxDataCount")){
		i++;
		if(!histOpts.keyExists("data" + std::to_string(i) + "ntuple")) continue;
		CUTS.emplace_back(histOpts.get("data" + std::to_string(i) + "ntuple"), histOpts.get("data" + std::to_string(i) + "legend"), histOpts.getInt("data" + std::to_string(i) + "marker"), histOpts.get("data" + std::to_string(i) + "color"));
	}

	std::vector<std::string> histList = getObjectList(CUTS[0].ntuple, "TH1F");

	mkdir(histOpts.get("writeDir"));

	for(const auto & hist : histList){
		plotHist(hist);
	}

	std::cout<<"Complete!"<<std::endl;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
