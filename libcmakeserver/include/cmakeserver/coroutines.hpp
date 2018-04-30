#pragma once
#include "use_boost_future.hpp"
#include <boost/asio/system_timer.hpp>
#include <boost/thread/future.hpp>
#include <experimental/coroutine>

// These adaptors enable the use of boost futures/promises with C++ coroutines
// Useful for IO using asio

template <typename... Args>
struct std::experimental::coroutine_traits<boost::future<void>, Args...> {
	struct promise_type {
		boost::promise<void> p;
		auto get_return_object() {
			return p.get_future();
		}
		std::experimental::suspend_never initial_suspend() {
			return {};
		}
		std::experimental::suspend_never final_suspend() {
			return {};
		}
		void set_exception(std::exception_ptr e) {
			p.set_exception(std::move(e));
		}
		void unhandled_exception() {
		}
		void return_void() {
			p.set_value();
		}
	};
};

template <typename R, typename... Args>
struct std::experimental::coroutine_traits<boost::future<R>, Args...> {
	struct promise_type {
		boost::promise<R> p;
		auto get_return_object() {
			return p.get_future();
		}
		std::experimental::suspend_never initial_suspend() {
			return {};
		}
		std::experimental::suspend_never final_suspend() {
			return {};
		}
		void set_exception(std::exception_ptr e) {
			p.set_exception(std::move(e));
		}
		void unhandled_exception() {
		}
		template <typename U>
		void return_value(U &&u) {
			p.set_value(std::forward<U>(u));
		}
	};
};

template <typename R>
auto operator co_await(boost::future<R> &&f) {
	struct awaiter {
		boost::future<R> &&input;
		boost::future<R> output;
		bool await_ready() {
			if(input.is_ready()) {
				output = std::move(input);
				return true;
			}
			return false;
		}
		auto await_resume() {
			return output.get();
		}
		void await_suspend(std::experimental::coroutine_handle<> coro) {
			input.then([this, coro](auto result_future) mutable {
				this->output = std::move(result_future);
				coro.resume();
			});
		}
	};
	return awaiter{static_cast<boost::future<R> &&>(f), {}};
}

namespace cmakeserver {
	/**
	 * @brief suspends the coroutine for a specified amount of time
	 *
	 * @param t timer to use
	 * @param d time duration to wait
	 * @return an anonymously typed object to wrap around the callback logic of the timer - use `co_await` and ignore it
	 */
	template <typename R, typename P>
	auto async_await(boost::asio::system_timer &t, std::chrono::duration<R, P> d) {
		struct awaiter {
			boost::asio::system_timer &t;
			std::chrono::duration<R, P> d;
			boost::system::error_code ec;

			bool await_ready() {
				return d.count() == 0;
			}
			void await_resume() const {
				if(ec) {
					throw boost::system::system_error(ec);
				}
			}
			void await_suspend(std::experimental::coroutine_handle<> coro) {
				t.expires_from_now(d);
				t.async_wait([this, coro](auto ec) mutable {
					this->ec = ec;
					coro.resume();
				});
			}
		};
		return awaiter{t, d, {}};
	}
}    // namespace cmakeserver
