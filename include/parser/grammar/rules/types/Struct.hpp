#pragma once

#include "parser/Parser.hpp"
#include "parser/grammar/GrammarRule.hpp"

namespace Parser
{
    // struct FunctionParameterListParseResult
    //     : public Core::Result<std::unique_ptr<
    //           AST::NodeCollection<AST::FunctionParameterIdentifier>>>
    // {
    //     FunctionParameterListParseResult(
    //         Core::ResultStatus status,
    //         std::unique_ptr<
    //             AST::NodeCollection<AST::FunctionParameterIdentifier>>
    //             data,
    //         Diagnostic::DiagnosticList diagnostics);
    // };

    class StructDefinition : public GrammarRule
    {
      public:
        StructDefinition();

        // FunctionParameterListParseResult parse_param_list(Parser &parser);
        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
