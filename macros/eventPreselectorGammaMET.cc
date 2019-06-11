#ifndef ANTGC_Gamma_MET_PRESELECTOR_CC
#define ANTGC_Gamma_MET_PRESELECTOR_CC

#include "eventPreselectorGammaMET.h"
#include "TSystem.h"

aNTGCpreselector::aNTGCpreselector(std::string _file_list, std::string _output_file, std::string _mcPUfile, std::string _dataPUfile){

	Char_t _isData =isData(_file_list);
	if(_isData != 0 && _isData != 1) return;
	isMC = (Bool_t) (1 - _isData);

	std::cout<<getCurrentTime()<<std::endl<<
	"File list path: "<<_file_list<<std::endl<<
	"Output file: "<<_output_file<<std::endl<<
	"IsMC: "<<isMC<<std::endl;

	if(file_exists(_file_list)) inputTree =openTChain(_file_list, "ggNtuplizer/EventTree");
	// else if(isDirectory(_file_list)) inputTree = openTChainWithFilesInDir(_file_list, "ggNtuplizer/EventTree");
	else {
		std::cout<<"Error! Cannot read given input path/list ("<<_file_list<<")"<<std::endl;
		return;
	}

	isQCD = matchRegex(_file_list, ".*QCD.*");
	isGJets = (matchRegex(_file_list, ".*GJets.*") && !matchRegex(_file_list, ".*TGJets.*"));

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

	if(_isData){
		genWeight_ 		= 1.;
		eventWeight_ 	= 1.;
	}

	if(isMC) initPileupReweighter(_mcPUfile, _dataPUfile);

	initIntputNtuples();

	setGlobalHists();

	setGammaMEThists();

	setOutputTree();

	analyze();

	outFile->Write();

	outFile->Close();

	closeTChain(inputTree);

	std::cout<<"\tOutput written to file\t"<<_output_file <<std::endl<<
	"Complete!"<<std::endl<<
	getCurrentTime()<<std::endl;
};


Char_t aNTGCpreselector::initIntputNtuples(){
	std::cout<<"*******************************************************************************"<<std::endl<<
	"Initializing branches in input ntuples... "<<std::endl;

	_run.set(inputTTreeReader, "run");
	_event.set(inputTTreeReader, "event");
	_lumis.set(inputTTreeReader, "lumis");
	_isPVGood.set(inputTTreeReader, "isPVGood");
	_HLTPho.set(inputTTreeReader, "HLTPho");
	_nVtx.set(inputTTreeReader, "nVtx");
	if(isMC){
		_genWeight.set(inputTTreeReader, "genWeight");
		_puTrue.set(inputTTreeReader, "puTrue");
		_mcHasDirectPromptPho.set(inputTTreeReader, "mcHasDirectPromptPho");
		_mcStatus.set(inputTTreeReader, "mcStatus");
		_mcEta.set(inputTTreeReader, "mcEta");
		_mcPhi.set(inputTTreeReader, "mcPhi");
		_mcPt.set(inputTTreeReader, "mcPt");
		_mcE.set(inputTTreeReader, "mcE");
		_mcIndex.set(inputTTreeReader, "mcIndex");
		_mcStatusFlag.set(inputTTreeReader, "mcStatusFlag");
		_genMET.set(inputTTreeReader, "genMET");
		_genMETPhi.set(inputTTreeReader, "genMETPhi");
		_pho_gen_index.set(inputTTreeReader, "pho_gen_index");
	}

	_nPho.set(inputTTreeReader, "nPho");
	_phoEta.set(inputTTreeReader, "phoEta");
	_phoPhi.set(inputTTreeReader, "phoPhi");
	_phoCalibE.set(inputTTreeReader, "phoCalibE");
	_phoSigmaCalibE.set(inputTTreeReader, "phoSigmaCalibE");
	_phoCalibEt.set(inputTTreeReader, "phoCalibEt");
	_phohasPixelSeed.set(inputTTreeReader, "phohasPixelSeed");
	_phoEleVeto.set(inputTTreeReader, "phoEleVeto");
	_phoR9.set(inputTTreeReader, "phoR9");
	_phoR9Full5x5.set(inputTTreeReader, "phoR9Full5x5");
	_phoSigmaIEtaIEtaFull5x5.set(inputTTreeReader, "phoSigmaIEtaIEtaFull5x5");
	_phoSigmaIEtaIPhiFull5x5.set(inputTTreeReader, "phoSigmaIEtaIPhiFull5x5");
	_phoSigmaIPhiIPhiFull5x5.set(inputTTreeReader, "phoSigmaIPhiIPhiFull5x5");
	_phoIDMVA.set(inputTTreeReader, "phoIDMVA");
	_phoIDbit.set(inputTTreeReader, "phoIDbit");
	_phoSeedTime.set(inputTTreeReader, "phoSeedTime");

	_metFilters.set(inputTTreeReader, "metFilters");
	_pfMET.set(inputTTreeReader, "pfMET");
	_pfMETPhi.set(inputTTreeReader, "pfMETPhi");
	_pfMET_metSig.set(inputTTreeReader, "pfMET_metSig");
	_pfMET_EtSig.set(inputTTreeReader, "pfMET_EtSig");

	_nEle.set(inputTTreeReader, "nEle");
	_eleCalibPt.set(inputTTreeReader, "eleCalibPt");
	_eleEta.set(inputTTreeReader, "eleEta");
	_elePhi.set(inputTTreeReader, "elePhi");
	_eleIDMVAIso.set(inputTTreeReader, "eleIDMVAIso");
	_eleIDbit.set(inputTTreeReader, "eleIDbit");

	_nMu.set(inputTTreeReader, "nMu");
	_muPt.set(inputTTreeReader, "muPt");
	_muEta.set(inputTTreeReader, "muEta");
	_muPhi.set(inputTTreeReader, "muPhi");
	_muIDbit.set(inputTTreeReader, "muIDbit");

	_nAK4CHSJet.set(inputTTreeReader, "nAK4CHSJet");
	_AK4CHSJet_Pt.set(inputTTreeReader, "AK4CHSJet_Pt");
	_AK4CHSJet_En.set(inputTTreeReader, "AK4CHSJet_En");
	_AK4CHSJet_Eta.set(inputTTreeReader, "AK4CHSJet_Eta");
	_AK4CHSJet_Phi.set(inputTTreeReader, "AK4CHSJet_Phi");
	_AK4CHSJet_ID.set(inputTTreeReader, "AK4CHSJet_ID");
	_AK4CHSJet_PUID.set(inputTTreeReader, "AK4CHSJet_PUID");
	_AK4CHSJet_PUFullID.set(inputTTreeReader, "AK4CHSJet_PUFullID");

	std::cout<<"\tBranches initialized!"<<std::endl<<
	"*******************************************************************************"<<std::endl;

	return 1;
};


