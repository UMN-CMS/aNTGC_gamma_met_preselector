#ifndef ANTGC_PRESELECTOR_CC
#define ANTGC_PRESELECTOR_CC

#include "eventPreselector.h"
#include "TSystem.h"



aNTGCpreselector::aNTGCpreselector(std::string _file_list, std::string _output_file){
	Char_t _isData =isData(_file_list);
	if(_isData != 0 && _isData != 1) return;
	isMC = (Bool_t) (1 - _isData);

	std::cout<<"File list path: "<<_file_list<<std::endl<<
	"Output file: "<<_output_file<<std::endl<<
	"IsMC: "<<isMC<<std::endl;

	if(file_exists(_file_list)) inputTree =openTChain(_file_list, "ggNtuplizer/EventTree");
	else if(isDirectory(_file_list)) inputTree = openTChainWithFilesInDir(_file_list, "ggNtuplizer/EventTree");
	else {
		std::cout<<"Error! Cannot read given input path/list ("<<_file_list<<")"<<std::endl;
		return;
	}

	isQCD = matchRegex(_file_list, ".*QCD.*");
	isGJets = matchRegex(_file_list, ".*GJets.*");

	if(!inputTree){
		std::cout<<"Error! Could not create input TChain."<<std::endl;
		return;
	}

	outFile = new TFile(_output_file.c_str(), "RECREATE");
	if(!outFile){
		std::cout<<"Error! Could not create output TFile."<<std::endl;
		return;
	}

	gSystem->cd(0);

	h_genWeightSum.SetDirectory(outFile->GetDirectory(""));
	h_genWeightSign.SetDirectory(outFile->GetDirectory(""));
	h_cutFlowEvts.SetDirectory(outFile->GetDirectory(""));
	h_cutFlowWeighted.SetDirectory(outFile->GetDirectory(""));
	h_cutFlowWeightSigns.SetDirectory(outFile->GetDirectory(""));

	inputTTreeReader.SetTree(inputTree);

	initIntputNtuples();

	setGlobalHists();

	setBoostedJetGhists();

	setOutputTree();

	analyze();

	writeGlobalHists();

	// writeBoostedJetGhists();

	writeOutputTree();

	closeTChain(inputTree);

	outFile->Close();

	// std::cout<"Complete!"<<std::endl
	std::cout<<"Complete!"<<std::endl;
};


