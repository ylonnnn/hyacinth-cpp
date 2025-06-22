#pragma once

#include <memory>
#include <vector>

#include "ast/Node.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"

namespace Parser
{
    struct ParseResult
    {
        std::unique_ptr<AST::Node> node;
        std::vector<Diagnostic::ErrorDiagnostic> errors;
    };
    
    struct UnbuiltParseResult
    {
        std::vector<std::unique_ptr<AST::Node>> nodes;
        std::vector<Diagnostic::ErrorDiagnostic> errors;
    };

} // namespace Parser
