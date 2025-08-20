#include "interpreter/impl/Expr.hpp"
#include "interpreter/Interpreter.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::Expr>::interpret(Interpreter &interpreter,
                                          AST::Expr &node)
    {
        if (typeid(node) == typeid(AST::LiteralExpr))
            return InterpreterImpl<AST::LiteralExpr>::interpret(
                interpreter, static_cast<AST::LiteralExpr &>(node));

        else if (typeid(node) == typeid(AST::IdentifierExpr))
            return InterpreterImpl<AST::IdentifierExpr>::interpret(
                interpreter, static_cast<AST::IdentifierExpr &>(node));

        else if (typeid(node) == typeid(AST::BinaryExpr))
            return InterpreterImpl<AST::BinaryExpr>::interpret(
                interpreter, static_cast<AST::BinaryExpr &>(node));

        else if (typeid(node) == typeid(AST::FunctionCallExpr))
            return InterpreterImpl<AST::FunctionCallExpr>::interpret(
                interpreter, static_cast<AST::FunctionCallExpr &>(node));

        else if (typeid(node) == typeid(AST::InstanceExpr))
            return InterpreterImpl<AST::InstanceExpr>::interpret(
                interpreter, static_cast<AST::InstanceExpr &>(node));

        else if (typeid(node) == typeid(AST::ArrayExpr))
            return InterpreterImpl<AST::ArrayExpr>::interpret(
                interpreter, static_cast<AST::ArrayExpr &>(node));

        // else if (typeid(node) == typeid(AST::TypeExpr))
        //     return InterpreterImpl<AST::TypeExpr>::interpret(
        //         interpreter, static_cast<AST::TypeExpr &>(node));

        return {Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Interpreter
