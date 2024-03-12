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
        ../src/interpreter/Interpreter.h
        interpreter/InterpreterUnaryExpressionsTest.cpp
        TestHelpers.cpp
        interpreter/InterpreterLiteralExpressionsTest.cpp
        ../src/error/exceptions/RuntimeError.h
        ../src/error/ErrorMessageArgument.h
        ../src/error/exceptions/WrongTypeError.h
        ../src/interpreter/RuntimeValue.cpp
        ../src/error/exceptions/ExceptionHelpers.cpp
        interpreter/InterpreterBinaryExpressionTest.cpp
        ../src/error/exceptions/WrongBinaryOperandTypes.h
        ../src/interpreter/Object.h
        ../src/interpreter/Object.cpp
        interpreter/InterpreterBinaryLogicalExpressionTest.cpp
        ../src/parser/ast/expressions/LogicalExpression.h
        parser/ParserLogicalExpressionTest.cpp
        parser/ParserBinaryComparisonExpressionTest.cpp
        interpreter/InterpreterBinaryComparisonExpressionTest.cpp
        ../src/error/exceptions/parser/ParserError.h
        ../src/error/exceptions/parser/ExpectedXBeforeY.h)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)