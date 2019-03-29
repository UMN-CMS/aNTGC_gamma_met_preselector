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
	aNTGCpreselector(std::string _file_list, std::string _output_file);
	Char_t isData(std::string _infilespath);
private:
	Char_t initIntputNtuples();
	void setGlobalHists();
	void setBoostedJetGhists();
	void setOutputTree();
	void copyEvent();
	void resetVars();
	void fillGlobalHists();
	void fillBoostedJetGhists();
	void writeGlobalHists();
	void writeBoostedJetGhists();
	void writeOutputTree();
	Bool_t selectBoostedJetGevent();
	void analyze();
	void registerCutFlow();
	Float_t lastCutStep = 0.5;
	TH1F h_cutFlowEvts{"cutFlowEvts", "Cut Flow (# of events)", 20, 0., 20.};
	TH1F h_cutFlowWeighted{"cutFlow", "Cut Flow", 20, 0., 20.};
	TH1F h_cutFlowWeightSigns{"cutFlowWgtSigns", "Cut Flow (weight signs)", 20, 0., 20.};

	Bool_t isQCD;
	Bool_t isGJets;
	Bool_t isMC;

	/***********************************************input TTree *******************************************/

	TTreeReader inputTTreeReader;
	TChain *inputTree = nullptr;

	TTreeReaderAnyValue<Int_t> _run;
	TTreeReaderAnyValue<Long64_t> _event;
	TTreeReaderAnyValue<Int_t> _lumis;


	TTreeReaderAnyValue<Bool_t> _isPVGood;


	TTreeReaderAnyValue<ULong64_t> _HLTPho;


	TTreeReaderAnyValue<Float_t> _genWeight;
	TTreeReaderAnyValue<Char_t> _mcHasDirectPromptPho;


	TTreeReaderAnyValue<Float_t> _pfMET;
	TTreeReaderAnyValue<Float_t> _pfMETPhi;

	TTreeReaderVectorValue< Float_t> _eleCalibPt;
	TTreeReaderVectorValue< Float_t> _eleEta;
	TTreeReaderVectorValue< Float_t> _elePhi;
	TTreeReaderVectorValue< Float_t> _eleIDMVAIso;
	TTreeReaderVectorValue<UShort_t> _eleIDbit;


	TTreeReaderVectorValue< Float_t> _muPt;
	TTreeReaderVectorValue< Float_t> _muEta;
	TTreeReaderVectorValue< Float_t> _muPhi;
	TTreeReaderVectorValue<Int_t> _muIDbit;


	TTreeReaderVectorValue< Float_t> _AK4CHSJet_Pt;
	TTreeReaderVectorValue< Float_t> _AK4CHSJet_En;
	TTreeReaderVectorValue< Float_t> _AK4CHSJet_Eta;
	TTreeReaderVectorValue< Float_t> _AK4CHSJet_Phi;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_ConsituentEtaPhiSpread;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_constituentPtDistribution;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_MaxConstituentDistance;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_CSV2BJetTags;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_DeepCSVTags_b;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_DeepCSVTags_bb;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_DeepCSVTags_c;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_DeepCSVTags_udsg;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_DeepFlavTags_bb;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_DeepFlavTags_bbb;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_DeepFlavTags_lepb;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_DeepFlavTags_c;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_DeepFlavTags_uds;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_DeepFlavTags_g;
		// TTreeReaderVectorValue< Float_t> _AK4CHSJet_CombMVA2Tags;
		// TTreeReaderVectorValue<Int_t> _AK4CHSJet_PartonFlavour;
		// TTreeReaderVectorValue<Int_t> _AK4CHSJet_HadronFlavour;
	TTreeReaderVectorValue<Char_t> _AK4CHSJet_ID;
	TTreeReaderVectorValue< Float_t> _AK4CHSJet_PUID;
		// TTreeReaderVectorValue<Char_t> _AK4CHSJet_PUFullID;
	TTreeReaderVectorValue<Float_t> _AK4CHSJet_qgLikelihood;



	TTreeReaderVectorValue< Float_t> _phoEta;
	TTreeReaderVectorValue< Float_t> _phoPhi;
	TTreeReaderVectorValue< Float_t> _phoCalibE;
	TTreeReaderVectorValue< Float_t> _phoSigmaCalibE;
	TTreeReaderVectorValue< Float_t> _phoCalibEt;
	TTreeReaderVectorValue<Int_t> _phohasPixelSeed;
	TTreeReaderVectorValue<Int_t> _phoEleVeto;
	TTreeReaderVectorValue< Float_t> _phoR9;
	TTreeReaderVectorValue< Float_t> _phoR9Full5x5;
	TTreeReaderVectorValue< Float_t> _phoIDMVA;
	TTreeReaderVectorValue<UChar_t> _phoIDbit;


	TTreeReaderVectorValue<Int_t> _AK8PuppiJet_Charge;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_Pt;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_En;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_Eta;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_Phi;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_Mass;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_tau1;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_tau2;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_tau3;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_tau4;
	TTreeReaderVectorValue<Char_t> _AK8PuppiJet_PFid;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_PUPPISoftDropMass;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_CHSPrunedMass;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_CHSSoftDropMass;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_pfBoostedDSVBTag;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_CSV;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_DDBvLTags_qcd;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_DDBvLTags_Hbb;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_DDCvLTags_qcd;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_DDCvLTags_Hcc;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_DDCvBTags_Hbb;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_DDCvBTags_Hcc;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_DDBDTags_bbcsLight;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_DDBDTags_ccvsLight;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_DDBDTags_TvsQCD;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_DDBDTags_ZHccvsQCD;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_DDBDTags_WvsQCD;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_DDBDTags_ZHbbvsQCD;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_nb1ecf2;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_nb1ecf3;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_nb2ecf2;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_nb2ecf3;
	TTreeReaderVectorValue<Int_t> _AK8PuppiJet_PartonFlavour;
	TTreeReaderVectorValue<Int_t> _AK8PuppiJet_HadronFlavour;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_GenJetEn;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_GenJetPt;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_GenJetEta;
	TTreeReaderVectorValue< Float_t> _AK8PuppiJet_GenJetPhi;

	// Running vars
	Int_t selectedPhotonIndex = -999;
	Int_t selectedAK8PuppiJetIndex = -999;



	/***********************************************output TTree branches***********************************/
	// only selected reconstructed objects

	TFile *outFile = nullptr;
	TTree *outTree = nullptr;

	Int_t run_;
	Long64_t event_;
	ULong64_t HLTPho_;
	Float_t genWeight_;
	Char_t mcHasDirectPromptPho_;
	// Float_t p_nAK4CHSJets_in_AK8PuppiJet_0p8;
	// Float_t p_nAK4CHSJets_in_AK8PuppiJet_1p0;
	// Float_t p_nAK4CHSJets_in_AK8PuppiJet_1p2;
	Float_t p_nAK4CHSJets_40GeV;
	Float_t p_deltaR_AK8PuppiJetG;

	Float_t phoEta_;
	Float_t phoPhi_;
	Float_t phoCalibE_;
	Float_t phoSigmaCalibE_;
	Float_t phoCalibEt_;
	Int_t phohasPixelSeed_;
	Int_t phoEleVeto_;
	Float_t phoR9_;
	Float_t phoR9Full5x5_;
	Float_t phoIDMVA_;
	UChar_t phoIDbit_;


	Int_t AK8PuppiJet_Charge_;
	Float_t AK8PuppiJet_Pt_;
	Float_t AK8PuppiJet_En_;
	Float_t AK8PuppiJet_Eta_;
	Float_t AK8PuppiJet_Phi_;
	Float_t AK8PuppiJet_Mass_;
	Float_t p_AK8PuppiJet_tau21;
	Float_t p_AK8PuppiJet_tau32;
	Float_t p_AK8PuppiJet_tau43;
	Float_t AK8PuppiJet_nb1ecf2_;
	Float_t AK8PuppiJet_nb1ecf3_;
	Float_t AK8PuppiJet_nb2ecf2_;
	Float_t AK8PuppiJet_nb2ecf3_;
	Char_t AK8PuppiJet_PFid_;
	Float_t AK8PuppiJet_PUPPISoftDropMass_;
	Float_t AK8PuppiJet_CHSPrunedMass_;
	Float_t AK8PuppiJet_CHSSoftDropMass_;
	Float_t AK8PuppiJet_pfBoostedDSVBTag_;
	Float_t AK8PuppiJet_CSV_;
	Float_t AK8PuppiJet_DDBvLTags_qcd_;
	Float_t AK8PuppiJet_DDBvLTags_Hbb_;
	Float_t AK8PuppiJet_DDCvLTags_qcd_;
	Float_t AK8PuppiJet_DDCvLTags_Hcc_;
	Float_t AK8PuppiJet_DDCvBTags_Hbb_;
	Float_t AK8PuppiJet_DDCvBTags_Hcc_;
	Float_t AK8PuppiJet_DDBDTags_bbcsLight_;
	Float_t AK8PuppiJet_DDBDTags_ccvsLight_;
	Float_t AK8PuppiJet_DDBDTags_TvsQCD_;
	Float_t AK8PuppiJet_DDBDTags_ZHccvsQCD_;
	Float_t AK8PuppiJet_DDBDTags_WvsQCD_;
	Float_t AK8PuppiJet_DDBDTags_ZHbbvsQCD_;
	Int_t AK8PuppiJet_PartonFlavour_;
	Int_t AK8PuppiJet_HadronFlavour_;
	Float_t AK8PuppiJet_GenJetEn_;
	Float_t AK8PuppiJet_GenJetPt_;
	Float_t AK8PuppiJet_GenJetEta_;
	Float_t AK8PuppiJet_GenJetPhi_;

	Float_t pfMET_;
	Float_t pfMETPhi_;

	Char_t leptVeto_;
	Float_t vetoElePt_;
	Float_t vetoEleEta_;
	Float_t vetoElePhi_;
	Float_t vetoMuPt_;
	Float_t vetoMuEta_;
	Float_t vetoMuPhi_;
		// std::vector< Float_t> eleCalibPt_;
		// std::vector< Float_t> eleEta_;
		// std::vector< Float_t> elePhi_;
		// std::vector< Float_t> eleCalibEn_;
		// // std::vector< Float_t> eleIDMVAIso_;
		// std::vector< UShort_t> eleIDbit_;
		// std::vector< Float_t> muPt_;
		// std::vector< Float_t> muEta_;
		// std::vector< Float_t> muPhi_;
		// std::vector< Float_t> muPt_;
		// std::vector<Int_t> muIDbit_;

	std::vector< Float_t> AK4CHSJet_Pt_;
	std::vector< Float_t> AK4CHSJet_En_;
	std::vector< Float_t> AK4CHSJet_Eta_;
	std::vector< Float_t> AK4CHSJet_Phi_;
	std::vector< Float_t> AK4CHSJet_deltaR_selectedAK8PuppiJet_;
		// std::vector< Float_t> AK4CHSJet_ConsituentEtaPhiSpread_;
		// std::vector< Float_t> AK4CHSJet_constituentPtDistribution_;
		// std::vector< Float_t> AK4CHSJet_MaxConstituentDistance_;
		// std::vector< Float_t> AK4CHSJet_CSV2BJetTags_;
		// std::vector< Float_t> AK4CHSJet_DeepCSVTags_b_;
		// std::vector< Float_t> AK4CHSJet_DeepCSVTags_bb_;
		// std::vector< Float_t> AK4CHSJet_DeepCSVTags_c_;
		// std::vector< Float_t> AK4CHSJet_DeepCSVTags_udsg_;
		// std::vector< Float_t> AK4CHSJet_DeepFlavTags_bb_;
		// std::vector< Float_t> AK4CHSJet_DeepFlavTags_bbb_;
		// std::vector< Float_t> AK4CHSJet_DeepFlavTags_lepb_;
		// std::vector< Float_t> AK4CHSJet_DeepFlavTags_c_;
		// std::vector< Float_t> AK4CHSJet_DeepFlavTags_uds_;
		// std::vector< Float_t> AK4CHSJet_DeepFlavTags_g_;
		// std::vector< Float_t> AK4CHSJet_CombMVA2Tags_;
		// std::vector<Int_t> AK4CHSJet_PartonFlavour_;
		// std::vector<Int_t> AK4CHSJet_HadronFlavour_;
	std::vector<Char_t> AK4CHSJet_ID_;
	std::vector< Float_t> AK4CHSJet_PUID_;
		// std::vector<Char_t> AK4CHSJet_PUFullID_;
	std::vector< Float_t> AK4CHSJet_qgLikelihood_;



	/***********************************************global histograms***********************************/
	TH1F h_genWeightSum{"sumGenWeights", "Sum of Gen Weights", 1, 0., 1};
	TH1F h_genWeightSign{"sumGenWeightSigns", "Sum of Gen Weight Signs", 1, 0., 1};


	/***********************************************BoostedJet+G histograms***********************************/


	plot_variable 	 	var_phoCalibEt{phoCalibEt_, 200., 1500., 130, "p_{T}(\\gamma)", "GeV"},
	var_phoEta{phoEta_, ECAL_ETA_BINS, 27, "\\eta(\\gamma)"},
	var_phoPhi{phoPhi_, -3.15, 3.15, 63, "\\phi(\\gamma)"},
	var_AK8PuppiJet_Pt{AK8PuppiJet_Pt_, 200., 1500., 130, "p_{T}(ak^{8}_{T}\\ jet)", "GeV"},
	var_AK8PuppiJet_Eta{AK8PuppiJet_Eta_, -2.5, 2.5, 25, "\\eta(ak^{8}_{T}\\ jet)"},
	var_AK8PuppiJet_Phi{AK8PuppiJet_Phi_, -3.15, 3.15, 63, "\\phi(ak^{8}_{T}\\ jet)"},
	var_AK8PuppiJet_Mass{AK8PuppiJet_Mass_, 20., 200., 180, "Mass(ak^{8}_{T}\\ jet)", "GeV"},
	var_AK8PuppiJet_PUPPISoftDropMass{AK8PuppiJet_PUPPISoftDropMass_, 20., 200., 180, "SD\\ Mass(ak^{8}_{T}\\ jet)", "GeV"},
	var_AK8PuppiJet_CHSPrunedMass{AK8PuppiJet_CHSPrunedMass_, 20., 200., 180, "CHS\\ Pruned\\ Mass(ak^{8}_{T}\\ jet)", "GeV"},
	var_AK8PuppiJet_CHSSoftDropMass{AK8PuppiJet_CHSSoftDropMass_, 20., 200., 180, "CHS\\ SD\\ Mass(ak^{8}_{T}\\ jet)", "GeV"},
	var_AK8PuppiJet_tau21{p_AK8PuppiJet_tau21, 0., 1., 100, "\\tau_{21}(ak^{8}_{T}\\ jet)"},
	var_AK8PuppiJet_tau32{p_AK8PuppiJet_tau32, 0., 1., 100, "\\tau_{32}(ak^{8}_{T}\\ jet)"},
	var_AK8PuppiJet_tau43{p_AK8PuppiJet_tau43, 0., 1., 100, "\\tau_{43}(ak^{8}_{T}\\ jet)"},
	var_AK8PuppiJet_CSVv2{AK8PuppiJet_CSV_, -2., 2., 400, "CSV_{v2}(ak^{8}_{T}\\ jet)"},
	// var_nAK4CHSJets_in_AK8PuppiJet_1p0{p_nAK4CHSJets_in_AK8PuppiJet_1p0, 0., 5., 5, "N^{o}\\ of\\ ak_{T}^{4} \\ jets\\ (p_{T}>40GeV)\\ within\\ \\Delta R<1.0\\ of\\ ak_{T}^{8}\\ jet"},
	// var_nAK4CHSJets_in_AK8PuppiJet_0p8{p_nAK4CHSJets_in_AK8PuppiJet_0p8, 0., 5., 5, "N^{o}\\ of\\ ak_{T}^{4} \\ jets\\ (p_{T}>40GeV)\\ within\\ \\Delta R<0.8\\ of\\ ak_{T}^{8}\\ jet"},
	// var_nAK4CHSJets_in_AK8PuppiJet_1p2{p_nAK4CHSJets_in_AK8PuppiJet_1p2, 0., 5., 5, "N^{o}\\ of\\ ak_{T}^{4} \\ jets\\ (p_{T}>40GeV)\\ within\\ \\Delta R<1.2\\ of\\ ak_{T}^{8}\\ jet"},
	var_nAK4CHSJets_40GeV{p_nAK4CHSJets_40GeV;, 0., 20., 20, "N^{o}\\ of\\ ak_{T}^{4} \\ jets\\ with\\ p_{T}>40\\ GeV"},
	var_deltaR_AK8PuppiJetG{p_deltaR_AK8PuppiJetG, 0., 5., 50, "\\Delta{R}(ak^{8}_{T}\\ jet, \\gamma)"},
	var_AK8PuppiJet_nb1ecf2_{AK8PuppiJet_nb1ecf2_, 0., 5., 500, "ECF(2,\\beta=1)"},
	var_AK8PuppiJet_nb1ecf3_{AK8PuppiJet_nb1ecf3_, 0., 5., 500, "ECF(3,\\beta=1)"},
	var_AK8PuppiJet_nb2ecf2_{AK8PuppiJet_nb2ecf2_, 0., 5., 500, "ECF(2,\\beta=2)"},
	var_AK8PuppiJet_nb2ecf3_{AK8PuppiJet_nb2ecf3_, 0., 5., 500, "ECF(3,\\beta=2)"};


	std::vector<histogram_template> boostedJetG_1d_Histograms = {
		{var_phoCalibEt},
		{var_phoEta},
		{var_phoPhi},
		{var_AK8PuppiJet_Pt},
		{var_AK8PuppiJet_Eta},
		{var_AK8PuppiJet_Phi},
		{var_AK8PuppiJet_Mass},
		{var_AK8PuppiJet_PUPPISoftDropMass},
		{var_AK8PuppiJet_CHSPrunedMass},
		{var_AK8PuppiJet_CHSSoftDropMass},
		{var_AK8PuppiJet_tau21},
		{var_AK8PuppiJet_tau32},
		{var_AK8PuppiJet_tau43},
		{var_AK8PuppiJet_nb1ecf2_},
		{var_AK8PuppiJet_nb1ecf3_},
		{var_AK8PuppiJet_nb2ecf2_},
		{var_AK8PuppiJet_nb2ecf3_},
		{var_AK8PuppiJet_CSVv2},
		{p_nAK4CHSJets_40GeV},
		// {var_nAK4CHSJets_in_AK8PuppiJet_0p8},
		// {var_nAK4CHSJets_in_AK8PuppiJet_1p0},
		// {var_nAK4CHSJets_in_AK8PuppiJet_1p2},
		{var_deltaR_AK8PuppiJetG}
	};


	std::vector<twoDhistogram_template> boostedJetG_2d_Histograms ={
		{var_phoCalibEt, var_AK8PuppiJet_Pt},
		{var_phoCalibEt, var_deltaR_AK8PuppiJetG},
		{var_AK8PuppiJet_Pt, p_nAK4CHSJets_40GeV}
	};
};


#endif