Char_t aNTGCpreselector::initIntputNtuples(){
	std::cout<<"Initializing branches in input ntuples... "<<std::endl;

	_run.set(inputTTreeReader, "run");
	_event.set(inputTTreeReader, "event");
	_lumis.set(inputTTreeReader, "lumis");
	_isPVGood.set(inputTTreeReader, "isPVGood");
	_HLTPho.set(inputTTreeReader, "HLTPho");

	if(isMC){
		_genWeight.set(inputTTreeReader, "genWeight");
		_mcHasDirectPromptPho.set(inputTTreeReader, "mcHasDirectPromptPho");
	}

	_pfMET.set(inputTTreeReader, "pfMET");
	_pfMETPhi.set(inputTTreeReader, "pfMETPhi");
	_eleCalibPt.set(inputTTreeReader, "eleCalibPt");
	_eleEta.set(inputTTreeReader, "eleEta");
	_elePhi.set(inputTTreeReader, "elePhi");
	_eleIDMVAIso.set(inputTTreeReader, "eleIDMVAIso");
	_eleIDbit.set(inputTTreeReader, "eleIDbit");
	_muPt.set(inputTTreeReader, "muPt");
	_muEta.set(inputTTreeReader, "muEta");
	_muPhi.set(inputTTreeReader, "muPhi");
	_muIDbit.set(inputTTreeReader, "muIDbit");
	_AK4CHSJet_Pt.set(inputTTreeReader, "AK4CHSJet_Pt");
	_AK4CHSJet_En.set(inputTTreeReader, "AK4CHSJet_En");
	_AK4CHSJet_Eta.set(inputTTreeReader, "AK4CHSJet_Eta");
	_AK4CHSJet_Phi.set(inputTTreeReader, "AK4CHSJet_Phi");
	// _AK4CHSJet_ConsituentEtaPhiSpread.set(inputTTreeReader, "AK4CHSJet_ConsituentEtaPhiSpread");
	// _AK4CHSJet_constituentPtDistribution.set(inputTTreeReader, "AK4CHSJet_constituentPtDistribution");
	// _AK4CHSJet_MaxConstituentDistance.set(inputTTreeReader, "AK4CHSJet_MaxConstituentDistance");
	// _AK4CHSJet_CSV2BJetTags.set(inputTTreeReader, "AK4CHSJet_CSV2BJetTags");
	// _AK4CHSJet_DeepCSVTags_b.set(inputTTreeReader, "AK4CHSJet_DeepCSVTags_b");
	// _AK4CHSJet_DeepCSVTags_bb.set(inputTTreeReader, "AK4CHSJet_DeepCSVTags_bb");
	// _AK4CHSJet_DeepCSVTags_c.set(inputTTreeReader, "AK4CHSJet_DeepCSVTags_c");
	// _AK4CHSJet_DeepCSVTags_udsg.set(inputTTreeReader, "AK4CHSJet_DeepCSVTags_udsg");
	// _AK4CHSJet_DeepFlavTags_bb.set(inputTTreeReader, "AK4CHSJet_DeepFlavTags_bb");
	// _AK4CHSJet_DeepFlavTags_bbb.set(inputTTreeReader, "AK4CHSJet_DeepFlavTags_bbb");
	// _AK4CHSJet_DeepFlavTags_lepb.set(inputTTreeReader, "AK4CHSJet_DeepFlavTags_lepb");
	// _AK4CHSJet_DeepFlavTags_c.set(inputTTreeReader, "AK4CHSJet_DeepFlavTags_c");
	// _AK4CHSJet_DeepFlavTags_uds.set(inputTTreeReader, "AK4CHSJet_DeepFlavTags_uds");
	// _AK4CHSJet_DeepFlavTags_g.set(inputTTreeReader, "AK4CHSJet_DeepFlavTags_g");
	// _AK4CHSJet_CombMVA2Tags.set(inputTTreeReader, "AK4CHSJet_CombMVA2Tags");
	// if(isMC){
	// 	_AK4CHSJet_PartonFlavour.set(inputTTreeReader, "AK4CHSJet_PartonFlavour");
	// 	_AK4CHSJet_HadronFlavour.set(inputTTreeReader, "AK4CHSJet_HadronFlavour");
	// }
	_AK4CHSJet_ID.set(inputTTreeReader, "AK4CHSJet_ID");
	_AK4CHSJet_PUID.set(inputTTreeReader, "AK4CHSJet_PUID");
	// _AK4CHSJet_PUFullID.set(inputTTreeReader, "AK4CHSJet_PUFullID");
	_AK4CHSJet_qgLikelihood.set(inputTTreeReader, "AK4CHSJet_qgLikelihood");
	_phoEta.set(inputTTreeReader, "phoEta");
	_phoPhi.set(inputTTreeReader, "phoPhi");
	_phoCalibE.set(inputTTreeReader, "phoCalibE");
	_phoSigmaCalibE.set(inputTTreeReader, "phoSigmaCalibE");
	_phoCalibEt.set(inputTTreeReader, "phoCalibEt");
	_phohasPixelSeed.set(inputTTreeReader, "phohasPixelSeed");
	_phoEleVeto.set(inputTTreeReader, "phoEleVeto");
	_phoR9.set(inputTTreeReader, "phoR9");
	_phoR9Full5x5.set(inputTTreeReader, "phoR9Full5x5");
	_phoIDMVA.set(inputTTreeReader, "phoIDMVA");
	_phoIDbit.set(inputTTreeReader, "phoIDbit");
	_AK8PuppiJet_Charge.set(inputTTreeReader, "AK8PuppiJet_Charge");
	_AK8PuppiJet_Pt.set(inputTTreeReader, "AK8PuppiJet_Pt");
	_AK8PuppiJet_En.set(inputTTreeReader, "AK8PuppiJet_En");
	_AK8PuppiJet_Eta.set(inputTTreeReader, "AK8PuppiJet_Eta");
	_AK8PuppiJet_Phi.set(inputTTreeReader, "AK8PuppiJet_Phi");
	_AK8PuppiJet_Mass.set(inputTTreeReader, "AK8PuppiJet_Mass");
	_AK8PuppiJet_tau1.set(inputTTreeReader, "AK8PuppiJet_tau1");
	_AK8PuppiJet_tau2.set(inputTTreeReader, "AK8PuppiJet_tau2");
	_AK8PuppiJet_tau3.set(inputTTreeReader, "AK8PuppiJet_tau3");
	_AK8PuppiJet_tau4.set(inputTTreeReader, "AK8PuppiJet_tau4");
	_AK8PuppiJet_PFid.set(inputTTreeReader, "AK8PuppiJet_PFid");
	_AK8PuppiJet_PUPPISoftDropMass.set(inputTTreeReader, "AK8PuppiJet_PUPPISoftDropMass");
	_AK8PuppiJet_CHSPrunedMass.set(inputTTreeReader, "AK8PuppiJet_CHSPrunedMass");
	_AK8PuppiJet_CHSSoftDropMass.set(inputTTreeReader, "AK8PuppiJet_CHSSoftDropMass");
	_AK8PuppiJet_pfBoostedDSVBTag.set(inputTTreeReader, "AK8PuppiJet_pfBoostedDSVBTag");
	_AK8PuppiJet_CSV.set(inputTTreeReader, "AK8PuppiJet_CSV");
	_AK8PuppiJet_DDBvLTags_qcd.set(inputTTreeReader, "AK8PuppiJet_DDBvLTags_qcd");
	_AK8PuppiJet_DDBvLTags_Hbb.set(inputTTreeReader, "AK8PuppiJet_DDBvLTags_Hbb");
	_AK8PuppiJet_DDCvLTags_qcd.set(inputTTreeReader, "AK8PuppiJet_DDCvLTags_qcd");
	_AK8PuppiJet_DDCvLTags_Hcc.set(inputTTreeReader, "AK8PuppiJet_DDCvLTags_Hcc");
	_AK8PuppiJet_DDCvBTags_Hbb.set(inputTTreeReader, "AK8PuppiJet_DDCvBTags_Hbb");
	_AK8PuppiJet_DDCvBTags_Hcc.set(inputTTreeReader, "AK8PuppiJet_DDCvBTags_Hcc");
	_AK8PuppiJet_DDBDTags_bbcsLight.set(inputTTreeReader, "AK8PuppiJet_DDBDTags_bbcsLight");
	_AK8PuppiJet_DDBDTags_ccvsLight.set(inputTTreeReader, "AK8PuppiJet_DDBDTags_ccvsLight");
	_AK8PuppiJet_DDBDTags_TvsQCD.set(inputTTreeReader, "AK8PuppiJet_DDBDTags_TvsQCD");
	_AK8PuppiJet_DDBDTags_ZHccvsQCD.set(inputTTreeReader, "AK8PuppiJet_DDBDTags_ZHccvsQCD");
	_AK8PuppiJet_DDBDTags_WvsQCD.set(inputTTreeReader, "AK8PuppiJet_DDBDTags_WvsQCD");
	_AK8PuppiJet_DDBDTags_ZHbbvsQCD.set(inputTTreeReader, "AK8PuppiJet_DDBDTags_ZHbbvsQCD");
	_AK8PuppiJet_nb1ecf2.set(inputTTreeReader, "AK8PuppiJet_nb1ecf2");
	_AK8PuppiJet_nb1ecf3.set(inputTTreeReader, "AK8PuppiJet_nb1ecf3");
	_AK8PuppiJet_nb2ecf2.set(inputTTreeReader, "AK8PuppiJet_nb2ecf2");
	_AK8PuppiJet_nb2ecf3.set(inputTTreeReader, "AK8PuppiJet_nb2ecf3");
	if(isMC){
		_AK8PuppiJet_PartonFlavour.set(inputTTreeReader, "AK8PuppiJet_PartonFlavour");
		_AK8PuppiJet_HadronFlavour.set(inputTTreeReader, "AK8PuppiJet_HadronFlavour");
		_AK8PuppiJet_GenJetEn.set(inputTTreeReader, "AK8PuppiJet_GenJetEn");
		_AK8PuppiJet_GenJetPt.set(inputTTreeReader, "AK8PuppiJet_GenJetPt");
		_AK8PuppiJet_GenJetEta.set(inputTTreeReader, "AK8PuppiJet_GenJetEta");
		_AK8PuppiJet_GenJetPhi.set(inputTTreeReader, "AK8PuppiJet_GenJetPhi");
	}

	std::cout<<"\tBranches initialized!"<<std::endl;

	gSystem->cd(0);
	return 1;
};


