#define BOOST_TEST_MODULE main

#include <boost/test/unit_test.hpp>
#include <boost/test/included/unit_test.hpp>

#include "SharedMap.hpp"

BOOST_AUTO_TEST_CASE(SharedMap_basic_test) {

	SharedMap<int, int> sm;
	sm.insert(0, 0);
	BOOST_CHECK_EQUAL(sm[0], 0);

}

BOOST_AUTO_TEST_CASE(SharedMap_instances_should_share_data) {
	SharedMap<int, int> sm1, sm2;

	sm1.insert(0, 0);
	BOOST_CHECK_EQUAL(sm1[0], sm2[0]);
}

BOOST_AUTO_TEST_CASE(SharedMap_should_throw_if_non_existant_value_is_requested) {
	SharedMap<int, int> sm;

	BOOST_CHECK_THROW(sm.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(SharedMap_instances_should_be_independent_if_they_are_destroyed) {
	{
		// run1
		SharedMap<int, int> sm;
		sm.insert(0, 0);
		BOOST_CHECK_EQUAL(sm[0], 0);
	}

	{
		// run2
		SharedMap<int, int> sm;
		BOOST_CHECK_THROW(sm.at(0), std::out_of_range);
	}

}
