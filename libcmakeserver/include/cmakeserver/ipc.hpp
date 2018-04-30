#pragma once
#include <boost/asio/local/stream_protocol.hpp>
#include <boost/process/child.hpp>
#include <experimental/filesystem>
#include <string_view>

namespace cmakeserver {
	struct tmp_pipe {
		boost::asio::local::stream_protocol::socket pipe;
		std::string name;
	};

	inline tmp_pipe open_temporary_pipe(boost::asio::io_context &context) {
		using namespace boost::asio::local;
		auto const generated_name = tmpnam(nullptr);
		return {.name = generated_name, .pipe = stream_protocol::socket{context}};
	}

	boost::process::child spawn_cmake_server(std::string_view pipe);
}    // namespace cmakeserver
