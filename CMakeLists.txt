cmake_minimum_required(VERSION 3.25)
project(Baklava)

set(CMAKE_CXX_STANDARD 20)

include_directories("src/interpreter/controlFlowExceptions" "src/lexer" "src/error" "src/error/exceptions/parser" "src/error/printer" "src/parser" "src/parser/ast/" "src/parser/ast/expressions" "src/parser/ast/statements" "src/interpreter/" "src/interpreter/runtimeValues" "src/error/exceptions" "src/interpreter/nativeFunctions" )
add_executable(baklava main.cpp
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
        src/parser/ast/expressions/LogicalExpression.h
        src/error/exceptions/parser/ParserError.h
        src/error/exceptions/parser/ExpectedXBeforeY.h
        src/parser/ast/expressions/GroupingExpression.h
        src/parser/ast/statements/ExpressionStatement.h
        src/parser/ast/statements/PrintStatement.h
        src/parser/ast/expressions/VariableExpression.h
        src/parser/ast/statements/VarDeclarationStatement.h
        src/error/exceptions/parser/UninitializedConst.h
        src/interpreter/Environment.cpp
        src/interpreter/Environment.h
        src/parser/ast/expressions/AssignmentExpression.h
        src/error/exceptions/VariableRedeclaration.h
        src/error/exceptions/UndeclaredVariable.h
        src/error/exceptions/ConstReassignment.h
        src/error/exceptions/parser/InvalidLValue.h
        src/parser/ast/statements/BlockStatement.h
        src/parser/ast/statements/IfStatement.h
        src/parser/ast/statements/WhileStatement.h
        src/parser/ast/expressions/CallExpression.h
        src/parser/ast/statements/FunctionDeclarationStatement.h
        src/error/exceptions/InvalidCall.h
        src/error/exceptions/InvalidArgumentCount.h
        src/parser/ast/statements/ReturnStatement.h
        src/parser/Resolver.cpp
        src/parser/Resolver.h
        src/error/exceptions/parser/InvalidReturnPosition.h
        src/error/exceptions/parser/SelfReferencingInitializer.h
        src/parser/ast/expressions/IndexingExpression.h
        src/parser/ast/expressions/ArrayLiteralExpression.h
        src/parser/ast/expressions/IndexAssignmentExpression.h
        src/error/exceptions/IndexOutOfBounds.h
        src/error/exceptions/NonIntegerIndex.h
        src/error/exceptions/IndexingNonArray.h
        src/interpreter/nativeFunctions/BaseFunctions.h
        src/interpreter/nativeFunctions/BaseFunctions.cpp
        src/interpreter/nativeFunctions/NativeFunction.h
        src/interpreter/nativeFunctions/NativeFunction.cpp
)

add_subdirectory(tests)

target_link_libraries(baklava -static)

target_compile_options(baklava PRIVATE -fexec-charset=utf-8)