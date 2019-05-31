#ifndef ANTGC_POSTSELECTOR_CC
#define ANTGC_POSTSELECTOR_CC

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "extra_tools.cc"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define REPORT_EVERY 200
#define BETRetaMin 1.4442
#define BETRetaMax 1.566
Double_t ECAL_ETA_BINS[28] = {-2.5, -2.3, -2.1, -1.9, -1.7, -BETRetaMax, -BETRetaMin, -1.3, -1.1, -0.9, -0.7, -0.5, -0.3, -0.1,  0.1, 0.3, 0.5, 0.7, 0.9, 1.1, 1.3, BETRetaMin, BETRetaMax, 1.7, 1.9, 2.1, 2.3, 2.5};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void eventPostselector(std::string _fileList, std::string _outDir);
void postSelect(std::string _file);


void postSelect(std::string _file, std::string _outDir){

	std::cout<<"Post selecting file "<<_file<<std::endl;

	TChain *inputTree =openTChain(std::vector<std::string> {_file}, "preSelected");
	if(!inputTree){
		std::cout<<"Error creating TChain with file "<<_file<<std::endl;
		return;
	}

	Bool_t isMC = !matchRegex(_file, ".*SinglePhoton.*");

	TTreeReader inputTTreeReader(inputTree);

	TTreeReaderAnyValue<Float_t> 		genWeight;
	if(isMC) genWeight.set(inputTTreeReader, "genWeight");
	TTreeReaderAnyValue<Char_t>        leptVeto(inputTTreeReader, "leptVeto");
	TTreeReaderAnyValue<Float_t>        deltaR_AK8PuppiJetG(inputTTreeReader, "deltaR_AK8PuppiJetG");
	TTreeReaderAnyValue<Float_t>        phoEta(inputTTreeReader, "phoEta");
	TTreeReaderAnyValue<Float_t>        phoPhi(inputTTreeReader, "phoPhi");
	TTreeReaderAnyValue<Float_t>        phoCalibE(inputTTreeReader, "phoCalibE");
	TTreeReaderAnyValue<Float_t>        phoCalibEt(inputTTreeReader, "phoCalibEt");
	TTreeReaderAnyValue<Float_t>        phoR9(inputTTreeReader, "phoR9");
	TTreeReaderAnyValue<Float_t>        phoR9Full5x5(inputTTreeReader, "phoR9Full5x5");
	TTreeReaderAnyValue<Int_t>			AK8PuppiJet_Charge(inputTTreeReader, "AK8PuppiJet_Charge");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_Pt(inputTTreeReader, "AK8PuppiJet_Pt");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_En(inputTTreeReader, "AK8PuppiJet_En");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_Eta(inputTTreeReader, "AK8PuppiJet_Eta");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_Phi(inputTTreeReader, "AK8PuppiJet_Phi");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_Mass(inputTTreeReader, "AK8PuppiJet_Mass");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_tau21(inputTTreeReader, "AK8PuppiJet_tau21");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_tau32(inputTTreeReader, "AK8PuppiJet_tau32");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_nb1ecf2(inputTTreeReader, "AK8PuppiJet_nb1ecf2");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_nb1ecf3(inputTTreeReader, "AK8PuppiJet_nb1ecf3");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_nb2ecf2(inputTTreeReader, "AK8PuppiJet_nb2ecf2");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_nb2ecf3(inputTTreeReader, "AK8PuppiJet_nb2ecf3");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_PUPPISoftDropMass(inputTTreeReader, "AK8PuppiJet_PUPPISoftDropMass");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_CHSPrunedMass(inputTTreeReader, "AK8PuppiJet_CHSPrunedMass");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_CHSSoftDropMass(inputTTreeReader, "AK8PuppiJet_CHSSoftDropMass");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_pfBoostedDSVBTag(inputTTreeReader, "AK8PuppiJet_pfBoostedDSVBTag");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_CSV(inputTTreeReader, "AK8PuppiJet_CSV");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_DDBvLTags_qcd(inputTTreeReader, "AK8PuppiJet_DDBvLTags_qcd");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_DDBvLTags_Hbb(inputTTreeReader, "AK8PuppiJet_DDBvLTags_Hbb");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_DDCvLTags_qcd(inputTTreeReader, "AK8PuppiJet_DDCvLTags_qcd");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_DDCvLTags_Hcc(inputTTreeReader, "AK8PuppiJet_DDCvLTags_Hcc");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_DDCvBTags_Hbb(inputTTreeReader, "AK8PuppiJet_DDCvBTags_Hbb");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_DDCvBTags_Hcc(inputTTreeReader, "AK8PuppiJet_DDCvBTags_Hcc");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_DDBDTags_bbcsLight(inputTTreeReader, "AK8PuppiJet_DDBDTags_bbcsLight");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_DDBDTags_ccvsLight(inputTTreeReader, "AK8PuppiJet_DDBDTags_ccvsLight");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_DDBDTags_TvsQCD(inputTTreeReader, "AK8PuppiJet_DDBDTags_TvsQCD");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_DDBDTags_ZHccvsQCD(inputTTreeReader, "AK8PuppiJet_DDBDTags_ZHccvsQCD");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_DDBDTags_WvsQCD(inputTTreeReader, "AK8PuppiJet_DDBDTags_WvsQCD");
	TTreeReaderAnyValue<Float_t>        AK8PuppiJet_DDBDTags_ZHbbvsQCD(inputTTreeReader, "AK8PuppiJet_DDBDTags_ZHbbvsQCD");
	TTreeReaderAnyValue<Float_t>        pfMET(inputTTreeReader, "pfMET");
	TTreeReaderAnyValue<Float_t>        pfMETPhi(inputTTreeReader, "pfMETPhi");
	TTreeReaderVectorValue<Float_t> 	AK4CHSJet_Pt(inputTTreeReader, "AK4CHSJet_Pt");

	Char_t	leptVeto_;
	Float_t	 deltaR_AK8PuppiJetG_;
	Float_t	 phoEta_;
	Float_t	 phoPhi_;
	Float_t	 phoCalibE_;
	Float_t	phoCalibEt_;
	Float_t	phoR9_;
	Float_t	phoR9Full5x5_;
	Int_t AK8PuppiJet_Charge_;
	Float_t	AK8PuppiJet_Pt_;
	Float_t	AK8PuppiJet_En_;
	Float_t	AK8PuppiJet_Eta_;
	Float_t	AK8PuppiJet_Phi_;
	Float_t	AK8PuppiJet_Mass_;
	Float_t	AK8PuppiJet_tau21_;
	Float_t	AK8PuppiJet_tau32_;
	Float_t	AK8PuppiJet_nb1ecf2_;
	Float_t	AK8PuppiJet_nb1ecf3_;
	Float_t	AK8PuppiJet_nb2ecf2_;
	Float_t	AK8PuppiJet_nb2ecf3_;
	Float_t	AK8PuppiJet_PUPPISoftDropMass_;
	Float_t	AK8PuppiJet_CHSPrunedMass_;
	Float_t	AK8PuppiJet_CHSSoftDropMass_;
	Float_t	AK8PuppiJet_pfBoostedDSVBTag_;
	Float_t	AK8PuppiJet_CSV_;
	Float_t	AK8PuppiJet_DDBvLTags_qcd_;
	Float_t	AK8PuppiJet_DDBvLTags_Hbb_;
	Float_t	AK8PuppiJet_DDCvLTags_qcd_;
	Float_t	AK8PuppiJet_DDCvLTags_Hcc_;
	Float_t	AK8PuppiJet_DDCvBTags_Hbb_;
	Float_t	AK8PuppiJet_DDCvBTags_Hcc_;
	Float_t	AK8PuppiJet_DDBDTags_bbcsLight_;
	Float_t	AK8PuppiJet_DDBDTags_ccvsLight_;
	Float_t	AK8PuppiJet_DDBDTags_TvsQCD_;
	Float_t	AK8PuppiJet_DDBDTags_ZHccvsQCD_;
	Float_t	AK8PuppiJet_DDBDTags_WvsQCD_;
	Float_t	AK8PuppiJet_DDBDTags_ZHbbvsQCD_;
	Float_t	pfMET_;
	Float_t	pfMETPhi_;

	Float_t nAK4CHSJets_40GeV_;
	Float_t genWeight_;

	plot_variable 	 	var_phoCalibEt{phoCalibEt_, 200., 1500., 130, "p_{T}(\\gamma)", "GeV"},
	var_phoCalibEt_unNorm{phoCalibEt_, 200., 1500., 130, "Unweighted p_{T}(\\gamma)", "GeV"},
	var_phoEta{phoEta_, ECAL_ETA_BINS, 27, "\\eta(\\gamma)"},
	var_phoPhi{phoPhi_, -3.15, 3.15, 63, "\\phi(\\gamma)"},
	var_AK8PuppiJet_Pt{AK8PuppiJet_Pt_, 200., 1500., 130, "p_{T}(ak^{8}_{T}\\ jet)", "GeV"},
	var_AK8PuppiJet_Eta{AK8PuppiJet_Eta_, -2.5, 2.5, 25, "\\eta(ak^{8}_{T}\\ jet)"},
	var_AK8PuppiJet_Phi{AK8PuppiJet_Phi_, -3.15, 3.15, 63, "\\phi(ak^{8}_{T}\\ jet)"},
	var_AK8PuppiJet_Mass{AK8PuppiJet_Mass_, 20., 200., 180, "Mass(ak^{8}_{T}\\ jet)", "GeV"},
	var_AK8PuppiJet_PUPPISoftDropMass{AK8PuppiJet_PUPPISoftDropMass_, 20., 200., 180, "SD\\ Mass(ak^{8}_{T}\\ jet)", "GeV"},
	var_AK8PuppiJet_CHSPrunedMass{AK8PuppiJet_CHSPrunedMass_, 20., 200., 180, "CHS\\ Pruned\\ Mass(ak^{8}_{T}\\ jet)", "GeV"},
	var_AK8PuppiJet_CHSSoftDropMass{AK8PuppiJet_CHSSoftDropMass_, 20., 200., 180, "CHS\\ SD\\ Mass(ak^{8}_{T}\\ jet)", "GeV"},
	var_AK8PuppiJet_tau21{AK8PuppiJet_tau21_, 0., 1., 100, "\\tau_{21}(ak^{8}_{T}\\ jet)"},
	var_AK8PuppiJet_tau32{AK8PuppiJet_tau32_, 0., 1., 100, "\\tau_{32}(ak^{8}_{T}\\ jet)"},
	var_AK8PuppiJet_CSVv2{AK8PuppiJet_CSV_, -2., 2., 400, "CSV_{v2}(ak^{8}_{T}\\ jet)"},
	var_nAK4CHSJets_40GeV{nAK4CHSJets_40GeV_, 0., 20., 20, "N^{o}\\ of\\ ak_{T}^{4} \\ jets\\ with\\ p_{T}>40\\ GeV"},
	var_deltaR_AK8PuppiJetG{deltaR_AK8PuppiJetG_, 0., 5., 50, "\\Delta{R}(ak^{8}_{T}\\ jet, \\gamma)"},
	var_AK8PuppiJet_nb1ecf2_{AK8PuppiJet_nb1ecf2_, 0., 5., 500, "ECF(2,\\beta=1)"},
	var_AK8PuppiJet_nb1ecf3_{AK8PuppiJet_nb1ecf3_, 0., 5., 500, "ECF(3,\\beta=1)"},
	var_AK8PuppiJet_nb2ecf2_{AK8PuppiJet_nb2ecf2_, 0., 5., 500, "ECF(2,\\beta=2)"},
	var_AK8PuppiJet_nb2ecf3_{AK8PuppiJet_nb2ecf3_, 0., 5., 500, "ECF(3,\\beta=2)"},
	var_MET_{pfMET_, 0., 1500., 1500, "\\slash{E}_{T}", "GeV"},
	var_METPhi_{pfMETPhi_, -3.15, 3.15, 63, "\\phi(\\slash{E}_{T})"};

	histogram_template	hist_var_phoCalibEt_unNorm(var_phoCalibEt_unNorm);

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
		{var_AK8PuppiJet_nb1ecf2_},
		{var_AK8PuppiJet_nb1ecf3_},
		{var_AK8PuppiJet_nb2ecf2_},
		{var_AK8PuppiJet_nb2ecf3_},
		{var_AK8PuppiJet_CSVv2},
		{var_nAK4CHSJets_40GeV},
		{var_deltaR_AK8PuppiJetG},
		{var_MET_},
		{var_METPhi_}
	};

	std::vector<twoDhistogram_template> boostedJetG_2d_Histograms ={
		{var_phoCalibEt, var_AK8PuppiJet_Pt},
		{var_phoCalibEt, var_deltaR_AK8PuppiJetG},
		{var_AK8PuppiJet_Pt, var_nAK4CHSJets_40GeV},
		{var_phoCalibEt, var_METPhi_}
	};

	std::string outFileName = _outDir + "/" + getFileName(_file);
	TFile *outFile = new TFile(outFileName.c_str(), "RECREATE");

	std::cout<<"Initializing TH1D for boosted Jet + gamma channel..."<<std::endl;
	for(auto & hist1d : boostedJetG_1d_Histograms){
		hist1d.initializehist();
		hist1d.hist->SetDirectory(outFile->GetDirectory(""));
	}

	hist_var_phoCalibEt_unNorm.initializehist();
	hist_var_phoCalibEt_unNorm.hist->SetDirectory(outFile->GetDirectory(""));

	std::cout<<"Initializing TH2D for boosted Jet + gamma channel..."<<std::endl;
	for(auto & hist2d : boostedJetG_2d_Histograms){
		hist2d.initializehist();
		hist2d.hist->SetDirectory(outFile->GetDirectory(""));
	}

	// inputTTreeReader.SetEntry(-1);

	std::cout<<"Analyzing..."<<std::endl;

	while (inputTTreeReader.Next()) {

		ULong64_t current_entry = inputTTreeReader.GetCurrentEntry();
		if(current_entry % REPORT_EVERY == 0){
			std::cout<<"\t\t\t Analyzing entry "<<current_entry<<std::endl;
		}

		if((deltaR_AK8PuppiJetG) < 1.9) continue;
		if((AK8PuppiJet_CHSSoftDropMass) > 120. || (AK8PuppiJet_CHSSoftDropMass) < 60.) continue;
		if((AK8PuppiJet_CHSSoftDropMass) > 120. || (AK8PuppiJet_CHSSoftDropMass) < 60.) continue;
		if((AK8PuppiJet_nb1ecf3) > 2.5 || (AK8PuppiJet_nb1ecf3) < 2.5) continue;


		genWeight_ = isMC ? genWeight : 1.;

		nAK4CHSJets_40GeV_ = AK4CHSJet_Pt.size() + 0.2;

		leptVeto_ = leptVeto;

		deltaR_AK8PuppiJetG_ = deltaR_AK8PuppiJetG;

		phoEta_ = phoEta;

		phoPhi_ = phoPhi;
		phoCalibE_ = phoCalibE;
		phoCalibEt_ = phoCalibEt;
		phoR9_ = phoR9;
		phoR9Full5x5_ = phoR9Full5x5;
		AK8PuppiJet_Charge_ = AK8PuppiJet_Charge;
		AK8PuppiJet_Pt_ = AK8PuppiJet_Pt;
		AK8PuppiJet_En_ = AK8PuppiJet_En;
		AK8PuppiJet_Eta_ = AK8PuppiJet_Eta;
		AK8PuppiJet_Phi_ = AK8PuppiJet_Phi;
		AK8PuppiJet_Mass_ = AK8PuppiJet_Mass;
		AK8PuppiJet_tau21_ = AK8PuppiJet_tau21;
		AK8PuppiJet_tau32_ = AK8PuppiJet_tau32;
		AK8PuppiJet_nb1ecf2_ = AK8PuppiJet_nb1ecf2;
		AK8PuppiJet_nb1ecf3_ = AK8PuppiJet_nb1ecf3;
		AK8PuppiJet_nb2ecf2_ = AK8PuppiJet_nb2ecf2;
		AK8PuppiJet_nb2ecf3_ = AK8PuppiJet_nb2ecf3;
		AK8PuppiJet_PUPPISoftDropMass_ = AK8PuppiJet_PUPPISoftDropMass;
		AK8PuppiJet_CHSPrunedMass_ = AK8PuppiJet_CHSPrunedMass;
		AK8PuppiJet_CHSSoftDropMass_ = AK8PuppiJet_CHSSoftDropMass;
		AK8PuppiJet_pfBoostedDSVBTag_ = AK8PuppiJet_pfBoostedDSVBTag;
		AK8PuppiJet_CSV_ = AK8PuppiJet_CSV;
		AK8PuppiJet_DDBvLTags_qcd_ = AK8PuppiJet_DDBvLTags_qcd;
		AK8PuppiJet_DDBvLTags_Hbb_ = AK8PuppiJet_DDBvLTags_Hbb;
		AK8PuppiJet_DDCvLTags_qcd_ = AK8PuppiJet_DDCvLTags_qcd;
		AK8PuppiJet_DDCvLTags_Hcc_ = AK8PuppiJet_DDCvLTags_Hcc;
		AK8PuppiJet_DDCvBTags_Hbb_ = AK8PuppiJet_DDCvBTags_Hbb;
		AK8PuppiJet_DDCvBTags_Hcc_ = AK8PuppiJet_DDCvBTags_Hcc;
		AK8PuppiJet_DDBDTags_bbcsLight_ = AK8PuppiJet_DDBDTags_bbcsLight;
		AK8PuppiJet_DDBDTags_ccvsLight_ = AK8PuppiJet_DDBDTags_ccvsLight;
		AK8PuppiJet_DDBDTags_TvsQCD_ = AK8PuppiJet_DDBDTags_TvsQCD;
		AK8PuppiJet_DDBDTags_ZHccvsQCD_ = AK8PuppiJet_DDBDTags_ZHccvsQCD;
		AK8PuppiJet_DDBDTags_WvsQCD_ = AK8PuppiJet_DDBDTags_WvsQCD;
		AK8PuppiJet_DDBDTags_ZHbbvsQCD_ = AK8PuppiJet_DDBDTags_ZHbbvsQCD;
		pfMET_ = pfMET;
		pfMETPhi_ = pfMETPhi;



		for(auto & hist :  boostedJetG_1d_Histograms){
			hist.fill(genWeight_);
		}

		hist_var_phoCalibEt_unNorm.fill();

		for(auto & hist2D : boostedJetG_2d_Histograms){
			hist2D.fill(genWeight_);
		}

	}

	closeTChain(inputTree);

	TH1F *h_genWeightSum = (TH1F*) getHistFromFile("sumGenWeights", _file);
	h_genWeightSum->SetDirectory(outFile->GetDirectory(""));
	outFile->Write();
	outFile->Close();

	std::cout<<"Written results to "<<outFileName<<std::endl;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void eventPostselector(std::string _fileList, std::string _outDir){

	std::vector<std::string> preSelectedFiles = getNonemptyLines(_fileList);

	mkdir(_outDir);

	for(std::string & inFile : preSelectedFiles){
		postSelect(inFile, _outDir);
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
