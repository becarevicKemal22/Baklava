project(tests)
add_subdirectory(lib/Catch2)

add_executable(tests ../src/error/printer/ErrorPrinter.cpp ../src/lexer/Lexer.cpp ../src/parser/Parser.cpp
        lexer/LexerTest.cpp
        parser/ParserLiteralTest.cpp
        parser/ParserTest.cpp
        parser/ParserUnaryExpressionTest.cpp
        TestHelpers.h
        parser/ParserBinaryExpressionTest.cpp
        ../src/interpreter/Interpreter.cpp
        ../src/interpreter/Interpreter.h)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)