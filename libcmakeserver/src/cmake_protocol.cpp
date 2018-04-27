#include <cmakeserver/cmakeserver.hpp>

#include <boost/hana.hpp>
#include <nlohmann/json.hpp>

template <typename T>
std::string_view constexpr type_name_string = [] { throw "unspecialized variable template type_name_string"; }();

#define CS_ADAPT_STRUCT(TYPENAME, ...)                            \
	BOOST_HANA_ADAPT_STRUCT(TYPENAME __VA_OPT__(, ) __VA_ARGS__); \
	template <>                                                   \
	std::string_view constexpr type_name_string<TYPENAME> = #TYPENAME;

CS_ADAPT_STRUCT(cmakeserver::protocol::message);
CS_ADAPT_STRUCT(cmakeserver::protocol::hello::version, major, minor, isExperimental);
CS_ADAPT_STRUCT(cmakeserver::protocol::hello, supportedProtocolVersions);
CS_ADAPT_STRUCT(cmakeserver::protocol::handshake,
                sourceDirectory,
                buildDirectory,
                generator,
                extraGenerator,
                platform,
                toolset);
CS_ADAPT_STRUCT(cmakeserver::protocol::globalSettings,
                buildDirectory,
                capabilities,
                checkSystemVars,
                extraGenerator,
                generator,
                debugOutput,
                inReplyTo,
                sourceDirectory,
                trace,
                traceExpand,
                warnUninitialized,
                warnUnused,
                warnUnusedCli);
CS_ADAPT_STRUCT(cmakeserver::protocol::configure, cacheArguments);
CS_ADAPT_STRUCT(cmakeserver::protocol::compute);
CS_ADAPT_STRUCT(cmakeserver::protocol::codemodel);
CS_ADAPT_STRUCT(cmakeserver::protocol::ctestinfo);
CS_ADAPT_STRUCT(cmakeserver::protocol::cmakeinputs);
CS_ADAPT_STRUCT(cmakeserver::protocol::cache);
CS_ADAPT_STRUCT(cmakeserver::protocol::fileSystemWatchers);

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
		extern auto to_string(T const &val) -> detail::enable_set<T, detail::all_protocol_types, std::string> {
			nlohmann::json res;
			to_json(res, val);
			return res.dump();
		}

		template <typename T>
		extern auto to_json(nlohmann::json const &j, T const &val)
		    -> detail::enable_set<T, detail::opt_protocol_types> {
			if(val) {
				to_json(*val);
			}
		}

		template <typename T>
		extern auto to_json(nlohmann::json const &j, T const &val) -> detail::enable_set<T, detail::protocol_types> {
			using namespace boost::hana;
			j["type"] = T::type_name_string.data();
			for_each(val, fuse([&](auto name, auto member) { j[to<char const *>(name)] = member; }));
		}
	}    // namespace v1_2
}    // namespace cmakeserver::protocol
