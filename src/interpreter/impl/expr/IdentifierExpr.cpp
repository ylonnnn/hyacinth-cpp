#include "ast/type/SimpleType.hpp"
#include "core/environment/Environment.hpp"
#include "interpreter/impl/Expr.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::IdentifierExpr>::interpret(Interpreter &interpreter,
                                                    AST::IdentifierExpr &node)
    {
        Core::Environment *current = interpreter.current_env();
        InterpretationResult result = {
            Core::ResultStatus::Success, node.value_ptr(), {}};

        if (result.data != nullptr)
            return result;

        std::string identifier(node.identifier().value);
        Core::Symbol *symbol = current->resolve_symbol(identifier);

        // if (symbol == nullptr)
        // {
        //     auto t_node = AST::TypeExpr(
        //         std::make_unique<AST::SimpleType>(node.identifier()));
        //     InterpretationResult t_res =
        //         InterpreterImpl<AST::TypeExpr>::interpret(interpreter,
        //         t_node);
        //     result.adapt(t_res.status, {}, t_res.data);
        //     result.symbol = t_res.symbol;

        //     if (result.status == Core::ResultStatus::Fail)
        //         result.error(
        //             &node,
        //             Diagnostic::ErrorTypes::Semantic::UnrecognizedSymbol,
        //             std::string("Unrecognized symbol \"") +
        //                 Diagnostic::ERR_GEN + identifier +
        //                 Utils::Styles::Reset + "\" provided.",
        //             "Used unrecognized symbol here");

        //     return result;
        // }

        result.symbol = symbol;

        if (auto ptr = dynamic_cast<Core::IdentifierSymbol *>(symbol))
            result.data = ptr->value;

        return result;
    }

} // namespace Interpreter
