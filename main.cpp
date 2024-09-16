#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <codecvt>
#include <format>
#include <chrono>

#include "Lexer.h"
#include "Parser.h"
#include "Program.h"
#include "PrettyPrint.h"
#include "Interpreter.h"
#include "ParserError.h"
#include "Resolver.h"

void runRepl() {
    std::wcout << "Running repl\n";
}

void loadFile(const char *path, std::wstring &source) {
    std::wifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        ErrorPrinter printer(L"");
        printer.printErrorMessage(ERROR_CANNOT_OPEN_FILE, {path});
    }
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));

    wchar_t ch;
    while (file.get(ch)) {
        source += ch;
    }
}

void runFile(const char *path) {
//    std::wcout << "Running file: " << path << "\n";
    std::wstring source;
    loadFile(path, source);
    ErrorPrinter printer(source);
    Lexer lexer(source, &printer);
    try {
        lexer.tokenize();
    } catch (std::exception &e) {
        exit(0);
    }
    std::vector<Token *> tokens = lexer.tokens;
    Parser parser(tokens);
    std::unique_ptr<Program> program;
    try {
        program = parser.parse();
    } catch (ParserError& e) {
        printer.printParserError(&e);
        exit(0);
    }
//    printAST(program);
    Interpreter interpreter(&printer);
    try {
        Resolver resolver(&interpreter);
        resolver.resolve(program);
        std::wcout << std::endl;
    } catch (ParserError& e) {
        printer.printParserError(&e);
        exit(0);
    } catch (RuntimeError& e) {
        printer.printRuntimeError(&e);
        exit(0);
    }
//    for (auto statement: program->statements) {
//        auto start = std::chrono::high_resolution_clock::now();
////        RuntimeValue value = interpreter.evaluate(static_cast<Expression*>(statement));
////        std::wcout << "VALUE: " << ((ObjectString*)value.as.object)->value << "\n";
////        for (int i = 0; i < 100; i++) {
////            interpreter.evaluate(static_cast<Expression *>(statement));
////        }
//        auto end = std::chrono::high_resolution_clock::now();
//        std::chrono::duration<double> duration = end - start;
//        double duration_seconds = duration.count();
//        std::wcout << std::format(L"Elapsed time: {} seconds\n", duration_seconds);
//        std::wcout << val->stringify() << std::endl;
//    }


//    auto start = std::chrono::high_resolution_clock::now();
//
    interpreter.interpret(program.get());
//
//    auto end = std::chrono::high_resolution_clock::now();
//    std::chrono::duration<double> duration = end - start;
//    double duration_seconds = duration.count();
//    std::wcout << std::format(L"Elapsed time: {} seconds\n", duration_seconds);
}

int main(int argc, char **argv) {
    // Required for unicode
    _setmode(_fileno(stdout), _O_U8TEXT);

    if (argc == 1) {
        runRepl();
    }
    if (argc == 2) {
        runFile(argv[1]);
    }
    // Moze se dodati ovdje kao proces zavrsio sa kodom 0, a gore dodati u catch blokovima da se exita sa drugim kodoom,
    // koji se moze spasiti u errorprinteru, jer svakako svaki kroz njega prolazi.
}
