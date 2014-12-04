#include <iostream>
#include <thread>
#include <chrono>

#include "SharedMap.hpp"

int main(int argc, char** argv) {

	if(argc != 2 && argc != 4) {
		exit(1);
	}

	SharedMap<std::size_t, SharedString> sm;

	if(argc == 2) {
		std::cout<<sm[std::atoi(argv[1])]<<std::endl;
	}

	if(argc == 4) {
		sm.insert(std::atoi(argv[1]), argv[2]);
		std::this_thread::sleep_for(std::chrono::seconds(std::atoi(argv[3])));
	}

}
