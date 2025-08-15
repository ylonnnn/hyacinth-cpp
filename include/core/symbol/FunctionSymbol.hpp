#pragma once

#include "ast/stmt/function/FunctionDeclStmt.hpp"
#include "ast/stmt/function/FunctionDefStmt.hpp"
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
        Type *type = nullptr;
        AST::FunctionParameter *node = nullptr;

        FunctionParameterSymbol(std::string_view name,
                                Core::Position &declared_at, bool is_mutable,
                                Type *type, std::shared_ptr<Value> value,
                                AST::FunctionParameter *node = nullptr);

        void print(std::ostream &os, uint8_t tab) const override;
    };

    struct FunctionSymbol : public Symbol
    {
        AST::FunctionDeclarationStmt *node = nullptr;
        AST::FunctionDefinitionStmt *definition = nullptr;

        Type *return_type;
        std::vector<FunctionParameter> parameters;

        std::string signature;

        FunctionSymbol(std::string_view name, SymbolAccessibility accessibility,
                       Core::Position &declared_at, Type *return_type,
                       std::vector<FunctionParameter> &&parameters,
                       AST::FunctionDeclarationStmt *node = nullptr);

        void construct_signature();

        void define(Core::Position *position) override;
        void define(AST::FunctionDefinitionStmt *definition);

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace Core
