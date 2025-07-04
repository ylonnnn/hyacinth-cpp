#include "parser/Parser.hpp"
#include "parser/typedef.hpp"

namespace Parser
{
    void
    ParseResult::error(Parser &parser,
                       std::unique_ptr<Diagnostic::ErrorDiagnostic> diagnostic)
    {
        status = ParseResultStatus::Failed;
        if (parser.is(ParserState::Panic))
            return;

        parser.panic();
        diagnostics.push_back(std::move(diagnostic));
    }

    void ParseResult::error(Parser &parser, std::unique_ptr<AST::Node> node,
                            Diagnostic::ErrorType error_type,
                            std::string message, std::string emphasis_message)
    {
        error(parser, std::make_unique<Diagnostic::ErrorDiagnostic>(
                          std::move(node), error_type, std::move(message),
                          std::move(emphasis_message)));
    }

    void
    ParseResult::force_error(Parser &parser,
                       std::unique_ptr<Diagnostic::ErrorDiagnostic> diagnostic)
    {
        parser.panic();

        status = ParseResultStatus::Failed;
        diagnostics.push_back(std::move(diagnostic));
    }

    void ParseResult::force_error(Parser &parser, std::unique_ptr<AST::Node> node,
                            Diagnostic::ErrorType error_type,
                            std::string message, std::string emphasis_message)
    {
        force_error(parser, std::make_unique<Diagnostic::ErrorDiagnostic>(
                          std::move(node), error_type, std::move(message),
                          std::move(emphasis_message)));
    }

    void
    ParseResult::warn(std::unique_ptr<Diagnostic::WarningDiagnostic> diagnostic)
    {
        diagnostics.push_back(std::move(diagnostic));
    }

    void ParseResult::warn(std::unique_ptr<AST::Node> node,
                           Diagnostic::WarningType warn_type,
                           std::string message, std::string emphasis_message)
    {
        warn(std::make_unique<Diagnostic::WarningDiagnostic>(
            std::move(node), warn_type, std::move(message),
            std::move(emphasis_message)));
    }

    void
    ParseResult::note(std::unique_ptr<Diagnostic::NoteDiagnostic> diagnostic)
    {
        diagnostics.push_back(std::move(diagnostic));
    }

    void ParseResult::note(std::unique_ptr<AST::Node> node,
                           Diagnostic::NoteType note_type, std::string message,
                           std::string emphasis_message)
    {
        note(std::make_unique<Diagnostic::NoteDiagnostic>(
            std::move(node), note_type, std::move(message),
            std::move(emphasis_message)));
    }

} // namespace Parser
