#include "parser/Parser.hpp"
#include "core/program/Program.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "lexer/Lexer.hpp"
#include "parser/grammar/Grammar.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"

namespace Parser
{
    ProgramParseResult::ProgramParseResult(
        Parser &parser, Core::ResultStatus status,
        std::unique_ptr<AST::Program> data,
        Diagnostic::DiagnosticList diagnostics)
        : ParseResult(parser, status, std::move(data), std::move(diagnostics))
    {
        std::unique_ptr<AST::Node> node = std::move(ParseResult::data);
        if (dynamic_cast<AST::Program *>(node.get()))
            this->data = std::unique_ptr<AST::Program>(
                static_cast<AST::Program *>(node.release()));
    }

    Parser::Parser(Core::ProgramFile &program, Lexer::Lexer &lexer)
        : program_(program), lexer_(lexer)
    {
        initialize_grammar();
    }

    void Parser::initialize_grammar() { Hyacinth::initialize(grammar_); }

    Core::ProgramFile &Parser::program() { return program_; }

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

    ProgramParseResult Parser::parse() { return grammar_.parse(*this); }

} // namespace Parser
