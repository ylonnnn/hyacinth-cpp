#pragma once

#include "ast/stmt/types/struct/StructDeclStmt.hpp"
#include "core/symbol/Symbol.hpp"
#include "core/type/Type.hpp"

namespace Core
{
    struct StructField
    {
        std::string_view name;
        std::unique_ptr<Type> type;
    };

    struct StructSymbol : public Symbol
    {
        AST::StructDeclarationStmt *node = nullptr;

        std::unordered_map<std::string_view, std::unique_ptr<Type>> fields;

        StructSymbol(std::string_view name, Core::Position declared_at,
                     std::vector<StructField> &&fields,
                     AST::StructDeclarationStmt *node = nullptr);
    };

} // namespace Core
