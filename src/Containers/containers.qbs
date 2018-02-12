import qbs

Project {
    id: cc_root
    Product {
        id: cc
        name: "Containers"
        type: "dynamiclibrary"
        files: ["*.cpp","*.hpp"]
        Depends { name: "cpp" }
        Depends { name: "TBB" }
        Depends { name: "cds" }
        Depends { name: "IdealCouscous" }
        cpp.cxxLanguageVersion: "c++1z"
        cpp.defines: ['BOOST_HANA_CONFIG_ENABLE_STRING_UDL','COUSOUS_DISABLE_MOST_CT_CHECKS']
        Export {
            Depends { name: "cpp" }
            Depends { name: "TBB" }
            Depends { name: "cds" }
            Depends { name: "IdealCouscous" }
            cpp.includePaths: [cc.sourceDirectory]
            cpp.cxxLanguageVersion: "c++1z"
            cpp.defines: ['BOOST_HANA_CONFIG_ENABLE_STRING_UDL']
            cpp.dynamicLibraries: ['atomic']
        }
        Group {
            fileTagsFilter: "dynamiclibrary"
            qbs.install: true
            qbs.installDir: "lib"
        }
    }
}
