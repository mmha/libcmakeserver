#pragma once
#include <boost/hana.hpp>
#include <string>
#include <string_view>
#include <unordered_map>
#include <optional>
#include <variant>
#include <vector>
#include <experimental/filesystem>

#define CMAKESERVER_PROTOCOL_MSG(name, ...) \
    struct name : message { \
        BOOST_HANA_DEFINE_STRUCT(name, \
            (opt<std::string>, cookie), \
            __VA_ARGS__ \
        ); \
    };
#define CMAKESERVER_PROTOCOL_REPLY(name, ...) \
    struct name : reply { \
        BOOST_HANA_DEFINE_STRUCT(name, \
            __VA_ARGS__ \
        ); \
    };
#define CMAKESERVER_PROTOCOL_ERROR(name, ...) \
    struct name : error { \
        BOOST_HANA_DEFINE_STRUCT(name, \
            __VA_ARGS__ \
        ); \
    };

namespace cmakeserver::protocol {
	inline namespace v1_2 {
        template<typename T>
        using opt = std::optional<T>;

		auto constexpr prologue = std::string_view{"[== \"CMake Server\" ==[\n"};
		auto constexpr epilogue = std::string_view{"]== \"CMake Server\" ==]\n"};

        struct message {
            BOOST_HANA_DEFINE_STRUCT(message,
                (opt<std::string>, cookie)
            );
        };

		// clang-format off
        struct hello {
            struct version {
                BOOST_HANA_DEFINE_STRUCT(version,
                    (int, major),
                    (int, minor),
                    (opt<bool>, isExperimental)
                );
            };
            BOOST_HANA_DEFINE_STRUCT(hello,
                (std::vector<version>, supportedProtocolVersions)
            );
        };

        CMAKESERVER_PROTOCOL_MSG(handshake,
                (opt<std::experimental::filesystem::path>, sourceDirectory),
                (std::experimental::filesystem::path, buildDirectory),
                (opt<std::string>, generator),
                (opt<std::string>, extraGenerator),
                (opt<std::string>, platform),
                (opt<std::string>, toolset)
            );

        struct globalSettings{
            BOOST_HANA_DEFINE_STRUCT(globalSettings);
        };

        struct configure {
            BOOST_HANA_DEFINE_STRUCT(configure,
                (std::vector<std::string>, cacheArguments)
            );
        };

        struct compute {
            BOOST_HANA_DEFINE_STRUCT(compute);
        };

        struct codemodel {
            BOOST_HANA_DEFINE_STRUCT(codemodel);
        };

        struct ctestinfo {
            BOOST_HANA_DEFINE_STRUCT(codemodel);
        };

        struct cmakeinputs {
            BOOST_HANA_DEFINE_STRUCT(cmakeinputs);
        };

        struct cache {
            BOOST_HANA_DEFINE_STRUCT(cache);
        };

        struct fileSystemWatchers {
            BOOST_HANA_DEFINE_STRUCT(cache);
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
