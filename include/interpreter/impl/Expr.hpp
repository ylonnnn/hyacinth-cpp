#pragma once

#include "ast/expr/BinaryExpr.hpp"
#include "ast/expr/FunctionCallExpr.hpp"
#include "ast/expr/IdentifierExpr.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "ast/expr/TypeExpr.hpp"
#include "ast/expr/UnaryExpr.hpp"
#include "ast/expr/compound/ArrayExpr.hpp"
#include "ast/expr/compound/InstanceExpr.hpp"
#include "interpreter/Interpreter.hpp"

namespace Interpreter
{
    template <> struct InterpreterImpl<AST::Expr>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::Expr &node);
    };

    template <> struct InterpreterImpl<AST::LiteralExpr>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::LiteralExpr &node);
    };

    template <> struct InterpreterImpl<AST::IdentifierExpr>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::IdentifierExpr &node);
    };

    // template <> struct InterpreterImpl<AST::UnaryExpr>
    // {
    //     static InterpretationResult interpret(Interpreter &interpreter,
    //                                           AST::UnaryExpr &node);
    // };

    template <> struct InterpreterImpl<AST::BinaryExpr>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::BinaryExpr &node);
    };

    template <> struct InterpreterImpl<AST::FunctionCallExpr>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::FunctionCallExpr &node);
    };

    template <> struct InterpreterImpl<AST::InstanceExpr>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::InstanceExpr &node);
    };

    template <> struct InterpreterImpl<AST::ArrayExpr>
    {
        static InterpretationResult interpret(Interpreter &interpreter,
                                              AST::ArrayExpr &node);
    };

    // template <> struct InterpreterImpl<AST::TypeExpr>
    // {
    //     static InterpretationResult interpret(Interpreter &interpreter,
    //                                           AST::TypeExpr &node);
    // };

} // namespace Interpreter
