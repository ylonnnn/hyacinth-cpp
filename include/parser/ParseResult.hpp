#pragma once

#include "core/result/Result.hpp"

namespace Parser
{
    class Parser;

    struct ParseResult : public Core::Result<std::unique_ptr<AST::Node>>
    {
        Parser &parser;

        ParseResult(Parser &parser, Core::ResultStatus status,
                    std::unique_ptr<AST::Node> data,
                    Diagnostic::DiagnosticList diagnostics);

        void
        error(std::unique_ptr<Diagnostic::ErrorDiagnostic> diagnostic) override;
        void error(AST::Node *node, Diagnostic::ErrorType error_type,
                   const std::string &message,
                   const std::string &submessage) override;

        void
        force_error(std::unique_ptr<Diagnostic::ErrorDiagnostic> diagnostic);
        void force_error(AST::Node *node, Diagnostic::ErrorType error_type,
                         const std::string &message,
                         const std::string &submessage);
    };

} // namespace Parser