void aNTGCpreselector::setGlobalHists(){

	gSystem->cd(0);
};


void aNTGCpreselector::setBoostedJetGhists(){
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

	gSystem->cd(0);

	std::cout<<"\tBoosted Jet+Gamma histograms initialized!"<<std::endl;
};


void aNTGCpreselector::setOutputTree(){
	std::cout<<"Initializing output TTree...";

	outTree = new TTree("preSelected", "aNTGC preselected events");
	outTree->SetDirectory(outFile->GetDirectory(""));
	outTree->SetMaxTreeSize(5000000000);
	outTree->Branch("run", &run_);
	outTree->Branch("event", &event_);
	outTree->Branch("HLTPho", &HLTPho_);
	if(isMC){
		outTree->Branch("genWeight", &genWeight_);
		outTree->Branch("mcHasDirectPromptPho", &mcHasDirectPromptPho_);
	}
	outTree->Branch("leptVeto", &leptVeto_);
	// outTree->Branch("nAK4CHSJets_in_AK8PuppiJet_0p8", &p_nAK4CHSJets_in_AK8PuppiJet_0p8);
	// outTree->Branch("nAK4CHSJets_in_AK8PuppiJet_1p0", &p_nAK4CHSJets_in_AK8PuppiJet_1p0);
	// outTree->Branch("nAK4CHSJets_in_AK8PuppiJet_1p2", &p_nAK4CHSJets_in_AK8PuppiJet_1p2);

	outTree->Branch("deltaR_AK8PuppiJetG", &p_deltaR_AK8PuppiJetG);
	outTree->Branch("phoEta", &phoEta_);
	outTree->Branch("phoPhi", &phoPhi_);
	outTree->Branch("phoCalibE", &phoCalibE_);
	outTree->Branch("phoSigmaCalibE", &phoSigmaCalibE_);
	outTree->Branch("phoCalibEt", &phoCalibEt_);
	outTree->Branch("phohasPixelSeed", &phohasPixelSeed_);
	outTree->Branch("phoEleVeto", &phoEleVeto_);
	outTree->Branch("phoR9", &phoR9_);
	outTree->Branch("phoR9Full5x5", &phoR9Full5x5_);
	outTree->Branch("phoIDMVA", &phoIDMVA_);
	outTree->Branch("phoIDbit", &phoIDbit_);
	outTree->Branch("AK8PuppiJet_Charge", &AK8PuppiJet_Charge_);
	outTree->Branch("AK8PuppiJet_Pt", &AK8PuppiJet_Pt_);
	outTree->Branch("AK8PuppiJet_En", &AK8PuppiJet_En_);
	outTree->Branch("AK8PuppiJet_Eta", &AK8PuppiJet_Eta_);
	outTree->Branch("AK8PuppiJet_Phi", &AK8PuppiJet_Phi_);
	outTree->Branch("AK8PuppiJet_Mass", &AK8PuppiJet_Mass_);
	outTree->Branch("AK8PuppiJet_tau21", &p_AK8PuppiJet_tau21);
	outTree->Branch("AK8PuppiJet_tau32", &p_AK8PuppiJet_tau32);
	outTree->Branch("AK8PuppiJet_tau43", &p_AK8PuppiJet_tau43);
	outTree->Branch("AK8PuppiJet_nb1ecf2", &AK8PuppiJet_nb1ecf2_);
	outTree->Branch("AK8PuppiJet_nb1ecf3", &AK8PuppiJet_nb1ecf3_);
	outTree->Branch("AK8PuppiJet_nb2ecf2", &AK8PuppiJet_nb2ecf2_);
	outTree->Branch("AK8PuppiJet_nb2ecf3", &AK8PuppiJet_nb2ecf3_);
	outTree->Branch("AK8PuppiJet_PFid", &AK8PuppiJet_PFid_);
	outTree->Branch("AK8PuppiJet_PUPPISoftDropMass", &AK8PuppiJet_PUPPISoftDropMass_);
	outTree->Branch("AK8PuppiJet_CHSPrunedMass", &AK8PuppiJet_CHSPrunedMass_);
	outTree->Branch("AK8PuppiJet_CHSSoftDropMass", &AK8PuppiJet_CHSSoftDropMass_);
	outTree->Branch("AK8PuppiJet_pfBoostedDSVBTag", &AK8PuppiJet_pfBoostedDSVBTag_);
	outTree->Branch("AK8PuppiJet_CSV", &AK8PuppiJet_CSV_);
	outTree->Branch("AK8PuppiJet_DDBvLTags_qcd", &AK8PuppiJet_DDBvLTags_qcd_);
	outTree->Branch("AK8PuppiJet_DDBvLTags_Hbb", &AK8PuppiJet_DDBvLTags_Hbb_);
	outTree->Branch("AK8PuppiJet_DDCvLTags_qcd", &AK8PuppiJet_DDCvLTags_qcd_);
	outTree->Branch("AK8PuppiJet_DDCvLTags_Hcc", &AK8PuppiJet_DDCvLTags_Hcc_);
	outTree->Branch("AK8PuppiJet_DDCvBTags_Hbb", &AK8PuppiJet_DDCvBTags_Hbb_);
	outTree->Branch("AK8PuppiJet_DDCvBTags_Hcc", &AK8PuppiJet_DDCvBTags_Hcc_);
	outTree->Branch("AK8PuppiJet_DDBDTags_bbcsLight", &AK8PuppiJet_DDBDTags_bbcsLight_);
	outTree->Branch("AK8PuppiJet_DDBDTags_ccvsLight", &AK8PuppiJet_DDBDTags_ccvsLight_);
	outTree->Branch("AK8PuppiJet_DDBDTags_TvsQCD", &AK8PuppiJet_DDBDTags_TvsQCD_);
	outTree->Branch("AK8PuppiJet_DDBDTags_ZHccvsQCD", &AK8PuppiJet_DDBDTags_ZHccvsQCD_);
	outTree->Branch("AK8PuppiJet_DDBDTags_WvsQCD", &AK8PuppiJet_DDBDTags_WvsQCD_);
	outTree->Branch("AK8PuppiJet_DDBDTags_ZHbbvsQCD", &AK8PuppiJet_DDBDTags_ZHbbvsQCD_);
	if(isMC){
		outTree->Branch("AK8PuppiJet_PartonFlavour", &AK8PuppiJet_PartonFlavour_);
		outTree->Branch("AK8PuppiJet_HadronFlavour", &AK8PuppiJet_HadronFlavour_);
		outTree->Branch("AK8PuppiJet_GenJetEn", &AK8PuppiJet_GenJetEn_);
		outTree->Branch("AK8PuppiJet_GenJetPt", &AK8PuppiJet_GenJetPt_);
		outTree->Branch("AK8PuppiJet_GenJetEta", &AK8PuppiJet_GenJetEta_);
		outTree->Branch("AK8PuppiJet_GenJetPhi", &AK8PuppiJet_GenJetPhi_);
	}
	outTree->Branch("pfMET", &pfMET_);
	outTree->Branch("pfMETPhi", &pfMETPhi_);
	// outTree->Branch("eleCalibPt", &eleCalibPt_);
	// outTree->Branch("eleIDMVAIso", &eleIDMVAIso_);
	// outTree->Branch("eleIDbit", &eleIDbit_);
	// outTree->Branch("muPt", &muPt_);
	// outTree->Branch("muIDbit", &muIDbit_);
	outTree->Branch("AK4CHSJet_Pt", &AK4CHSJet_Pt_);
	outTree->Branch("AK4CHSJet_En", &AK4CHSJet_En_);
	outTree->Branch("AK4CHSJet_Eta", &AK4CHSJet_Eta_);
	outTree->Branch("AK4CHSJet_Phi", &AK4CHSJet_Phi_);
	// outTree->Branch("AK4CHSJet_ConsituentEtaPhiSpread", &AK4CHSJet_ConsituentEtaPhiSpread_);
	// outTree->Branch("AK4CHSJet_constituentPtDistribution", &AK4CHSJet_constituentPtDistribution_);
	// outTree->Branch("AK4CHSJet_MaxConstituentDistance", &AK4CHSJet_MaxConstituentDistance_);
	// outTree->Branch("AK4CHSJet_CSV2BJetTags", &AK4CHSJet_CSV2BJetTags_);
	// outTree->Branch("AK4CHSJet_DeepCSVTags_b", &AK4CHSJet_DeepCSVTags_b_);
	// outTree->Branch("AK4CHSJet_DeepCSVTags_bb", &AK4CHSJet_DeepCSVTags_bb_);
	// outTree->Branch("AK4CHSJet_DeepCSVTags_c", &AK4CHSJet_DeepCSVTags_c_);
	// outTree->Branch("AK4CHSJet_DeepCSVTags_udsg", &AK4CHSJet_DeepCSVTags_udsg_);
	// outTree->Branch("AK4CHSJet_DeepFlavTags_bb", &AK4CHSJet_DeepFlavTags_bb_);
	// outTree->Branch("AK4CHSJet_DeepFlavTags_bbb", &AK4CHSJet_DeepFlavTags_bbb_);
	// outTree->Branch("AK4CHSJet_DeepFlavTags_lepb", &AK4CHSJet_DeepFlavTags_lepb_);
	// outTree->Branch("AK4CHSJet_DeepFlavTags_c", &AK4CHSJet_DeepFlavTags_c_);
	// outTree->Branch("AK4CHSJet_DeepFlavTags_uds", &AK4CHSJet_DeepFlavTags_uds_);
	// outTree->Branch("AK4CHSJet_DeepFlavTags_g", &AK4CHSJet_DeepFlavTags_g_);
	// outTree->Branch("AK4CHSJet_CombMVA2Tags", &AK4CHSJet_CombMVA2Tags_);
	// if(isMC){
	// 	outTree->Branch("AK4CHSJet_PartonFlavour", &AK4CHSJet_PartonFlavour_);
	// 	outTree->Branch("AK4CHSJet_HadronFlavour", &AK4CHSJet_HadronFlavour_);
	// }
	outTree->Branch("AK4CHSJet_ID", &AK4CHSJet_ID_);
	outTree->Branch("AK4CHSJet_PUID", &AK4CHSJet_PUID_);
	// outTree->Branch("AK4CHSJet_PUFullID", &AK4CHSJet_PUFullID_);
	outTree->Branch("AK4CHSJet_qgLikelihood", &AK4CHSJet_qgLikelihood_);
	outTree->Branch("AK4CHSJet_deltaR_selectedAK8PuppiJet", &AK4CHSJet_deltaR_selectedAK8PuppiJet_);



	// outTree->Branch("vetoElePt", &vetoElePt_);
	// outTree->Branch("vetoEleEta", &vetoEleEta_);
	// outTree->Branch("vetoElePhi", &vetoElePhi_);
	// outTree->Branch("vetoMuPt", &vetoMuPt_);
	// outTree->Branch("vetoMuEta", &vetoMuEta_);
	// outTree->Branch("vetoMuPhi", &vetoMuPhi_);

	std::cout<<"\t\t Output tree initialized!"<<std::endl;

	gSystem->cd(0);
};


