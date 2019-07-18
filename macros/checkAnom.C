#include "extra_tools.cc"

#define BETRetaMin 1.4442
#define BETRetaMax 1.566

void checkAnom(std::string _ntuple_list = "SinglePhotonRun2017B17Nov2017v1MINIAOD_000", std::string _out_file = "anom.root"){
	std::cout<<"Using ntuple list "<<_ntuple_list<<" and will write to "<<_out_file<<std::endl;
	std::vector<std::string> _ntuples = getNonemptyLines(_ntuple_list);

	TChain *_ntuple_chain = openTChain(_ntuples, "ggNtuplizer/EventTree");

	TTreeReader _ntuple_reader(_ntuple_chain);

	TTreeReaderVectorValue<Float_t> 		_phoPhi(_ntuple_reader, "phoPhi");
	TTreeReaderVectorValue<Float_t> 		_phoCalibEt(_ntuple_reader, "phoCalibEt");
	TTreeReaderVectorValue<Float_t> 		_phoEta(_ntuple_reader, "phoEta");
	TTreeReaderVectorValue<UChar_t> 		_phoIDbit(_ntuple_reader, "phoIDbit");
	TTreeReaderVectorValue<Bool_t> 			_phoEleVeto(_ntuple_reader, "phoEleVeto");
	TTreeReaderAnyValue<Float_t> 			_pfMET(_ntuple_reader, "pfMET");
	TTreeReaderAnyValue<Float_t> 			_pfMETPhi(_ntuple_reader, "pfMETPhi");
	TTreeReaderAnyValue<UShort_t> 			_metFilters(_ntuple_reader, "metFilters");

	TH1F metPhi("metPhi", "#phi(MET); #phi(MET);# of events", 700, -3.5, 3.5);
	TH1F phoPhi("phoPhi", "#phi(#gamma); #phi(MET);# of events", 700, -3.5, 3.5);


	Int_t reportEvery = 10000;

	while (_ntuple_reader.Next()) {
		ULong64_t current_entry = _ntuple_reader.GetCurrentEntry();
		if(current_entry % reportEvery == 0 ) {
			std::cout<<"\tProcessing entry "<<current_entry<<std::endl
			<<"\t\t"<< _ntuple_chain->GetFile()->GetName()<<std::endl;
		}

		if((_pfMET) < 150.) continue;

		Short_t _220GeVphoCounter = 0;
		Short_t selectedPhotonIndex = -999;
		Float_t highestPtPhotonPt = -999.;
		for(Int_t i = 0; i < (_phoCalibEt.size()); i++){
			Float_t candPhoPt = (_phoCalibEt[i]);
			if(candPhoPt < 220.) continue;
			Float_t candPhoAbsEta = std::abs(_phoEta[i]);
			if(candPhoAbsEta > 2.5 ) continue;
			if((candPhoAbsEta > BETRetaMin) && (candPhoAbsEta < BETRetaMax)) continue;
			UChar_t candPhoIDbit= (_phoIDbit[i]);
			if(!getBit(candPhoIDbit, 2)) continue;
			if(!(_phoEleVeto[i])) continue;
			_220GeVphoCounter++;
			if(candPhoPt > highestPtPhotonPt){
				highestPtPhotonPt = candPhoPt;
				selectedPhotonIndex = i;
			}
		}

		if(selectedPhotonIndex < 0) continue;;

		metPhi.Fill((_pfMETPhi));
		phoPhi.Fill((_phoPhi[selectedPhotonIndex]));

	}

	closeTChain(_ntuple_chain);


	TFile outRoot(_out_file.c_str(), "RECREATE");
	outRoot.cd();
	metPhi.Write();
	phoPhi.Write();
	outRoot.Close();
	std::cout<<"Histograms written to "<<_out_file<<std::endl;
};