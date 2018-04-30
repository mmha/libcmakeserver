#pragma once
#include <cmakeserver/export.h>
#include <experimental/filesystem>
#include <nlohmann/json_fwd.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cmakeserver::protocol {
	inline namespace v1_2 {
		auto constexpr prologue = std::string_view{"[== \"CMake Server\" ==[\n"};
		auto constexpr epilogue = std::string_view{"\n]== \"CMake Server\" ==]\n"};

		struct message {};

		struct version {
			int major;
			int minor;
			std::optional<bool> isExperimental;
		};

		struct hello {
			std::vector<version> supportedProtocolVersions;
		};

		struct handshake {
			version protocolVersion;
			std::optional<std::experimental::filesystem::path> sourceDirectory;
			std::experimental::filesystem::path buildDirectory;
			std::optional<std::string> generator;
			std::optional<std::string> extraGenerator;
			std::optional<std::string> platform;
			std::optional<std::string> toolset;
		};

		struct globalSettings {
			std::experimental::filesystem::path buildDirectory;
			int capabilities;
			bool checkSystemVars;
			std::string extraGenerator;
			std::string generator;
			bool debugOutput;
			std::string inReplyTo;
			std::experimental::filesystem::path sourceDirectory;
			bool trace;
			bool traceExpand;
			bool warnUninitialized;
			bool warnUnused;
			bool warnUnusedCli;
		};

		struct configure {
			std::vector<std::string> cacheArguments;
		};

		struct compute {};
		struct codemodel {};
		struct ctestinfo {};
		struct cmakeinputs {};
		struct cache {};
		struct fileSystemWatchers {};

		struct error {
			std::string errorMessage;
		};

		struct progress {
			std::string progressMessage;
			int progressMinimum;
			int progressMaximum;
			int progressCurrent;
		};

		struct signal {
			std::string name;
		};

		struct fileChange : signal {
			std::experimental::filesystem::path path;
			std::vector<std::string> properties;    // TODO enum class {change, rename}
		};

#define CMAKESERVER_ADL_OVERLOADS(type)                                             \
	std::string CMAKESERVER_EXPORT to_string(type const &);                         \
	void CMAKESERVER_EXPORT to_json(nlohmann::json &, type const &);                \
	void CMAKESERVER_EXPORT to_json(nlohmann::json &, std::optional<type> const &); \
	void CMAKESERVER_EXPORT from_json(nlohmann::json const &, type const &);        \
	void CMAKESERVER_EXPORT from_json(nlohmann::json const &, std::optional<type> const &);

		CMAKESERVER_ADL_OVERLOADS(message)
		CMAKESERVER_ADL_OVERLOADS(version)
		CMAKESERVER_ADL_OVERLOADS(hello)
		CMAKESERVER_ADL_OVERLOADS(handshake)
		CMAKESERVER_ADL_OVERLOADS(globalSettings)
		CMAKESERVER_ADL_OVERLOADS(configure)
		CMAKESERVER_ADL_OVERLOADS(compute)
		CMAKESERVER_ADL_OVERLOADS(codemodel)
		CMAKESERVER_ADL_OVERLOADS(ctestinfo)
		CMAKESERVER_ADL_OVERLOADS(cmakeinputs)
		CMAKESERVER_ADL_OVERLOADS(cache)
		CMAKESERVER_ADL_OVERLOADS(fileSystemWatchers)

#undef CMAKESERVER_ADL_OVERLOADS
	}    // namespace v1_2
}    // namespace cmakeserver::protocol
