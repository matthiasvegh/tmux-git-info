#ifndef SHARED_MAP_HPP_
#define SHARED_MAP_HPP_

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/sync/interprocess_recursive_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

template<typename Key, typename Value>
class SharedMap {

	constexpr static const char* sharedMemoryName = "tmuxinfod_shared_memory" __DATE__ __TIME__;
	constexpr static const char* mapImplInstanceName = "tmuxinfod_SharedMapImpleInstance";
	constexpr static const char* sharedMutexName = "tmuxinfod_SharedMutex";
	constexpr static const char* referenceCountName = "tmuxinfod_referenceCount";

	using Comparator = std::less<Key>;
	using Allocator = boost::interprocess::allocator<std::pair<Key const, Value>, boost::interprocess::managed_shared_memory::segment_manager>;
	using SharedMapImpl = boost::interprocess::map<Key, Value, Comparator, Allocator>;
	using SharedMutex = boost::interprocess::interprocess_mutex;

	boost::interprocess::managed_shared_memory sharedMemory;
	Allocator allocatorInstance;
	SharedMapImpl* sharedMapImplInstance;
	SharedMutex* sharedMutex;
	std::size_t* referenceCount;


public:

	SharedMap() :
		sharedMemory(
			boost::interprocess::open_or_create,
			sharedMemoryName,
			65536),
		allocatorInstance(sharedMemory.get_segment_manager()),
		sharedMapImplInstance(
				sharedMemory.find_or_construct<SharedMapImpl>(mapImplInstanceName)(Comparator(), allocatorInstance)
				),
		sharedMutex(
				sharedMemory.find_or_construct<SharedMutex>(sharedMutexName)()
				),
		referenceCount(
				sharedMemory.find_or_construct<std::size_t>(referenceCountName)(0))

	{
		++*referenceCount;
	}

	Value operator[](const Key& key) const {
		boost::interprocess::scoped_lock<SharedMutex> lock(*sharedMutex);
		return (*sharedMapImplInstance)[key];
	}

	void insert(Key key, Value value) {
		boost::interprocess::scoped_lock<SharedMutex> lock(*sharedMutex);
		(*sharedMapImplInstance)[key] = value;
	}

	Value at(const Key& k) const {
		boost::interprocess::scoped_lock<SharedMutex> lock(*sharedMutex);
		return sharedMapImplInstance->at(k);
	}

	~SharedMap() {
		boost::interprocess::scoped_lock<SharedMutex> lock(*sharedMutex);
		if(*referenceCount > 0) {
			if(--*referenceCount == 0) {
				boost::interprocess::shared_memory_object::remove(sharedMemoryName);
			}
		}
	}


};

#endif /* SHARED_MAP_HPP_ */
