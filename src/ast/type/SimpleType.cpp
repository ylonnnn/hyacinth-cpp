#include "ast/type/SimpleType.hpp"

namespace AST
{
    SimpleType::SimpleType(Lexer::Token &identifier)
        : Type(::Program::Position(identifier.position)),
          identifier_(identifier)
    {
        end_pos_ = identifier_.position.col + identifier_.value.size();
    }

    Lexer::Token &SimpleType::identifier() { return identifier_; }

    void SimpleType::print(std::ostream &os, uint8_t tab) const
    {
        os << "SimpleType { identifier: " << identifier_ << " }";
    }

} // namespace AST
