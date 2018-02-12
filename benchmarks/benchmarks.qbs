import qbs

Project {
    references: [
        "01_queue_bench/queue_bench.qbs",
        "02_set_bench/set_bench.qbs",
        "03_map_bench/map_bench.qbs"
    ]
}
