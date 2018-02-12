from conans import ConanFile, CMake, tools
import os, shutil


class TBBConan(ConanFile):
    name = "TBB"
    version = "2018.2"
    license = "Apache-2.0"
    url = "https://github.com/01org/tbb.git"
    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False]}
    default_options = "shared=False"

    def source(self):
        tools.download("https://github.com/01org/tbb/archive/2018_U2.tar.gz", "tbb.zip")
        tools.untargz("tbb.zip")
        os.unlink("tbb.zip")
        shutil.move("tbb-2018_U2", "tbb")
     
    def build(self):
        arch="ia32" if self.settings.arch=="x86" else "intel64"
        if self.settings.compiler == "Visual Studio":
            vcvars = tools.vcvars_command(self.settings) 
            self.run("%s && cd tbb && mingw32-make arch=%s" % (vcvars, arch))
        else:
           self.run("cd tbb && make arch=%s" % ( arch)) 

    def package(self):
        self.copy("*.h", "include", "tbb/include")
        build_folder = "tbb/build/"
        build_type = "debug" if self.settings.build_type == "Debug" else "release"
        self.copy("*%s*.lib" % build_type, "lib", build_folder, keep_path=False)
        self.copy("*%s*.a" % build_type, "lib", build_folder, keep_path=False) 
        self.copy("*%s*.dll" % build_type, "bin", build_folder, keep_path=False)
        self.copy("*%s*.dylib" % build_type, "lib", build_folder, keep_path=False)
        self.copy("*%s*.so*" % build_type, "lib", build_folder, keep_path=False)

    def package_info(self):
        if self.settings.build_type == "Debug":
            self.cpp_info.libs.extend(["tbb_debug", "tbbmalloc_debug", "tbbmalloc_proxy_debug"])
        else:
            self.cpp_info.libs.extend(["tbb", "tbbmalloc", "tbbmalloc_proxy"])  
