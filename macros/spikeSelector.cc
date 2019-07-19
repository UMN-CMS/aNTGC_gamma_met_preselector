#include "TSystem.h"
#include "extra_tools.cc"

#ifndef SPIKE_SELECTOR_CC
#define SPIKE_SELECTOR_CC


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////                    CLASS DECLARATIONS                   //////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Barrel-Endcap transition region eta
#define BETRetaMin 1.4442
#define BETRetaMax 1.566
#define HBetaMax 1.3920 				// Josh H
#define ZMASS 91.1876
#define REPORT_EVERY 10000

Double_t ECAL_ETA_BINS[28] = {-2.5, -2.3, -2.1, -1.9, -1.7, -BETRetaMax, -BETRetaMin, -1.3, -1.1, -0.9, -0.7, -0.5, -0.3, -0.1,  0.1, 0.3, 0.5, 0.7, 0.9, 1.1, 1.3, BETRetaMin, BETRetaMax, 1.7, 1.9, 2.1, 2.3, 2.5};
Double_t ECAL_ABS_ETA_BINS[13] = { 0., 0.2, 0.4, 0.6, 0.8, 1.0, 1.2, BETRetaMin, BETRetaMax, 1.8, 2.0, 2.2, 2.5};

class spikeSelector {
public:
	spikeSelector(std::string _file_list, std::string _output_file);
private:
	Char_t initIntputNtuples();
	void setHists();
	void fillPho1Hists();
	void fillPhoHists();
	void fillGlobalHists();
	void resolveDuplicates();

	void analyze();
	Bool_t selectSpike();
	void registerCutFlow();

	/////////////////////////////////////////////////input TTree////////////////////////////////////////////////////////////
	TTreeReader inputTTreeReader;
	TChain *inputTree = nullptr;

	TTreeReaderAnyValue<Int_t> 				_run;
	TTreeReaderAnyValue<Long64_t> 			_event;
	TTreeReaderAnyValue<UShort_t> 			_lumis;
	TTreeReaderAnyValue<UChar_t>			_puTrue;
	TTreeReaderAnyValue<UChar_t>			_nVtx;
	TTreeReaderAnyValue<Bool_t> 			_isPVGood;
	TTreeReaderAnyValue<ULong64_t> 			_HLTPho;
	TTreeReaderAnyValue<UShort_t>			_beamHaloSummary;

	TTreeReaderAnyValue<UShort_t>			_nPho;
	TTreeReaderVectorValue<Float_t> 		_phoEta;
	TTreeReaderVectorValue<Float_t> 		_phoPhi;
	TTreeReaderVectorValue<Float_t> 		_phoCalibE;
	TTreeReaderVectorValue<Float_t> 		_phoSigmaCalibE;
	TTreeReaderVectorValue<Float_t> 		_phoCalibEt;
	TTreeReaderVectorValue<UChar_t> 		_phoQualityBits;
	TTreeReaderVectorValue<Float_t> 		_phoR9;
	TTreeReaderVectorValue<Float_t>		 	_phoR9Full5x5;
	TTreeReaderVectorValue<Float_t>		 	_phoSigmaIEtaIEtaFull5x5;
	TTreeReaderVectorValue<Float_t>		 	_phoSigmaIEtaIPhiFull5x5;
	TTreeReaderVectorValue<Float_t> 		_phoSigmaIPhiIPhiFull5x5;
	TTreeReaderVectorValue<Float_t> 		_phoIDMVA;
	TTreeReaderVectorValue<UChar_t> 		_phoIDbit;
	TTreeReaderVectorValue<Float_t>			_phoSeedTime;
	TTreeReaderVectorValue<Float_t>			_phoSeedEnergy;
	TTreeReaderVectorValue<Float_t>			_phoMIPChi2;
	TTreeReaderVectorValue<Float_t>			_phoMIPTotEnergy;
	TTreeReaderVectorValue<Float_t>			_phoMIPSlope;
	TTreeReaderVectorValue<Float_t>			_phoMIPIntercept;
	TTreeReaderVectorValue<Short_t>			_phoMIPNhitCone;
	TTreeReaderVectorValue<Short_t>			_phoSCindex;
	TTreeReaderVectorValue<UShort_t> 		_ecalSCindex;
	TTreeReaderVectorValue<Float_t> 		_ecalSCeta;
	TTreeReaderVectorValue<Float_t> 		_ecalSCphi;
	TTreeReaderVectorValue<Float_t> 		_ecalSCEn;
	TTreeReaderVectorValue<Float_t> 		_ecalSCRawEn;
	TTreeReaderVectorValue<Float_t> 		_ecalSCetaWidth;
	TTreeReaderVectorValue<Float_t> 		_ecalSCphiWidth;
	TTreeReaderVectorValue<Float_t> 		_ecalSC_LICTD;
	TTreeReaderVectorValue<Float_t> 		_ecalSC_maxEnXtalTime;
	TTreeReaderVectorValue<Float_t> 		_ecalSC_maxEnXtalSwissCross;
	TTreeReaderVectorValue<Float_t> 		_ecalSC_maxEnXtalBits;
	TTreeReaderVectorValue<UChar_t> 		_ecalSC_nL1Spike;
	TTreeReaderVectorValue<UChar_t> 		_ecalSC_nDiweird;
	TTreeReaderVectorValue<UChar_t> 		_ecalSC_nWeird;
	TTreeReaderVectorValue<UChar_t> 		_ecalSC_nSaturated;
	TTreeReaderVectorValue<UChar_t> 		_ecalSC_nOutOfTime;
	TTreeReaderVectorValue<UChar_t> 		_ecalSC_nXtals;

