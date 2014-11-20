#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <string>
#include <atomic>
#include <memory>
#include <thread>

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
