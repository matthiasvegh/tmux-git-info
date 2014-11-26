#ifndef SHARED_MAP_HPP_
#define SHARED_MAP_HPP_

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>

template<typename Key, typename Value>
class SharedMap {

	boost::interprocess::managed_shared_memory sharedMemory;
	constexpr static const char* sharedMemoryName = "tmuxinfod_shared_memory";
	constexpr static const char* mapImplInstanceName = "SharedMapImpleInstance";
	using Allocator = boost::interprocess::allocator<std::pair<const Key, Value>, boost::interprocess::managed_shared_memory::segment_manager>;
	using SharedMapImpl = boost::interprocess::map<Key, Value, std::less<Key>, Allocator>;

	const Allocator allocatorInstance;
	SharedMapImpl* sharedMapImplInstance;

public:

	SharedMap() :
		sharedMemory(
			boost::interprocess::open_or_create,
			sharedMemoryName,
			65536),
		allocatorInstance(sharedMemory.get_segment_manager()),
		sharedMapImplInstance(sharedMemory.find_or_construct(mapImplInstanceName))
	{
	}

};

#endif /* SHARED_MAP_HPP_ */
