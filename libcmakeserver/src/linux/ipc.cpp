#include <cmakeserver/ipc.hpp>
#include <cstdlib>

namespace cmakeserver {
	temp_pipe open_temporary_pipe(boost::asio::io_context &context) {
		using namespace boost::asio::local;
		return {.name = make_temporary_filename(), .pipe = boost::asio::local::stream_protocol::socket{context}};
	}
}    // namespace cmakeserver
