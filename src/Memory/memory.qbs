import qbs

Project {
    id: memory_root
    Product {
        id: memory
        name: "Memory"
        type: "dynamiclibrary"
        files: ["*.cpp","*.hpp"]
        Export {
            Depends { name: "cpp" }
            Depends { name: "Containers" }
            cpp.includePaths: [memory.sourceDirectory]
            cpp.cxxLanguageVersion: "c++1z"
        }
        Depends { name: "Containers" }
        cpp.cxxLanguageVersion: "c++1z"
        Group {
            fileTagsFilter: "dynamiclibrary"
            qbs.install: true
            qbs.installDir: "lib"
        }
    }
}

