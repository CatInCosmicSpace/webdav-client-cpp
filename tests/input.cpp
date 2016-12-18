#include <fstream>
#include <boost/filesystem.hpp>
#include "catch.hpp"
#include <iostream>

SCENARIO("Open file") {
	std::ifstream test("test/3.txt");

	REQUIRE(test.is_open());
}