	TTreeReaderAnyValue<UShort_t>			_nootPho;
	TTreeReaderVectorValue<Float_t> 		_ootPhoEta;
	TTreeReaderVectorValue<Float_t> 		_ootPhoPhi;
	TTreeReaderVectorValue<Float_t> 		_ootPhoCalibE;
	TTreeReaderVectorValue<Float_t> 		_ootPhoSigmaCalibE;
	TTreeReaderVectorValue<Float_t> 		_ootPhoCalibEt;
	TTreeReaderVectorValue<UChar_t> 		_ootPhoQualityBits;
	TTreeReaderVectorValue<Float_t> 		_ootPhoR9;
	TTreeReaderVectorValue<Float_t>		 	_ootPhoR9Full5x5;
	TTreeReaderVectorValue<Float_t>		 	_ootPhoSigmaIEtaIEtaFull5x5;
	TTreeReaderVectorValue<Float_t>		 	_ootPhoSigmaIEtaIPhiFull5x5;
	TTreeReaderVectorValue<Float_t> 		_ootPhoSigmaIPhiIPhiFull5x5;
	TTreeReaderVectorValue<Float_t> 		_ootPhoIDMVA;
	TTreeReaderVectorValue<UChar_t> 		_ootPhoIDbit;
	TTreeReaderVectorValue<Float_t>			_ootPhoSeedTime;
	TTreeReaderVectorValue<Float_t>			_ootPhoSeedEnergy;
	TTreeReaderVectorValue<Float_t>			_ootPhoMIPChi2;
	TTreeReaderVectorValue<Float_t>			_ootPhoMIPTotEnergy;
	TTreeReaderVectorValue<Float_t>			_ootPhoMIPSlope;
	TTreeReaderVectorValue<Float_t>			_ootPhoMIPIntercept;
	TTreeReaderVectorValue<Short_t>			_ootPhoMIPNhitCone;
	TTreeReaderVectorValue<Short_t>			_ootPhoSCindex;

	TTreeReaderVectorValue<UShort_t> 		_ecalootSCindex;
	TTreeReaderVectorValue<Float_t> 		_ecalootSCeta;
	TTreeReaderVectorValue<Float_t> 		_ecalootSCphi;
	TTreeReaderVectorValue<Float_t> 		_ecalootSCEn;
	TTreeReaderVectorValue<Float_t> 		_ecalootSCRawEn;
	TTreeReaderVectorValue<Float_t> 		_ecalootSCetaWidth;
	TTreeReaderVectorValue<Float_t> 		_ecalootSCphiWidth;
	TTreeReaderVectorValue<Float_t> 		_ecalootSC_LICTD;
	TTreeReaderVectorValue<Float_t> 		_ecalootSC_maxEnXtalTime;
	TTreeReaderVectorValue<Float_t> 		_ecalootSC_maxEnXtalSwissCross;
	TTreeReaderVectorValue<Float_t> 		_ecalootSC_maxEnXtalBits;
	TTreeReaderVectorValue<UChar_t> 		_ecalootSC_nL1Spike;
	TTreeReaderVectorValue<UChar_t> 		_ecalootSC_nDiweird;
	TTreeReaderVectorValue<UChar_t> 		_ecalootSC_nWeird;
	TTreeReaderVectorValue<UChar_t> 		_ecalootSC_nSaturated;
	TTreeReaderVectorValue<UChar_t> 		_ecalootSC_nOutOfTime;
	TTreeReaderVectorValue<UChar_t> 		_ecalootSC_nXtals;

	TTreeReaderAnyValue<UShort_t> 			_metFilters;
	TTreeReaderAnyValue<Float_t> 			_pfMET;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



	//////////////////////////////////////////////////////////global histograms/////////////////////////////////////////////
	Float_t lastCutStep = 0.5;
	TH1F h_cutFlowEvts{"cutFlowEvts", "Cut Flow (# of events)", 20, 0., 20.};
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////// 									Histogram variables									////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Float_t 	met_;

	Short_t 	pho1index_;
	Float_t 	pho1Pt_;
	Float_t 	pho1Eta_;
	Float_t 	pho1Phi_;
	Float_t 	pho1SeedTime_;
	Float_t 	pho1maxXtalTime_;
	Float_t 	pho1LICTD_;
	Float_t 	pho1SwissCross_;
	Float_t 	pho1SigmaIEtaIEta_;
	Float_t 	pho1SigmaIPhiIPhi_;
	Float_t 	pho1MIP_;
	Float_t 	pho1R9Full5x5_;
	Float_t 	pho1HoverE_;

	std::vector<Short_t> phoIndices;
	Float_t 	phoPt_;
	Float_t 	phoEta_;
	Float_t 	phoPhi_;
	Float_t 	phoSeedTime_;
	Float_t 	phomaxXtalTime_;
	Float_t 	phoLICTD_;
	Float_t 	phoSwissCross_;
	Float_t 	phoSigmaIEtaIEta_;
	Float_t 	phoSigmaIPhiIPhi_;
	Float_t 	phoMIP_;
	Float_t 	phoR9Full5x5_;
	Float_t 	phoHoverE_;

