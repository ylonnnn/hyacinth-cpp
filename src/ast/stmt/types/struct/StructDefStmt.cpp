#include "ast/stmt/types/struct/StructDefStmt.hpp"
#include "ast/Node.hpp"
#include "ast/common/Identifier.hpp"
#include "utils/style.hpp"

namespace AST
{
    StructField::StructField(Lexer::Token &identifier,
                             IdentifierMutabilityState mut_state,
                             std::unique_ptr<Type> &&type,
                             std::unique_ptr<Expr> &&default_value)
        : Node(identifier.range.start),
          IdentifierDecl(identifier, mut_state, std::move(type))
    {
    }

    void StructField::print(std::ostream &os,
                            [[maybe_unused]] uint8_t tab) const
    {
        os << "Field { " << identifier.value << ": " << type->to_string()
           << *default_value << " }";
    }

    StructDefinitionStmt::StructDefinitionStmt(
        Lexer::Token &identifier,
        std::unordered_map<std::string_view, StructField> &&fields)
        : Node(identifier.range.start), StructDeclarationStmt(identifier),
          fields(std::move(fields))
    {
    }

    void StructDefinitionStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "StructDefinitionStmt {\n"
           << inner_indentation << "accessibility: " << accessibility << "\n"
           << inner_indentation << "name: " << identifier << "\n"
           << inner_indentation << "fields: {";

        for (auto &field_ : fields)
        {
            std::string inner_indentation = utils::tab(tab + 1, 4);
            os << "\n" << inner_indentation << field_.second;
        }

        os << "\n" << inner_indentation << "}\n" << indentation << "}";
    }

} // namespace AST
