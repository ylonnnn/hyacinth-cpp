#include "semantic/analyzer/impl/Program.hpp"
#include "semantic/analyzer/Analyzer.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::Program>::analyze(Analyzer &analyzer,
                                                       AST::Program &node)
    {
        AnalysisResult result = {
            std::nullopt, Core::ResultStatus::Success, nullptr, {}};

        for (auto &declaration : node.declarations())
        {
            AnalysisResult decl_result =
                AnalyzerImpl<std::remove_cv_t<std::remove_reference_t<
                    decltype(*declaration)>>>::analyze(analyzer, *declaration);

            result.diagnostics.insert(
                result.diagnostics.end(),
                std::make_move_iterator(decl_result.diagnostics.begin()),
                std::make_move_iterator(decl_result.diagnostics.end()));

            if (decl_result.status == Core::ResultStatus::Fail)
                result.status = decl_result.status;
        }

        return result;
    }

} // namespace Semantic
