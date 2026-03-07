//
// Created by kemal on 01-Mar-26.
//
#include <benchmark/benchmark.h>
#include "Environment.h"

static void BM_EnvRangedSizeLookup(benchmark::State& state) {
    Environment *env = Environment::allocate(nullptr);
    int num_vars = state.range(0); // Uzima broj iz Range-a ispod

    // SETUP: Napuni environment sa N varijabli
    for (int i = 0; i < num_vars; ++i) {
        Token* name = new Token(TokenType::Identifier, L"var" + std::to_wstring(i), 0, 0);
        env->define(name, {ValueType::Number, {.number = static_cast<double>(i)}}, false);
    }

    Token* search_key = new Token(TokenType::Identifier, L"var" + std::to_wstring(num_vars - 1), 0, 0); // Tests worst case when env is implemented as linear lookup since this looks for the last one. Maybe should change to look in the middle as an additional metric for this test.

    for (auto _ : state) {
        // MERENJE: Tražimo zadnju ubačenu varijablu (worst case za linear search)
        auto res = env->get(search_key);
        benchmark::DoNotOptimize(res);
    }

    // Opciono: prikaži kompleksnost u finalnom reportu
    state.SetComplexityN(state.range(0));
}

// Pokreni test za 8, 64, 512, i 4096 varijabli
BENCHMARK(BM_EnvRangedSizeLookup)->Range(8, 4096)->Complexity();
