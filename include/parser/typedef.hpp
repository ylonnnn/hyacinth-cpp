#pragma once

#include <memory>
#include <vector>

#include "ast/Node.hpp"
#include "diagnostic/Diagnostic.hpp"

namespace Parser
{
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
    };

} // namespace Parser
