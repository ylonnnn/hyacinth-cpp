
// #include "ast/expr/BinaryExpr.hpp"
// #include "ast/expr/FunctionCallExpr.hpp"
// #include "ast/expr/IdentifierExpr.hpp"
#include "ast/common/Identifier.hpp"
#include "ast/expr/BinaryExpr.hpp"
#include "ast/expr/LiteralExpr.hpp"
// #include "ast/expr/UnaryExpr.hpp"
// #include "ast/expr/compound/ArrayExpr.hpp"
// #include "ast/expr/compound/InstanceExpr.hpp"
#include "ast/expr/Path.hpp"
#include "ast/expr/UnaryExpr.hpp"
#include "ast/type/PrefixedType.hpp"
#include "ast/type/SimpleType.hpp"
#include "ast/type/SuffixedType.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/common/pratt/Pratt.hpp"

namespace Parser
{
    // Default

    std::unique_ptr<AST::LiteralExpr> parse_literal(Parser &parser,
                                                    PrattParseResult &result);

    std::unique_ptr<AST::Identifier> parse_identifier(Parser &parser,
                                                      PrattParseResult &result);

    std::unique_ptr<AST::Path> parse_path(Parser &parser,
                                          std::unique_ptr<AST::Node> &left,
                                          float right_bp,
                                          PrattParseResult &result);

    std::unique_ptr<AST::BinaryExpr>
    parse_binary(Parser &parser, std::unique_ptr<AST::Node> &left,
                 float right_bp, PrattParseResult &result);

    std::unique_ptr<AST::UnaryExpr> parse_unary(Parser &parser,
                                                PrattParseResult &result);

    std::unique_ptr<AST::UnaryExpr>
    parse_unary(Parser &parser, std::unique_ptr<AST::Node> &left,
                float right_bp, PrattParseResult &result);

    // Type

    std::unique_ptr<AST::SimpleType>
    parse_type_identifier(Parser &parser, PrattParseResult &result);

    std::unique_ptr<AST::PrefixedType>
    parse_type_array(Parser &parser, PrattParseResult &result);

    NudHandler<AST::PrefixedType>
    make_type_nud_handler(TypeBindingPower bp, AST::PrefixedTypeKind kind);

    LedHandler<AST::SuffixedType>
    make_type_led_handler(TypeBindingPower bp, AST::SuffixedTypeKind kind);
    // std::unique_ptr<AST::BinaryExpr>
    // parse_memaccess(Parser &parser, std::unique_ptr<AST::Expr> &left,
    //                 float right_bp, ExprParseResult &result);

    // std::unique_ptr<AST::BinaryExpr>
    // parse_elaccess(Parser &parser, std::unique_ptr<AST::Expr> &left,
    //                float right_bp, ExprParseResult &result);

    // std::unique_ptr<AST::FunctionCallExpr>
    // parse_fncall(Parser &parser, std::unique_ptr<AST::Expr> &left,
    //              float right_bp, ExprParseResult &result);

    // // Type-Related Parser
    // std::unique_ptr<AST::ArrayExpr> parse_array(Parser &parser,
    //                                             ExprParseResult &result);

    // std::unique_ptr<AST::InstanceExpr>
    // parse_instance(Parser &parser, std::unique_ptr<AST::Expr> &left,
    //                float right_bp, ExprParseResult &result);

} // namespace Parser