void aNTGCpreselector::setGlobalHists(){
	h_genWeightSum.SetDirectory(outFile->GetDirectory(""));
	h_genWeightSignSum.SetDirectory(outFile->GetDirectory(""));
	h_eventWeightSum.SetDirectory(outFile->GetDirectory(""));
	h_cutFlowEvts.SetDirectory(outFile->GetDirectory(""));
	h_cutFlowWeighted.SetDirectory(outFile->GetDirectory(""));
	h_cutFlowWeightSigns.SetDirectory(outFile->GetDirectory(""));
	h_nVtxPrereweight.SetDirectory(outFile->GetDirectory(""));

	if(isMC){
		h_pileupPrereweight.SetDirectory(outFile->GetDirectory(""));
		h_pileupReweighted.SetDirectory(outFile->GetDirectory(""));
		h_nVtxReweighted.SetDirectory(outFile->GetDirectory(""));
	}
};


void aNTGCpreselector::setGammaMEThists(){
	std::cout<<"Initializing TH1D for Gamma + MET channel..."<<std::endl;
	for(auto & hist1d : GammaMET_1d_reco_histograms){
		hist1d.initializehist();
		hist1d.hist->SetDirectory(outFile->GetDirectory(""));
	}

	std::cout<<"Initializing TH2D for Gamma + MET channel..."<<std::endl;
	for(auto & hist2d : GammaMET_2d_Histograms){
		hist2d.initializehist();
		hist2d.hist->SetDirectory(outFile->GetDirectory(""));
	}

	if(isMC){
		std::cout<<"Initializing gen TH1D for Gamma + MET channel..."<<std::endl;
		for(auto & hist1d : GammaMET_1d_gen_histograms){
			hist1d.initializehist();
			hist1d.hist->SetDirectory(outFile->GetDirectory(""));
		}


		std::cout<<"Initializing gen TH2D for Gamma + MET channel..."<<std::endl;
		for(auto & hist2d : GammaMET_2d_gen_Histograms){
			hist2d.initializehist();
			hist2d.hist->SetDirectory(outFile->GetDirectory(""));
		}
	}

	std::cout<<"\tBoosted Gamma + MET histograms initialized!"<<std::endl;
};


