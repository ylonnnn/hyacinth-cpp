#include <unordered_map>

#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/Grammar.hpp"
#include "parser/grammar/GrammarRuleSet.hpp"
#include "utils/control.hpp"

namespace Parser
{
    GrammarRuleSet *Grammar::get_rule_set(Lexer::TokenType token_type)
    {
        auto it = rule_sets_.find(token_type);

        if (it == rule_sets_.end())
            return nullptr;

        return it->second.get();
    }

    ParseResult Grammar::parse(Parser &parser)
    {
        Lexer::Lexer &lexer = parser.lexer();
        if (lexer.eof())
            Utils::terminate("Cannot start parsing at EOF!", EXIT_FAILURE);

        Lexer::Token *token = lexer.peek();
        if (token == nullptr)
            return {nullptr, {}};

        auto it = rule_sets_.find(token->type);

        if (it == rule_sets_.end())
            Utils::terminate(
                (std::string("Unable to find a grammar rule set for ") +
                 Lexer::type_to_string(token->type) + "!")
                    .c_str(),
                EXIT_FAILURE);

        return it->second->parse(parser);
    }

} // namespace Parser