void aNTGCpreselector::copyEvent(){

	run_ = _run;
	event_ = _event;
	HLTPho_ = _HLTPho;
	if(isMC){
		genWeight_ = _genWeight;
		mcHasDirectPromptPho_ = _mcHasDirectPromptPho;
	}

	Float_t _selectedPhoEta =_phoEta[selectedPhotonIndex];
	Float_t _selectedPhoPhi =_phoPhi[selectedPhotonIndex];
	Float_t _selectedAK8JetEta =_AK8PuppiJet_Eta[selectedAK8PuppiJetIndex];
	Float_t _selectedAK8JetPhi =_AK8PuppiJet_Phi[selectedAK8PuppiJetIndex];

	phoEta_ = _phoEta[selectedPhotonIndex];
	phoPhi_ = _phoPhi[selectedPhotonIndex];
	phoCalibE_ = _phoCalibE[selectedPhotonIndex];
	phoSigmaCalibE_ = _phoSigmaCalibE[selectedPhotonIndex];
	phoCalibEt_ = _phoCalibEt[selectedPhotonIndex];
	phohasPixelSeed_ = _phohasPixelSeed[selectedPhotonIndex];
	phoEleVeto_ = _phoEleVeto[selectedPhotonIndex];
	phoR9_ = _phoR9[selectedPhotonIndex];
	phoR9Full5x5_ = _phoR9Full5x5[selectedPhotonIndex];
	phoIDMVA_ = _phoIDMVA[selectedPhotonIndex];
	phoIDbit_ = _phoIDbit[selectedPhotonIndex];


	AK8PuppiJet_Charge_ = _AK8PuppiJet_Charge[selectedAK8PuppiJetIndex];
	AK8PuppiJet_Pt_ = _AK8PuppiJet_Pt[selectedAK8PuppiJetIndex];
	AK8PuppiJet_En_ = _AK8PuppiJet_En[selectedAK8PuppiJetIndex];
	AK8PuppiJet_Eta_ = _AK8PuppiJet_Eta[selectedAK8PuppiJetIndex];
	AK8PuppiJet_Phi_ = _AK8PuppiJet_Phi[selectedAK8PuppiJetIndex];
	AK8PuppiJet_Mass_ = _AK8PuppiJet_Mass[selectedAK8PuppiJetIndex];
	p_AK8PuppiJet_tau21 = _AK8PuppiJet_tau2[selectedAK8PuppiJetIndex] / _AK8PuppiJet_tau1[selectedAK8PuppiJetIndex];
	p_AK8PuppiJet_tau32 = _AK8PuppiJet_tau3[selectedAK8PuppiJetIndex] / _AK8PuppiJet_tau2[selectedAK8PuppiJetIndex];
	p_AK8PuppiJet_tau43 = _AK8PuppiJet_tau4[selectedAK8PuppiJetIndex] / _AK8PuppiJet_tau3[selectedAK8PuppiJetIndex];
	AK8PuppiJet_nb1ecf2_ = _AK8PuppiJet_nb1ecf2[selectedAK8PuppiJetIndex];
	AK8PuppiJet_nb1ecf3_ = _AK8PuppiJet_nb1ecf3[selectedAK8PuppiJetIndex];
	AK8PuppiJet_nb2ecf2_ = _AK8PuppiJet_nb2ecf2[selectedAK8PuppiJetIndex];
	AK8PuppiJet_nb2ecf3_ = _AK8PuppiJet_nb2ecf3[selectedAK8PuppiJetIndex];
	AK8PuppiJet_PFid_ = _AK8PuppiJet_PFid[selectedAK8PuppiJetIndex];
	AK8PuppiJet_PUPPISoftDropMass_ = _AK8PuppiJet_PUPPISoftDropMass[selectedAK8PuppiJetIndex];
	AK8PuppiJet_CHSPrunedMass_ = _AK8PuppiJet_CHSPrunedMass[selectedAK8PuppiJetIndex];
	AK8PuppiJet_CHSSoftDropMass_ = _AK8PuppiJet_CHSSoftDropMass[selectedAK8PuppiJetIndex];
	AK8PuppiJet_pfBoostedDSVBTag_ = _AK8PuppiJet_pfBoostedDSVBTag[selectedAK8PuppiJetIndex];
	AK8PuppiJet_CSV_ = _AK8PuppiJet_CSV[selectedAK8PuppiJetIndex];
	AK8PuppiJet_DDBvLTags_qcd_ = _AK8PuppiJet_DDBvLTags_qcd[selectedAK8PuppiJetIndex];
	AK8PuppiJet_DDBvLTags_Hbb_ = _AK8PuppiJet_DDBvLTags_Hbb[selectedAK8PuppiJetIndex];
	AK8PuppiJet_DDCvLTags_qcd_ = _AK8PuppiJet_DDCvLTags_qcd[selectedAK8PuppiJetIndex];
	AK8PuppiJet_DDCvLTags_Hcc_ = _AK8PuppiJet_DDCvLTags_Hcc[selectedAK8PuppiJetIndex];
	AK8PuppiJet_DDCvBTags_Hbb_ = _AK8PuppiJet_DDCvBTags_Hbb[selectedAK8PuppiJetIndex];
	AK8PuppiJet_DDCvBTags_Hcc_ = _AK8PuppiJet_DDCvBTags_Hcc[selectedAK8PuppiJetIndex];
	AK8PuppiJet_DDBDTags_bbcsLight_ = _AK8PuppiJet_DDBDTags_bbcsLight[selectedAK8PuppiJetIndex];
	AK8PuppiJet_DDBDTags_ccvsLight_ = _AK8PuppiJet_DDBDTags_ccvsLight[selectedAK8PuppiJetIndex];
	AK8PuppiJet_DDBDTags_TvsQCD_ = _AK8PuppiJet_DDBDTags_TvsQCD[selectedAK8PuppiJetIndex];
	AK8PuppiJet_DDBDTags_ZHccvsQCD_ = _AK8PuppiJet_DDBDTags_ZHccvsQCD[selectedAK8PuppiJetIndex];
	AK8PuppiJet_DDBDTags_WvsQCD_ = _AK8PuppiJet_DDBDTags_WvsQCD[selectedAK8PuppiJetIndex];
	AK8PuppiJet_DDBDTags_ZHbbvsQCD_ = _AK8PuppiJet_DDBDTags_ZHbbvsQCD[selectedAK8PuppiJetIndex];
	if(isMC){
		AK8PuppiJet_PartonFlavour_ = _AK8PuppiJet_PartonFlavour[selectedAK8PuppiJetIndex];
		AK8PuppiJet_HadronFlavour_ = _AK8PuppiJet_HadronFlavour[selectedAK8PuppiJetIndex];
		AK8PuppiJet_GenJetEn_ = _AK8PuppiJet_GenJetEn[selectedAK8PuppiJetIndex];
		AK8PuppiJet_GenJetPt_ = _AK8PuppiJet_GenJetPt[selectedAK8PuppiJetIndex];
		AK8PuppiJet_GenJetEta_ = _AK8PuppiJet_GenJetEta[selectedAK8PuppiJetIndex];
		AK8PuppiJet_GenJetPhi_ = _AK8PuppiJet_GenJetPhi[selectedAK8PuppiJetIndex];
	}


	pfMET_ = _pfMET;
	pfMETPhi_ = _pfMETPhi;

	Int_t _nAk4Jets = _AK4CHSJet_Pt.size();
	// p_nAK4CHSJets_in_AK8PuppiJet_0p8 = 0.;
	// p_nAK4CHSJets_in_AK8PuppiJet_1p0 = 0.;
	// p_nAK4CHSJets_in_AK8PuppiJet_1p2 = 0.;

	p_nAK4CHSJets_40GeV=0.;


	for(Int_t i =0; i < _nAk4Jets; i++){
		if(_AK4CHSJet_Pt[i] < 40.) continue;
		AK4CHSJet_Pt_.push_back(_AK4CHSJet_Pt[i]);
		AK4CHSJet_En_.push_back(_AK4CHSJet_En[i]);
		AK4CHSJet_Eta_.push_back(_AK4CHSJet_Eta[i]);
		AK4CHSJet_Phi_.push_back(_AK4CHSJet_Phi[i]);
		AK4CHSJet_ID_.push_back(_AK4CHSJet_ID[i]);
		AK4CHSJet_PUID_.push_back(_AK4CHSJet_PUID[i]);
		AK4CHSJet_qgLikelihood_.push_back(_AK4CHSJet_qgLikelihood[i]);
		p_nAK4CHSJets_40GeV++;
		// {
		// 	// if(_deltaR_ak4_slectedak8<0.8) p_nAK4CHSJets_in_AK8PuppiJet_0p8++;
		// 	// if(_deltaR_ak4_slectedak8<1.0) p_nAK4CHSJets_in_AK8PuppiJet_1p0++;
		// 	// if(_deltaR_ak4_slectedak8<1.2) p_nAK4CHSJets_in_AK8PuppiJet_1p2++;
		// }
	};
	p_deltaR_AK8PuppiJetG = deltaR(_selectedPhoEta, _selectedPhoPhi, _selectedAK8JetEta, _selectedAK8JetPhi);
};


