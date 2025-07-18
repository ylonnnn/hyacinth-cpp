#include "ast/type/ConstantType.hpp"
#include "utils/style.hpp"

namespace AST
{
    ConstantType::ConstantType(Lexer::Token &constant)
        : Type(constant), constant_(constant)
    {
        end_pos_ = constant_.position.col + constant_.value.size();
    }

    Lexer::Token &ConstantType::constant() { return constant_; }

    std::string ConstantType::to_string() const
    {
        return std::string(value_.value);
    }

    void ConstantType::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);
        os << "ConstantType {";

        os << "\n" << inner_indentation << "constant: " << constant_;

        os << "\n" << indentation << "}";
    }

} // namespace AST
