#include "parser/Parser.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "lexer/Lexer.hpp"
#include "parser/grammar/Grammar.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "program/Program.hpp"

namespace Parser
{
    Parser::Parser(Program::ProgramFile &program, Lexer::Lexer &lexer)
        : program_(program), lexer_(lexer)
    {
        initialize_grammar();
    }

    void Parser::initialize_grammar() { Hyacinth::initialize(grammar_); }

    Program::ProgramFile &Parser::program() { return program_; }

    Lexer::Lexer &Parser::lexer() { return lexer_; }

    Grammar &Parser::grammar() { return grammar_; }

    ParserState Parser::state() const { return state_; }

    void Parser::update_state(ParserState state) { state_ = state; }

    bool Parser::is(ParserState state) const { return state_ == state; }

    void Parser::panic() { state_ = ParserState::Panic; }

    void Parser::synchronize() { state_ = ParserState::Synchronized; }

    bool Parser::expect(Lexer::TokenType type, bool consume)
    {
        Lexer::Token *token =
            lexer_.eof() ? nullptr : (consume ? lexer_.next() : lexer_.peek());
        auto expected = token == nullptr ? false : token->type == type;

        if (expected)
            synchronize();

        return expected;
    }

    std::unique_ptr<Diagnostic::ErrorDiagnostic>
    Parser::expect_or_error(Lexer::TokenType type, bool consume)
    {
        Lexer::Token *token = lexer_.eof() ? nullptr : lexer_.peek();
        return expect(type, consume)
                   ? nullptr
                   : Diagnostic::create_syntax_error(
                         token ? token : &lexer_.current(), type);
    }

    bool Parser::parse()
    {
        auto [status, _, diagnostics] = grammar_.parse(*this);

        for (auto &diagnostic : diagnostics)
            diagnostic->report();

        return status == ParseResultStatus::Success;
    }

} // namespace Parser