void aNTGCpreselector::resetVars(){
	if(!isMC) genWeight_ = 1.;
	// eleCalibPt_.clear();
	// eleIDMVAIso_.clear();
	// eleIDbit_.clear();
	// muPt_.clear();
	// muIDbit_.clear();
	AK4CHSJet_Pt_.clear();
	AK4CHSJet_En_.clear();
	AK4CHSJet_Eta_.clear();
	AK4CHSJet_Phi_.clear();
	// AK4CHSJet_ConsituentEtaPhiSpread_.clear();
	// AK4CHSJet_constituentPtDistribution_.clear();
	// AK4CHSJet_MaxConstituentDistance_.clear();
	// AK4CHSJet_CSV2BJetTags_.clear();
	// AK4CHSJet_DeepCSVTags_b_.clear();
	// AK4CHSJet_DeepCSVTags_bb_.clear();
	// AK4CHSJet_DeepCSVTags_c_.clear();
	// AK4CHSJet_DeepCSVTags_udsg_.clear();
	// AK4CHSJet_DeepFlavTags_bb_.clear();
	// AK4CHSJet_DeepFlavTags_bbb_.clear();
	// AK4CHSJet_DeepFlavTags_lepb_.clear();
	// AK4CHSJet_DeepFlavTags_c_.clear();
	// AK4CHSJet_DeepFlavTags_uds_.clear();
	// AK4CHSJet_DeepFlavTags_g_.clear();
	// AK4CHSJet_CombMVA2Tags_.clear();
	// AK4CHSJet_PartonFlavour_.clear();
	// AK4CHSJet_HadronFlavour_.clear();
	AK4CHSJet_ID_.clear();
	AK4CHSJet_PUID_.clear();
	// AK4CHSJet_PUFullID_.clear();
	AK4CHSJet_qgLikelihood_.clear();
	AK4CHSJet_deltaR_selectedAK8PuppiJet_.clear();


	lastCutStep = 0.5;

	leptVeto_ = 0;

	vetoElePt_ = -999.;
	vetoEleEta_ = -999.;
	vetoElePhi_ = -999.;
	vetoMuPt_ = -999.;
	vetoMuEta_ = -999.;
	vetoMuPhi_ = -999.;
};


