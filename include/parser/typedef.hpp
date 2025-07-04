#pragma once

#include <memory>
#include <vector>

#include "ast/Node.hpp"
#include "diagnostic/Diagnostic.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "diagnostic/NoteDiagnostic.hpp"
#include "diagnostic/WarningDiagnostic.hpp"

namespace Parser
{
    class Parser;

    enum class ParseResultStatus
    {
        Success,
        Failed,
    };

    using DiagnosticList = std::vector<std::unique_ptr<Diagnostic::Diagnostic>>;

    struct ParseResult
    {
        ParseResultStatus status = ParseResultStatus::Failed;
        std::unique_ptr<AST::Node> node;
        DiagnosticList diagnostics;

        void error(Parser &parser,
                   std::unique_ptr<Diagnostic::ErrorDiagnostic> diagnostic);
        void error(Parser &parser, std::unique_ptr<AST::Node> node,
                   Diagnostic::ErrorType error_type, std::string message,
                   std::string emphasis_message);

        void
        force_error(Parser &parser,
                    std::unique_ptr<Diagnostic::ErrorDiagnostic> diagnostic);
        void force_error(Parser &parser, std::unique_ptr<AST::Node> node,
                         Diagnostic::ErrorType error_type, std::string message,
                         std::string emphasis_message);

        void warn(std::unique_ptr<Diagnostic::WarningDiagnostic> diagnostic);
        void warn(std::unique_ptr<AST::Node> node,
                  Diagnostic::WarningType warn_type, std::string message,
                  std::string emphasis_message);

        void note(std::unique_ptr<Diagnostic::NoteDiagnostic> diagnostic);
        void note(std::unique_ptr<AST::Node> node,
                  Diagnostic::NoteType note_type, std::string message,
                  std::string emphasis_message);
    };

} // namespace Parser
