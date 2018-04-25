#include <array>
#include <boost/asio/ip/tcp.hpp>
#include <cmakeserver/cmakeserver.hpp>
#include <iostream>

namespace asio = boost::asio;
using asio::ip::tcp;
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
	boost::future<void> rw_test_server(asio::io_service &io) {
		tcp::endpoint tcp_endpoint{tcp::v6(), 15000};
		tcp::acceptor tcp_acceptor{io, tcp_endpoint};
		tcp::socket socket{io};

		tcp_acceptor.listen();
		co_await tcp_acceptor.async_accept(socket, use_boost_future);
		std::cout << "after accept\n";

		protocol::hello h {
			.supportedProtocolVersions = {
				{.major = 1, .minor = 0, .isExperimental = false},
				{.major = 1, .minor = 2, .isExperimental = true}
			}
		};
		[[maybe_unused]] auto transmitted = co_await async_cmake_send(socket, to_string(h));
		std::cout << "after send\n";

		co_return;
	}

	boost::future<void> rw_test_client(asio::io_service &io) {
		tcp::resolver resolver{io};
		tcp::socket socket{io};
		asio::streambuf buf;

		tcp::resolver::query q{"localhost", "15000"};
		auto endpoint_iterator = co_await resolver.async_resolve(q, use_boost_future);
		std::cout << "after resolve\n";
		if(endpoint_iterator == tcp::resolver::iterator{}) {
			std::cout << "No endpoints found.\n";
			co_return;
		}
		co_await socket.async_connect(*endpoint_iterator, use_boost_future);
		std::cout << "after connect\n";

		auto msg = co_await async_cmake_recv(socket, buf);
		std::cout << "Received message: " << msg << '\n';
		co_return;
	}
}    // namespace

int main() {
	io_service service;
	protocol::globalSettings g {
		.buildDirectory = "."
	};
	say(to_string(g), 50ms, service);

	rw_test_server(service);
	rw_test_client(service);

	service.run();
	std::cout << "Done.\n";
	// std::cin.get();
}