void aNTGCpreselector::fillGlobalHists(){
	h_genWeightSum.Fill(0.5, genWeight_);
	h_genWeightSign.Fill(0.5, genWeight_/std::abs(genWeight_));
};


void aNTGCpreselector::fillBoostedJetGhists(){
	for(auto & hist :  boostedJetG_1d_Histograms){
		hist.fill(genWeight_);
	}

	for(auto & hist2D : boostedJetG_2d_Histograms){
		hist2D.fill(genWeight_);
	}
};


void aNTGCpreselector::writeGlobalHists(){
};


void aNTGCpreselector::writeBoostedJetGhists(){
	outFile->cd();
	for(auto & it : boostedJetG_1d_Histograms){
		std::cout<<"\t\t Writing Boosted Jet+G 1D histogram "<<it.hist->GetName()<<std::endl;
		it.hist->Write();
	}

	for(auto & it : boostedJetG_2d_Histograms){
		std::cout<<"\t\t Writing Boosted Jet+G 2D histogram "<<it.hist->GetName()<<std::endl;
		it.hist->Write();
	}
	gSystem->cd(0);
};


void aNTGCpreselector::writeOutputTree(){
	outFile->cd();
	outTree->GetCurrentFile()->Write();
	std::string _outfilename = outFile->GetName();
	std::cout<<"\tOutput tree written to file "<<_outfilename<<std::endl;
	gSystem->cd(0);
};


