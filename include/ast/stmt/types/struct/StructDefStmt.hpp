#pragma once

#include <unordered_map>

#include "ast/common/Identifier.hpp"
#include "ast/stmt/types/struct/StructDeclStmt.hpp"

namespace AST
{
    class StructField : public Identifier
    {
      public:
        StructField(Lexer::Token &name, IdentifierMutabilityState mut_state,
                    std::unique_ptr<Type> type);

        void print(std::ostream &os, uint8_t tab) const override;
    };

    class StructDefinitionStmt : public StructDeclarationStmt
    {
      private:
        std::unordered_map<std::string_view, StructField> fields_;

      public:
        StructDefinitionStmt(
            Lexer::Token &name,
            std::unordered_map<std::string_view, StructField> &&fields);

        bool is_definition() const override;

        void set_end_pos(size_t end_pos);

        std::unordered_map<std::string_view, StructField> &fields();

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
