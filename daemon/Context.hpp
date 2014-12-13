#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <string>
#include <atomic>
#include <memory>
#include <thread>
#include <cstdio>
#include <mutex>
#include <chrono>

#include "SharedMap.hpp"

std::string runCommand(std::string command) {
	std::FILE* pipe = popen(command.c_str(), "r");

	if(!pipe) return "";

	char buffer[128];

	std::string result;

	while(!std::feof(pipe)) {
		std::fgets(buffer, sizeof(buffer), pipe);
		result += buffer;
	}
	pclose(pipe);
	return result;
}

class Context: public std::enable_shared_from_this<Context> {
	std::atomic_bool isRunning;
	std::string cwd;
	std::size_t paneId;

	SharedMap<std::size_t, SharedString>& sm;

public:

	Context(std::size_t paneId, SharedMap<std::size_t, SharedString>& sm): paneId(paneId), sm(sm) {
		isRunning.store(false);
	}

	const std::string& getCwd() const { return cwd; }

	void run() {
		isRunning = true;
		std::thread t(
				[&](){
					auto self = this->shared_from_this();
					for(;;) {
						if(isRunning.load()) {
							std::string result = runCommand("cd "+cwd+"&& ls -la");
							self->sm.insert(paneId, result.c_str());
							std::this_thread::sleep_for(std::chrono::seconds(5));
						}
					}
				}
		);
	}

	void stop() {
		isRunning = false;
	}

	std::string getResult() const {
		return sm[paneId].c_str();
	}

	~Context() {
	}

};

#endif /* CONTEXT_HPP_ */