Bool_t aNTGCpreselector::selectBoostedJetGevent(){
	lastCutStep = 0.5;
	registerCutFlow();

	// Double counting removal for QCD & GJets
	if(isGJets || isQCD){
		Char_t eventHasDirectPromptPho_ = isMC ? (_mcHasDirectPromptPho) : 0;
		Bool_t hasDirectPomptPhoBit = getBit((eventHasDirectPromptPho_), 0);
		if(isGJets && !hasDirectPomptPhoBit) return 0;
		if(isQCD && hasDirectPomptPhoBit) return 0;
	}
	registerCutFlow();

	// 200GeV photon trigger
	ULong64_t photTriggers = _HLTPho;
	if(!getBit(photTriggers, 9)) return 0;
	registerCutFlow();


	// At least one photon
	Int_t _nPhotons = _phoCalibEt.size();
	if(_nPhotons < 1) return 0;
	registerCutFlow();


	// At least one fat jet
	Int_t _nAK8PuppiJets = _AK8PuppiJet_Pt.size();
	if(_nAK8PuppiJets < 1) return 0;
	registerCutFlow();


	// Get highest pT photon
	// 220 GeV pT cut
	// MVA 80% efficiency ID
	// Conversion safe electron veto
	// |eta| < 2.5
	// Exclude B-E transition region
	Int_t _220GeVphoCounter = 0;
	selectedPhotonIndex = -999;
	Float_t _highestPtPhoton = -999.;
	for(Int_t i = 0; i < _nPhotons; i++){
		Float_t _candPhoPt = _phoCalibEt[i];
		if(_candPhoPt < 220.) continue;
		Float_t _candPhoAbsEta = std::abs(_phoEta[i]);
		if(_candPhoAbsEta > 2.5 ) continue;
		if((_candPhoAbsEta > BETRetaMin) && (_candPhoAbsEta < BETRetaMax)) continue;
		UChar_t _phoID = _phoIDbit[i];
		if(!getBit(_phoID, 4)) continue;
		Int_t _phoPassEleVeto = _phoEleVeto[i];
		if(_phoPassEleVeto<1) continue;
		_220GeVphoCounter++;
		if(_candPhoPt > _highestPtPhoton){
			_highestPtPhoton =_candPhoPt;
			selectedPhotonIndex = i;
		}
	}
	if(selectedPhotonIndex < 0) return 0;
	registerCutFlow();


	// Not more than one candidate photon
	if(_220GeVphoCounter > 1) return 0;
	registerCutFlow();


	// Get highest pT AK8PuppiJet with dR > 1.0 with photon
	// 200 GeV pT cut
	// Tight PD ID
	// |eta| < 2.5
	// Mass Within 30 GeV of Z mass
	Int_t _200GeVak8JetCounter = 0;
	selectedAK8PuppiJetIndex = -999;
	Float_t _highestPtAK8Jet = -999.;
	for(Int_t i = 0; i < _nAK8PuppiJets; i++){
		if((_AK8PuppiJet_Pt[i]) < 200.) continue;
		if(std::abs((_AK8PuppiJet_Eta[i])) > 2.5) continue;
		if(!getBit((_AK8PuppiJet_PFid[i]),1)) continue;
		if(std::abs((_AK8PuppiJet_Mass[i]) - ZMASS) > 50.) continue;
		_200GeVak8JetCounter++;
		if((_AK8PuppiJet_Pt[i]) > _highestPtAK8Jet){
			_highestPtAK8Jet =(_AK8PuppiJet_Pt[i]);
			selectedAK8PuppiJetIndex = i;
		}
	}
	if(selectedAK8PuppiJetIndex < 0) return 0;
	registerCutFlow();


	// Not more than one ak8Jet with pT > 200 GeV
	if(_200GeVak8JetCounter > 1) return 0;
	registerCutFlow();


	// deltaR(photon, ak8puppiJet) > 1.0
	Float_t _selectedPhoEta =_phoEta[selectedPhotonIndex];
	Float_t _selectedPhoPhi =_phoPhi[selectedPhotonIndex];
	Float_t _selectedAK8JetEta =_AK8PuppiJet_Eta[selectedAK8PuppiJetIndex];
	Float_t _selectedAK8JetPhi =_AK8PuppiJet_Phi[selectedAK8PuppiJetIndex];
	Float_t deltaR_pho_ak8Jet = deltaR(_selectedPhoEta, _selectedPhoPhi, _selectedAK8JetEta, _selectedAK8JetPhi);
	if(deltaR_pho_ak8Jet < 1.0) return 0;
	registerCutFlow();


	// No electrons with pT > 50 GeV outside deltaR < 1.0 of selected ak8jet
	// tight ID
	leptVeto_ = 0;
	Bool_t eleVeto_ = 0;
	Int_t _nElectrons = _eleCalibPt.size();
	for(Int_t i = 0; i < _nElectrons; i++){
		if((_eleCalibPt[i]) < 50.) continue;
		if(!getBit((_eleIDbit[i]), 3)) continue;
		Float_t _dR_ele_jet = deltaR(_eleEta[i], _elePhi[i], _selectedAK8JetEta, _selectedAK8JetPhi);
		if(_dR_ele_jet > 2.0) {
			eleVeto_ = 1;
			// vetoElePt_ = _eleCalibPt[i];
			// vetoEleEta_ = _eleEta[i];
			// vetoElePhi_ = _elePhi[i];
			setBit(leptVeto_, 0, 1);
			break;
		}
	}
	if(eleVeto_ == 0){
		registerCutFlow();
	}

	// No muons with pT > 50 GeV outside deltaR < 1.0 of selected ak8jet
	// tight ID
	Bool_t muVeto_ = 0;
	Int_t _nMuons = _muPt.size();
	for(Int_t i = 0; i < _nMuons; i++){
		if((_muPt[i]) < 50.) continue;
		if(!getBit((_muIDbit[i]),9)) continue;
		Float_t _dR_muon_jet = deltaR((_muEta[i]), (_muPhi[i]), _selectedAK8JetEta, _selectedAK8JetPhi);
		if(_dR_muon_jet > 2.0) {
			muVeto_ = 1;
			// vetoMuPt_ = _muPt[i];
			// vetoMuEta_ = _muEta[i];
			// vetoMuPhi_ = _muPhi[i];
			setBit(leptVeto_, 1, 1);
			break;
		}
	}
	if(muVeto_ == 0){
		registerCutFlow();
	}

	return 1;
};


