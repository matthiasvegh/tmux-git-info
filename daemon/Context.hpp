#ifndef CONTEXT_HPP_
#define CONTEXT_HPP_

#include <string>
#include <atomic>
#include <memory>
#include <thread>
#include <cstdio>
#include <mutex>

template<typename T>
class AtomicWrapper {

	mutable std::mutex lock;

	T value;

public:

	template<typename U>
	AtomicWrapper(U&& u) : lock() {
		std::lock_guard<decltype(lock)> g(lock);
		value = T(u);
	}

	template<typename U>
	AtomicWrapper& operator=(U&& newValue) {
		std::lock_guard<decltype(lock)> g(lock);
		value = std::forward<U>(newValue);
		return *this;
	}

	operator T() const {
		std::lock_guard<decltype(lock)> g(lock);
		T ret = value;
		return ret;
	}


};

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

	AtomicWrapper<std::string> currentResult = "";

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
					currentResult = runCommand("ls -la");
				}
		);
	}

	void stop() {
		isRunning = false;
	}

	std::string getResult() const {
		return currentResult;
	}

};

#endif /* CONTEXT_HPP_ */
