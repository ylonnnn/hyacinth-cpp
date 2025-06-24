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

    struct ParseResult
    {
        ParseResultStatus status = ParseResultStatus::Failed;
        std::unique_ptr<AST::Node> node;
        std::vector<std::unique_ptr<Diagnostic::Diagnostic>> diagnostics;
    };

    struct UnbuiltParseResult
    {
        ParseResultStatus status = ParseResultStatus::Failed;
        std::vector<std::unique_ptr<AST::Node>> nodes;
        std::vector<std::unique_ptr<Diagnostic::Diagnostic>> diagnostics;
    };

} // namespace Parser
