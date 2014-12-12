#include <iostream>
#include <boost/lexical_cast.hpp>

#include "SharedMap.hpp"

int main(int argc, const char** argv) {

	if(argc != 2 || !boost::lexical_cast<std::size_t>(argv[1])) {
		std::cerr<<"Please specify a paneid to fetch info from"<<std::endl;
		std::exit(1);
	}

	try {
		SharedMap<std::size_t, SharedString> sm;
		std::cout<<sm[boost::lexical_cast<std::size_t>(argv[1])]<<std::endl;
	} catch(...) {
		std::exit(1);
	}

}
