#pragma once

#include "ast/Program.hpp"
#include "lexer/Lexer.hpp"
#include "parser/grammar/Grammar.hpp"
// #include "parser/ParseResult.hpp"
#include "core/program/Program.hpp"
#include "parser/ParseResult.hpp"

namespace Parser
{
    enum class ParserState
    {
        Panic,
        Synchronized,
    };

    enum class ParserTokenConsumptionType
    {
        Absolute,
        UponSuccess,
        Preserve,
    };

    struct ProgramParseResult : ParseResult
    {
        std::unique_ptr<AST::Program> data;

        ProgramParseResult(Parser &parser, Core::ResultStatus status,
                           std::unique_ptr<AST::Program> data,
                           Diagnostic::DiagnosticList &&diagnostics);
    };

    struct Parser
    {
        Core::Program &program;
        Lexer::Lexer &lexer;

        Grammar grammar;

        ParserState state = ParserState::Synchronized;

        Parser(Core::Program &program, Lexer::Lexer &lexer);
        virtual ~Parser() = default;

        void initialize_grammar();

        bool is(ParserState state) const;

        void panic();
        void synchronize();
        void synchronize(const std::vector<Lexer::TokenType> &types);

        bool expect(Lexer::TokenType type, bool consume = true);
        std::unique_ptr<Diagnostic::Diagnostic>
        expect_or_error(Lexer::TokenType type, bool consume = true);

        Lexer::Token *
        expect_or_error(Lexer::TokenType type, ParseResult &result,
                        ParserTokenConsumptionType consumption =
                            ParserTokenConsumptionType::UponSuccess);

        ProgramParseResult parse();
    };

} // namespace Parser
