#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <string>
#include <atomic>
#include <memory>
#include <thread>
#include <cstdio>

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

class Context: std::enable_shared_from_this<Context> {
	std::atomic_bool isRunning;
	std::string cwd;

public:

	Context() {
		isRunning.store(false);
	}

	void setCwd(std::string c) { cwd = std::move(c); }
	const std::string& getCwd() const { return cwd; }

	void run() {
		isRunning = true;
		std::thread t(
				[&](){
				std::system("ls -la");
				}
		);
	}

	void stop() {
		isRunning = false;
	}

};

#endif /* CONTEXT_HPP_ */