	Short_t 	OOTpho1index_;
	Float_t 	OOTpho1Pt_;
	Float_t 	OOTpho1Eta_;
	Float_t 	OOTpho1Phi_;
	Float_t 	OOTpho1SeedTime_;
	Float_t 	OOTpho1maxXtalTime_;
	Float_t 	OOTpho1LICTD_;
	Float_t 	OOTpho1SwissCross_;
	Float_t 	OOTpho1SigmaIEtaIEta_;
	Float_t 	OOTpho1SigmaIPhiIPhi_;
	Float_t 	OOTpho1MIP_;
	Float_t 	OOTpho1R9Full5x5_;
	Float_t 	OOTpho1HoverE_;

	std::vector<Short_t> OOTphoIndices;
	Float_t 	OOTphoPt_;
	Float_t 	OOTphoEta_;
	Float_t 	OOTphoPhi_;
	Float_t 	OOTphoSeedTime_;
	Float_t 	OOTphomaxXtalTime_;
	Float_t 	OOTphoLICTD_;
	Float_t 	OOTphoSwissCross_;
	Float_t 	OOTphoSigmaIEtaIEta_;
	Float_t 	OOTphoSigmaIPhiIPhi_;
	Float_t 	OOTphoMIP_;
	Float_t 	OOTphoR9Full5x5_;
	Float_t 	OOTphoHoverE_;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	plot_variable 			var_met{met_, 0., 2000., 400, "#slash{E}_{T}", "GeV"},

							var_pho1Pt{pho1Pt_, 0., 2000., 400, "p_{T}(#gamma^{max})", "GeV"},
							var_pho1Eta{pho1Eta_, ECAL_ETA_BINS, 27, "#eta(#gamma^{max})"},
							var_pho1Phi{pho1Phi_, -3.15, 3.15, 63, "#phi(#gamma^{max})"},
							var_pho1SeedTime{pho1SeedTime_, -10., 10., 100, "#tau_{seed}(#gamma^{max})", "ns"},
							var_pho1maxXtalTime{pho1maxXtalTime_, -50., 50., 1000, "#tau_{max}(#gamma^{max})", "ns"},
							var_pho1LICTD{pho1LICTD_, 0., 100., 1000, "LICTD(#gamma^{max})", "ns"},
							var_phoSCmaxEnXtalSwissCross{pho1SwissCross_, -2., 2., 400, "1 - E_{4}/E_{max}(#gamma^{max})(#gamma^{max})"},
							var_pho1SigmaIEtaIEta{pho1SigmaIEtaIEta_, 0., 0.1, 1000, "#sigma_{i#eta i#eta}(#gamma^{max})"},
							var_pho1SigmaIPhiIPhi{pho1SigmaIPhiIPhi_, 0., 0.1, 1000, "#sigma_{i#phi i#phi}(#gamma^{max})"},
							var_pho1MIP{pho1MIP_, 0., 100., 200, "MIP(#gamma^{max})", "GeV"},
							var_pho1R9Full5x5{pho1R9Full5x5_, 0., 1.5, 150, "R_{9}(#gamma^{max})"},
							var_pho1HoverE{pho1HoverE_, 0., 2., 200., "H/E(#gamma^{max})"},


							var_phoPt{phoPt_, 0., 2000., 400, "p_{T}(#gamma)", "GeV"},
							var_phoEta{phoEta_, ECAL_ETA_BINS, 27, "#eta(#gamma)"},
							var_phoPhi{phoPhi_, -3.15, 3.15, 63, "#phi(#gamma)"},
							var_phoSeedTime{phoSeedTime_, -10., 10., 100, "#tau_{seed}(#gamma)", "ns"},
							var_phomaxXtalTime{phomaxXtalTime_, -50., 50., 1000, "#tau_{max}(#gamma)", "ns"},
							var_phoLICTD{phoLICTD_, 0., 100., 1000, "LICTD(#gamma)", "ns"},
							var_phoSCmaxEnXtalSwissCross{phoSwissCross_, -2., 2., 400, "1 - E_{4}/E_{max}(#gamma)(#gamma)"},
							var_phoSigmaIEtaIEta{phoSigmaIEtaIEta_, 0., 0.1, 1000, "#sigma_{i#eta i#eta}(#gamma)"},
							var_phoSigmaIPhiIPhi{phoSigmaIPhiIPhi_, 0., 0.1, 1000, "#sigma_{i#phi i#phi}(#gamma)"},
							var_phoMIP{phoMIP_, 0., 100., 200, "MIP(#gamma)", "GeV"},
							var_phoR9Full5x5{phoR9Full5x5_, 0., 1.5, 150, "R_{9}(#gamma)"},
							var_phoHoverE{phoHoverE_, 0., 2., 200., "H/E(#gamma)"},


