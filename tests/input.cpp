#include <fstream>
#include "catch.hpp"

SCENARIO("Open file") {
	std::ifstream test("test.txt");
	
	REQUIRE(test.is_open());
}