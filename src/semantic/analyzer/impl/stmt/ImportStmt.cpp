#include "core/program/ProgramRegistry.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    AnalysisResult AnalyzerImpl<AST::ImportStmt>::analyze(Analyzer &analyzer,
                                                          AST::ImportStmt &node)
    {
        std::string target(node.target_strv());

        Core::ProgramFile &current = analyzer.program(),
                          &program = current.registry()->file(
                              current.path().parent_path() /
                              target.substr(1, target.size() - 2));

        AnalysisResult result = program.lex_parse_analyze();

        current.depend(program.environment());

        return result;
    }

} // namespace Semantic
