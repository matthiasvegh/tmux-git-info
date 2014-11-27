#define BOOST_TEST_MODULE main

#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include "SharedMap.hpp"

BOOST_AUTO_TEST_CASE(SharedMap_basic_test) {

	SharedMap<int, int> sm;
	sm.insert(0, 0);
	BOOST_CHECK_EQUAL(sm[0], 0);

}
