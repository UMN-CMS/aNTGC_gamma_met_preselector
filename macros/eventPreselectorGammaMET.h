#ifndef ANTGC_PRESELECTOR_H
#define ANTGC_PRESELECTOR_H

#include "extra_tools.cc"

// Barrel-Endcap transition region eta
#define BETRetaMin 1.4442
#define BETRetaMax 1.566
#define HBetaMax 1.3920 				// Josh H
/***************************************/

#define ZMASS 91.1876

#define REPORT_EVERY 1000

Double_t ECAL_ETA_BINS[28] = {-2.5, -2.3, -2.1, -1.9, -1.7, -BETRetaMax, -BETRetaMin, -1.3, -1.1, -0.9, -0.7, -0.5, -0.3, -0.1,  0.1, 0.3, 0.5, 0.7, 0.9, 1.1, 1.3, BETRetaMin, BETRetaMax, 1.7, 1.9, 2.1, 2.3, 2.5};

class aNTGCpreselector {
public:
	aNTGCpreselector(std::string _file_list, std::string _output_file, std::string _mcPUfile, std::string _dataPUfile);
private:
	Char_t initIntputNtuples();
	Char_t isData(std::string _infilespath);
	void initPileupReweighter(std::string _mcPUfile, std::string _dataPUfile);
	void setGlobalHists();
	void setGammaMEThists();
	void setOutputTree();
	void copyEvent();
	void fillGlobalHists();
	void fillGammaMEThists();
	void writeOutputFile();

	void analyze();
	Bool_t selectGammaMETevent();

	void registerCutFlow();

	PileupReWeighting puReWeighter;

	Float_t lastCutStep = 0.5;
	TH1F h_cutFlowEvts{"cutFlowEvts", "Cut Flow (# of events)", 20, 0., 20.};
	TH1F h_cutFlowWeighted{"cutFlow", "Cut Flow", 20, 0., 20.};
	TH1F h_cutFlowWeightSigns{"cutFlowWgtSigns", "Cut Flow (weight signs)", 20, 0., 20.};

	Bool_t isQCD;
	Bool_t isGJets;
	Bool_t isMC;
	Bool_t is2016MCsample;

	/***********************************************input TTree *******************************************/

	TTreeReader inputTTreeReader;
	TChain *inputTree = nullptr;

	TTreeReaderAnyValue<Int_t> 				_run;
	TTreeReaderAnyValue<Long64_t> 			_event;
	TTreeReaderAnyValue<Int_t> 				_lumis;

	TTreeReaderAnyValue<Int_t>				_puTrue;

	TTreeReaderAnyValue<Bool_t> 			_isPVGood;

	TTreeReaderAnyValue<ULong64_t> 			_HLTPho;

	TTreeReaderAnyValue<Float_t>			_genWeight;
	TTreeReaderAnyValue<Char_t> 			_mcHasDirectPromptPho;
	TTreeReaderVectorValue<Short_t> 			_mcStatus;
	TTreeReaderVectorValue<Int_t> 			_mcPID;
	TTreeReaderVectorValue<Float_t> 		_mcEta;
	TTreeReaderVectorValue<Float_t> 		_mcPhi;
	TTreeReaderVectorValue<Float_t> 		_mcPt;
	TTreeReaderVectorValue<Float_t> 		_mcE;
	TTreeReaderVectorValue<Short_t>			_mcIndex;
	TTreeReaderVectorValue<UShort_t> 		_mcStatusFlag;
	TTreeReaderAnyValue<Float_t> 			_genMET;
	TTreeReaderAnyValue<Float_t> 			_genMETPhi;

