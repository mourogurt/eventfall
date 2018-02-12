import qbs

Project {
    id: utils_root
    Product {
        id: utils
        name: "Utils"
        type: "dynamiclibrary"
        files: ["*.cpp","*.hpp"]
        Depends { name: "cpp" }
        cpp.cxxLanguageVersion: "c++1z"
        Export {
            Depends { name: "cpp" }
            cpp.includePaths: [utils.sourceDirectory]
            cpp.cxxLanguageVersion: "c++1z"
        }
    }
}
