#pragma once

#include "ast/stmt/function/FunctionDeclStmt.hpp"
#include "core/symbol/IdentifierSymbol.hpp"
#include "core/symbol/Symbol.hpp"
#include "core/type/Type.hpp"

namespace Core
{
    struct FunctionParameter
    {
        std::string_view name;
        bool is_mutable;
        Type *type;
    };

    struct FunctionParameterSymbol : public IdentifierSymbol
    {
        AST::FunctionParameter *node = nullptr;

        FunctionParameterSymbol(std::string_view name,
                                Core::Position declared_at, bool is_mutable,
                                std::unique_ptr<Type> type,
                                std::optional<Value> value,
                                AST::FunctionParameter *node = nullptr);
    };

    struct FunctionSymbol : public Symbol
    {
        AST::FunctionDeclarationStmt *node = nullptr;

        std::unique_ptr<Type> return_type;
        std::vector<FunctionParameter> parameters;

        std::string signature;

        FunctionSymbol(std::string_view name, Core::Position declared_at,
                       std::unique_ptr<Type> return_type,
                       std::vector<FunctionParameter> &&parameters,
                       AST::FunctionDeclarationStmt *node = nullptr);

        void construct_signature();
    };

} // namespace Core
