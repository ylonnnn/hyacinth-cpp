#include <iostream>

#include "diagnostic/WarningDiagnostic.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/Grammar.hpp"
// #include "parser/grammar/default/Hyacinth.hpp"
#include "parser/grammar/default/Hyacinth.hpp"
#include "program/Program.hpp"
#include "utils/style.hpp"

namespace Parser
{
    // ParserErrorHandler::ParserErrorHandler() { errors_.reserve(16); }

    // ParserState ParserErrorHandler::state() { return state_; }

    // std::vector<Diagnostic::ErrorDiagnostic> &ParserErrorHandler::errors()
    // {
    //     return errors_;
    // }

    // void ParserErrorHandler::update_state(ParserState state) { state_ =
    // state; }

    // void
    // ParserErrorHandler::attempt_panic(Diagnostic::ErrorDiagnostic
    // &&diagnostic)
    // {
    //     attempt_diagnostic_ = std::make_unique<Diagnostic::ErrorDiagnostic>(
    //         std::move(diagnostic));
    // }

    // void ParserErrorHandler::abort_panic_attempt()
    // {
    //     attempt_diagnostic_.reset();
    // }

    // void ParserErrorHandler::proceed_panic()
    // {
    //     state_ = ParserState::Panic;

    //     errors_.push_back(std::move(*attempt_diagnostic_));
    //     attempt_diagnostic_.reset();
    // }

    // void ParserErrorHandler::panic(Diagnostic::ErrorDiagnostic &&diagnostic)
    // {
    //     attempt_panic(std::move(diagnostic));
    //     proceed_panic();
    // }

    // void ParserErrorHandler::undo_panic()
    // {
    //     if (errors_.empty())
    //         return;

    //     state_ = errors_.size() == 1 ? ParserState::Normal
    //                                  : ParserState::Synchronized;

    //     errors_.pop_back();
    // }

    // void ParserErrorHandler::synchronize()
    // {
    //     state_ = ParserState::Synchronized;
    // }

    Parser::Parser(Program::ProgramFile &program, Lexer::Lexer &lexer)
        : program_(program), lexer_(lexer)
    {
        std::unique_ptr<Grammar> default_grammar = initialize_grammar();
        if (default_grammar != nullptr)
            grammar_ = std::move(default_grammar);
    }

    std::unique_ptr<Grammar> Parser::initialize_grammar()
    {
        std::unique_ptr<Grammar> grammar = std::make_unique<Grammar>();

        Hyacinth::initialize(grammar.get());

        return grammar;
    }

    Program::ProgramFile &Parser::program() { return program_; }
    Lexer::Lexer &Parser::lexer() { return lexer_; }

    const Grammar *Parser::grammar() const { return grammar_.get(); }

    // ParserErrorHandler &Parser::error_handler() { return error_handler_; }

    ParserState Parser::state() { return state_; }

    void Parser::update_state(ParserState state) { state_ = state; }

    bool Parser::parse()
    {
        auto [status, _, diagnostics] = grammar_->parse(*this);
        // size_t errors = 0, warns = 0;

        for (auto &diagnostic : diagnostics)
        {
            // if (dynamic_cast<Diagnostic::ErrorDiagnostic *>(diagnostic.get()))
            //     errors++;

            // if (dynamic_cast<Diagnostic::WarningDiagnostic *>(diagnostic.get()))
            //     warns++;

            diagnostic->report();
        }

        return status == ParseResultStatus::Success;

        // std::cout << "\n\n"
        //           << (errors == 0 ? warns == 0 ? Utils::Colors::Green
        //                                        : Utils::Colors::Yellow
        //                           : Utils::Colors::Red)
        //           << " [" << (errors == 0 ? warns == 0 ? "/" : "~" : "X")
        //           << "] Program Parsed.\n\n\n";
    }

} // namespace Parser
