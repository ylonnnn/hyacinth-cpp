#include "ast/type/SimpleType.hpp"

namespace AST
{
    SimpleType::SimpleType(Lexer::Token &identifier) : Type(identifier) {}

    Lexer::Token &SimpleType::identifier() { return value_; }

    std::string SimpleType::to_string() const
    {
        return std::string(value_.value);
    }

    void SimpleType::print(std::ostream &os, [[maybe_unused]] uint8_t tab) const
    {
        os << "SimpleType { identifier: " << value_ << " }";
    }

} // namespace AST
