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



// Barrel-Endcap transition region eta
#define BETRetaMin 1.4442
#define BETRetaMax 1.566

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string histOptionsFile="../data/signal_eta_vs_pt_options.txt";

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

sample SM;
std::vector<sample> BACKGROUNDS;
parseOptions histOpts;
void plotHist(std::string _histName);
void plot_1D();

std::vector<std::string> ptBins = {"200500", "5001200"};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void plotHist(std::string _histName){
	std::vector<TObject*> thingsToDelete;

	// gStyle->SetOptTitle(0);
	// gStyle->SetOptStat(0);

	// std::cout<<"Plotting "<<_histName<<std::endl;

	// TCanvas canvas((_histName + "_canvas").c_str(), "", 2400, 1800);
	// canvas.Draw();

	// TPad pad0("pad0", "", histOpts.getFloat("pad0x1"), histOpts.getFloat("pad0y1"), histOpts.getFloat("pad0x2"), histOpts.getFloat("pad0y2"));
	// pad0.SetMargin(0., 0., 0., 0.);
	// pad0.SetFillStyle(0);
	// pad0.SetFillColor(0);
	// pad0.SetFrameFillStyle(0);
	// canvas.cd();
	// pad0.Draw();

	// TLegend legend(0.10, 0.0, 0.99, 1.0);
	// legend.SetTextSize(histOpts.getFloat("legendTextSize"));
	// legend.SetNColumns(histOpts.getInt("legendNcols"));
	// legend.SetFillStyle(1001);
	// legend.SetBorderSize(0);
	// pad0.cd();
	// legend.Draw();

	// /////////////////////////////////////////////////Draw data, bg & signal///////////////////////////////////////////////////
	// TPad pad1("pad1", "", histOpts.getFloat("pad1x1"), histOpts.getFloat("pad1y1"), histOpts.getFloat("pad1x2"), histOpts.getFloat("pad1y2"));
	// pad1.SetMargin(histOpts.getFloat("pad1marginL"), histOpts.getFloat("pad1marginR"), histOpts.getFloat("pad1marginB"), histOpts.getFloat("pad1marginT"));
	// pad1.SetFillStyle(0);
	// pad1.SetFillColor(0);
	// pad1.SetFrameFillStyle(0);
	// canvas.cd();
	// pad1.Draw();
	// pad1.SetGrid(1,1);
	// // pad1.SetLogy();

	// std::vector<Double_t> newBins = { 0., BETRetaMin, BETRetaMax, 2.5};

	// std::string x_axis_title, y_axis_title;

	// THStack bgHistStack((_histName + "_bg_stack").c_str(), "");
	// THStack fractionStack((_histName + "_fraction_stack").c_str(), "");

	TH2F* testHist = (TH2F*) getHistFromFile(_histName, BACKGROUNDS[0].ntuple+ "200500.root");
	Int_t nBinsY = testHist->GetNbinsY();
	Int_t nBinsX = testHist->GetNbinsX();
	Float_t yMin = testHist->GetYaxis()->GetBinLowEdge(1);
	Float_t yMax = testHist->GetYaxis()->GetBinUpEdge(nBinsY);
	Int_t xBETRetaMin_Binnve = testHist->GetXaxis()->FindBin(-BETRetaMin);
	Int_t xBETRetaMin_Binpsive = testHist->GetXaxis()->FindBin(BETRetaMin);

	cout<<"betr"<<testHist->GetXaxis()->GetBinCenter(xBETRetaMin_Binnve)<<"\t\t"<<testHist->GetXaxis()->GetBinCenter(xBETRetaMin_Binnve)<<std::endl;

	// cout<<nBinsY<<"\t\t"<<nBinsX<<"\t\t"<<yMin<<"\t\t"<<yMax<<"\t\t"<<xBETRetaMin_Bin<<std::endl;

	testHist->Delete();


	for(Int_t i =0; i< BACKGROUNDS.size(); i ++){
		
		TH2F *mergedSample = nullptr;
		
		for(Int_t j = 0; j < ptBins.size(); j++){
			
			std::string _histFile = BACKGROUNDS[i].ntuple + ptBins[j] + ".root";	

			TH2F *bgHist = (TH2F*) getHistFromFile(_histName, _histFile, 1);
			
			bgHist->SetName((getFileName(_histFile) + _histName).c_str());
			
			TH1F *sumGenWeights = (TH1F*) getHistFromFile("sumGenWeights", _histFile);
			
			bgHist->Scale(1./sumGenWeights->GetBinContent(1));
			
			delete sumGenWeights;
			
			sumGenWeights = nullptr;
			
			if(j==0){
				mergedSample = bgHist;	
			} else{
				mergedSample->Add(bgHist);
				delete bgHist;
			}
			
		}
		
		std::string _nameForHist = findAndReplaceAll(getFileName(BACKGROUNDS[i].ntuple), ".root", "") + "_eta_vs_pt";
		
		TH1F *fracHist = new TH1F(_nameForHist.c_str(), "", nBinsY, yMin, yMax);
		
		for(Int_t nYbin = 1; nYbin < nBinsY+1; nYbin++){
			TH1D * ptBinHist = (TH1D *) mergedSample->ProjectionX((_nameForHist + "_bin_" + to_string(nYbin)).c_str(), nYbin, nBinsY, "e");
			Float_t binIntegral = ptBinHist->Integral(1, xBETRetaMin_Binnve) + ptBinHist->Integral(xBETRetaMin_Binpsive, nBinsX);
			if(!(binIntegral>0.))continue;
			if(!(ptBinHist->Integral()>0.))continue;
			Float_t ee_fraction = binIntegral / ptBinHist->Integral();
			cout<<"\t\t\t"<<nYbin<<"\t"<<ptBinHist->GetYaxis()->GetBinLowEdge(nYbin)<<"\t"<<ee_fraction<<"\t"<<ptBinHist->Integral()<<endl;
			ptBinHist->Delete();
			fracHist->SetBinContent(nYbin, ee_fraction);
		}

		fracHist->GetXaxis()->SetTitle("p_{T}(#gamma) [GeV]");
		fracHist->GetYaxis()->SetTitle("Cumulative fraction events in #gamma in EE [GeV]");

		
		writeToFile(fracHist, histOpts.get("writeDir")+getFileName(BACKGROUNDS[i].ntuple)+_nameForHist+".root", "RECREATE");
		
		fracHist->Delete();
		
		mergedSample->Delete();
		

		// bgHistStack.Add(fracHist, "HIST ");
		// legend.AddEntry(fracHist, BACKGROUNDS[i].legend.c_str(), "FLE");
		// TH1F *mergedSampleFraction = (TH1F*) mergedSample->Clone((BACKGROUNDS[i].ntuple + "_merged").c_str());
		// mergedSampleFraction->Scale(1./mergedSampleFraction->Integral(0,mergedSampleFraction->GetNbinsX()));
		// fractionStack.Add(mergedSampleFraction, "HIST ");
	}


	// std::string yUnit = getUnit(y_axis_title);
	// y_axis_title = eraseUnit(y_axis_title);
	// y_axis_title = y_axis_title + ((yUnit.empty()) ? "" : ("/"+yUnit));
	// y_axis_title = findAndReplaceAll(y_axis_title, "\\", "#");

	// std::vector<Float_t> xLimits = getXlimits({(TH1F*) bgHistStack.GetStack()->Last()}, histOpts.getFloat("binThreshold"));

	// pad1.cd();
	// bgHistStack.Draw("NOSTACK HIST");
	// bgHistStack.GetYaxis()->SetRangeUser(histOpts.getFloat("stackMin"), histOpts.getFloat("maxBinRatio") * ((TH1F*)bgHistStack.GetStack()->Last())->GetMaximum());
	// bgHistStack.GetXaxis()->SetRangeUser(xLimits[1], xLimits[0]);
	// bgHistStack.GetYaxis()->SetTitle(y_axis_title.c_str());
	// bgHistStack.GetXaxis()->SetRangeUser(xLimits[1], xLimits[0]);
	// bgHistStack.GetXaxis()->SetLabelSize(0);
	// bgHistStack.GetYaxis()->SetLabelSize(histOpts.getFloat("pad1axislabelsize"));
	// bgHistStack.GetYaxis()->SetTitleSize(histOpts.getFloat("pad1axisTitleSize"));
	// bgHistStack.GetYaxis()->SetTitleOffset(histOpts.getFloat("pad1ytitleoffset"));
	// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	// //////////////////////////////////////////////////////////Ratio plot//////////////////////////////////////////////////////
	// TPad pad2("pad2", "", histOpts.getFloat("pad2x1"), histOpts.getFloat("pad2y1"), histOpts.getFloat("pad2x2"), histOpts.getFloat("pad2y2"));
	// // thingsToDelete.push_back(pad2);
	// pad2.SetMargin(histOpts.getFloat("pad2marginL"), histOpts.getFloat("pad2marginR"), histOpts.getFloat("pad2marginB"), histOpts.getFloat("pad2marginT"));
	// pad2.SetFillStyle(0);
	// pad2.SetFillColor(0);
	// pad2.SetFrameFillStyle(0);
	// canvas.cd();
	// pad2.Draw();
	// pad2.SetGrid(1,1);
	// pad2.SetLogy();
	// pad2.cd();

	// // fractionStack.Draw("NOSTACK HIST");
	// // fractionStack.GetXaxis()->SetTitle(x_axis_title.c_str());
	// // fractionStack.GetXaxis()->SetTitleSize(histOpts.getFloat("pad2axisTitleSize"));
	// // fractionStack.GetXaxis()->SetTitleOffset(histOpts.getFloat("pad2xtitleoffset"));
	// // fractionStack.GetXaxis()->SetLabelSize(histOpts.getFloat("pad2axislabelsize"));
	// // fractionStack.GetYaxis()->SetTitle("Fraction");

	// // fractionStack.GetYaxis()->SetNdivisions(3,6,0);
	// // fractionStack.GetYaxis()->SetMoreLogLabels();
	// // fractionStack.GetYaxis()->SetNoExponent();
	// // fractionStack.GetYaxis()->SetTitleSize(histOpts.getFloat("pad2axisTitleSize"));
	// // fractionStack.GetYaxis()->SetTitleOffset(histOpts.getFloat("pad2ytitleoffset"));
	// // fractionStack.GetYaxis()->SetLabelSize(histOpts.getFloat("pad2axislabelsize"));
	// // fractionStack.GetXaxis()->SetRangeUser(xLimits[1], xLimits[0]);

	// // fractionStack.SetMinimum(0.32);
	// // fractionStack.SetMaximum(0.67);



	// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// gPad->RedrawAxis();
	// gPad->RedrawAxis("g");
	// gPad->Update();

	// //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// canvas.RedrawAxis();
	// canvas.Update();
	// canvas.Modified();

	// std::cout<<"\t\tComplete drawing TH1F "<<_histName<<std::endl;

	// std::string writeDir = histOpts.get("writeDir");
	// canvas.SaveAs( (writeDir + "/" + _histName + "_largeEta.png").c_str());
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void plot_1D_eta_signal(){
	gROOT->SetBatch();

	std::cout<<"Begin plotting program..."<<std::endl;

	histOpts.parseIt(histOptionsFile);

	std::string inDir = histOpts.get("inDir") + "/";


	BACKGROUNDS.emplace_back(inDir+"aNTGC0p0p0p0p", "h_{3}^{Z}=0.", 29, "#252525");
	BACKGROUNDS.emplace_back(inDir+"aNTGC0p00030p0p0p", "h_{3}^{Z}=3#times10^{-4}", 29, "#1b7837");
	BACKGROUNDS.emplace_back(inDir+"aNTGC0p00100p0p0p", "h_{3}^{Z}=1#times10^{-3}", 29, "#b2182b");
	BACKGROUNDS.emplace_back(inDir+"aNTGC0p00100p0000050p0p", "h_{3}^{Z}=1#times10^{-3},h_{4}^{Z}=5#times10^{-6}", 29, "#2166ac");
	BACKGROUNDS.emplace_back(inDir+"aNTGC0p00050p00000080p0p", "h_{3}^{Z}=5#times10^{-4},h_{4}^{Z}=8#times10^{-7}", 29, "#ff7f00");
	BACKGROUNDS.emplace_back(inDir+"aNTGC0p0p0000050p0p", "h_{4}^{Z}=5#times10^{-6}", 29, "#762a83");
	BACKGROUNDS.emplace_back(inDir+"aNTGC0p0p00000080p0p", "h_{4}^{Z}=8#times10^{-7}", 29, "#8c510a");

	std::vector<std::string> histList = {"_1D_void8etagammavoid8"};

	std::string writeDir = histOpts.get("writeDir");
	mkdir(writeDir);

	plotHist("_2D_etagammaVSpTgamma");

	std::cout<<"Complete!"<<std::endl;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
