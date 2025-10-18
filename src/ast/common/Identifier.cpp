#include <iostream>

#include "ast/common/Identifier.hpp"
#include "utils/style.hpp"

namespace AST
{
    Identifier::Identifier(Lexer::Token &identifier,
                           std::vector<IdentifierArgument> &&arguments)
        : Node(identifier.range.start), identifier(identifier),
          arguments(std::move(arguments))
    {
    }

    void Identifier::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);

        os << "Identifier {\n"
           << inner_indentation << "identifier: " << identifier << "\n"
           << inner_indentation << "arguments: {";

        for (auto &argument : arguments)
        {
            std::string inner_indentation = utils::tab(tab + 1, 4);
            os << "\n" << inner_indentation;

            if (auto ptr = std::get_if<std::unique_ptr<Type>>(&argument))
                (*ptr)->print(os, tab + 2);

            else if (auto ptr = std::get_if<std::unique_ptr<Expr>>(&argument))
                (*ptr)->print(os, tab + 2);
        }

        os << "\n" << inner_indentation << "}\n" << indentation << " }";
    }

} // namespace AST
