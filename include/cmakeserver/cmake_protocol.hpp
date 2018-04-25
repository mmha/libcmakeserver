#pragma once
#include <boost/hana.hpp>
#include <experimental/filesystem>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <variant>
#include <vector>

#define CMAKESERVER_PROTOCOL_MSG(TYPENAME, ...)                                                   \
	struct TYPENAME {                                                                             \
		constexpr static std::string_view type_name_string = #TYPENAME;                           \
		BOOST_HANA_DEFINE_STRUCT(TYPENAME, (opt<std::string>, cookie)__VA_OPT__(, ) __VA_ARGS__); \
	};                                                                                            \
	template <>                                                                                   \
	constexpr bool is_cmake_protocol_struct<TYPENAME> = true;

namespace nlohmann {
	template <typename T>
	struct adl_serializer<std::optional<T>> {
		static void to_json(json &j, std::optional<T> const &opt) {
			if(opt) {
				j = *opt;
			} else {
				j = nullptr;
			}
		}

		static void from_json(json const &j, std::optional<T> &opt) {
			if(!j) {
				opt = {};
			} else {
				opt = j.get<T>();
			}
		}
	};
}    // namespace nlohmann

namespace cmakeserver::protocol {
	inline namespace v1_2 {
		template <typename T>
		using opt = std::optional<T>;

		template <typename T>
		constexpr bool is_cmake_protocol_struct = false;

		template <typename T>
		auto to_json(nlohmann::json &j, T const &val) -> std::enable_if_t<is_cmake_protocol_struct<T>>;
		template <typename T>
		auto to_json(nlohmann::json &j, opt<T> const &val) -> std::enable_if_t<is_cmake_protocol_struct<T>>;

		auto constexpr prologue = std::string_view{"[== \"CMake Server\" ==[\n"};
		auto constexpr epilogue = std::string_view{"\n]== \"CMake Server\" ==]\n"};

		// clang-format off

        CMAKESERVER_PROTOCOL_MSG(message);

        struct hello {
            constexpr static std::string_view type_name_string = "hello";
            struct version {
                constexpr static std::string_view type_name_string = "version";
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
        template<> constexpr bool is_cmake_protocol_struct<hello> = true;
        template<> constexpr bool is_cmake_protocol_struct<hello::version> = true;

        CMAKESERVER_PROTOCOL_MSG(handshake,
                (opt<std::experimental::filesystem::path>, sourceDirectory),
                (std::experimental::filesystem::path, buildDirectory),
                (opt<std::string>, generator),
                (opt<std::string>, extraGenerator),
                (opt<std::string>, platform),
                (opt<std::string>, toolset)
            );

        CMAKESERVER_PROTOCOL_MSG(globalSettings,
            (std::experimental::filesystem::path, buildDirectory),
            (int, capabilities),
            (bool, checkSystemVars),
            (std::string, extraGenerator),
            (std::string, generator),
            (bool, debugOutput),
            (std::string, inReplyTo),
            (std::experimental::filesystem::path, sourceDirectory),
            (bool, trace),
            (bool, traceExpand),
            (bool, warnUninitialized),
            (bool, warnUnused),
            (bool, warnUnusedCli)
        );

        CMAKESERVER_PROTOCOL_MSG(configure,
            (std::vector<std::string>, cacheArguments)
        );

        CMAKESERVER_PROTOCOL_MSG(compute);
        CMAKESERVER_PROTOCOL_MSG(codemodel);
        CMAKESERVER_PROTOCOL_MSG(ctestinfo);
        CMAKESERVER_PROTOCOL_MSG(cmakeinputs);
        CMAKESERVER_PROTOCOL_MSG(cache);
        CMAKESERVER_PROTOCOL_MSG(fileSystemWatchers);

        struct reply {
            using value_type = std::variant<std::string, double>;
            value_type const& operator[](std::string const& key) const {
                return values.find(key)->second;
            }
        private:
            std::unordered_map<std::string, value_type> values;
        };

        struct error {
            constexpr static std::string_view type_name_string = "error";
            BOOST_HANA_DEFINE_STRUCT(error,
                (std::string, errorMessage)
            );
        };
        template<> constexpr bool is_cmake_protocol_struct<error> = true;

        struct progress {
            constexpr static std::string_view type_name_string = "progress";
            BOOST_HANA_DEFINE_STRUCT(progress,
                (std::string, progressMessage),
                (int, progressMinimum),
                (int, progressMaximum),
                (int, progressCurrent)
            );
        };
        template<> constexpr bool is_cmake_protocol_struct<progress> = true;

        struct signal {
            constexpr static std::string_view type_name_string = "signal";
            BOOST_HANA_DEFINE_STRUCT(signal,
                (std::string, name)
            );
        };

        struct fileChange : signal {
            constexpr static std::string_view type_name_string = "fileChange";
            BOOST_HANA_DEFINE_STRUCT(fileChange,
                (std::experimental::filesystem::path, path),
                (std::vector<std::string>, properties) // TODO enum class {change, rename}
            );
        };
		// clang-format on

		template <typename T>
		auto to_json(nlohmann::json &j, T const &val) -> std::enable_if_t<is_cmake_protocol_struct<T>> {
			using namespace boost::hana;
			j["type"] = T::type_name_string.data();
			for_each(val, fuse([&](auto name, auto member) { j[to<char const *>(name)] = member; }));
		}

		template <typename T>
		auto to_json(nlohmann::json &j, opt<T> const &val) -> std::enable_if_t<is_cmake_protocol_struct<T>> {
			if(val) {
				to_json(*val);
			}
		}

		template <typename T>
		std::string to_string(T const &val) {
			nlohmann::json res;
			to_json(res, val);
			return res.dump();
		}
	}    // namespace v1_2
}    // namespace cmakeserver::protocol

#undef CMAKESERVER_PROTOCOL_MSG
#undef CMAKESERVER_PROTOCOL_REPLY
#undef CMAKESERVER_PROTOCOL_ERROR