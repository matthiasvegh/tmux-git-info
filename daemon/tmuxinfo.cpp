#include <iostream>

#include "SharedMap.hpp"

int main(int argc, const char** argv) {

	if(argc != 2 || !std::atoi(argv[1])) {
		std::cerr<<"Please specify a paneid to fetch info from"<<std::endl;
		std::exit(1);
	}

	try {
		SharedMap<std::size_t, SharedString> sm;
		std::cout<<sm[std::atoi(argv[1])]<<std::endl;
	} catch(boost::interprocess::interprocess_exception& e) {
		std::cerr<<"exiting"<<std::endl;
		std::cerr<<e.what()<<std::endl;
		std::exit(1);
	}

}
