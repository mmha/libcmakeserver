#include <cmakeserver/ipc.hpp>
#include <doctest.h>

TEST_CASE("make_temporary_filename") {
	using namespace cmakeserver;
	SUBCASE("make_filename") {
		auto const generated = make_temporary_filename();
		CHECK(!exists(generated));
		CHECK(generated.parent_path() == fs::temp_directory_path() / "cmakeserver");
	}
	SUBCASE("uniqueness") {
		std::array<fs::path, 256> paths;
		std::generate(begin(paths), end(paths), make_temporary_filename);
		std::sort(begin(paths), end(paths));

		for(auto const &p : paths) {
			CHECK(!exists(p));
			CHECK(p.parent_path() == fs::temp_directory_path() / "cmakeserver");
		}
		CHECK(std::adjacent_find(begin(paths), end(paths)) == end(paths));
	}
	SUBCASE("directory_is_file") {
		fs::path base = fs::temp_directory_path() / "cmakeserver";
		if(exists(base)) {
			remove_all(base);
		}

		std::ofstream{base};
		struct remove_file {
			fs::path const &p;
			~remove_file() {
				remove_all(p);
			}
		} r{base};

		CHECK_THROWS(make_temporary_filename());
	}
}
