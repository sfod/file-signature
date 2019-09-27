#include <algorithm>

#include "benchmark/benchmark.h"

#include "reader.hpp"


static void bench_read(benchmark::State &state)
{
    signature::Reader reader("./test/data/big", state.range(0));

    for (auto _ : state) {
        while (reader.has_next()) {
            benchmark::DoNotOptimize(reader.next());
        }
        benchmark::ClobberMemory();
    }
}
BENCHMARK(bench_read)->RangeMultiplier(4)->Range(1<<8, 1<<24)->Repetitions(5)->ReportAggregatesOnly(true);

BENCHMARK_MAIN();
