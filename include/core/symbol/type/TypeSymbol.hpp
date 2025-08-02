#pragma once

#include "ast/stmt/DeclarationStmt.hpp"
#include "core/symbol/Symbol.hpp"

namespace Core
{

    struct TypeSymbol : public Symbol
    {
        AST::DeclarationStmt *node = nullptr;
        AST::DeclarationStmt *definition = nullptr;

        TypeSymbol(std::string_view name, SymbolAccessibility accessibility,
                   Core::Position &declared_at,
                   AST::DeclarationStmt *node = nullptr);

        virtual ~TypeSymbol() override = default;
    };

} // namespace Core
