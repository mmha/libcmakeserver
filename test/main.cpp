#include <boost/asio/io_service.hpp>
#include <boost/asio/streambuf.hpp>
#include <cmakeserver/cmakeserver.hpp>
#include <iostream>

namespace asio = boost::asio;
using namespace cmakeserver;

using namespace boost::asio;
using namespace std::chrono_literals;

namespace {
	boost::future<void> rw_test_client(asio::io_service &io, std::string_view build_dir) {
		system_timer timer(io);
		auto [name, pipe] = open_temporary_pipe(io);
		auto cmake = spawn_cmake_server(name);
		co_await async_await(timer, 50ms);
		auto endpoint = boost::asio::local::stream_protocol::endpoint{name};
		pipe.connect(endpoint);

		asio::streambuf buf;
		auto msg = co_await async_recv_msg(pipe, buf);
		std::cout << msg;

		protocol::handshake h{.buildDirectory = build_dir,
		                      .protocolVersion = {.major = 1, .minor = 2, .isExperimental = true}};
		std::cout << "\t\tSending: " << to_string(h);
		[[maybe_unused]] auto transmitted = co_await async_send_msg(pipe, to_string(h));

		msg = co_await async_recv_msg(pipe, buf);
		std::cout << msg;

		co_return;
	}
}    // namespace

int main(int argc, char **argv) {
	if(argc != 2) {
		std::cout << "Usage: " << argv[0] << "path/to/build\n";
		std::exit(EXIT_FAILURE);
	}

	io_service service;
	auto work = boost::asio::io_service::work{service};
	auto test = rw_test_client(service, argv[1]);
	service.run();
}
