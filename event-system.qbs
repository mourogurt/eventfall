import qbs

Project {

    name: "eventfall"

    minimumQbsVersion: "1.8.0"

    references: [
        "conanbuildinfo.qbs",
        "benchmarks/benchmarks.qbs",
        "src/src.qbs",
        "tests/tests.qbs",
        "plugins/plugins.qbs"
    ]

    AutotestRunner { }
}
