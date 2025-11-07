
#include "ast/common/Identifier.hpp"
#include "ast/expr/ArrayLiteralExpr.hpp"
#include "ast/expr/BinaryExpr.hpp"
#include "ast/expr/FunctionCallExpr.hpp"
#include "ast/expr/LiteralExpr.hpp"
// #include "ast/expr/FunctionCallExpr.hpp"
// #include "ast/expr/compound/ArrayExpr.hpp"
// #include "ast/expr/compound/InstanceExpr.hpp"
#include "ast/expr/Path.hpp"
#include "ast/expr/StructLiteralExpr.hpp"
#include "ast/expr/UnaryExpr.hpp"
#include "ast/type/ModifiedType.hpp"
#include "ast/type/PrefixedType.hpp"
#include "ast/type/SimpleType.hpp"
#include "ast/type/SuffixedType.hpp"
#include "parser/Parser.hpp"
#include "parser/grammar/common/pratt/Pratt.hpp"

namespace Parser
{
    using GroupHandlerFn = std::function<ParseResult(Parser &)>;

    NudHandler<AST::Node> make_group_handler(Lexer::TokenType &&cl_pair,
                                             GroupHandlerFn &&handler);

    NudHandler<AST::Node> make_grouped_expr_handler(Lexer::TokenType &&cl_pair);

    // Default

    std::unique_ptr<AST::LiteralExpr> parse_literal(Parser &parser,
                                                    ParseResult &result);

    std::unique_ptr<AST::Identifier> parse_identifier(Parser &parser,
                                                      ParseResult &result);

    std::unique_ptr<AST::Path> parse_path(Parser &parser,
                                          std::unique_ptr<AST::Node> &left,
                                          float right_bp, ParseResult &result);

    std::unique_ptr<AST::BinaryExpr>
    parse_binary(Parser &parser, std::unique_ptr<AST::Node> &left,
                 float right_bp, ParseResult &result);

    std::unique_ptr<AST::UnaryExpr> parse_unary(Parser &parser,
                                                ParseResult &result);

    std::unique_ptr<AST::UnaryExpr>
    parse_unary(Parser &parser, std::unique_ptr<AST::Node> &left,
                float right_bp, ParseResult &result);

    std::unique_ptr<AST::FunctionCallExpr>
    parse_func_call(Parser &parser, std::unique_ptr<AST::Node> &left,
                    float right_bp, ParseResult &result);

    std::unique_ptr<AST::ArrayLiteralExpr>
    parse_array_literal_expr(Parser &parser, ParseResult &result);

    std::unique_ptr<AST::StructLiteralExpr> parse_struct_literal_expr();

    // Type

    std::unique_ptr<AST::SimpleType> parse_type_identifier(Parser &parser,
                                                           ParseResult &result);

    std::unique_ptr<AST::PrefixedType> parse_type_array(Parser &parser,
                                                        ParseResult &result);

    NudHandler<AST::PrefixedType>
    make_type_pref_nud_handler(TypeBindingPower bp, AST::PrefixedTypeKind kind);

    LedHandler<AST::SuffixedType>
    make_type_suf_led_handler(TypeBindingPower bp, AST::SuffixedTypeKind kind);

    NudHandler<AST::ModifiedType>
    make_type_mod_nud_handler(TypeBindingPower bp, AST::ModifierType type);

    // NudHandler<AST::ModifiedType>
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