							var_OOTpho1Pt{OOTpho1Pt_, 0., 2000., 400, "p_{T}(#gamma_{OOT}^{max})", "GeV"},
							var_OOTpho1Eta{OOTpho1Eta_, ECAL_ETA_BINS, 27, "#eta(#gamma_{OOT}^{max})"},
							var_OOTpho1Phi{OOTpho1Phi_, -3.15, 3.15, 63, "#phi(#gamma_{OOT}^{max})"},
							var_OOTpho1SeedTime{OOTpho1SeedTime_, -10., 10., 100, "#tau_{seed}(#gamma_{OOT}^{max})", "ns"},
							var_OOTpho1maxXtalTime{OOTpho1maxXtalTime_, -50., 50., 1000, "#tau_{max}(#gamma_{OOT}^{max})", "ns"},
							var_OOTpho1LICTD{OOTpho1LICTD_, 0., 100., 1000, "LICTD(#gamma_{OOT}^{max})", "ns"},
							var_OOTphoSCmaxEnXtalSwissCross{OOTpho1SwissCross_, -2., 2., 400, "1 - E_{4}/E_{max}(#gamma_{OOT}^{max})(#gamma_{OOT}^{max})"},
							var_OOTpho1SigmaIEtaIEta{OOTpho1SigmaIEtaIEta_, 0., 0.1, 1000, "#sigma_{i#eta i#eta}(#gamma_{OOT}^{max})"},
							var_OOTpho1SigmaIPhiIPhi{OOTpho1SigmaIPhiIPhi_, 0., 0.1, 1000, "#sigma_{i#phi i#phi}(#gamma_{OOT}^{max})"},
							var_OOTpho1MIP{OOTpho1MIP_, 0., 100., 200, "MIP(#gamma_{OOT}^{max})", "GeV"},
							var_OOTpho1R9Full5x5{OOTpho1R9Full5x5_, 0., 1.5, 150, "R_{9}(#gamma_{OOT}^{max})"},
							var_OOTpho1HoverE{OOTpho1HoverE_, 0., 2., 200., "H/E(#gamma_{OOT}^{max})"},


							var_OOTphoPt{OOTphoPt_, 0., 2000., 400, "p_{T}(#gamma_{OOT})", "GeV"},
							var_OOTphoEta{OOTphoEta_, ECAL_ETA_BINS, 27, "#eta(#gamma_{OOT})"},
							var_OOTphoPhi{OOTphoPhi_, -3.15, 3.15, 63, "#phi(#gamma_{OOT})"},
							var_OOTphoSeedTime{OOTphoSeedTime_, -10., 10., 100, "#tau_{seed}(#gamma_{OOT})", "ns"},
							var_OOTphomaxXtalTime{OOTphomaxXtalTime_, -50., 50., 1000, "#tau_{max}(#gamma_{OOT})", "ns"},
							var_OOTphoLICTD{OOTphoLICTD_, 0., 100., 1000, "LICTD(#gamma_{OOT})", "ns"},
							var_OOTphoSCmaxEnXtalSwissCross{OOTphoSwissCross_, -2., 2., 400, "1 - E_{4}/E_{max}(#gamma_{OOT})(#gamma_{OOT})"},
							var_OOTphoSigmaIEtaIEta{OOTphoSigmaIEtaIEta_, 0., 0.1, 1000, "#sigma_{i#eta i#eta}(#gamma_{OOT})"},
							var_OOTphoSigmaIPhiIPhi{OOTphoSigmaIPhiIPhi_, 0., 0.1, 1000, "#sigma_{i#phi i#phi}(#gamma_{OOT})"},
							var_OOTphoMIP{OOTphoMIP_, 0., 100., 200, "MIP(#gamma_{OOT})", "GeV"},
							var_OOTphoR9Full5x5{OOTphoR9Full5x5_, 0., 1.5, 150, "R_{9}(#gamma_{OOT})"},
							var_OOTphoHoverE{OOTphoHoverE_, 0., 2., 200., "H/E(#gamma_{OOT})"};
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<histogram_template> global1D = {
							{var_met}
	};

	std::vector<histogram_template> global2D = {
		
	};
							
	std::vector<histogram_template> inTime1DPho1 = {
							{var_pho1Pt},
							{var_pho1Eta},
							{var_pho1Phi},
							{var_pho1SeedTime},
							{var_pho1maxXtalTime},
							{var_pho1LICTD},
							{var_phoSCmaxEnXtalSwissCross},
							{var_pho1SigmaIEtaIEta},
							{var_pho1SigmaIPhiIPhi},
							{var_pho1MIP},
							{var_pho1R9Full5x5},
							{var_pho1HoverE},
	};

	std::vector<twoDhistogram_template> inTime2DPho1 ={
							{var_pho1SeedTime, var_pho1maxXtalTime},
							{var_pho1SeedTime, var_pho1SigmaIEtaIEta},
							{var_pho1SeedTime, var_pho1SigmaIPhiIPhi},
							{var_pho1SeedTime, var_pho1R9Full5x5},
							{var_pho1SeedTime, var_pho1HoverE},
							{var_pho1SeedTime, var_phoSCmaxEnXtalSwissCross},
							{var_pho1SeedTime, var_pho1LICTD},

							{var_pho1Pt, var_met},
							{var_pho1Pt, var_pho1SeedTime},
							{var_pho1Pt, var_pho1maxXtalTime},
							{var_pho1Pt, var_pho1LICTD},
							{var_pho1Pt, var_pho1MIP},
							{var_pho1Pt, var_pho1HoverE},
							{var_pho1Pt, var_pho1SigmaIEtaIEta},
							{var_pho1Pt, var_pho1SigmaIPhiIPhi},
							{var_pho1Pt, var_phoSCmaxEnXtalSwissCross},

							{var_pho1Eta, var_pho1SeedTime},
							{var_pho1Eta, var_pho1maxXtalTime},

							{var_pho1Phi, var_pho1SeedTime},
							{var_pho1Phi, var_pho1maxXtalTime},
	};

