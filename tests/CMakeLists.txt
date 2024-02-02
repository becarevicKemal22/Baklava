project(tests)
add_subdirectory(lib/Catch2)

add_executable(tests ../src/error/printer/ErrorPrinter.cpp ../src/lexer/Lexer.cpp
        lexer/LexerTest.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)