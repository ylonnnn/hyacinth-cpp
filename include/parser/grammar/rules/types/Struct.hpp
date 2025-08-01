#pragma once

#include "ast/stmt/DeclarationStmt.hpp"
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

        ParseResult parse(Parser &parser,
                          AST::DeclarationAccessibility accessibility);
        ParseResult parse(Parser &parser) override;
    };

} // namespace Parser
