#pragma once

#include "ast/Program.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "lexer/Lexer.hpp"
#include "parser/grammar/Grammar.hpp"
// #include "parser/ParseResult.hpp"
#include "core/program/Program.hpp"
#include "parser/ParseResult.hpp"

namespace Parser
{
    enum class ParserState
    {
        Normal,
        Panic,
        Synchronized,
    };

    struct ProgramParseResult : public ParseResult
    {
        std::unique_ptr<AST::Program> data;

        ProgramParseResult(Parser &parser, Core::ResultStatus status,
                           std::unique_ptr<AST::Program> data,
                           Diagnostic::DiagnosticList diagnostics);
    };

    class Parser
    {
      private:
        Core::ProgramFile &program_;
        Lexer::Lexer &lexer_;

        Grammar grammar_;

        ParserState state_ = ParserState::Normal;

      public:
        Parser(Core::ProgramFile &program, Lexer::Lexer &lexer);
        virtual ~Parser() = default;

      protected:
        void initialize_grammar();

      public:
        Core::ProgramFile &program();
        Lexer::Lexer &lexer();
        Grammar &grammar();

        ParserState state() const;
        bool is(ParserState state) const;
        void update_state(ParserState state);

        void panic();
        void synchronize();

        bool expect(Lexer::TokenType type, bool consume = true);
        std::unique_ptr<Diagnostic::ErrorDiagnostic>
        expect_or_error(Lexer::TokenType type, bool consume = true);

        ProgramParseResult parse();
    };

} // namespace Parser
