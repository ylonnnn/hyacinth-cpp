#pragma once

#include "ast/stmt/types/struct/StructDeclStmt.hpp"
#include "ast/stmt/types/struct/StructDefStmt.hpp"
#include "core/symbol/Symbol.hpp"
#include "core/symbol/type/TypeSymbol.hpp"
#include "core/type/Type.hpp"
#include "core/type/compound/Struct.hpp"

namespace Core
{
    struct StructField
    {
        std::string_view name;
        Type *type;
    };

    struct StructSymbol : public TypeSymbol
    {
        AST::StructDeclarationStmt *node = nullptr;
        AST::StructDefinitionStmt *definition = nullptr;

        StructType *type = nullptr;

        StructSymbol(std::string_view name, SymbolAccessibility accessibility,
                     Core::Position &declared_at,
                     AST::StructDeclarationStmt *node = nullptr);

        void define(Core::Position *position) override;
        void define(AST::StructDefinitionStmt *definition);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace Core
