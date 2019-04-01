#include "#ccfilepath"
#include <time.h>

void #macroname(){
	time_t my_time = time(NULL);
	std::cout<<ctime(&my_time)<<std::endl;
	std::cout<<"Begin root macro..."<<std::endl;

	aNTGCpreselector("#fileList", "#outfilepath");

	std::cout<<"End root macro!"<<std::endl;
	my_time = time(NULL);
	std::cout<<ctime(&my_time)<<std::endl;
};
