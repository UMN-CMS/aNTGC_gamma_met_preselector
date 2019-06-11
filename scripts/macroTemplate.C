#include "#ccfilepath"

void #macroname(){
	std::cout<<getCurrentTime()<<std::endl;
	std::cout<<"Begin root macro..."<<std::endl;

	aNTGCpreselector("#fileList", "#outfilepath", "#mcPU", "#dataPU");

	std::cout<<"End root macro!"<<std::endl;
	std::cout<<getCurrentTime()<<std::endl;
};
