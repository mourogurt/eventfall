import qbs

Project {
    id: node_root
    Product {
        id: node
        name: "Node"
        type: "dynamiclibrary"
        files: ["*.cpp","*.hpp"]
        Export {
            Depends { name: "cpp" }
            Depends { name: "TBB" }
            Depends { name: "IdealCouscous" }
            Depends { name: "Memory" }
            cpp.includePaths: [node.sourceDirectory]
            cpp.cxxLanguageVersion: "c++1z"
            cpp.defines: ['BOOST_HANA_CONFIG_ENABLE_STRING_UDL']
        }
        Depends { name: "cpp" }
        Depends { name: "TBB" }
        Depends { name: "IdealCouscous" }
        Depends { name: "Memory" }
        Depends { name: "Utils" }
        cpp.cxxLanguageVersion: "c++1z"
        cpp.defines: ['BOOST_HANA_CONFIG_ENABLE_STRING_UDL','COUSOUS_DISABLE_MOST_CT_CHECKS']
        Group {
            fileTagsFilter: "dynamiclibrary"
            qbs.install: true
            qbs.installDir: "lib"
        }
    }
}
