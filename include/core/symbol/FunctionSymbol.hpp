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
        std::unique_ptr<Type> type;
    };

    struct FunctionSymbol : public Symbol
    {
        AST::FunctionDeclarationStmt *node = nullptr;

        std::unique_ptr<Type> return_type;
        std::vector<FunctionParameter> parameters;

        std::string signature;

        FunctionSymbol(std::string name, Core::Position declared_at,
                       std::unique_ptr<Type> return_type,
                       std::vector<FunctionParameter> &&parameters,
                       AST::FunctionDeclarationStmt *node = nullptr);

        void construct_signature();
    };

} // namespace Core
