#pragma once
#include "detail/filesystem.hpp"
#include <cmakeserver/export.h>
#include <iostream>
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
			int major = 0;
			int minor = 0;
			std::optional<bool> isExperimental;
		};

		struct hello {
			std::vector<version> supportedProtocolVersions;
		};

		struct handshake {
			version protocolVersion;
			std::optional<fs::path> sourceDirectory;
			fs::path buildDirectory;
			std::optional<std::string> generator;
			std::optional<std::string> extraGenerator;
			std::optional<std::string> platform;
			std::optional<std::string> toolset;
		};

		struct globalSettings {
			fs::path buildDirectory;
			int capabilities = 0;
			bool checkSystemVars = false;
			std::string extraGenerator;
			std::string generator;
			bool debugOutput = false;
			std::string inReplyTo;
			fs::path sourceDirectory;
			bool trace = false;
			bool traceExpand = false;
			bool warnUninitialized = false;
			bool warnUnused = false;
			bool warnUnusedCli = false;
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
			int progressMinimum = 0;
			int progressMaximum = 0;
			int progressCurrent = 0;
		};

		struct signal {
			std::string name;
		};

		struct fileChange : signal {
			fs::path path;
			std::vector<std::string> properties;    // TODO enum class {change, rename}
		};

#define CMAKESERVER_ADL_OVERLOADS(type)                                               \
	std::string CMAKESERVER_EXPORT to_string(type const &);                           \
	std::ostream &operator<<(std::ostream &os, type const &t);                        \
	void CMAKESERVER_EXPORT to_json(nlohmann::json &, type const &);                  \
	void CMAKESERVER_EXPORT to_json(nlohmann::json &, std::optional<type> const &);   \
	void CMAKESERVER_EXPORT from_json(nlohmann::json const &, type &);                \
	void CMAKESERVER_EXPORT from_json(nlohmann::json const &, std::optional<type> &); \
	bool CMAKESERVER_EXPORT operator==(type const &lhs, type const &rhs);

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
