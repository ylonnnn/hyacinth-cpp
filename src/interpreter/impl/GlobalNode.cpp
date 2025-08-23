#include "ast/block/GlobalBlock.hpp"
#include "interpreter/Interpreter.hpp"
#include "interpreter/impl/Stmt.hpp"

namespace Interpreter
{
    InterpretationResult
    InterpreterImpl<AST::GlobalNode>::interpret(Interpreter &interpreter,
                                                AST::GlobalNode &node)
    {
        if (typeid(node) == typeid(AST::ImportStmt))
            return interpreter.interpret(static_cast<AST::ImportStmt &>(node));

        else if (auto ptr = dynamic_cast<AST::GlobalBlock *>(&node))
            return interpreter.interpret(*ptr);

        else if (auto ptr = dynamic_cast<AST::DeclarationStmt *>(&node))
            return interpreter.interpret(*ptr);

        return {Core::ResultStatus::Fail, nullptr, {}};
    }

} // namespace Interpreter