	TTreeReaderAnyValue<UShort_t>			_nPho;
	TTreeReaderVectorValue<Float_t> 		_phoEta;
	TTreeReaderVectorValue<Float_t> 		_phoPhi;
	TTreeReaderVectorValue<Float_t> 		_phoCalibE;
	TTreeReaderVectorValue<Float_t> 		_phoSigmaCalibE;
	TTreeReaderVectorValue<Float_t> 		_phoCalibEt;
	TTreeReaderVectorValue<Bool_t> 			_phohasPixelSeed;
	TTreeReaderVectorValue<Bool_t> 			_phoEleVeto;
	TTreeReaderVectorValue<Float_t> 		_phoR9;
	TTreeReaderVectorValue<Float_t>		 	_phoR9Full5x5;
	TTreeReaderVectorValue<Float_t>		 	_phoSigmaIEtaIEtaFull5x5;
	TTreeReaderVectorValue<Float_t>		 	_phoSigmaIEtaIPhiFull5x5;
	TTreeReaderVectorValue<Float_t> 		_phoSigmaIPhiIPhiFull5x5;
	TTreeReaderVectorValue<Float_t> 		_phoIDMVA;
	TTreeReaderVectorValue<UChar_t> 		_phoIDbit;
	TTreeReaderVectorValue<Float_t>			_phoSeedTime;
	TTreeReaderVectorValue<Short_t> 		_pho_gen_index;

	TTreeReaderAnyValue<UShort_t> 			_metFilters;
	TTreeReaderAnyValue<Float_t> 			_pfMET;
	TTreeReaderAnyValue<Float_t> 			_pfMETPhi;
	TTreeReaderAnyValue<Float_t> 			_pfMET_metSig;
	TTreeReaderAnyValue<Float_t> 			_pfMET_EtSig;


	TTreeReaderAnyValue<UShort_t>			_nEle;
	TTreeReaderVectorValue<Float_t> 		_eleCalibPt;
	TTreeReaderVectorValue<Float_t> 		_eleEta;
	TTreeReaderVectorValue<Float_t> 		_elePhi;
	TTreeReaderVectorValue<Float_t> 		_eleCalibEn;
	TTreeReaderVectorValue<Float_t> 		_eleIDMVAIso;
	TTreeReaderVectorValue<Char_t> 			_eleIDbit;

	TTreeReaderAnyValue<UShort_t>			_nMu;
	TTreeReaderVectorValue<Float_t> 		_muPt;
	TTreeReaderVectorValue<Float_t> 		_muEta;
	TTreeReaderVectorValue<Float_t> 		_muPhi;
	TTreeReaderVectorValue<Float_t> 		_muEn;
	TTreeReaderVectorValue<Int_t> 			_muIDbit;


	TTreeReaderAnyValue<UShort_t>			_nAK4CHSJet;
	TTreeReaderVectorValue<Float_t> 		_AK4CHSJet_Pt;
	TTreeReaderVectorValue<Float_t> 		_AK4CHSJet_En;
	TTreeReaderVectorValue<Float_t> 		_AK4CHSJet_Eta;
	TTreeReaderVectorValue<Float_t> 		_AK4CHSJet_Phi;
	TTreeReaderVectorValue<Char_t> 			_AK4CHSJet_ID;
	TTreeReaderVectorValue<Float_t> 		_AK4CHSJet_PUID;
	TTreeReaderVectorValue<Char_t> 			_AK4CHSJet_PUFullID;

	// Running vars
	Short_t selectedPhotonIndex = -999;
	Float_t puWeight;


	/***********************************************output TTree branches***********************************/
	// only selected reconstructed objects

	TFile *outFile = nullptr;
	TTree *outTree = nullptr;

	Int_t 						run_;
	Long64_t 					event_;
	UChar_t						puTrue_;
	ULong64_t 					HLTPho_;
	Float_t 					genWeight_;
	Float_t						eventWeight_ = 1.;
	Char_t 						mcHasDirectPromptPho_;

	Float_t 					phoEta_;
	Float_t 					phoPhi_;
	Float_t 					phoCalibE_;
	Float_t 					phoSigmaCalibE_;
	Float_t 					phoCalibEt_;
	Float_t 					phoR9_;
	Float_t 					phoR9Full5x5_;
	Float_t		 				phoSigmaIEtaIEtaFull5x5_;
	Float_t		 				phoSigmaIEtaIPhiFull5x5_;
	Float_t						phoSigmaIPhiIPhiFull5x5_;

