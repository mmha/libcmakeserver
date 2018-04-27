#pragma once
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

		struct hello {
			struct version {
				int major;
				int minor;
				std::optional<bool> isExperimental;
			};
			std::vector<version> supportedProtocolVersions;
		};

		struct handshake {
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

		namespace detail {
			template <typename... T>
			struct metalist {};

			template <template <typename> typename UnaryOp, typename List>
			struct transform;

			template <template <typename> typename UnaryOp, typename... Seq>
			struct transform<UnaryOp, metalist<Seq...>> {
				using type = metalist<UnaryOp<Seq>...>;
			};

			template <template <typename> typename UnaryOp, typename List>
			using transform_t = typename transform<UnaryOp, List>::type;

			template <typename Left, typename Right>
			struct join2;

			template <typename... LSeq, typename... RSeq>
			struct join2<metalist<LSeq...>, metalist<RSeq...>> {
				using type = metalist<LSeq..., RSeq...>;
			};

			template <typename T, typename List>
			constexpr bool in_list = [] { throw 0; };

			template <typename T, typename... Seq>
			constexpr bool in_list<T, metalist<Seq...>> = (std::is_same_v<T, Seq> || ...);

			template <typename T, typename OverloadSet, typename Ret = void>
			using enable_set = std::enable_if_t<in_list<T, OverloadSet>, Ret>;

			using protocol_types = metalist<message,
			                                hello::version,
			                                hello,
			                                handshake,
			                                globalSettings,
			                                configure,
			                                compute,
			                                codemodel,
			                                ctestinfo,
			                                cmakeinputs,
			                                cache,
			                                fileSystemWatchers>;
			using opt_protocol_types = transform_t<std::optional, protocol_types>;
			using all_protocol_types = join2<protocol_types, opt_protocol_types>;
		}    // namespace detail

		template <typename T>
		extern auto to_string(T const &) -> detail::enable_set<T, detail::all_protocol_types, std::string>;

		template <typename T>
		extern auto to_json(nlohmann::json const &, T const &) -> detail::enable_set<T, detail::protocol_types>;
		template <typename T>
		extern auto to_json(nlohmann::json const &, T const &) -> detail::enable_set<T, detail::opt_protocol_types>;
		template <typename T>
		extern auto from_json(nlohmann::json const &, T const &) -> detail::enable_set<T, detail::protocol_types>;
		template <typename T>
		extern auto from_json(nlohmann::json const &, T const &) -> detail::enable_set<T, detail::opt_protocol_types>;
	}    // namespace v1_2
}    // namespace cmakeserver::protocol
