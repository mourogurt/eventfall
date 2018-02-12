from conans import ConanFile, CMake, tools


class CdsConan(ConanFile):
    name = "cds"
    version = "2.3.1"
    license = "BDS"
    url = "http://libcds.sourceforge.net"
    description = "Concurent data structures library"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=True"
    generators = "cmake"
    exports_sources = "cds/*"

    def source(self):
        tools.download("https://github.com/khizmax/libcds/archive/v2.3.1.zip", "v2.3.1.zip")
        tools.unzip("v2.3.1.zip")
        self.run("mv libcds-2.3.1 cds")
        self.run("cd cds")
        # This small hack might be useful to guarantee proper /MT /MD linkage in MSVC
        # if the packaged project doesn't have variables to set it properly
        # tools.replace_in_file("cds/CMakeLists.txt", "PROJECT(cds)", '''PROJECT(cds)include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)conan_basic_setup()''')
        

    def build(self):
        cmake = CMake(self)
        shared = ""
        self.run('cmake cds -DENABLE_STRESS_TEST=0 -DENABLE_UNIT_TEST=0 %s %s' % (cmake.command_line, shared))
        self.run("cmake --build . %s" % cmake.build_config)

    def package(self):
        self.copy("*.h", dst="include", src="cds")
        self.copy("*.hpp", dst="include", src="cds")
        self.copy("*.lib", dst="lib", keep_path=False)
        self.copy("*.dll", dst="bin", keep_path=False)
        self.copy("*.so*", dst="lib", keep_path=False)
        self.copy("*.dylib", dst="lib", keep_path=False)
        self.copy("*.a", dst="lib", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["cds"]
