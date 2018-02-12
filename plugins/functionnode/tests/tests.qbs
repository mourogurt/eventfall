import qbs

Project {
    Product {
        type: ["application", "autotest"]
        name: "Function Node tests"
        files: ["*.cpp","*.hpp"]
        consoleApplication: true

        Group {
            fileTagsFilter: product.type
            qbs.install: true
        }

        cpp.cxxLanguageVersion: "c++1z"

        Depends { name: "Function Node" }
        Depends { name: "Qt"; submodules: ["core","test"] }
    }
}
