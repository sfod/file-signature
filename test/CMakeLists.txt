option(ENABLE_BENCHMARK "Enable benchmarking" ON)

if (ENABLE_BENCHMARK)
    # Something is wrong with benchmark library tests, disable them.
    option(BENCHMARK_ENABLE_TESTING OFF)

    add_subdirectory(benchmark)

    add_executable(bench_read bench_read.cpp ../src/reader.cpp)
    target_include_directories(bench_read
        PRIVATE ${PROJECT_SOURCE_DIR}/src/
    )
    target_link_libraries(bench_read pthread benchmark stdc++fs)
endif()
