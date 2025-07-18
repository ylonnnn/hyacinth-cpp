#include "parser/ParseResult.hpp"
#include "core/result/Result.hpp"
#include "parser/Parser.hpp"

namespace Parser
{
    ParseResult::ParseResult(Parser &parser, Core::ResultStatus status,
                             std::unique_ptr<AST::Node> data,
                             Diagnostic::DiagnosticList diagnostics)
        : Core::Result<std::unique_ptr<AST::Node>>(status, std::move(data),
                                                   std::move(diagnostics)),
          parser(parser)
    {
    }

    void
    ParseResult::error(std::unique_ptr<Diagnostic::ErrorDiagnostic> diagnostic)
    {
        status = Core::ResultStatus::Fail;
        if (parser.is(ParserState::Panic))
            return;

        parser.panic();
        diagnostics.push_back(std::move(diagnostic));
    }

    void ParseResult::error(AST::Node *node, Diagnostic::ErrorType error_type,
                            const std::string & message,
                            const std::string & submessage)
    {
        status = Core::ResultStatus::Fail;
        if (parser.is(ParserState::Panic))
            return;

        parser.panic();
        diagnostics.push_back(std::make_unique<Diagnostic::ErrorDiagnostic>(
            node, error_type, message, submessage));
    }

    void ParseResult::force_error(
        std::unique_ptr<Diagnostic::ErrorDiagnostic> diagnostic)
    {
        parser.panic();

        status = Core::ResultStatus::Fail;
        diagnostics.push_back(std::move(diagnostic));
    }

    void ParseResult::force_error(AST::Node *node,
                                  Diagnostic::ErrorType error_type,
                                  const std::string & message,
                                  const std::string & submessage)
    {
        parser.panic();

        status = Core::ResultStatus::Fail;
        diagnostics.push_back(std::make_unique<Diagnostic::ErrorDiagnostic>(
            std::move(node), error_type, message, submessage));
    }

} // namespace Parser
