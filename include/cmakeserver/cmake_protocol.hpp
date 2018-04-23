#pragma once
#include <boost/hana.hpp>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

namespace cmakeserver::protocol {
	inline namespace v3_11 {
		auto constexpr prologue = std::string_view{"[== \"CMake Server\" ==[\n"};
		auto constexpr epilogue = std::string_view{"]== \"CMake Server\" ==]\n"};

		// clang-format off
        struct hello {
            struct version {
                BOOST_HANA_DEFINE_STRUCT(version,
                    (int, major),
                    (int, minor)
                );
            };
            BOOST_HANA_DEFINE_STRUCT(hello,
                (std::vector<version>, supportedProtocolVersions)
            );
        };

        struct reply {
            using value_type = std::variant<std::string, double>;
            value_type const& operator[](std::string const& key) const {
                return values.find(key)->second;
            }
        private:
            std::unordered_map<std::string, value_type> values;
        };

        struct error {
            BOOST_HANA_DEFINE_STRUCT(error,
                (std::string, errorMessage)
            );
        };

        struct progress {
            BOOST_HANA_DEFINE_STRUCT(progress,
                (std::string, progressMessage),
                (int, progressMinimum),
                (int, progressMaximum),
                (int, progressCurrent)
            );
        };

        struct message {
            BOOST_HANA_DEFINE_STRUCT(message,
                (std::string, message),
                (std::string, title)
            );
        };

        struct signal {
            BOOST_HANA_DEFINE_STRUCT(signal);
        };

        struct dirty_signal : signal {

        };

        struct fileChange : signal {

        };
		// clang-format on
	}    // namespace v3_11
}    // namespace cmakeserver::protocol
