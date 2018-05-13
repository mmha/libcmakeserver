#include <boost/process/search_path.hpp>
#include <cmakeserver/ipc.hpp>
#include <numeric>
#include <random>

namespace {
	/** `constexpr` iota() function from the STL.
	 * @returns past-the-end ForwardIterator in the destination range
	 */
	template <typename ForwardIterator, typename T>
	constexpr ForwardIterator cx_iota(ForwardIterator first, ForwardIterator last, T val) {
		for(; first != last; ++first) {
			*first = val;
			++val;
		}
		return first;
	}
}    // namespace

namespace cmakeserver {
	boost::process::child spawn_cmake_server(std::string_view const pipe) {
		using namespace boost::process;
		return child{search_path("cmake"), "-E", "server", std::string{"--pipe="} += pipe, "--experimental"};
	}

	fs::path make_temporary_filename() {
		fs::path base = fs::temp_directory_path() / "cmakeserver";
		if(exists(base) && !is_directory(base)) {
			// TODO custom error codes
			throw fs::filesystem_error{"not a directory", base, {}};
		} else if(!exists(base)) {
			create_directory(base);
		}

		constexpr auto char_table = [] {
			auto result = std::array<char, 26 * 2 + 10>{};
			auto it = begin(result);
			it = cx_iota(it, it + 26, 'a');
			it = cx_iota(it, it + 26, 'A');
			cx_iota(it, it + 10, '0');
			return result;
		}();

		fs::path result;
		std::random_device rd;
		std::mt19937 g(rd());
		std::uniform_int_distribution<size_t> dist(0, char_table.size());
		do {
			std::array<char, 16> file_name;
			std::generate_n(begin(file_name), file_name.size() - 1, [&] { return char_table[dist(g)]; });
			file_name.back() = '\0';
			result = base / file_name.data();
		} while(exists(result));
		return result;
	}
}    // namespace cmakeserver
