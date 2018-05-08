#pragma once
#if defined(WIN32)
#include "win32/ipc.hpp"
#elif defined(__unix__)
#include "posix/ipc.hpp"
#endif
#include <boost/process/child.hpp>
#include <string_view>

namespace cmakeserver {
	boost::process::child spawn_cmake_server(std::string_view pipe);
}    // namespace cmakeserver