	Float_t 					genPhoPt_;
	Float_t 					genPhoEta_;
	Float_t 					genPhoPhi_;
	Float_t 					genPhoEn_;

	Float_t 					pfMET_;
	Float_t 					pfMETPhi_;
	Float_t			 			pfMET_metSig_;
	Float_t 					pfMET_EtSig_;

	Float_t 					genMET_;
	Float_t 					genMETPhi_;

	Float_t						deltaPhi_phoMET_;
	Float_t						deltaPhi_gen_phoMET_;



	/***********************************************global histograms***********************************/
	TH1F h_genWeightSum{"sumGenWeights", "Sum of Gen Weights", 1, 0., 1};
	TH1F h_genWeightSignSum{"sumGenWeightSigns", "Sum of Gen Weight Signs", 1, 0., 1};
	TH1F h_eventWeightSum{"sumEventWeights", "Sum of Event Weights", 1, 0., 1};


	/***********************************************BoostedJet+G histograms***********************************/
	plot_variable 	 	var_phoCalibEt{phoCalibEt_, 200., 1500., 130, "p_{T}(\\gamma)", "GeV"},
						var_phoEta{phoEta_, ECAL_ETA_BINS, 27, "\\eta(\\gamma)"},
						var_phoPhi{phoPhi_, -3.15, 3.15, 63, "\\phi(\\gamma)"},
						var_phoSigmaIEtaIEtaFull5x5{phoSigmaIEtaIEtaFull5x5_, 0., 0.1, 1000, "#sigma_{i#etai#eta}", ""},
						var_phoGenPt_{genPhoPt_, 200., 1500., 130, "p_{T}(gen\\ \\gamma)", "GeV"},
						var_phoGenEta_{genPhoEta_,  ECAL_ETA_BINS, 27, "\\eta(gen\\ \\gamma)"},
						var_phoGenPhi_{genPhoPhi_, -3.15, 3.15, 63, "\\phi(gen\\ \\gamma)"},
						var_genMET{genMET_, 0., 2000., 2000, "gen #slash{E}_{T}", "GeV"},
						var_genMETPhi{genMETPhi_, -3.15, 3.15, 63, "#phi(gen #slash{E}_{T})", ""},
						var_pfMET{pfMET_, 0., 2000., 1000, "#slash{E}_{T}", "GeV"},
						var_pfMETPhi_{pfMETPhi_, -3.15, 3.15, 63, "#phi(#slash{E}_{T})", ""},
						var_deltaPhiphoMET_{deltaPhi_phoMET_, 0, 6.3, 63, "#Delta #phi (#gamma, #slash{E}_{T})", ""},
						var_deltaPhi_gen_phoMET{deltaPhi_gen_phoMET_, 0, 6.3, 63, "#Delta #phi (gen #gamma, gen #slash{E}_{T})", ""},
						var_pfMET_metSig{pfMET_metSig_, 0., 5000., 500, "#slash{E}_{T} Significance", ""};



	std::vector<histogram_template> GammaMET_1d_reco_histograms = {
		{var_phoCalibEt},
		{var_phoEta},
		{var_phoPhi},
		{var_pfMET},
		{var_pfMETPhi_},
		{var_deltaPhiphoMET_},
		{var_phoSigmaIEtaIEtaFull5x5},
		{var_pfMET_metSig}
	};

	std::vector<histogram_template> GammaMET_1d_gen_histograms ={
		{var_phoGenPt_},
		{var_phoGenEta_},
		{var_phoGenPhi_},
		{var_genMET},
		{var_genMETPhi},
		{var_deltaPhi_gen_phoMET}
	};


	std::vector<twoDhistogram_template> GammaMET_2d_Histograms ={
		{var_phoCalibEt, var_pfMETPhi_},
		{var_phoEta, var_phoSigmaIEtaIEtaFull5x5}
	};
};


#endif