void aNTGCpreselector::setOutputTree(){
	std::cout<<"Initializing output TTree...";

	outTree = new TTree("preSelected", "aNTGC Gamma + MET preselected events");
	outTree->SetDirectory(outFile->GetDirectory(""));
	outTree->SetMaxTreeSize(5000000000);
	outTree->Branch("run", &run_);
	outTree->Branch("event", &event_);
	outTree->Branch("HLTPho", &HLTPho_);

	if(isMC){
		outTree->Branch("genWeight", &genWeight_);
		outTree->Branch("puWeight", &puWeight_);
		outTree->Branch("eventWeight", &eventWeight_);
		outTree->Branch("puTrue", &puTrue_);
		outTree->Branch("mcHasDirectPromptPho", &mcHasDirectPromptPho_);
	}

	outTree->Branch("phoEta", &phoEta_);
	outTree->Branch("phoPhi", &phoPhi_);
	outTree->Branch("phoCalibE", &phoCalibE_);
	outTree->Branch("phoSigmaCalibE", &phoSigmaCalibE_);
	outTree->Branch("phoCalibEt", &phoCalibEt_);
	outTree->Branch("phoR9", &phoR9_);
	outTree->Branch("phoR9Full5x5", &phoR9Full5x5_);
	outTree->Branch("phoSigmaIEtaIEtaFull5x5", &phoSigmaIEtaIEtaFull5x5_);
	outTree->Branch("phoSigmaIEtaIPhiFull5x5", &phoSigmaIEtaIPhiFull5x5_);
	outTree->Branch("phoSigmaIPhiIPhiFull5x5", &phoSigmaIPhiIPhiFull5x5_);

	if(isMC){
		outTree->Branch("genPhoPt", &genPhoPt_);
		outTree->Branch("genPhoEta", &genPhoEta_);
		outTree->Branch("genPhoPhi", &genPhoPhi_);
		outTree->Branch("genPhoEn", &genPhoEn_);
	}

	outTree->Branch("pfMET", &pfMET_);
	outTree->Branch("pfMETPhi", &pfMETPhi_);
	outTree->Branch("pfMET_metSig", &pfMET_metSig_);
	outTree->Branch("pfMET_EtSig", &pfMET_EtSig_);
	outTree->Branch("deltaPhi_phoMET", &deltaPhi_phoMET_);

	if(isMC){
		outTree->Branch("genMET", &genMET_);
		outTree->Branch("genMETPhi", &genMETPhi_);
		outTree->Branch("deltaPhi_gen_phoMET", &deltaPhi_gen_phoMET_);

	}

	std::cout<<"\t\t Output tree initialized!"<<std::endl;
};


void aNTGCpreselector::copyEvent(){

	run_ = _run;
	event_ = _event;
	HLTPho_ = _HLTPho;

	phoEta_ = _phoEta[selectedPhotonIndex];
	phoPhi_ = _phoPhi[selectedPhotonIndex];
	phoCalibE_ = _phoCalibE[selectedPhotonIndex];
	phoSigmaCalibE_ = _phoSigmaCalibE[selectedPhotonIndex];
	phoCalibEt_ = _phoCalibEt[selectedPhotonIndex];
	phoR9_ = _phoR9[selectedPhotonIndex];
	phoR9Full5x5_ = _phoR9Full5x5[selectedPhotonIndex];
	phoSigmaIEtaIEtaFull5x5_ = _phoSigmaIEtaIEtaFull5x5[selectedPhotonIndex];
	phoSigmaIEtaIPhiFull5x5_ = _phoSigmaIEtaIPhiFull5x5[selectedPhotonIndex];
	phoSigmaIPhiIPhiFull5x5_ = _phoSigmaIPhiIPhiFull5x5[selectedPhotonIndex];

	pfMET_ = _pfMET;
	pfMETPhi_ = _pfMETPhi;
	pfMET_metSig_ = _pfMET_metSig;
	pfMET_EtSig_ = _pfMET_EtSig;

	if(isMC){
		mcHasDirectPromptPho_ = _mcHasDirectPromptPho;
		Short_t modifiedPhoGenIndex = findSecondaryIndex((_pho_gen_index[selectedPhotonIndex]), (_mcIndex));
		if((modifiedPhoGenIndex<0)){
			genPhoPt_ 	= -9999.;
			genPhoEta_ 	= -9999.;
			genPhoPhi_ 	= -9999.;
			genPhoEn_ 	= -9999.;
			deltaPhi_gen_phoMET_ = -9999.;
		} else{
			genPhoPt_ 	= _mcPt[modifiedPhoGenIndex];
			genPhoEta_ 	= _mcEta[modifiedPhoGenIndex];
			genPhoPhi_ 	= _mcPhi[modifiedPhoGenIndex];
			genPhoEn_ 	= _mcE[modifiedPhoGenIndex];
			deltaPhi_gen_phoMET_ = deltaPhi(genPhoPhi_, (_genMETPhi));
		}

		genMET_ = _genMET;
		genMETPhi_ = _genMETPhi;

		p_RECOoverGEN_MET = pfMET_/genMET_;
	}
};


