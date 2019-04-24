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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void eventPostselector(std::string _preSelectedList, std::string _outDir, std::string _xSecMap);
void postSelect(std::string _file);


void postSelect(std::string _file, std::string _outDir, std::string _xSecMap){

	std::cout<<"Post selecting file "<<_file<<std::endl;

	///////////////////////////Open Tchain/////////////////////////////////////////////
	TChain *inputTree =openTChain(std::vector<std::string> {_file}, "preSelected");
	if(!inputTree){
		std::cout<<"Error creating TChain with file "<<_file<<std::endl;
		return;
	}
	///////////////////////////////////////////////////////////////////////////////////

	Bool_t isMC = !matchRegex(_file, ".*SinglePhoton.*");
	std::cout<<"\t\tIsMC: "<<isMC<<std::endl;

	///////////////////////////////////////////////////////////////////////////////////
	Float_t _xSec = -999.;
	if(isMC){
		std::string _sampleName = getFileName(_file);
		_sampleName = findAndReplaceAll(_sampleName, ".root", "");
		_xSec = std::stod(vLookup(_sampleName, _xSecMap, 0, 1));
	}
	std::cout<<"\t\tCross section: "<<_xSec<<std::endl;
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	TTreeReader inputTTreeReader(inputTree);
	TTreeReaderAnyValue<Int_t> 			run(inputTTreeReader, "run");
	TTreeReaderAnyValue<Long64_t> 		event(inputTTreeReader, "event");
	TTreeReaderAnyValue<Float_t> 		genWeight;
	if(isMC) genWeight.set(inputTTreeReader, "genWeight");
	TTreeReaderAnyValue<Float_t>        deltaR_AK8PuppiJetG(inputTTreeReader, "deltaR_AK8PuppiJetG");
	TTreeReaderAnyValue<Float_t>        phoEta(inputTTreeReader, "phoEta");
	TTreeReaderAnyValue<Float_t>        phoPhi(inputTTreeReader, "phoPhi");
	TTreeReaderAnyValue<Float_t>        phoCalibE(inputTTreeReader, "phoCalibE");
	TTreeReaderAnyValue<Float_t>        phoCalibEt(inputTTreeReader, "phoCalibEt");
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
	TTreeReaderAnyValue<Float_t>        pfMET(inputTTreeReader, "pfMET");
	TTreeReaderAnyValue<Float_t>        pfMETPhi(inputTTreeReader, "pfMETPhi");
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	Int_t run_;
	Long64_t event_;
	Double_t weight_;
	Float_t	 deltaR_AK8PuppiJet_G_;
	Float_t	 phoEta_;
	Float_t	 phoPhi_;
	Float_t	 phoCalibE_;
	Float_t	phoCalibEt_;
	Float_t	phoR9_;
	Float_t	phoR9Full5x5_;
	Float_t	AK8PuppiJet_Pt_;
	Float_t	AK8PuppiJet_En_;
	Float_t	AK8PuppiJet_Eta_;
	Float_t	AK8PuppiJet_Phi_;
	Float_t	AK8PuppiJet_Mass_;
	Float_t	AK8PuppiJet_tau21_;
	Float_t	AK8PuppiJet_tau32_;
	Float_t AK8PuppiJet_nb1ecfR32_;
	Float_t	AK8PuppiJet_nb2ecfR32_;
	Float_t	AK8PuppiJet_PUPPISoftDropMass_;
	Float_t	AK8PuppiJet_CHSPrunedMass_;
	Float_t	AK8PuppiJet_CHSSoftDropMass_;
	Float_t	AK8PuppiJet_pfBoostedDSVBTag_;
	Float_t	AK8PuppiJet_CSV_;
	Float_t AK8PuppiJet_DDBvLTags_bb_;
	Float_t	AK8PuppiJet_DDCvLTags_cc_;
	Float_t	AK8PuppiJet_DDCvBTags_bb_;
	Float_t	AK8PuppiJet_DDCvBTags_cc_;
	Float_t	AK8PuppiJet_DDBDTags_bbcsLight_;
	Float_t	AK8PuppiJet_DDBDTags_ccvsLight_;
	Float_t	pfMET_;
	Float_t	pfMETPhi_;
///////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////
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
	var_AK8PuppiJet_tau21{AK8PuppiJet_tau21_, 0., 1., 100, "\\tau_{21}(ak^{8}_{T}\\ jet)"},
	var_AK8PuppiJet_tau32{AK8PuppiJet_tau32_, 0., 1., 100, "\\tau_{32}(ak^{8}_{T}\\ jet)"},
	var_deltaR_AK8PuppiJet_G{deltaR_AK8PuppiJet_G_, 0., 5., 50, "\\Delta{R}(ak^{8}_{T}\\ jet, \\gamma)"},
	var_AK8PuppiJet_nb1ecfR32{AK8PuppiJet_nb1ecfR32_, 0., 10., 1000, "ECF_{32}(\\beta=1)"},
	var_AK8PuppiJet_nb2ecfR32{AK8PuppiJet_nb1ecfR32_, 0., 10., 1000, "ECF_{32}(\\beta=2)"},
	var_AK8PuppiJet_CSVv2{AK8PuppiJet_CSV_, 0., 1., 100, "CSV_{v2}(ak^{8}_{T}\\ jet)"},
	var_AK8PuppiJet_pfBoostedDSVBTag{AK8PuppiJet_pfBoostedDSVBTag_, 0., 1., 100, "Boosted double SV B-tag"},
	var_AK8PuppiJet_DDBvLTags_bb{AK8PuppiJet_DDBvLTags_bb_, 0., 1., 100, "DDB vs L: bb/(qcd+bb)"},
	var_AK8PuppiJet_DDCvLTags_cc{AK8PuppiJet_DDCvLTags_cc_, 0., 1., 100, "DDC vs L: cc/(qcd+cc)"},
	var_AK8PuppiJet_DDCvBTags_bb{AK8PuppiJet_DDCvBTags_bb_, 0., 1., 100, "DDC vs B: bb"},
	var_AK8PuppiJet_DDCvBTags_cc{AK8PuppiJet_DDCvBTags_cc_, 0., 1., 100, "DDC vs B: cc"},
	var_AK8PuppiJet_DDBDTags_bbcsLight{AK8PuppiJet_DDBDTags_bbcsLight_, 0., 5., 500, "DDBD bb vs L"},
	var_AK8PuppiJet_DDBDTags_ccvsLight{AK8PuppiJet_DDBDTags_ccvsLight_, 0., 5., 500, "DDBD cc vs L"},
	var_MET{pfMET_, 0., 1500., 1500, "#slash{E}_{T}", "GeV"},
	var_METPhi{pfMETPhi_, -3.15, 3.15, 63, "#phi(#slash{E}_{T})"};

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
		{var_deltaR_AK8PuppiJet_G},
		{var_AK8PuppiJet_nb1ecfR32},
		{var_AK8PuppiJet_nb2ecfR32},
		{var_AK8PuppiJet_CSVv2},
		{var_AK8PuppiJet_pfBoostedDSVBTag},
		{var_AK8PuppiJet_DDBvLTags_bb},
		{var_AK8PuppiJet_DDCvLTags_cc},
		{var_AK8PuppiJet_DDCvBTags_bb},
		{var_AK8PuppiJet_DDCvBTags_cc},
		{var_AK8PuppiJet_DDBDTags_bbcsLight},
		{var_AK8PuppiJet_DDBDTags_ccvsLight},
		{var_MET},
		{var_METPhi}
	};

	std::vector<twoDhistogram_template> boostedJetG_2d_Histograms ={
		{var_phoCalibEt, var_deltaR_AK8PuppiJet_G},
		{var_phoCalibEt, var_METPhi}
	};
	///////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////
	std::string outFileName = _outDir + "/" + getFileName(_file);
	TFile *outFile = new TFile(outFileName.c_str(), "RECREATE");

	std::cout<<"Initializing TH1D for boosted Jet + gamma channel..."<<std::endl;
	for(auto & hist1d : boostedJetG_1d_Histograms){
		hist1d.initializehist();
		hist1d.hist->SetDirectory(outFile->GetDirectory(""));
	}

	std::cout<<"Initializing TH2D for boosted Jet + gamma channel..."<<std::endl;
	for(auto & hist2d : boostedJetG_2d_Histograms){
		hist2d.initializehist();
		hist2d.hist->SetDirectory(outFile->GetDirectory(""));
	}

	TTree *miniTree = new TTree("miniTree", "miniTree");
	miniTree->SetDirectory(outFile->GetDirectory(""));
	miniTree->Branch("run", &run_);
	miniTree->Branch("event", &event_);
	miniTree->Branch("weight", &weight_);
	miniTree->Branch("deltaR_AK8PuppiJet_G", &deltaR_AK8PuppiJet_G_);
	miniTree->Branch("phoCalibEt", &phoCalibEt_);
	miniTree->Branch("phoEta", &phoEta_);
	miniTree->Branch("phoPhi", &phoPhi_);
	miniTree->Branch("phoCalibE", &phoCalibE_);
	miniTree->Branch("AK8PuppiJet_Pt", &AK8PuppiJet_Pt_);
	miniTree->Branch("AK8PuppiJet_Eta", &AK8PuppiJet_Eta_);
	miniTree->Branch("AK8PuppiJet_Phi", &AK8PuppiJet_Phi_);
	miniTree->Branch("AK8PuppiJet_En", &AK8PuppiJet_En_);
	miniTree->Branch("AK8PuppiJet_tau21", &AK8PuppiJet_tau21_);
	miniTree->Branch("AK8PuppiJet_tau32", &AK8PuppiJet_tau32_);
	miniTree->Branch("AK8PuppiJet_nb1ecfR32", &AK8PuppiJet_nb1ecfR32_);
	miniTree->Branch("AK8PuppiJet_nb2ecfR32", &AK8PuppiJet_nb2ecfR32_);
	miniTree->Branch("AK8PuppiJet_PUPPISoftDropMass", &AK8PuppiJet_PUPPISoftDropMass_);
	miniTree->Branch("AK8PuppiJet_CHSPrunedMass", &AK8PuppiJet_CHSPrunedMass_);
	miniTree->Branch("AK8PuppiJet_CHSSoftDropMass", &AK8PuppiJet_CHSSoftDropMass_);
	miniTree->Branch("AK8PuppiJet_pfBoostedDSVBTag", &AK8PuppiJet_pfBoostedDSVBTag_);
	miniTree->Branch("AK8PuppiJet_CSV", &AK8PuppiJet_CSV_);
	miniTree->Branch("AK8PuppiJet_DDBvLTags_bb", &AK8PuppiJet_DDBvLTags_bb_);
	miniTree->Branch("AK8PuppiJet_DDCvLTags_cc", &AK8PuppiJet_DDCvLTags_cc_);
	miniTree->Branch("AK8PuppiJet_DDCvBTags_bb", &AK8PuppiJet_DDCvBTags_bb_);
	miniTree->Branch("AK8PuppiJet_DDCvBTags_cc", &AK8PuppiJet_DDCvBTags_cc_);
	miniTree->Branch("AK8PuppiJet_DDBDTags_bbcsLight", &AK8PuppiJet_DDBDTags_bbcsLight_);
	miniTree->Branch("AK8PuppiJet_DDBDTags_ccvsLight", &AK8PuppiJet_DDBDTags_ccvsLight_);
	std::cout<<"Created miniTree for limit computation..."<<std::endl;
	///////////////////////////////////////////////////////////////////////////////////

	// inputTTreeReader.SetEntry(-1);

	///////////////////////////////////////////////////////////////////////////////////
	TH1F *h_genWeightSum = (TH1F*) getHistFromFile("sumGenWeights", _file);
	h_genWeightSum->SetDirectory(outFile->GetDirectory(""));
	Double_t _sumGenWeight = h_genWeightSum->GetBinContent(1);
	std::cout<<"\t\tSum of gen weights: "<<_sumGenWeight<<std::endl;
	///////////////////////////////////////////////////////////////////////////////////

	std::cout<<"Analyzing..."<<std::endl;

	while (inputTTreeReader.Next()) {

		ULong64_t current_entry = inputTTreeReader.GetCurrentEntry();
		if(current_entry % REPORT_EVERY == 0){
			std::cout<<"\t\t\t Analyzing entry "<<current_entry<<std::endl;
		}

		run_ = run;
		event_ = event;
		weight_ = isMC ? ((genWeight) * _xSec) : 1.;
		deltaR_AK8PuppiJet_G_ = deltaR_AK8PuppiJetG;
		phoEta_ = phoEta;
		phoPhi_ = phoPhi;
		phoCalibE_ = phoCalibE;
		phoCalibEt_ = phoCalibEt;
		AK8PuppiJet_Pt_ = AK8PuppiJet_Pt;
		AK8PuppiJet_En_ = AK8PuppiJet_En;
		AK8PuppiJet_Eta_ = AK8PuppiJet_Eta;
		AK8PuppiJet_Phi_ = AK8PuppiJet_Phi;
		AK8PuppiJet_Mass_ = AK8PuppiJet_Mass;
		AK8PuppiJet_tau21_ = AK8PuppiJet_tau21;
		AK8PuppiJet_tau32_ = AK8PuppiJet_tau32;
		AK8PuppiJet_nb1ecfR32_ = (AK8PuppiJet_nb1ecf3)/(AK8PuppiJet_nb1ecf2);
		AK8PuppiJet_nb2ecfR32_ = (AK8PuppiJet_nb2ecf3)/(AK8PuppiJet_nb2ecf2);
		AK8PuppiJet_PUPPISoftDropMass_ = AK8PuppiJet_PUPPISoftDropMass;
		AK8PuppiJet_CHSPrunedMass_ = AK8PuppiJet_CHSPrunedMass;
		AK8PuppiJet_CHSSoftDropMass_ = AK8PuppiJet_CHSSoftDropMass;
		AK8PuppiJet_pfBoostedDSVBTag_ = AK8PuppiJet_pfBoostedDSVBTag;
		AK8PuppiJet_CSV_ = AK8PuppiJet_CSV;
		AK8PuppiJet_DDBvLTags_bb_ = (AK8PuppiJet_DDBvLTags_Hbb)/((AK8PuppiJet_DDBvLTags_Hbb) + (AK8PuppiJet_DDBvLTags_qcd));
		AK8PuppiJet_DDCvLTags_cc_ = (AK8PuppiJet_DDCvLTags_Hcc)/((AK8PuppiJet_DDCvLTags_Hcc) + (AK8PuppiJet_DDCvLTags_qcd));
		AK8PuppiJet_DDCvBTags_bb_ = AK8PuppiJet_DDCvBTags_Hbb;
		AK8PuppiJet_DDCvBTags_cc_ = AK8PuppiJet_DDCvBTags_Hcc;
		AK8PuppiJet_DDBDTags_bbcsLight_ = AK8PuppiJet_DDBDTags_bbcsLight;
		AK8PuppiJet_DDBDTags_ccvsLight_ = AK8PuppiJet_DDBDTags_ccvsLight;
		pfMET_ = pfMET;
		pfMETPhi_ = pfMETPhi;

		for(auto & hist :  boostedJetG_1d_Histograms){
			hist.fill(weight_);
		}

		for(auto & hist2D : boostedJetG_2d_Histograms){
			hist2D.fill(weight_);
		}

		miniTree->Fill();

	}

	closeTChain(inputTree);

	outFile->Write();
	outFile->Close();

	std::cout<<"Written results to "<<outFileName<<std::endl;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void eventPostselector(std::string _preSelectedList	="/scratch/mwadud/anTGCpreselector/out/preSelected_wDCremoval_op4/preSelectedListForMiniTree.txt",	std::string	_outDir="/scratch/mwadud/anTGCpreselector/out/preSelected_wDCremoval_op4/miniTree", std::string _xSecMap="/scratch/mwadud/anTGCpreselector/data/meanXsections_flatKfactor1.3.txt"){
	CSVReader preSlectedColumns(_preSelectedList);
	std::vector<std::vector<std::string>> preSelectedBinnedSamples = preSlectedColumns.getData();

	mkdir(_outDir);

	for(std::vector<std::string> preSelectedSamples : preSelectedBinnedSamples){
		std::vector<std::string> preSelectedBins = getNonemptyLines(preSelectedSamples[1]);
		for(std::string & inFile : preSelectedBins){
			postSelect(inFile, _outDir, _xSecMap);
		}
	}

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif
