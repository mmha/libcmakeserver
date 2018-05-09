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
	SUBCASE("from_json") {
		SUBCASE("") {
			auto uut = version{};
			nlohmann::json const j{
			    {"major", 1},
			    {"minor", 2},
			    {"isExperimental", true},
			    {"type", "version"},
			};
			auto const expected = version{.major = 1, .minor = 2, .isExperimental = true};
			from_json(j, uut);
			CHECK(uut == expected);
		}
		SUBCASE("") {
			auto uut = version{};
			nlohmann::json const j{
			    {"major", 123},
			    {"minor", 0},
			    {"type", "version"},
			};
			auto const expected = version{.major = 123, .minor = 0};
			from_json(j, uut);
			CHECK(uut == expected);
		}
		SUBCASE("missing required 'minor' key") {
			auto uut = version{};
			nlohmann::json const j{
			    {"major", 123},
			    {"type", "version"},
			};
			CHECK_THROWS(from_json(j, uut));
		}
		SUBCASE("empty") {
			auto uut = version{};
			nlohmann::json const j;
			CHECK_THROWS(from_json(j, uut));
		}
	}
}
