#include <cmakeserver/ipc.hpp>

#include <boost/process/search_path.hpp>

namespace cmakeserver {
	boost::process::child spawn_cmake_server(std::string_view const pipe) {
		using namespace boost::process;
		return child{search_path("cmake"), "-E", "server", std::string{"--pipe="} += pipe.data(), "--experimental"};
	}
}    // namespace cmakeserver
