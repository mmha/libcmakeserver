#include <cmakeserver/cmake_protocol.hpp>

#include <boost/hana.hpp>
#include <nlohmann/json.hpp>

template <typename T>
std::string_view constexpr type_name_string = [] { throw "unspecialized variable template type_name_string"; }();

#define CS_ADAPT_STRUCT(NAMESPACE, TYPENAME, ...)                            \
	BOOST_HANA_ADAPT_STRUCT(NAMESPACE::TYPENAME __VA_OPT__(, ) __VA_ARGS__); \
	template <>                                                              \
	std::string_view constexpr type_name_string<NAMESPACE::TYPENAME> = #TYPENAME;

CS_ADAPT_STRUCT(cmakeserver::protocol, message);
CS_ADAPT_STRUCT(cmakeserver::protocol, version, major, minor, isExperimental);
CS_ADAPT_STRUCT(cmakeserver::protocol, hello, supportedProtocolVersions);
CS_ADAPT_STRUCT(cmakeserver::protocol,
                handshake,
                protocolVersion,
                sourceDirectory,
                buildDirectory,
                generator,
                extraGenerator,
                platform,
                toolset);
CS_ADAPT_STRUCT(cmakeserver::protocol,
                globalSettings,
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
CS_ADAPT_STRUCT(cmakeserver::protocol, configure, cacheArguments);
CS_ADAPT_STRUCT(cmakeserver::protocol, compute);
CS_ADAPT_STRUCT(cmakeserver::protocol, codemodel);
CS_ADAPT_STRUCT(cmakeserver::protocol, ctestinfo);
CS_ADAPT_STRUCT(cmakeserver::protocol, cmakeinputs);
CS_ADAPT_STRUCT(cmakeserver::protocol, cache);
CS_ADAPT_STRUCT(cmakeserver::protocol, fileSystemWatchers);

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

template <typename T>
constexpr bool is_optional = false;

template <typename T>
constexpr bool is_optional<std::optional<T>> = true;

#define CMAKESERVER_ADL_OVERLOAD_IMPL(type)                                      \
	std::string to_string(type const &val) {                                     \
		nlohmann::json res;                                                      \
		to_json(res, val);                                                       \
		return res.dump();                                                       \
	}                                                                            \
	void to_json(nlohmann::json &j, type const &val) {                           \
		using namespace boost::hana;                                             \
		j["type"] = type_name_string<type>.data();                               \
		for_each(val, fuse([&](auto name, auto member) {                         \
			         if constexpr(is_optional<std::decay_t<decltype(member)>>) { \
				         if(!member) {                                           \
					         return;                                             \
				         }                                                       \
			         }                                                           \
			         j[to<char const *>(name)] = member;                         \
		         }));                                                            \
	}                                                                            \
	void to_json(nlohmann::json &j, std::optional<type> const &val) {            \
		if(val) {                                                                \
			to_json(j, *val);                                                    \
		}                                                                        \
	}                                                                            \
	void from_json(nlohmann::json const &, type const &);                        \
	void from_json(nlohmann::json const &, std::optional<type> const &);


namespace cmakeserver::protocol {
	inline namespace v1_2 {
		CMAKESERVER_ADL_OVERLOAD_IMPL(message)
		CMAKESERVER_ADL_OVERLOAD_IMPL(version)
		CMAKESERVER_ADL_OVERLOAD_IMPL(hello)
		CMAKESERVER_ADL_OVERLOAD_IMPL(handshake)
		CMAKESERVER_ADL_OVERLOAD_IMPL(globalSettings)
		CMAKESERVER_ADL_OVERLOAD_IMPL(configure)
		CMAKESERVER_ADL_OVERLOAD_IMPL(compute)
		CMAKESERVER_ADL_OVERLOAD_IMPL(codemodel)
		CMAKESERVER_ADL_OVERLOAD_IMPL(ctestinfo)
		CMAKESERVER_ADL_OVERLOAD_IMPL(cmakeinputs)
		CMAKESERVER_ADL_OVERLOAD_IMPL(cache)
		CMAKESERVER_ADL_OVERLOAD_IMPL(fileSystemWatchers)
	}    // namespace v1_2
}    // namespace cmakeserver::protocol
