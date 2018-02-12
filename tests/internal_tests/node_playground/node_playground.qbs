import qbs

Project {
    CppApplication {
        files: ["*.cpp","*.hpp"]
        consoleApplication: true

        Group {     // Properties for the produced executable
            fileTagsFilter: product.type
            qbs.install: true
        }

        cpp.defines: ['BOOST_HANA_CONFIG_ENABLE_STRING_UDL','COUSOUS_DISABLE_MOST_CT_CHECKS']
        //cpp.cxxFlags: ['-Wno-non-template-friend']
        //cpp.cxxStandardLibrary: "libc++"
        cpp.cxxLanguageVersion: "c++1z"

        Depends { name: "Containers" }
        Depends { name: "Function Node" }
        Depends { name: "Utils" }
    }
}
