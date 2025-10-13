#pragma once

#include "ast/Node.hpp"
#include "core/result/Result.hpp"

namespace Parser
{
    struct Parser;

    struct ParseResult : Core::Result<std::unique_ptr<AST::Node>>
    {
        Parser &parser;

        ParseResult(Parser &parser, Core::ResultStatus status,
                    std::unique_ptr<AST::Node> data,
                    Diagnostic::DiagnosticList diagnostics);

        using Core::Result<std::unique_ptr<AST::Node>>::adapt;
        void adapt(Core::ResultStatus status,
                   Diagnostic::DiagnosticList &&diagnostics) override;

        Diagnostic::Diagnostic *
        error(std::unique_ptr<Diagnostic::Diagnostic> &&diagnostic) override;
        Diagnostic::Diagnostic *error(Core::PositionRange &&range,
                                      Diagnostic::ErrorType type,
                                      std::string &&message) override;

        Diagnostic::Diagnostic *
        force_error(std::unique_ptr<Diagnostic::Diagnostic> &&diagnostic);
        Diagnostic::Diagnostic *force_error(Core::PositionRange &&range,
                                            Diagnostic::ErrorType type,
                                            const std::string &message);
    };

} // namespace Parser
