#include "eventPreselectorGammaMETv2Data.cc"

std::string dataPU = "/hdfs/store/user/mwadud/aNTGCmet/pileup/Processed2017DataPileup.root";
std::string mcPU = "/hdfs/store/user/mwadud/aNTGCmet/pileup/ZNuNuGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph.root";
std::string mcNtuples = "/hdfs/store/user/mwadud/aNTGCmet/ntupleLists/ZNuNuGJets_MonoPhoton_PtG-130_TuneCUETP8M1_13TeV-madgraph.txt";
void test(){
	aNTGCpreselector t("SinglePhotonRun2017E17Nov2017v1MINIAOD_001", "test_outv2.root", "", "");
};
