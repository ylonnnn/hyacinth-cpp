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

    // class ParserErrorHandler
    // {
    //   private:
    //     ParserState state_ = ParserState::Normal;
    //     std::vector<Diagnostic::ErrorDiagnostic> errors_;

    //     std::unique_ptr<Diagnostic::ErrorDiagnostic> attempt_diagnostic_;

    //   public:
    //     ParserErrorHandler();

    //   public:
    //     ParserState state();

    //     void update_state(ParserState state);

    //     void attempt_panic(Diagnostic::ErrorDiagnostic &&diagnostic);
    //     void abort_panic_attempt();
    //     void proceed_panic();

    //     void panic(Diagnostic::ErrorDiagnostic &&diagnostic);

    //     void undo_panic();

    //     void synchronize();
    // };

    class Parser
    {
      private:
        Program::ProgramFile &program_;
        Lexer::Lexer &lexer_;

        std::unique_ptr<Grammar> grammar_ = nullptr;

        ParserState state_ = ParserState::Normal;

      public:
        Parser(Program::ProgramFile &program, Lexer::Lexer &lexer);
        virtual ~Parser() = default;

      protected:
        std::unique_ptr<Grammar> initialize_grammar();

      public:
        Program::ProgramFile &program();
        Lexer::Lexer &lexer();
        const Grammar *grammar() const;
         
        // ParserErrorHandler &error_handler();

        ParserState state();
        void update_state(ParserState state);

        void parse();
    };

} // namespace Parser
