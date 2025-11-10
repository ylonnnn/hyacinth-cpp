#include "core/symbol/VariableSymbol.hpp"
#include "ast/stmt/variable/VariableDefStmt.hpp"
#include "core/symbol/IdentifierSymbol.hpp"
#include "core/symbol/Symbol.hpp"
#include "core/value/Value.hpp"
#include "utils/style.hpp"

namespace Core
{
    VariableSymbol::VariableSymbol(std::string_view name,
                                   SymbolAccessibility acessibility,
                                   AST::VariableDeclarationStmt *decl_node,
                                   bool is_mutable, InstantiatedType *type,
                                   Value *value)
        : IdentifierSymbol(name, decl_node, is_mutable, type, value),
          decl_node(decl_node)
    {
        this->accessibility = accessibility;
    }

    void VariableSymbol::define(AST::Node *node)
    {
        if (auto ptr = dynamic_cast<AST::VariableDefinitionStmt *>(node))
            def_node = ptr;
    }

    void VariableSymbol::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "(var) " << name << " {"
           << (accessibility == SymbolAccessibility::Public ? "Public"
                                                            : "Private")
           << ", " << (is_mutable ? "Mutable" : "Immutable") << ", "
           << (type == nullptr ? "nullptr" : type->to_string()) << ", "
           << (value != nullptr ? get_rvalue(*value).to_string() : "null")
           << " }";
    }

} // namespace Core
