#include "ast/stmt/types/struct/StructDefStmt.hpp"
#include "ast/Node.hpp"
#include "ast/common/Identifier.hpp"
#include "utils/style.hpp"

namespace AST
{
    StructField::StructField(Lexer::Token &name,
                             IdentifierMutabilityState mut_state,
                             std::unique_ptr<Type> type)
        : Node(name.position), Identifier(name, mut_state, std::move(type))
    {
    }

    void StructField::print(std::ostream &os,
                            [[maybe_unused]] uint8_t tab) const
    {
        os << name_.value << ": " << type_->to_string();
    }

    StructDefinitionStmt::StructDefinitionStmt(
        Lexer::Token &name,
        std::unordered_map<std::string_view, StructField> &&fields)
        : Node(name.position), StructDeclarationStmt(name),
          fields_(std::move(fields))
    {
    }

    bool StructDefinitionStmt::is_definition() const { return true; }

    void StructDefinitionStmt::set_end_pos(size_t end_pos)
    {
        end_pos_ = end_pos;
    }

    std::unordered_map<std::string_view, StructField> &
    StructDefinitionStmt::fields()
    {
        return fields_;
    }

    void StructDefinitionStmt::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);

        os << "StructDefinitionStmt {\n"
           << inner_indentation << "name: " << name_.value << "\n"
           << inner_indentation << "fields: {";

        for (auto &field_ : fields_)
        {
            std::string_view name = field_.first;
            StructField &field = const_cast<StructField &>(field_.second);

            std::string inner_indentation = Utils::tab(tab + 1, 4);
            os << "\n"
               << inner_indentation << name << ": " << field.type().to_string();
        }

        os << "\n" << inner_indentation << "}\n" << indentation << "}";
    }

} // namespace AST
