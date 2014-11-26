#ifndef SHARED_MAP_HPP_
#define SHARED_MAP_HPP_

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>

template<typename Key, typename Value>
class SharedMap {

	boost::interprocess::shared_memory_object sharedMemory;
	boost::interprocess::mapped_region sharedRegion;
	constexpr static const char* sharedMemoryName = "tmuxinfod_shared_memory";

public:

	SharedMap() :
		sharedMemory(
			boost::interprocess::open_or_create,
			sharedMemoryName,
			boost::interprocess::read_write),
		sharedRegion(
			sharedMemory,
			boost::interprocess::read_write)
	{
		sharedMemory.truncate(1024);
		// maybe make dynamic?
	}

};

#endif /* SHARED_MAP_HPP_ */
