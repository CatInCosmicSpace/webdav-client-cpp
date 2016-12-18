#include <fstream>
#include <boost/filesystem.hpp>
#include "catch.hpp"
#include <iostream>

SCENARIO("Open file") {
	std::ifstream test("test.txt");
	std::cout << boost::filesystem::current_path().generic_string();

	REQUIRE(test.is_open());
}
