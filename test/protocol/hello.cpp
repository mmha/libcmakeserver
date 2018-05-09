#include <cmakeserver/cmake_protocol.hpp>
#include <doctest.h>
#include <nlohmann/json.hpp>

TEST_CASE("protocol::hello") {
	using cmakeserver::protocol::hello;
	SUBCASE("to_json") {
		SUBCASE("") {
			auto const h = hello{.supportedProtocolVersions = {{.major = 1, .minor = 2, .isExperimental = true}, {}}};
			nlohmann::json uut;
			to_json(uut, h);
			auto expected = nlohmann::json{};
			expected["supportedProtocolVersions"] = {{
			                                             {"major", 1},
			                                             {"minor", 2},
			                                             {"isExperimental", true},
			                                             {"type", "version"},
			                                         },
			                                         {
			                                             {"major", 0},
			                                             {"minor", 0},
			                                             {"type", "version"},
			                                         }};
			expected["type"] = "hello";
			CHECK(uut == expected);
		}
	}
	SUBCASE("from_json") {
		SUBCASE("") {
			auto uut = hello{};
			nlohmann::json j;
			j["supportedProtocolVersions"] = {{
			                                      {"major", 1},
			                                      {"minor", 2},
			                                      {"isExperimental", true},
			                                      {"type", "version"},
			                                  },
			                                  {
			                                      {"major", 0},
			                                      {"minor", 0},
			                                      {"type", "version"},
			                                  }};
			j["type"] = "hello";
			auto const expected =
			    hello{.supportedProtocolVersions = {{.major = 1, .minor = 2, .isExperimental = true}, {}}};
			from_json(j, uut);
			CHECK(uut == expected);
		}
		SUBCASE("missing 'major' field") {
			auto uut = hello{};
			nlohmann::json j;
			j["supportedProtocolVersions"] = {{
			                                      {"major", 1},
			                                      {"minor", 2},
			                                      {"isExperimental", true},
			                                      {"type", "version"},
			                                  },
			                                  {
			                                      {"minor", 0},
			                                      {"type", "version"},
			                                  }};
			j["type"] = "hello";
			CHECK_THROWS(from_json(j, uut));
		}
		SUBCASE("missing 'supportedProtocolVersions' field") {
			auto uut = hello{};
			nlohmann::json j;
			j["type"] = "hello";
			CHECK_THROWS(from_json(j, uut));
		}
	}
}
