#include <cmakeserver/ipc.hpp>
#include <cstdlib>

namespace cmakeserver {
	temp_pipe open_temporary_pipe(boost::asio::io_context &context) {
		using namespace boost::asio::local;
		auto const generated_name = tmpnam(nullptr);
		return {.name = generated_name, .pipe = boost::asio::local::stream_protocol::socket{context}};
	}
}    // namespace cmakeserver