	std::vector<histogram_template> inTime1DPho = {
							{var_phoPt},
							{var_phoEta},
							{var_phoPhi},
							{var_phoSeedTime},
							{var_phomaxXtalTime},
							{var_phoLICTD},
							{var_phoSCmaxEnXtalSwissCross},
							{var_phoSigmaIEtaIEta},
							{var_phoSigmaIPhiIPhi},
							{var_phoMIP},
							{var_phoR9Full5x5},
							{var_phoHoverE},
	};

	std::vector<twoDhistogram_template> inTime2DPho ={
							{var_phoSeedTime, var_phomaxXtalTime},
							{var_phoSeedTime, var_phoSigmaIEtaIEta},
							{var_phoSeedTime, var_phoSigmaIPhiIPhi},
							{var_phoSeedTime, var_phoR9Full5x5},
							{var_phoSeedTime, var_phoHoverE},
							{var_phoSeedTime, var_phoSCmaxEnXtalSwissCross},
							{var_phoSeedTime, var_phoLICTD},

							{var_phoPt, var_met},
							{var_phoPt, var_phoSeedTime},
							{var_phoPt, var_phomaxXtalTime},
							{var_phoPt, var_phoLICTD},
							{var_phoPt, var_phoMIP},
							{var_phoPt, var_phoHoverE},
							{var_phoPt, var_phoSigmaIEtaIEta},
							{var_phoPt, var_phoSigmaIPhiIPhi},
							{var_phoPt, var_phoSCmaxEnXtalSwissCross},

							{var_phoEta, var_phoSeedTime},
							{var_phoEta, var_phomaxXtalTime},

							{var_phoPhi, var_phoSeedTime},
							{var_phoPhi, var_phomaxXtalTime},
	};

	std::vector<histogram_template> OOT1DPho1 = {
							{var_OOTpho1Pt},
							{var_OOTpho1Eta},
							{var_OOTpho1Phi},
							{var_OOTpho1SeedTime},
							{var_OOTpho1maxXtalTime},
							{var_OOTpho1LICTD},
							{var_OOTphoSCmaxEnXtalSwissCross},
							{var_OOTpho1SigmaIEtaIEta},
							{var_OOTpho1SigmaIPhiIPhi},
							{var_OOTpho1MIP},
							{var_OOTpho1R9Full5x5},
							{var_OOTpho1HoverE},
	};

	std::vector<twoDhistogram_template> OOT2DPho1 ={
							{var_OOTpho1SeedTime, var_OOTpho1maxXtalTime},
							{var_OOTpho1SeedTime, var_OOTpho1SigmaIEtaIEta},
							{var_OOTpho1SeedTime, var_OOTpho1SigmaIPhiIPhi},
							{var_OOTpho1SeedTime, var_OOTpho1R9Full5x5},
							{var_OOTpho1SeedTime, var_OOTpho1HoverE},
							{var_OOTpho1SeedTime, var_OOTphoSCmaxEnXtalSwissCross},
							{var_OOTpho1SeedTime, var_OOTpho1LICTD},

							{var_OOTpho1Pt, var_met},
							{var_OOTpho1Pt, var_OOTpho1SeedTime},
							{var_OOTpho1Pt, var_OOTpho1maxXtalTime},
							{var_OOTpho1Pt, var_OOTpho1LICTD},
							{var_OOTpho1Pt, var_OOTpho1MIP},
							{var_OOTpho1Pt, var_OOTpho1HoverE},
							{var_OOTpho1Pt, var_OOTpho1SigmaIEtaIEta},
							{var_OOTpho1Pt, var_OOTpho1SigmaIPhiIPhi},
							{var_OOTpho1Pt, var_OOTphoSCmaxEnXtalSwissCross},

							{var_OOTpho1Eta, var_OOTpho1SeedTime},
							{var_OOTpho1Eta, var_OOTpho1maxXtalTime},

							{var_OOTpho1Phi, var_OOTpho1SeedTime},
							{var_OOTpho1Phi, var_OOTpho1maxXtalTime},
	};

	std::vector<histogram_template> OOT1DPho = {
							{var_OOTphoPt},
							{var_OOTphoEta},
							{var_OOTphoPhi},
							{var_OOTphoSeedTime},
							{var_OOTphomaxXtalTime},
							{var_OOTphoLICTD},
							{var_OOTphoSCmaxEnXtalSwissCross},
							{var_OOTphoSigmaIEtaIEta},
							{var_OOTphoSigmaIPhiIPhi},
							{var_OOTphoMIP},
							{var_OOTphoR9Full5x5},
							{var_OOTphoHoverE},
	};

