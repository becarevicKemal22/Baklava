#include <iostream>
#include <fstream>
#include <io.h>
#include <fcntl.h>
#include <codecvt>
#include <format>

#include "Lexer.h"
#include "Parser.h"
#include "Program.h"
#include "PrettyPrint.h"


void runRepl(){
    std::wcout << "Running repl\n";
}

void loadFile(const char* path, std::wstring& source){
    std::wifstream file(path, std::ios::binary);
    if(!file.is_open()){
        ErrorPrinter::printErrorMessage(ERROR_CANNOT_OPEN_FILE, {path});
    }
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));

    wchar_t ch;
    while (file.get(ch)) {
        source += ch;
    }
}

void runFile(const char* path){
    std::wcout << "Running file: " << path << "\n";
    std::wstring source;
    loadFile(path, source);
    ErrorPrinter printer(source);
    Lexer lexer(source, &printer);
    try{
        lexer.tokenize();
    }catch(std::exception& e){
        exit(0);
    }
    std::vector<Token> tokens = lexer.tokens;
    Parser parser(tokens);
    std::unique_ptr<Program> program = parser.parse();
    printAST(program);

}

int main(int argc, char** argv) {
    // Required for unicode
    _setmode(_fileno(stdout), _O_U8TEXT);

    if(argc == 1){
        runRepl();
    }
    if(argc == 2){
        runFile(argv[1]);
    }
}
