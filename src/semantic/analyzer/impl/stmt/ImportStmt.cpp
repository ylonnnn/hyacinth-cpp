#include "core/program/ProgramRegistry.hpp"
#include "semantic/analyzer/impl/Stmt.hpp"

namespace Semantic
{
    namespace fs = std::filesystem;

    AnalysisResult AnalyzerImpl<AST::ImportStmt>::analyze(Analyzer &analyzer,
                                                          AST::ImportStmt &node)
    {
        std::string target(node.target_strv());
        fs::path path = target.substr(1, target.size() - 2);

        Core::ProgramFile &current = analyzer.program(),
                          &program = current.registry()->file(
                              (path.is_relative()
                                   ? current.path().parent_path() / path
                                   : path)
                                  .string());

        AnalysisResult result = program.lex_parse_analyze();

        current.depend(program.environment());

        return result;
    }

} // namespace Semantic
