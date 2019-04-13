#include "extra_tools.cc"

// std::string INFILE="../data/xSections.txt";
// std::string OUTFILE="../data/meanXsections.txt";

std::string INFILE="../data/2019_aNTGC_xSections.txt";
std::string OUTFILE="../data/2019_aNTGC_mean_xSections.txt";

void calcXsec(std::string _inFile=INFILE, std::string _outFile=OUTFILE){
	CSVReader _infileCSV(_inFile);
	std::vector<std::vector<std::string>> _infileParsed = _infileCSV.getData();

	std::vector<std::string> _sampleNames;

	for(auto _row : _infileParsed){
		_sampleNames.push_back(_row[0]);
	}

	std::sort( _sampleNames.begin(), _sampleNames.end() );
	_sampleNames.erase( std::unique( _sampleNames.begin(), _sampleNames.end() ), _sampleNames.end() );

	// tuple: sumX*W, sumW, sumW2, count, displacement from mean
	std::map<std::string, std::tuple<Double_t, Double_t, Double_t, Double_t, Double_t>> _xSecMap;

	for(auto _sample: _sampleNames){
		std::tuple<Double_t, Double_t, Double_t, Double_t, Double_t> _sample_init(0., 0., 0., 0., 0.);
		_xSecMap[_sample] = _sample_init;
	}

	for(auto _row :_infileParsed){
		std::string _sampleName = _row[0];

		std::string _unit =  _row[4];
		Double_t _unitConv = xsec_unit_map.at(_unit);

		Double_t _xsec = std::stod(_row[2]) * _unitConv;
		Double_t _unc =  std::stod(_row[3]) * _unitConv;

		Double_t _weight = 1./(_unc * _unc);


		if(_unc > _xsec) cout<<"Anomaly: "<<_sampleName<<", "<<_row[1]<<", "<<_xsec<<", "<<_unc<<std::endl;

		std::get<0>(_xSecMap[_sampleName]) += _xsec*_weight;
		std::get<1>(_xSecMap[_sampleName]) += _weight;
		std::get<2>(_xSecMap[_sampleName]) += _weight * _weight;
		std::get<3>(_xSecMap[_sampleName]) += 1.;
	}

	for(auto _row :_infileParsed){
		std::string _sampleName = _row[0];
		std::string _unit =  _row[4];
		Double_t _unitConv = xsec_unit_map.at(_unit);
		Double_t _xsec = std::stod(_row[2]) * _unitConv;
		Double_t _unc =  std::stod(_row[3]) * _unitConv;
		Double_t _weight = 1./(_unc * _unc);

		Double_t _meanXsec = std::get<0>(_xSecMap[_sampleName]) / std::get<1>(_xSecMap[_sampleName]);
		Double_t _displacement = _xsec - _meanXsec;

		std::get<4>(_xSecMap[_sampleName]) += _displacement * _displacement * _weight;
	}

	ofstream _fileout;
	_fileout.open (_outFile);
	for(auto sample : _xSecMap){
		std::string _sampleName = sample.first;
		Double_t _meanXsec = std::get<0>(sample.second) / std::get<1>(sample.second);
		Double_t _denom = std::get<1>(_xSecMap[_sampleName]) -  std::get<2>(_xSecMap[_sampleName]) / std::get<1>(_xSecMap[_sampleName]);

		Double_t _varXsec = std::get<4>(sample.second) /_denom;
		Double_t _errorXsec = std::sqrt(_varXsec);

		std::cout<<sample.first<<","<< _meanXsec <<","<<_errorXsec<<","<< to_string_with_precision(std::get<3>(_xSecMap[_sampleName]), 0) <<std::endl;

		_fileout<<sample.first<<","<<_meanXsec<<","<<_errorXsec<<","<<to_string_with_precision(std::get<3>(_xSecMap[_sampleName]), 0)<<std::endl;
	}
	_fileout.close();
}
