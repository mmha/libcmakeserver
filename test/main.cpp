#include <boost/asio/io_service.hpp>
#include <boost/asio/streambuf.hpp>
#include <cmakeserver/cmakeserver.hpp>
#include <iostream>

namespace asio = boost::asio;
// using asio::ip::tcp;
using namespace cmakeserver;

using namespace boost::asio;
using namespace std::chrono_literals;

template <typename R, typename P>
boost::future<void> say(std::string str, std::chrono::duration<R, P> d, io_service &service) {
	system_timer timer(service);

	co_await async_await(timer, d);
	std::cout << str;
}

namespace {
#if 0
	boost::future<void> rw_test_server(asio::io_service &io) {
		tcp::endpoint tcp_endpoint{tcp::v6(), 15000};
		tcp::acceptor tcp_acceptor{io, tcp_endpoint};
		tcp::socket socket{io};

		tcp_acceptor.listen();
		co_await tcp_acceptor.async_accept(socket, use_boost_future);
		std::cout << "after accept\n";

		protocol::hello h{.supportedProtocolVersions = {{.major = 1, .minor = 0, .isExperimental = false},
		                                                {.major = 1, .minor = 2, .isExperimental = true}}};
		[[maybe_unused]] auto transmitted = co_await async_cmake_send(socket, to_string(h));
		std::cout << "after send\n";

		co_return;
	}
#endif

	boost::future<void> rw_test_client(asio::io_service &io) {
		system_timer timer(io);
		auto [pipe, name] = open_temporary_pipe(io);
		std::cout << "is_open 1: " << pipe.is_open() << std::endl;
		auto cmake = spawn_cmake_server(name);
		co_await async_await(timer, 100ms);
		std::cout << "is_open 2: " << pipe.is_open() << std::endl;
		auto endpoint = boost::asio::local::stream_protocol::endpoint{name};
		std::cout << "is_open 3: " << pipe.is_open() << std::endl;
		pipe.connect(endpoint);
		std::cout << "is_open 4: " << pipe.is_open() << std::endl;

		asio::streambuf buf;
		// auto msg = co_await async_cmake_recv(pipe, buf);
		// std::cout << "Received message: " << msg << '\n';

		co_await async_await(timer, 100ms);

		protocol::handshake h{.buildDirectory = "/home/morris/Projects/cgui/libcmakeserver/build",
		                      .protocolVersion = {.major = 1, .minor = 2, .isExperimental = true}};
		std::string x = protocol::prologue.data();
		x += to_string(h);
		x += protocol::epilogue.data();
		std::cout << "Sending: " << x << std::endl;
		[[maybe_unused]] auto transmitted = co_await async_write(pipe, asio::buffer(x));
		std::cout << "Result: " << transmitted << std::endl;

		while(true) {
			// auto msg2 = co_await async_cmake_recv(pipe, buf);
			asio::streambuf buf;
			auto msg2 = asio::read_until(pipe, buf, '\n');
			std::cout << "Received message: " << std::string{buffer_cast<const char *>(buf.data()), buf.size()} << '\n';
		}


		co_return;
	}
}    // namespace

int main() {
	io_service service;
	boost::asio::io_service::work work(io_service);
	auto test = rw_test_client(service);

	/*
	    rw_test_server(service);
	    rw_test_client(service);
	*/
	service.run();
	test.get();
	// std::cout << "Done.\n";
	// std::cin.get();
}
