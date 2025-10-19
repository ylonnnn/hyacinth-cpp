#include "ast/stmt/ImportStmt.hpp"
#include "ast/Node.hpp"
#include "utils/style.hpp"

namespace AST
{
    ImportStmt::ImportStmt(Lexer::Token &target,
                           std::vector<Lexer::Token *> &&symbols)
        : Node(target.range.start),
          DeclarationStmt(target, DeclarationAccessibility::Private, true),
          target(target), symbols(std::move(symbols))
    {
    }

    void ImportStmt::print(std::ostream &os, [[maybe_unused]] uint32_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "ImportStmt {\n"
           << inner_indentation << "target: " << target << "\n"
           << inner_indentation << "symbols: {";

        for (const auto &symbol : symbols)
        {
            std::string inner_indentation = utils::tab(tab + 1, 4);
            os << "\n" << inner_indentation << *symbol;
        }

        os << "\n" << inner_indentation << "}\n" << indentation << "}";
    }

} // namespace AST
