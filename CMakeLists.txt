cmake_minimum_required(VERSION 3.25)
project(Maturski_2)

set(CMAKE_CXX_STANDARD 20)

include_directories("src/lexer" "src/error" "src/error/printer")
add_executable(Maturski_2 main.cpp
        src/lexer/Lexer.cpp
        src/lexer/Lexer.h
        src/lexer/Token.h
        src/lexer/TokenType.h
        src/error/ErrorCode.h
        src/error/printer/ErrorPrinter.h
        src/error/ErrorMessages.h
        src/error/printer/ErrorPrinter.cpp
        src/lexer/Keywords.h
)

target_link_libraries(Maturski_2 -static)

target_compile_options(Maturski_2 PRIVATE -fexec-charset=utf-8)