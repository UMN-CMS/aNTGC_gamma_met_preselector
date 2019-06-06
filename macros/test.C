#include "eventPreselectorGammaMET.cc"

std::string dataPU = "/scratch/mwadud/anTGCpreselector/macros/dataPUhist.root";
std::string mcPU = "/hdfs/store/user/mwadud/aNTGCmet/pileup/aNTGC_0p0003_0p0000004_0p_0p_200_500.root";
std::string mcNtuples = "/hdfs/store/user/mwadud/aNTGCmet/ntupleLists/aNTGC_0p0003_0p0000004_0p_0p_200_500.txt";
void test(){
	aNTGCpreselector t(mcNtuples, "test_out.root", mcPU, dataPU);
};
