import qbs

Project {
    Product {
        type: ["application", "autotest"]
        files: ["*.cpp","*.hpp"]
        consoleApplication: true

        Group {     // Properties for the produced executable
            fileTagsFilter: product.type
            qbs.install: true
        }

        cpp.cxxLanguageVersion: "c++1z"

        Depends { name: "Containers" }
        Depends { name: "Qt"; submodules: ["core","test"] }
    }
}
