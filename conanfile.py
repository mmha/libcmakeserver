from conans import ConanFile, CMake

class CMakeServerConan(ConanFile):
    name = "libcmakeserver"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    requires = "boost/[>=1.67]@conan/stable", "jsonformoderncpp/3.1.2@vthiery/stable"
    generators = "cmake"
    no_copy_source = True
    exports_sources = "*", "!build/*", "!.git/*"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        cmake.install()

    def package_info(self):
        self.cpp_info.includedirs = ['include']

    def package_id(self):
        self.info.header_only()
