//
// Created by kemal on 28-Feb-26.
//
#include <benchmark/benchmark.h>
#include <iostream>
#include "Environment.h"

static void BM_EnvironmentLookup(benchmark::State& state) {
    for (auto _ : state) {
        // Sve UNUTAR petlje se meri (hiljade puta)
        Environment env;

        benchmark::DoNotOptimize(env);
    }
}
BENCHMARK(BM_EnvironmentLookup);
