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

    Diagnostic::Diagnostic &
    ParseResult::error(std::unique_ptr<Diagnostic::Diagnostic> diagnostic)
    {
        status = Core::ResultStatus::Fail;
        // if (parser.is(ParserState::Panic)) return;

        parser.panic();
        diagnostics.push_back(std::move(diagnostic));

        return *diagnostics.back().get();
    }

    Diagnostic::Diagnostic &ParseResult::error(Core::PositionRange &&range,
                                               Diagnostic::ErrorType type,
                                               std::string &&message)
    {
        status = Core::ResultStatus::Fail;
        // if (parser.is(ParserState::Panic))
        //     return;

        parser.panic();
        diagnostics.push_back(std::make_unique<Diagnostic::Diagnostic>(
            Diagnostic::DiagnosticSeverity::Error, static_cast<uint32_t>(type),
            std::move(range), std::string(message)));

        return *diagnostics.back().get();
    }

    Diagnostic::Diagnostic &
    ParseResult::force_error(std::unique_ptr<Diagnostic::Diagnostic> diagnostic)
    {
        parser.panic();

        status = Core::ResultStatus::Fail;
        diagnostics.push_back(std::move(diagnostic));

        return *diagnostics.back().get();
    }

    Diagnostic::Diagnostic &
    ParseResult::force_error(Core::PositionRange &&range,
                             Diagnostic::ErrorType type,
                             const std::string &message)
    {
        parser.panic();

        status = Core::ResultStatus::Fail;
        diagnostics.push_back(std::make_unique<Diagnostic::Diagnostic>(
            Diagnostic::DiagnosticSeverity::Error, static_cast<uint32_t>(type),
            std::move(range), std::string(message)));

        return *diagnostics.back().get();
    }

} // namespace Parser
