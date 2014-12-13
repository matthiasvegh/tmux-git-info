#include <iostream>
#include <memory>

#include "Context.hpp"
#include "TaskRunner.hpp"
#include "ConfigReader/ConfigReader.hpp"

int main() {

	SharedMap<std::size_t, SharedString> sm;

	std::vector<std::shared_ptr<Context>> contexts;

	const std::size_t n = 1;

	for(std::size_t i=0; i<n; ++i) {
		contexts.push_back(std::make_shared<Context>(i, sm));
	}

}