	std::vector<twoDhistogram_template> OOT2DPho ={
							{var_OOTphoSeedTime, var_OOTphomaxXtalTime},
							{var_OOTphoSeedTime, var_OOTphoSigmaIEtaIEta},
							{var_OOTphoSeedTime, var_OOTphoSigmaIPhiIPhi},
							{var_OOTphoSeedTime, var_OOTphoR9Full5x5},
							{var_OOTphoSeedTime, var_OOTphoHoverE},
							{var_OOTphoSeedTime, var_OOTphoSCmaxEnXtalSwissCross},
							{var_OOTphoSeedTime, var_OOTphoLICTD},

							{var_OOTphoPt, var_met},
							{var_OOTphoPt, var_OOTphoSeedTime},
							{var_OOTphoPt, var_OOTphomaxXtalTime},
							{var_OOTphoPt, var_OOTphoLICTD},
							{var_OOTphoPt, var_OOTphoMIP},
							{var_OOTphoPt, var_OOTphoHoverE},
							{var_OOTphoPt, var_OOTphoSigmaIEtaIEta},
							{var_OOTphoPt, var_OOTphoSigmaIPhiIPhi},
							{var_OOTphoPt, var_OOTphoSCmaxEnXtalSwissCross},

							{var_OOTphoEta, var_OOTphoSeedTime},
							{var_OOTphoEta, var_OOTphomaxXtalTime},

							{var_OOTphoPhi, var_OOTphoSeedTime},
							{var_OOTphoPhi, var_OOTphomaxXtalTime},
	};

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
spikeSelector::spikeSelector(std::string _file_list, std::string _output_file){
	std::cout<<getCurrentTime()<<std::endl<<
	"File list path: "<<_file_list<<std::endl<<
	"Output file: "<<_output_file<<std::endl<<;

	if(file_exists(_file_list)) inputTree =openTChain(_file_list, "ggNtuplizer/EventTree");
	else {
		std::cout<<"Error! Cannot read given input path/list ("<<_file_list<<")"<<std::endl;
		return;
	}

	if(!inputTree){
		std::cout<<"Error! Could not create input TChain."<<std::endl;
		return;
	}

	outFile = new TFile(_output_file.c_str(), "RECREATE");
	if(!outFile){
		std::cout<<"Error! Could not create output TFile."<<std::endl;
		return;
	}

	inputTTreeReader.SetTree(inputTree);

	initIntputNtuples();

	setGlobalHists();

	setGammaMEThists();

	analyze();

	outFile->Write();

	outFile->Close();

	closeTChain(inputTree);

	std::cout<<"\tOutput written to file\t"<<_output_file <<std::endl<<
	"Complete!"<<std::endl<<
	getCurrentTime()<<std::endl;
};


Char_t spikeSelector::initIntputNtuples(){
	std::cout<<"*******************************************************************************"<<std::endl<<
	"Initializing branches in input ntuples..."<<std::endl;

	_run.set(inputTTreeReader, "run");
	_event.set(inputTTreeReader, "event");
	_lumis.set(inputTTreeReader, "lumis");
	_isPVGood.set(inputTTreeReader, "isPVGood");
	_HLTPho.set(inputTTreeReader, "HLTPho");
	_beamHaloSummary.set(inputTTreeReader, "beamHaloSummary");
	_nVtx.set(inputTTreeReader, "nVtx");

	_nPho.set(inputTTreeReader, "nPho");
	_phoEta.set(inputTTreeReader, "phoEta");
	_phoPhi.set(inputTTreeReader, "phoPhi");
	_phoCalibE.set(inputTTreeReader, "phoCalibE");
	_phoSigmaCalibE.set(inputTTreeReader, "phoSigmaCalibE");
	_phoCalibEt.set(inputTTreeReader, "phoCalibEt");
	_phoQualityBits.set(inputTTreeReader, "phoQualityBits");
	_phoR9.set(inputTTreeReader, "phoR9");
	_phoR9Full5x5.set(inputTTreeReader, "phoR9Full5x5");
	_phoSigmaIEtaIEtaFull5x5.set(inputTTreeReader, "phoSigmaIEtaIEtaFull5x5");
	_phoSigmaIEtaIPhiFull5x5.set(inputTTreeReader, "phoSigmaIEtaIPhiFull5x5");
	_phoSigmaIPhiIPhiFull5x5.set(inputTTreeReader, "phoSigmaIPhiIPhiFull5x5");
	_phoIDMVA.set(inputTTreeReader, "phoIDMVA");
	_phoIDbit.set(inputTTreeReader, "phoIDbit");
	_phoSeedTime.set(inputTTreeReader, "phoSeedTime");
	_phoSeedEnergy.set(inputTTreeReader, "phoSeedEnergy");
	_phoMIPChi2.set(inputTTreeReader, "phoMIPChi2");
	_phoMIPTotEnergy.set(inputTTreeReader, "phoMIPTotEnergy");
	_phoMIPSlope.set(inputTTreeReader, "phoMIPSlope");
	_phoMIPIntercept.set(inputTTreeReader, "phoMIPIntercept");
	_phoMIPNhitCone.set(inputTTreeReader, "phoMIPNhitCone");
	_phoSCindex.set(inputTTreeReader, "phoSCindex");

	_ecalSCindex.set(inputTTreeReader, "ecalSCindex");
	_ecalSCeta.set(inputTTreeReader, "ecalSCeta");
	_ecalSCphi.set(inputTTreeReader, "ecalSCphi");
	_ecalSCEn.set(inputTTreeReader, "ecalSCEn");
	_ecalSCRawEn.set(inputTTreeReader, "ecalSCRawEn");
	_ecalSCetaWidth.set(inputTTreeReader, "ecalSCetaWidth");
	_ecalSCphiWidth.set(inputTTreeReader, "ecalSCphiWidth");
	_ecalSC_LICTD.set(inputTTreeReader, "ecalSC_LICTD");
	_ecalSC_maxEnXtalTime.set(inputTTreeReader, "ecalSC_maxEnXtalTime");
	_ecalSC_maxEnXtalSwissCross.set(inputTTreeReader, "ecalSC_maxEnXtalSwissCross");
	_ecalSC_maxEnXtalBits.set(inputTTreeReader, "ecalSC_maxEnXtalSwissCross");
	_ecalSC_nL1Spike.set(inputTTreeReader, "ecalSC_nL1Spike");
	_ecalSC_nDiweird.set(inputTTreeReader, "ecalSC_nDiweird");
	_ecalSC_nWeird.set(inputTTreeReader, "ecalSC_nWeird");
	_ecalSC_nSaturated.set(inputTTreeReader, "ecalSC_nSaturated");
	_ecalSC_nOutOfTime.set(inputTTreeReader, "ecalSC_nOutOfTime");
	_ecalSC_nXtals.set(inputTTreeReader, "ecalSC_nXtals");

	_nootPho.set(inputTTreeReader, "nootPho");
	_ootPhoEta.set(inputTTreeReader, "ootPhoEta");
	_ootPhoPhi.set(inputTTreeReader, "ootPhoPhi");
	_ootPhoCalibE.set(inputTTreeReader, "ootPhoCalibE");
	_ootPhoSigmaCalibE.set(inputTTreeReader, "ootPhoSigmaCalibE");
	_ootPhoCalibEt.set(inputTTreeReader, "ootPhoCalibEt");
	_ootPhoQualityBits.set(inputTTreeReader, "ootPhoQualityBits");
	_ootPhoR9.set(inputTTreeReader, "ootPhoR9");
	_ootPhoR9Full5x5.set(inputTTreeReader, "ootPhoR9Full5x5");
	_ootPhoSigmaIEtaIEtaFull5x5.set(inputTTreeReader, "ootPhoSigmaIEtaIEtaFull5x5");
	_ootPhoSigmaIEtaIPhiFull5x5.set(inputTTreeReader, "ootPhoSigmaIEtaIPhiFull5x5");
	_ootPhoSigmaIPhiIPhiFull5x5.set(inputTTreeReader, "ootPhoSigmaIPhiIPhiFull5x5");
	_ootPhoIDMVA.set(inputTTreeReader, "ootPhoIDMVA");
	_ootPhoIDbit.set(inputTTreeReader, "ootPhoIDbit");
	_ootPhoSeedTime.set(inputTTreeReader, "ootPhoSeedTime");
	_ootPhoSeedEnergy.set(inputTTreeReader, "ootPhoSeedEnergy");
	_ootPhoMIPChi2.set(inputTTreeReader, "ootPhoMIPChi2");
	_ootPhoMIPTotEnergy.set(inputTTreeReader, "ootPhoMIPTotEnergy");
	_ootPhoMIPSlope.set(inputTTreeReader, "ootPhoMIPSlope");
	_ootPhoMIPIntercept.set(inputTTreeReader, "ootPhoMIPIntercept");
	_ootPhoMIPNhitCone.set(inputTTreeReader, "ootPhoMIPNhitCone");
	_ootPhoSCindex.set(inputTTreeReader, "ootPhoSCindex");

	_ecalootSCindex.set(inputTTreeReader, "ecalootSCindex");
	_ecalootSCeta.set(inputTTreeReader, "ecalootSCeta");
	_ecalootSCphi.set(inputTTreeReader, "ecalootSCphi");
	_ecalootSCEn.set(inputTTreeReader, "ecalootSCEn");
	_ecalootSCRawEn.set(inputTTreeReader, "ecalootSCRawEn");
	_ecalootSCetaWidth.set(inputTTreeReader, "ecalootSCetaWidth");
	_ecalootSCphiWidth.set(inputTTreeReader, "ecalootSCphiWidth");
	_ecalootSC_LICTD.set(inputTTreeReader, "ecalootSC_LICTD");
	_ecalootSC_maxEnXtalTime.set(inputTTreeReader, "ecalootSC_maxEnXtalTime");
	_ecalootSC_maxEnXtalSwissCross.set(inputTTreeReader, "ecalootSC_maxEnXtalSwissCross");
	_ecalootSC_maxEnXtalBits.set(inputTTreeReader, "ecalootSC_maxEnXtalSwissCross");
	_ecalootSC_nL1Spike.set(inputTTreeReader, "ecalootSC_nL1Spike");
	_ecalootSC_nDiweird.set(inputTTreeReader, "ecalootSC_nDiweird");
	_ecalootSC_nWeird.set(inputTTreeReader, "ecalootSC_nWeird");
	_ecalootSC_nSaturated.set(inputTTreeReader, "ecalootSC_nSaturated");
	_ecalootSC_nOutOfTime.set(inputTTreeReader, "ecalootSC_nOutOfTime");
	_ecalootSC_nXtals.set(inputTTreeReader, "ecalootSC_nXtals");

	_metFilters.set(inputTTreeReader, "metFilters");
	_pfMET.set(inputTTreeReader, "pfMET");

	std::cout<<"\tBranches initialized!"<<std::endl<<
	"*******************************************************************************"<<std::endl;
	return 1;
};


void spikeSelector::setHists(){
	std::cout<<"\t\tInitializing TH1D for spike selector..."<<std::endl;

	for(auto & hist1d : inTime1DPho1){
		hist1d.initializehist();
		hist1d.hist->SetDirectory(outFile->GetDirectory(""));
	}

	for(auto & hist1d : OOT1DPho1){
		hist1d.initializehist();
		hist1d.hist->SetDirectory(outFile->GetDirectory(""));
	}

	for(auto & hist1d : inTime1DPho){
		hist1d.initializehist();
		hist1d.hist->SetDirectory(outFile->GetDirectory(""));
	}

	for(auto & hist1d : OOT1DPho){
		hist1d.initializehist();
		hist1d.hist->SetDirectory(outFile->GetDirectory(""));
	}

	std::cout<<"\t\tInitializing TH2D for spike selector..."<<std::endl;

	for(auto & hist2d : inTime2DPho1){
		hist2d.initializehist();
		hist2d.hist->SetDirectory(outFile->GetDirectory(""));
	}

	for(auto & hist2d : OOT2DPho1){
		hist2d.initializehist();
		hist2d.hist->SetDirectory(outFile->GetDirectory(""));
	}

	for(auto & hist2d : inTime2DPho){
		hist2d.initializehist();
		hist2d.hist->SetDirectory(outFile->GetDirectory(""));
	}

	for(auto & hist2d : OOT2DPho){
		hist2d.initializehist();
		hist2d.hist->SetDirectory(outFile->GetDirectory(""));
	}

	std::cout<<"\tSpike selector histograms initialized!"<<std::endl;
};


void spikeSelector::fillPho1Hists(){
	for(auto & hist :  inTime1DPho1){
		hist.fill();
	}

	for(auto & hist2D : inTime2DPho1){
		hist2D.fill();
	}

	for(auto & hist :  OOT1DPho1){
		hist.fill();
	}

	for(auto & hist2D : OOT2DPho1){
		hist2D.fill();
	}
};


void spikeSelector::fillPhoHists(){
	for(auto & hist :  inTime1DPho){
		hist.fill();
	}

	for(auto & hist2D : inTime2DPho){
		hist2D.fill();
	}

	for(auto & hist :  OOT1DPho){
		hist.fill();
	}

	for(auto & hist2D : OOT2DPho){
		hist2D.fill();
	}
};


Bool_t spikeSelector::selectSpike(){

	lastCutStep = 0.5;
	registerCutFlow();

	if(!_isPVGood) return 0;
	registerCutFlow();

	if((_nPho) < 1) return 0;
	registerCutFlow();

	pho1index_ = -99;
	Float_t highestPtInTimePhotonPt = -999.;
	phoIndices.clear();
	for(Int_t i = 0; i < (_nPho); i++){
		Float_t candPhoPt = _phoCalibEt[i];
		if(candPhoPt < 30.) continue;
		Float_t candPhoAbsEta = std::abs(_phoEta[i]);
		if(candPhoAbsEta > 2.5 ) continue;
		if(candPhoAbsEta > BETRetaMin ) continue;	// EB only
		UChar_t tmpphoQualityBits = _phoQualityBits[i];
		if(!getBit(tmpphoQualityBits,1)) continue;		//electron veto
		if(candPhoPt > highestPtInTimePhotonPt){
			highestPtInTimePhotonPt = candPhoPt;
			pho1index_ = i;
		}
		phoIndices.push_back(i);
	}


	OOTpho1index_ = -99;
	Float_t highestPtOOTPhotonPt = -999.;
	OOTphoIndices.clear();
	for(Int_t i = 0; i < (_nootPho); i++){
		Float_t candOOTPhoPt = _ootphoCalibEt[i];
		if(candOOTPhoPt < 30.) continue;
		Float_t candOOTPhoAbsEta = std::abs(_ootphoEta[i]);
		if(candOOTPhoAbsEta > 2.5 ) continue;
		if(candOOTPhoAbsEta > BETRetaMin ) continue;	// EB only
		UChar_t tmpOOTphoQualityBits = _ootphoQualityBits[i];
		if(!getBit(tmpOOTphoQualityBits,1)) continue;		//electron veto
		if(candOOTPhoPt > highestPtOOTPhotonPt){
			highestPtOOTPhotonPt = candOOTPhoPt;
			OOTpho1index_ = i;
		}
		OOTphoIndices.push_back(i);
	}


	if((pho1index_ < 0) && (OOTpho1index_ < 0)){
		return 0;
	};

	return 1;
};


void spikeSelector::analyze(){
	std::cout<<"************************************************************************************************************"<<std::endl<<
	"Analyzing events.."<<std::endl<<
	"************************************************************************************************************"<<std::endl;
	while (inputTTreeReader.Next()) {
		ULong64_t current_entry = inputTTreeReader.GetCurrentEntry();
		if(current_entry % REPORT_EVERY == 0){
			std::cout<<"\t"<< getCurrentTime()<<"\tAnalyzing entry\t"<<current_entry<<",\tevent "<<  (_event) <<std::endl;
		}
		if(!selectGammaMETevent()) continue;
	}
};


void spikeSelector::registerCutFlow(){
	h_cutFlowEvts.Fill(lastCutStep);
	lastCutStep = lastCutStep + 1.;
};

#endif
