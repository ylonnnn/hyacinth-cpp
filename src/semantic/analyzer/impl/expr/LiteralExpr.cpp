#include <iostream>

#include "ast/expr/LiteralExpr.hpp"
#include "semantic/analyzer/Analyzer.hpp"
#include "semantic/analyzer/impl/Expr.hpp"
#include "utils/value.hpp"

namespace Semantic
{
    AnalysisResult
    AnalyzerImpl<AST::LiteralExpr>::analyze(Analyzer &analyzer,
                                            AST::LiteralExpr &node)
    {
        Core::Environment &current = analyzer.current_env();
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        Lexer::Token &value = node.value();

        if (auto ptr = std::get_if<Lexer::TokenTypes::Primary>(&value.type))
        {
            using Lexer::TokenTypes::Primary;

            switch (*ptr)
            {
                case Primary::Int:
                {
                    result.data = current.resolve_type("int");

                    if (result.data == nullptr)
                        result.status = Core::ResultStatus::Fail;

                    result.value = Utils::parse_val(value);

                    break;
                }

                default:
                {
                    break;
                }
            }
        }

        // if (result.type)
        //     if (!result.type->assignable(Utils::parse_val(value)))
        //     {
        //         result.status = AnalysisResultStatus::Invalid;
        //         // TODO: Add diagnostics
        //     }

        return result;
    }

} // namespace Semantic
