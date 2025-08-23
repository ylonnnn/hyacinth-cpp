#pragma once

#include "ast/GlobalNode.hpp"
#include "ast/stmt/BlockStmt.hpp"
#include "ast/stmt/DeclarationStmt.hpp"
#include "ast/stmt/ExprStmt.hpp"
#include "ast/stmt/ImportStmt.hpp"
#include "ast/stmt/function/FunctionDeclStmt.hpp"
#include "ast/stmt/function/FunctionRetStmt.hpp"
#include "ast/stmt/variable/VariableDeclStmt.hpp"
#include "interpreter/Interpreter.hpp"

namespace Interpreter
{
    template <> struct InterpreterImpl<AST::Stmt>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::Stmt &node);
    };

    template <> struct InterpreterImpl<AST::ExprStmt>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::ExprStmt &node);
    };

    template <> struct InterpreterImpl<AST::BlockStmt>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::BlockStmt &node);
    };

    template <> struct InterpreterImpl<AST::GlobalNode>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::GlobalNode &node);
    };

    template <> struct InterpreterImpl<AST::ImportStmt>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::ImportStmt &node);
    };

    template <> struct InterpreterImpl<AST::DeclarationStmt>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::DeclarationStmt &node);
    };

    template <> struct InterpreterImpl<AST::VariableDeclarationStmt>
    {
        static InterpretationResult
        interpret(Interpreter &interpreter, AST::VariableDeclarationStmt &node);
    };

    template <> struct InterpreterImpl<AST::FunctionDeclarationStmt>
    {
        static InterpretationResult
        interpret(Interpreter &interpreter, AST::FunctionDeclarationStmt &node);
    };

    template <> struct InterpreterImpl<AST::FunctionReturnStmt>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::FunctionReturnStmt &node);
    };

} // namespace Interpreter
