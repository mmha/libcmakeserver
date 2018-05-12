#pragma once
#include "cmake_protocol.hpp"
#include "coroutines.hpp"
#include "ipc.hpp"
#include "use_boost_future.hpp"
#include <boost/asio/read.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/write.hpp>
#include <nlohmann/json.hpp>

namespace cmakeserver {
	namespace detail {
		template <typename AsyncReadStream, typename Allocator>
		auto async_read_msg(AsyncReadStream &s, boost::asio::basic_streambuf<Allocator> &b) {
			return boost::asio::async_read_until(s, b, protocol::epilogue, use_boost_future);
		}

		template <typename AsyncWriteStream, typename ConstBufferSequence>
		auto async_write(AsyncWriteStream &s, ConstBufferSequence const &buffers) -> boost::future<std::size_t> {
			return boost::asio::async_write(s, buffers, use_boost_future);
		}
	}    // namespace detail

	template <typename ProtocolType, typename AsyncReadStream, typename Allocator>
	auto async_recv_msg(AsyncReadStream &s, boost::asio::basic_streambuf<Allocator> &b) -> boost::future<ProtocolType> {
		co_await detail::async_read_msg(s, b);
		auto const str = std::string((std::istreambuf_iterator<char>(&b)), std::istreambuf_iterator<char>());
		auto const input_json_view = std::string_view{
		    str.data() + protocol::prologue.size(), str.size() - protocol::prologue.size() - protocol::epilogue.size()};
		auto const json = nlohmann::json::parse(input_json_view);
		auto result = ProtocolType{};
		from_json(json, result);
		co_return result;
	}

	template <typename ProtocolType, typename AsyncWriteStream>
	auto async_send_msg(AsyncWriteStream &s, ProtocolType const &msg) -> boost::future<bool> {
		using std::to_string;
		auto const str = to_string(msg);
		auto transmitted = co_await detail::async_write(s, boost::asio::buffer(protocol::prologue));
		transmitted += co_await detail::async_write(s, boost::asio::buffer(str));
		transmitted += co_await detail::async_write(s, boost::asio::buffer(protocol::epilogue));
		co_return transmitted == (protocol::prologue.size() + str.size() + protocol::epilogue.size());
	}
}    // namespace cmakeserver
