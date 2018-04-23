#pragma once
#include <boost/asio/async_result.hpp>
#include <boost/asio/handler_type.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>
#include <boost/thread/future.hpp>
#include <future>
#include <memory>

/**
 * This is almost a 1:1 copy of Boost's use_std_future, with std::future switched out for boost::future.
 */

namespace cmakeserver {

	/// Class used to specify that an asynchronous operation should return a future.
	/**
	 * The use_boost_future_t class is used to indicate that an asynchronous operation
	 * should return a boost::future object. A use_boost_future_t object may be passed as a
	 * handler to an asynchronous operation, typically using the special value @c
	 * use_boost_future. For example:
	 *
	 * @code boost::future<std::size_t> my_future
	 *   = my_socket.async_read_some(my_buffer, boost::asio::use_boost_future); @endcode
	 *
	 * The initiating function (async_read_some in the above example) returns a
	 * future that will receive the result of the operation. If the operation
	 * completes with an error_code indicating failure, it is converted into a
	 * system_error and passed back to the caller via the future.
	 */
	template <typename Allocator = std::allocator<void>>
	class use_boost_future_t {
		public:
		/// The allocator type. The allocator is used when constructing the
		/// @c std::promise object for a given asynchronous operation.
		using allocator_type = Allocator;

		/// Construct using default-constructed allocator.
		constexpr use_boost_future_t() = default;

		/// Construct using specified allocator.
		explicit use_boost_future_t(Allocator const &allocator)
		    : allocator_(allocator) {
		}

		/// Specify an alternate allocator.
		template <typename OtherAllocator>
		use_boost_future_t<OtherAllocator> operator[](OtherAllocator const &allocator) const {
			return use_boost_future_t<OtherAllocator>(allocator);
		}

		/// Obtain allocator.
		allocator_type get_allocator() const {
			return allocator_;
		}

		private:
		Allocator allocator_;
	};

/// A special value, similar to std::nothrow.
/**
 * See the documentation for boost::asio::use_future_t for a usage example.
 */
#ifdef _MSC_VER
	__declspec(selectany) use_boost_future_t<> use_boost_future;
#else
	constexpr use_boost_future_t<> use_boost_future;
#endif

	namespace detail {
		// Completion handler to adapt a promise as a completion handler.
		template <typename T>
		class promise_handler {
			public:
			// Construct from use_future special value.
			template <typename Alloc>
			promise_handler(use_boost_future_t<Alloc> uf)
			    : promise_(std::allocate_shared<boost::promise<T>>(
			          typename std::allocator_traits<Alloc>::template rebind_alloc<char>(uf.get_allocator()),
			          std::allocator_arg,
			          typename std::allocator_traits<Alloc>::template rebind_alloc<char>(uf.get_allocator()))) {
			}

			void operator()(T t) {
				promise_->set_value(t);
			}

			void operator()(const boost::system::error_code &ec, T t) {
				if(ec) {
					promise_->set_exception(std::make_exception_ptr(boost::system::system_error(ec)));
				} else {
					promise_->set_value(t);
				}
			}

			// private:
			std::shared_ptr<boost::promise<T>> promise_;
		};

		// Completion handler to adapt a void promise as a completion handler.
		template <>
		class promise_handler<void> {
			public:
			// Construct from use_future special value. Used during rebinding.
			template <typename Alloc>
			promise_handler(use_boost_future_t<Alloc> uf)
			    : promise_(std::allocate_shared<boost::promise<void>>(
			          typename std::allocator_traits<Alloc>::template rebind_alloc<char>(uf.get_allocator()),
			          std::allocator_arg,
			          typename std::allocator_traits<Alloc>::template rebind_alloc<char>(uf.get_allocator()))) {
			}

			void operator()() {
				promise_->set_value();
			}

			void operator()(const boost::system::error_code &ec) {
				if(ec) {
					promise_->set_exception(std::make_exception_ptr(boost::system::system_error(ec)));
				} else {
					promise_->set_value();
				}
			}

			// private:
			std::shared_ptr<boost::promise<void>> promise_;
		};

		// Ensure any exceptions thrown from the handler are propagated back to the
		// caller via the future.
		template <typename Function, typename T>
		void asio_handler_invoke(Function f, promise_handler<T> *h) {
			std::shared_ptr<boost::promise<T>> p(h->promise_);
			try {
				f();
			} catch(...) { p->set_exception(std::current_exception()); }
		}

	}    // namespace detail
}    // namespace cmakeserver

namespace boost::asio {
	// Handler traits specialization for promise_handler.
	template <typename T>
	class async_result<cmakeserver::detail::promise_handler<T>> {
		public:
		// The initiating function will return a future.
		using type = boost::future<T>;

		// Constructor creates a new promise for the async operation, and obtains the
		// corresponding future.
		explicit async_result(cmakeserver::detail::promise_handler<T> &h) {
			value_ = h.promise_->get_future();
		}

		// Obtain the future to be returned from the initiating function.
		type get() {
			return std::move(value_);
		}

		private:
		type value_;
	};

	// Handler type specialisation for use_future.
	template <typename Allocator, typename ReturnType>
	struct handler_type<cmakeserver::use_boost_future_t<Allocator>, ReturnType()> {
		using type = cmakeserver::detail::promise_handler<void>;
	};

	// Handler type specialisation for use_future.
	template <typename Allocator, typename ReturnType, typename Arg1>
	struct handler_type<cmakeserver::use_boost_future_t<Allocator>, ReturnType(Arg1)> {
		using type = cmakeserver::detail::promise_handler<Arg1>;
	};

	// Handler type specialisation for use_future.
	template <typename Allocator, typename ReturnType>
	struct handler_type<cmakeserver::use_boost_future_t<Allocator>, ReturnType(boost::system::error_code)> {
		using type = cmakeserver::detail::promise_handler<void>;
	};

	// Handler type specialisation for use_future.
	template <typename Allocator, typename ReturnType, typename Arg2>
	struct handler_type<cmakeserver::use_boost_future_t<Allocator>, ReturnType(boost::system::error_code, Arg2)> {
		using type = cmakeserver::detail::promise_handler<Arg2>;
	};
}    // namespace boost::asio
