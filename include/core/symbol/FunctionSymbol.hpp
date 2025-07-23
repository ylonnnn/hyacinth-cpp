#pragma once

#include "ast/stmt/function/FunctionDeclStmt.hpp"
#include "core/symbol/Symbol.hpp"
#include "core/type/Type.hpp"

namespace Core
{
    struct FunctionParameter
    {
        std::string_view name;
        bool is_mutable;
        AST::Type *type;
        Type *resolved_type;

        bool operator==(const FunctionParameter &other) const;
    };

    struct FunctionSymbol : public Symbol
    {
        AST::FunctionDeclarationStmt *node = nullptr;

        Type *return_type;
        std::vector<FunctionParameter> parameters;

        FunctionSymbol(std::string name, Core::Position declared_at,
                       Type *return_type,
                       std::vector<FunctionParameter> parameters,
                       AST::FunctionDeclarationStmt *node = nullptr);
    };

} // namespace Core
