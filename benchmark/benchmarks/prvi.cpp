//
// Created by kemal on 28-Feb-26.
//
#include <benchmark/benchmark.h>
#include <iostream>
#include "Environment.h"

static void BM_EnvironmentLookup(benchmark::State& state) {
    for (auto _ : state) {
        // Sve UNUTAR petlje se meri (hiljade puta)
        for (int i = 0; i < 10000; i++) {
            Environment *env = Environment::allocate(nullptr);
            env->define(new Token(TokenType::Identifier, L"myVar", 0, 0), {ValueType::Null, {}}, false);
            benchmark::DoNotOptimize(env);
        }
    }
}
BENCHMARK(BM_EnvironmentLookup);
