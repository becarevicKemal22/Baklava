cmake_minimum_required(VERSION 3.25)
project(Maturski_2)

set(CMAKE_CXX_STANDARD 20)

include_directories("src/lexer" "src/error" "src/error/printer" "src/parser" "src/parser/ast/" "src/parser/ast/expressions" "src/parser/ast/statements" "src/interpreter/" "src/interpreter/runtimeValues" "src/error/exceptions" )
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
        src/parser/Parser.cpp
        src/parser/Parser.h
        src/parser/ast/Ast.h
        src/parser/ast/statements/Statement.h
        src/parser/ast/statements/Program.h
        src/parser/ast/expressions/Expression.h
        src/parser/ast/expressions/BinaryExpression.h
        src/parser/ast/expressions/UnaryExpression.h
        src/parser/ast/expressions/NullLiteralExpression.h
        src/parser/ast/expressions/StringLiteralExpression.h
        src/parser/ast/expressions/NumericLiteralExpression.h
        src/parser/ast/expressions/BooleanLiteralExpression.h
        src/parser/ast/PrettyPrint.h
        src/parser/ast/PrettyPrint.cpp
        src/interpreter/Interpreter.cpp
        src/interpreter/Interpreter.h
        src/interpreter/RuntimeValue.h
        src/error/exceptions/RuntimeError.h
        src/error/ErrorMessageArgument.h
        src/error/exceptions/WrongTypeError.h
        src/interpreter/RuntimeValue.cpp
        src/error/exceptions/ExceptionHelpers.h
        src/error/exceptions/ExceptionHelpers.cpp
        src/error/exceptions/WrongBinaryOperandTypes.h
        src/interpreter/Object.h
        src/interpreter/Object.cpp
)

add_subdirectory(tests)

target_link_libraries(Maturski_2 -static)

target_compile_options(Maturski_2 PRIVATE -fexec-charset=utf-8)