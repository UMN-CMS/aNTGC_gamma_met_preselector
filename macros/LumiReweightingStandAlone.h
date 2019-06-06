#ifndef PhysicsTools_Utilities_interface_LumiReWeighting_h
#define PhysicsTools_Utilities_interface_LumiReWeighting_h


#include "TH1F.h"
#include "TH3.h"
#include "TFile.h"
#include "TRandom1.h"
#include "TRandom2.h"
#include "TRandom3.h"
#include "TStopwatch.h"
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>

namespace reweight {

	class LumiReWeighting {
	public:

		LumiReWeighting( ){ };

		LumiReWeighting( std::string generatedFile, std::string dataFile, std::string GenHistName, std::string DataHistName):
		generatedFileName_( generatedFile),
		dataFileName_     ( dataFile ),
		GenHistName_      ( GenHistName ),
		DataHistName_     ( DataHistName )	{
				//MC distribution
			generatedFile_ = new TFile( generatedFileName_.c_str() ) ;
				//Data distribution
			dataFile_      = new TFile( dataFileName_.c_str() );

			Data_distr_ = (TH1F*) dataFile_->Get(DataHistName_.c_str());
			MC_distr_ = (TH1F*) generatedFile_->Get(GenHistName_.c_str());

			Data_distr_->SetDirectory(0);
			MC_distr_->SetDirectory(0);

			generatedFile_->Close();
			dataFile_->Close();

				// normalize both histograms first
			Data_distr_->Scale( 100.0/ Data_distr_->Integral() );
			MC_distr_->Scale( 100.0/ MC_distr_->Integral() );

			weights_ = new TH1F( *(Data_distr_)) ;

				// MC * data/MC = data, so the weights are data/MC:
			weights_->SetName("lumiWeights");

				// so now the average weight should be 1.0
			weights_->Divide( MC_distr_ );

			Data_distr_->Delete();
			Data_distr_ = nullptr;
			MC_distr_->Delete();
			Data_distr_ = nullptr;

			std::cout << " Lumi/Pileup Reweighting: Computed Weights per In-Time Nint " << std::endl;

			int NBins = weights_->GetNbinsX();

			for(int ibin = 1; ibin<NBins+1; ++ibin){
				std::cout << "   " << ibin-1 << " " << weights_->GetBinContent(ibin) << std::endl;
			}


		};

		double weight( float n_int ){
			int bin = weights_->GetXaxis()->FindBin( n_int );
			return weights_->GetBinContent( bin );
		}

		~ LumiReWeighting( ){
			delete weights_;
		};

	protected:

		std::string generatedFileName_;
		std::string dataFileName_;
		std::string GenHistName_;
		std::string DataHistName_;
		TFile *generatedFile_ = nullptr;
		TFile *dataFile_  = nullptr;
		TH1F  *weights_  = nullptr;

      		//keep copies of normalized distributions:
		TH1F*      MC_distr_  = nullptr;
		TH1F*      Data_distr_  = nullptr;
	};
}



#endif
