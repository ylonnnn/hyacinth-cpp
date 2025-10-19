#include <typeindex>

#include "analyzer/Analyzer.hpp"
#include "analyzer/impl/Stmt.hpp"
#include "ast/stmt/variable/VariableDefStmt.hpp"

#define STMT_DISPATCH(T) DISPATCH(T, AST::Stmt)

namespace Semantic
{
    static const std::unordered_map<std::type_index, AnalyzerImplFn<AST::Stmt>>
        stmt_dispatch_table = {

            // BlockStmt
            STMT_DISPATCH(AST::BlockStmt),

            // ExprStmt
            STMT_DISPATCH(AST::ExprStmt),

            // Variable
            STMT_DISPATCH(AST::VariableDeclarationStmt),
            {typeid(AST::VariableDefinitionStmt),
             [](Analyzer &analyzer, AST::Stmt &node)
             {
                 return AnalyzerImpl<AST::VariableDeclarationStmt>::analyze(
                     analyzer,
                     static_cast<AST::VariableDefinitionStmt &>(node));
             }},

            // Function
            STMT_DISPATCH(AST::FunctionReturnStmt),
    };

    // TODO: Implementation of:
    // if (auto ptr = dynamic_cast<AST::DeclarationStmt *>(&node))
    //     return analyzer.analyze(*ptr);

    AnalysisResult AnalyzerImpl<AST::Stmt>::analyze(Analyzer &analyzer,
                                                    AST::Stmt &node)
    {
        auto it = stmt_dispatch_table.find(typeid(node));
        return it == stmt_dispatch_table.end()
                   ? AnalysisResult{nullptr,
                                    Core::ResultStatus::Fail,
                                    nullptr,
                                    {}}
                   : it->second(analyzer, node);
    }

} // namespace Semantic
