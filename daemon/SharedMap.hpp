#ifndef SHARED_MAP_HPP_
#define SHARED_MAP_HPP_

#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/map.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/interprocess/sync/interprocess_recursive_mutex.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>

using SharedString = boost::interprocess::basic_string<char, boost::interprocess::string::traits_type, boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager>>;

namespace detail {

template<typename T>
struct void_ { typedef void type; };

template<typename T, typename = void>
struct HasAllocator: std::false_type {};

template<typename T>
struct HasAllocator<T, typename void_<typename T::allocator_type>::type>: std::true_type {};

template<bool b>
struct insertHelper {
	template<typename T, typename Key, typename Value, typename Allocator>
	static void insert(T& t, const Key& key, Value&& value, const Allocator&) {
		auto p = std::make_pair(key, std::forward<Value>(value));
		t.emplace(std::move(p));
	}
};

template<>
struct insertHelper<true> {
	template<typename T, typename Key, typename Value, typename Allocator>
	static void insert(T& t, const Key& key, Value&& value, const Allocator& allocator) {
		auto p = std::make_pair(key, typename std::decay<decltype(t[key])>::type{value, allocator});
		t.emplace(std::move(p));
	}
};

template<typename T>
struct isBoostAllocator: std::false_type { };

template<typename... Ts>
struct isBoostAllocator<boost::interprocess::allocator<Ts...>>: std::true_type { };

template<typename T, typename = void>
struct isBoostAllocated: std::false_type { };

template<typename T>
struct isBoostAllocated<T, typename void_<typename isBoostAllocator<typename T::allocator_type>::type>::type>: std::true_type { };

template<typename T>
struct nonAllocatedOrBoostAllocated:
	std::conditional<HasAllocator<T>::value,
			isBoostAllocated<T>,
			std::true_type>::type
{
};


} // namespace detail

template<typename Key, typename Value>
class SharedMap {

	constexpr static const char* sharedMemoryName = "tmuxinfod_shared_memory" __DATE__ __TIME__;
	constexpr static const char* mapImplInstanceName = "tmuxinfod_SharedMapImpleInstance";
	constexpr static const char* sharedMutexName = "tmuxinfod_SharedMutex";
	constexpr static const std::size_t memorySize = 65536;

	using Comparator = std::less<Key>;
	using Allocator = boost::interprocess::allocator<std::pair<Key const, Value>, boost::interprocess::managed_shared_memory::segment_manager>;
	using SharedMapImpl = boost::interprocess::map<Key, Value, Comparator, Allocator>;
	using SharedMutex = boost::interprocess::interprocess_mutex;

	boost::interprocess::managed_shared_memory sharedMemory;
	Allocator allocatorInstance;
	SharedMapImpl* sharedMapImplInstance;
	SharedMutex* sharedMutex;


public:

	SharedMap() :
		sharedMemory(
			boost::interprocess::open_or_create,
			sharedMemoryName,
			memorySize),
		allocatorInstance(sharedMemory.get_segment_manager()),
		sharedMapImplInstance(
				sharedMemory.find_or_construct<SharedMapImpl>(mapImplInstanceName)(Comparator(), allocatorInstance)
				),
		sharedMutex(
				sharedMemory.find_or_construct<SharedMutex>(sharedMutexName)()
				)
	{
	}

	Value operator[](const Key& key) const {
		boost::interprocess::scoped_lock<SharedMutex> lock(*sharedMutex);
		return sharedMapImplInstance->at(key);
	}

	template<typename U>
	void insert(Key key, U value) {
		boost::interprocess::scoped_lock<SharedMutex> lock(*sharedMutex);
		detail::insertHelper<detail::HasAllocator<Value>::value>::insert(
				*sharedMapImplInstance,
				key,
				value,
				allocatorInstance);
	}

	Value at(const Key& k) const {
		boost::interprocess::scoped_lock<SharedMutex> lock(*sharedMutex);
		return sharedMapImplInstance->at(k);
	}

	bool isPointerShared(void* ptr) const {
		void* start = sharedMemory.get_address();
		void* end = (char*)start + memorySize;

		return start <= ptr && ptr <= end;
	}

	~SharedMap() {
		boost::interprocess::shared_memory_object::remove(sharedMemoryName);
	}


};

#endif /* SHARED_MAP_HPP_ */
