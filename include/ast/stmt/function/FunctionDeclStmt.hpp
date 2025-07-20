#pragma once

#include "ast/NodeCollection.hpp"
#include "ast/common/Identifier.hpp"
#include "ast/stmt/BlockStmt.hpp"
#include "ast/stmt/DeclarationStmt.hpp"
#include "ast/type/Type.hpp"

namespace AST
{
    class FunctionParameterIdentifier : public Identifier
    {
      protected:
        // TODO: Default Value

      public:
        FunctionParameterIdentifier(Lexer::Token &name,
                                    IdentifierMutabilityState mut_state,
                                    std::unique_ptr<Type> type);

        void print(std::ostream &os, uint8_t tab) const override;
    };

    class FunctionDeclarationStmt : public DeclarationStmt
    {
      protected:
        std::unique_ptr<Type> return_type_;
        std::unique_ptr<NodeCollection<FunctionParameterIdentifier>>
            parameters_;

        // Empty
        std::unique_ptr<BlockStmt> body_; 

      public:
        FunctionDeclarationStmt(
            Lexer::Token &name, std::unique_ptr<Type> return_type,
            std::unique_ptr<NodeCollection<FunctionParameterIdentifier>>
                parameters);

        virtual bool is_definition() const override;

        Type &return_type();
        std::vector<std::unique_ptr<FunctionParameterIdentifier>> &parameters();

        std::unique_ptr<Type> &return_type_ptr();

        virtual void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