void aNTGCpreselector::analyze(){
	std::cout<<"************************************************************************************************************"<<std::endl<<
	"Analyzing events.."<<std::endl<<
	"************************************************************************************************************"<<std::endl;

	while (inputTTreeReader.Next()) {
		resetVars();
		ULong64_t current_entry = inputTTreeReader.GetCurrentEntry();
		if(current_entry % REPORT_EVERY == 0){
			std::cout<<"\t\t\t Analyzing entry "<<current_entry<<std::endl;
		}
		genWeight_ = isMC ? _genWeight : 1.;
		fillGlobalHists();
		if(!selectBoostedJetGevent()) continue;
		copyEvent();
		fillBoostedJetGhists();
		outTree->Fill();
	}
};


void aNTGCpreselector::registerCutFlow(){
	h_cutFlowEvts.Fill(lastCutStep);
	h_cutFlowWeighted.Fill(lastCutStep, genWeight_);
	h_cutFlowWeightSigns.Fill(lastCutStep, genWeight_/std::abs(genWeight_));
	lastCutStep = lastCutStep + 1.;
};


Char_t aNTGCpreselector::isData(std::string _infilespath){
	std::vector<std::string> _filePaths;
	if(file_exists(_infilespath)){
		_filePaths = getNonemptyLines(_infilespath);
	} else{
		_filePaths = listFilesInDir(_infilespath, ".*\.root");
	}

	if(_filePaths.empty()){
		std::cout<<"Error! Cannot find input files specified by "<<_infilespath<<std::endl;
		return -1;
	}

	Char_t _isData_ = 0;
	Char_t _isMC_ = 0;
	for(auto _file : _filePaths){
		if(matchRegex(_file, ".*_data.*\.root")) _isData_ = 1;
		if(matchRegex(_file, ".*_mc.*\.root")) _isMC_ = 1;
	}

	if(_isData_ * _isMC_) {
		std::cout<<"Error! Both data and MC files are in "<<_infilespath<<std::endl;
		return -2;
	}

	return  _isData_;
};

#endif