void aNTGCpreselector::fillGlobalHists(){
	h_genWeightSum.Fill(0.5, genWeight_);
	h_genWeightSignSum.Fill(0.5, genWeight_/std::abs(genWeight_));
	h_eventWeightSum.Fill(0.5, eventWeight_);
	h_nVtxPrereweight.Fill(p_nVtx);
	if(isMC){
		h_pileupPrereweight.Fill((Float_t)(puTrue_)+0.01);
		h_pileupReweighted.Fill((Float_t)(puTrue_)+0.01, puWeight_);
		h_nVtxReweighted.Fill(p_nVtx + 0.01, puWeight_);
	}
};


void aNTGCpreselector::fillGammaMEThists(){
	for(auto & hist :  GammaMET_1d_reco_histograms){
		hist.fill(eventWeight_);
	}

	for(auto & hist2D : GammaMET_2d_Histograms){
		hist2D.fill(eventWeight_);
	}

	if(isMC){
		for(auto & hist1D : GammaMET_1d_gen_histograms){
			hist1D.fill(eventWeight_);
		}

		for(auto & hist2D : GammaMET_2d_gen_Histograms){
			hist2D.fill(eventWeight_);
		}
	}


};


Bool_t aNTGCpreselector::selectGammaMETevent(){


	lastCutStep = 0.5;


	registerCutFlow();

	if(!_isPVGood) return 0;

	registerCutFlow();



	// // Double counting removal for QCD & GJets
	// if(isGJets || isQCD){
	// 	Char_t eventHasDirectPromptPho_ = isMC ? (_mcHasDirectPromptPho) : 0;
	// 	Bool_t hasDirectPomptPhoBit = getBit((eventHasDirectPromptPho_), 0);
	// 	if(isGJets && !hasDirectPomptPhoBit) return 0;
	// 	if(isQCD && hasDirectPomptPhoBit ) return 0;
	// }
	// registerCutFlow();


	// 200GeV photon trigger

	ULong64_t tmp_HLTPho = (_HLTPho);
	if(!getBit(tmp_HLTPho, 19)) return 0;// 9 = HLT_Photon200_v, 11 = HLT_Photon300_NoHE_v, 19 = HLT_Photon135_PFMET100_v

	registerCutFlow();




	// At least one photon

	if((_nPho) < 1) return 0;

	registerCutFlow();



	//MET pT cut
	if((_pfMET) < 150.) return 0;

	registerCutFlow();



	//MET filters
	UShort_t metFilters = (_metFilters);
	if(getBit(metFilters, 0)) return 0;			//Flag_goodVertices
	registerCutFlow();

	if(getBit(metFilters, 1)) return 0;			//Flag_globalSuperTightHalo2016Filter
	registerCutFlow();
	if(getBit(metFilters, 2)) return 0;			//Flag_HBHENoiseFilter
	registerCutFlow();
	if(getBit(metFilters, 3)) return 0;			//Flag_HBHENoiseIsoFilter
	registerCutFlow();
	if(getBit(metFilters, 4)) return 0;			//Flag_EcalDeadCellTriggerPrimitiveFilter
	registerCutFlow();
	if(getBit(metFilters, 5)) return 0;			//Flag_BadPFMuonFilter
	registerCutFlow();
	// if(getBit(metFilters, 6)) return 0; 		// Flag_BadChargedCandidateFilter
	// if(getBit(metFilters, 7)) return 0;		//Flag_eeBadScFilter
	// if(getBit(metFilters, 8)) return 0;		//Flag_ecalBadCalibFilter - deprecated
	if(getBit(metFilters, 9)) return 0;			//Updated ecalBadCalibFilter
	registerCutFlow();



	// Get highest pT photon
	// 220 GeV pT cut
	// MVA 80% efficiency ID
	// Conversion safe electron veto
	// |eta| < 2.5
	// Exclude B-E transition region

	Short_t _220GeVphoCounter = 0;

	selectedPhotonIndex = -999;

	Float_t highestPtPhotonPt = -999.;


	for(Int_t i = 0; i < (_nPho); i++){

		Float_t candPhoPt =_phoCalibEt[i];

		if(candPhoPt < 220.) continue;

		Float_t candPhoAbsEta = std::abs(_phoEta[i]);

		if(candPhoAbsEta > 2.5 ) continue;

		if((candPhoAbsEta > BETRetaMin) && (candPhoAbsEta < BETRetaMax)) continue;

		UChar_t candPhoIDbit= (_phoIDbit[i]);
		if(!getBit(candPhoIDbit, 3)) continue;

		if(!(_phoEleVeto[i])) continue;

		_220GeVphoCounter++;

		if(candPhoPt > selectedPhotonIndex){
			selectedPhotonIndex = candPhoPt;

			selectedPhotonIndex = i;

		}

	}

	if(selectedPhotonIndex < 0) return 0;

	registerCutFlow();



	// delta phi cut of 2. between selected photon and MET
	deltaPhi_phoMET_ = deltaPhi((_pfMETPhi), (_phoPhi[selectedPhotonIndex]));

	if(deltaPhi_phoMET_ < 2.) return 0;

	registerCutFlow();



	// Electron rejection
	for(Int_t i = 0; i < (_nEle); i++){

		if((_eleCalibPt[i]) < 10.) continue;

		Char_t tmpeleIDbit = _eleIDbit[i];
		if(!getBit(tmpeleIDbit, 0)) continue; 		// veto ID

		return 0;

	}
	registerCutFlow();



	// Muon rejection
	for(Int_t i = 0; i < (_nMu); i++){

		if((_muPt[i]) < 10.) continue;

		Int_t tmpmuIDbit = _muIDbit[i];
		if(!getBit(tmpmuIDbit, 3) || !getBit((_muIDbit[i]), 11)) continue;		// cut based tight ID && loose tracker iso ID

		return 0;

	}

	registerCutFlow();



	// Jet rejection
	UShort_t jetCounter = 0;
	for(Int_t i = 0; i < (_nAK4CHSJet); i++){

		if((_AK4CHSJet_Pt[i]) < 30.) continue;

		Char_t tmpJetPUIDbit = (_AK4CHSJet_PUFullID[i]);
		if(!getBit(tmpJetPUIDbit, 1)) continue;					// medium pileup ID

		jetCounter++;

	}

	if(jetCounter > 1) return 0;
	registerCutFlow();

	return 1;
};


