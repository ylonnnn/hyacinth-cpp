#include "parser/Parser.hpp"
#include "core/program/Program.hpp"
#include "diagnostic/helpers.hpp"
#include "lexer/Lexer.hpp"
#include "parser/ParseResult.hpp"
#include "parser/grammar/Grammar.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "utils/control.hpp"

namespace Parser
{
    ProgramParseResult::ProgramParseResult(
        Parser &parser, Core::ResultStatus status,
        std::unique_ptr<AST::Program> data,
        Diagnostic::DiagnosticList &&diagnostics)
        : ParseResult(parser, status, std::move(data), std::move(diagnostics))
    {
        std::unique_ptr<AST::Node> node = std::move(ParseResult::data);
        if (node == nullptr)
            return;

        auto ptr = dynamic_cast<AST::Program *>(node.release());
        if (ptr == nullptr)
            utils::terminate(
                "[ProgramParseResult] data must contain an AST::Program");

        this->data = std::unique_ptr<AST::Program>(ptr);
    }

    Parser::Parser(Core::Program &program, Lexer::Lexer &lexer)
        : program(program), lexer(lexer)
    {
        initialize_grammar();
    }

    void Parser::initialize_grammar() { Hyacinth::initialize(grammar); }

    bool Parser::is(ParserState state) const { return this->state == state; }

    void Parser::panic() { state = ParserState::Panic; }

    void Parser::synchronize() { state = ParserState::Synchronized; }

    void Parser::synchronize(const std::vector<Lexer::TokenType> &types)
    {
        std::unordered_map<Lexer::TokenType, int32_t> types_;
        types_.reserve(types.size());

        for (const auto &key : types)
            types_.emplace(key, 0);

        while (!lexer.eof())
        {
            Lexer::Token *token = lexer.next();
            if (token == nullptr)
                break;

            auto it = types_.find(token->type);
            if (it == types_.end())
                continue;

            state = ParserState::Synchronized;
            break;
        }
    }

    bool Parser::expect(Lexer::TokenType type, bool consume)
    {
        Lexer::Token *token =
            lexer.eof() ? nullptr : (consume ? lexer.next() : lexer.peek());
        return token == nullptr ? false : token->type == type;
    }

    std::unique_ptr<Diagnostic::Diagnostic>
    Parser::expect_or_error(Lexer::TokenType type, bool consume)
    {
        Lexer::Token *token =
            lexer.eof() ? nullptr : (consume ? lexer.next() : lexer.peek());
        auto expected = token == nullptr ? false : token->type == type;

        return expected ? nullptr
                        : Diagnostic::create_syntax_error(*token, type);
    }

    ProgramParseResult Parser::parse() { return grammar.parse(*this); }

} // namespace Parser
