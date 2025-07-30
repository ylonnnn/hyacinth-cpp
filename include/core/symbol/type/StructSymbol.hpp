#pragma once

#include "ast/stmt/types/struct/StructDeclStmt.hpp"
#include "ast/stmt/types/struct/StructDefStmt.hpp"
#include "core/symbol/Symbol.hpp"
#include "core/type/Type.hpp"
#include "core/type/compound/Struct.hpp"

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
        AST::StructDefinitionStmt *definition = nullptr;

        StructType *type = nullptr;

        StructSymbol(std::string_view name, Core::Position declared_at,
                     AST::StructDeclarationStmt *node = nullptr);

        void define(Core::Position *position) override;
        void define(AST::StructDefinitionStmt *definition);
    };

} // namespace Core
