#pragma once
#include "../detail/filesystem.hpp"
#include <boost/asio/local/stream_protocol.hpp>

namespace cmakeserver {
	struct temp_pipe {
		// The order of the members matters because of a bug in clang.
		// If the order is swapped, the destructor of the socket won't
		// be generated
		std::string name;
		boost::asio::local::stream_protocol::socket pipe;
	};

	temp_pipe open_temporary_pipe(boost::asio::io_context &context);
}    // namespace cmakeserver
