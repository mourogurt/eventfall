import qbs

Project {
    Product {
        id: FunctionNode
        name: "Function Node"
        cpp.cxxLanguageVersion: "c++1z"
        files: [
            "include/*.hpp"
        ]
        Depends { name: "cpp" }
        Depends { name: "Memory" }
        Depends { name: "Node" }
        cpp.includePaths: [FunctionNode.sourceDirectory+"/include"]
        Export {
                Depends { name: "cpp" }
                Depends { name: "Memory" }
                Depends { name: "Node" }
                cpp.includePaths: [FunctionNode.sourceDirectory+"/include"]
                cpp.cxxLanguageVersion: "c++1z"
        }
    }
    references: [
        "tests/tests.qbs"
    ]
}
