import qbs

Project {
    Product {
        type: ["application", "autotest"]
        name: "Memory Tests"
        files: ["*.cpp","*.hpp"]
        consoleApplication: true

        Group {
            fileTagsFilter: product.type
            qbs.install: true
        }

        Properties {
            condition: qbs.toolchain.contains("gcc")
            cpp.cxxFlags: outer.concat(["-fno-strict-aliasing"])
        }
        cpp.cxxLanguageVersion: "c++1z"

        Depends { name: "Memory" }
        Depends { name: "Qt"; submodules: ["core","test"] }
    }
}
