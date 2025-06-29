#pragma once

#include "diagnostic/ErrorDiagnostic.hpp"
#include "lexer/Lexer.hpp"
#include "parser/grammar/Grammar.hpp"
#include "program/Program.hpp"

namespace Parser
{
    enum class ParserState
    {
        Normal,
        Panic,
        Synchronized,
    };

    class Parser
    {
      private:
        Program::ProgramFile &program_;
        Lexer::Lexer &lexer_;

        Grammar grammar_;

        ParserState state_ = ParserState::Normal;

      public:
        Parser(Program::ProgramFile &program, Lexer::Lexer &lexer);
        virtual ~Parser() = default;

      protected:
        void initialize_grammar();

      public:
        Program::ProgramFile &program();
        Lexer::Lexer &lexer();
        Grammar &grammar();

        ParserState state() const;
        void update_state(ParserState state);

        bool expect(Lexer::TokenType type, bool consume = true);
        std::unique_ptr<Diagnostic::ErrorDiagnostic>
        expect_or_error(Lexer::TokenType type, bool consume = true);

        bool parse();
    };

} // namespace Parser
