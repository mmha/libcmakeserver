#include <cmakeserver/cmake_protocol.hpp>
#include <doctest.h>
#include <nlohmann/json.hpp>

TEST_CASE("protocol::version") {
	using cmakeserver::protocol::version;
	SUBCASE("to_json") {
		SUBCASE("") {
			auto const v = version{.major = 1, .minor = 2, .isExperimental = true};
			nlohmann::json uut;
			to_json(uut, v);
			auto const expected = nlohmann::json{
			    {"major", 1},
			    {"minor", 2},
			    {"isExperimental", true},
			    {"type", "version"},
			};
			CHECK(uut == expected);
		}
		SUBCASE("") {
			auto const v = version{.major = 123, .minor = 0};
			nlohmann::json uut;
			to_json(uut, v);
			auto const expected = nlohmann::json{
			    {"major", 123},
			    {"minor", 0},
			    {"type", "version"},
			};
			CHECK(uut == expected);
		}
	}
}
