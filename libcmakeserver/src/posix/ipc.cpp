#include <cmakeserver/ipc.hpp>
#include <cstdlib>

namespace cmakeserver {
	temp_pipe open_temporary_pipe(boost::asio::io_context &context) {
		using namespace boost::asio::local;
		auto const generated_name = tmpnam(nullptr);
		return {.name = generated_name, .pipe = {context, stream_protocol::endpoint{generated_name}}};
	}
}    // namespace cmakeserver
