#pragma once

#include "ast/stmt/function/FunctionDeclStmt.hpp"
#include "core/result/Result.hpp"
#include "parser/ParseResult.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    struct FunctionParameterListParseResult
        : public Core::Result<std::unique_ptr<
              AST::NodeCollection<AST::FunctionParameterIdentifier>>>
    {
        FunctionParameterListParseResult(
            Core::ResultStatus status,
            std::unique_ptr<
                AST::NodeCollection<AST::FunctionParameterIdentifier>>
                data,
            Diagnostic::DiagnosticList diagnostics);
    };

    class FunctionDefinition : public GrammarRule
    {
      public:
        FunctionDefinition();

        FunctionParameterListParseResult parse_param_list(Parser &parser);
        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