void aNTGCpreselector::analyze(){
	std::cout<<"************************************************************************************************************"<<std::endl<<
	"Analyzing events.."<<std::endl<<
	"************************************************************************************************************"<<std::endl;


	while (inputTTreeReader.Next()) {

		ULong64_t current_entry = inputTTreeReader.GetCurrentEntry();

		if(current_entry % REPORT_EVERY == 0){
			std::cout<<"\t"<< getCurrentTime()<<"\t\t\tAnalyzing entry\t"<<current_entry<<std::endl;
		}

		if(isMC){

			genWeight_ = _genWeight;

			puTrue_ = _puTrue;

			puWeight_ = puReWeighter.weight((Float_t)(puTrue_) + 0.01);

			eventWeight_ = (_genWeight) * puWeight_;
			// eventWeight_ = (_genWeight);
		}

		p_nVtx = (Float_t) _nVtx;


		fillGlobalHists();

		if(!selectGammaMETevent()) continue;

		copyEvent();

		fillGammaMEThists();

		outTree->Fill();

	}
};


void aNTGCpreselector::registerCutFlow(){
	h_cutFlowEvts.Fill(lastCutStep);
	h_cutFlowWeighted.Fill(lastCutStep, eventWeight_);
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
		if(matchRegex(_file, ".*data.*\.root")) _isData_ = 1;
		if(matchRegex(_file, ".*mc.*\.root")) _isMC_ = 1;
		if(matchRegex(_file, ".*TuneCUETP8M1.*\.root")) is2016MCsample = 1;
	}

	if(_isData_ * _isMC_) {
		std::cout<<"Error! Both data and MC files are in "<<_infilespath<<std::endl;
		return -2;
	}

	return  _isData_;
};



void aNTGCpreselector::initPileupReweighter(std::string _mcPUfile, std::string _dataPUfile){
	std::cout<<"\t\tPileup reweighting to be done with data pileup profile "<<_dataPUfile<<std::endl<<
	"\t\t\t and MC pileup profile "<<_mcPUfile<<std::endl;
	puReWeighter.init(_mcPUfile, _dataPUfile, "hPUTrue", "pileup");
};

#endif
