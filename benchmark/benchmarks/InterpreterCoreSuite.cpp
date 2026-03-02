//
// Created by kemal on 01-Mar-26.
//
#include <benchmark/benchmark.h>
#include <fstream>
#include <sstream>
#include "Interpreter.h"
#include "Lexer.h"
#include "Parser.h"
#include "Resolver.h"

void loadFile(const wchar_t* path, std::wstring& out)
{
    std::wifstream file(path);
    if (!file.is_open())
        throw std::runtime_error("Failed to open file");

    std::wstringstream buffer;
    buffer << file.rdbuf();
    out = buffer.str();
}

static void BM_InterpreterCoreSuite(benchmark::State& state, const std::wstring& path) {
    std::wstring source;
    loadFile(path.c_str(), source);
    Lexer lexer(source);
    lexer.tokenize();
    Parser parser(lexer.tokens);
    auto program = parser.parse();


    for (auto _ : state) {
        state.PauseTiming();
        Interpreter interpreter;
        Resolver resolver(&interpreter);
        resolver.resolve(program);
        state.ResumeTiming();
        interpreter.interpret(program.get());
    }
}

BENCHMARK_CAPTURE(BM_InterpreterCoreSuite, emptyForLoop, BENCHMARK_SCRIPTS_DIR L"/emptyForLoop.bk");
BENCHMARK_CAPTURE(BM_InterpreterCoreSuite, emptyWhileLoop, BENCHMARK_SCRIPTS_DIR L"/emptyWhileLoop.bk");
BENCHMARK_CAPTURE(BM_InterpreterCoreSuite, emptyWhileWithScope, BENCHMARK_SCRIPTS_DIR L"/emptyWhileWithScope.bk");
BENCHMARK_CAPTURE(BM_InterpreterCoreSuite, scopingStressTest, BENCHMARK_SCRIPTS_DIR L"/scopingStressTest.bk");
BENCHMARK_CAPTURE(BM_InterpreterCoreSuite, nestedScopingStressTest, BENCHMARK_SCRIPTS_DIR L"/nestedScopingStressTest.bk");
BENCHMARK_CAPTURE(BM_InterpreterCoreSuite, fib, BENCHMARK_SCRIPTS_DIR L"/fib.bk");
BENCHMARK_CAPTURE(BM_InterpreterCoreSuite, arithmetic, BENCHMARK_SCRIPTS_DIR L"/arithmetic.bk");
BENCHMARK_CAPTURE(BM_InterpreterCoreSuite, methodCall, BENCHMARK_SCRIPTS_DIR L"/methodCall.bk");
BENCHMARK_CAPTURE(BM_InterpreterCoreSuite, emptyFunctionCall, BENCHMARK_SCRIPTS_DIR L"/emptyFunctionCall.bk");
BENCHMARK_CAPTURE(BM_InterpreterCoreSuite, functionWith5ParamsCall, BENCHMARK_SCRIPTS_DIR L"/functionWith5ParamsCall.bk");
