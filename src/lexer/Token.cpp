#include <ostream>
#include <re2/re2.h>
#include <vector>

#include "lexer/Token.hpp"

namespace Lexer
{
    std::ostream &operator<<(std::ostream &os, const Token &token)
    {
        return os << "<" << type_to_string(token.type) << " "
                  << token.position.row << ":" << token.position.col << "> "
                  << token.value;
    }

    TokenHandler::TokenHandler(TokenType type, const char *pattern)
        : type(type), pattern(pattern)
    {
    }

    const re2::RE2 &TokenHandler::get_regex()
    {
        if (!regex)
            regex = std::make_unique<re2::RE2>(pattern);

        return *regex;
    }

    std::vector<TokenHandler> TOKEN_HANDLERS;

    void init_handlers()
    {
        TOKEN_HANDLERS.reserve(128);

        TOKEN_HANDLERS.emplace_back(TokenTypes::Miscellaneous::CommentSingle,
                                    "//[^\\n]*");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Miscellaneous::CommentMultiple,
                                    "\\/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*\\/");

        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Unary::Increment,
                                    "\\+\\+");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Unary::Decrement,
                                    "\\-\\-");

        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Dot::Double,
                                    "\\.\\.");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Dot::Single, "\\.");

        TOKEN_HANDLERS.emplace_back(TokenTypes::Reserved::Import, "import");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Reserved::Public, "pub");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Reserved::Private, "priv");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Reserved::Protected, "prot");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Reserved::Function, "fn");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Reserved::Return, "return");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Reserved::If, "if");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Reserved::Else, "else");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Reserved::For, "for");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Reserved::While, "while");

        TOKEN_HANDLERS.emplace_back(
            TokenTypes::Primary::Float,
            "[+-]?((\\d(?:_?\\d)*\\.\\d(?:_?\\d)*|\\d(?:_?\\d)*\\.)([f]?)|"
            "\\d(?:_?\\d)*f)");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Primary::Int,
                                    "-?\\d(?:_?\\d)*");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Primary::Boolean,
                                    "\\b(true|false)\\b");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Primary::Character,
                                    "'(\\\\.|[^'\\\\])'");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Primary::InvalidCharacter,
                                    "'([^']{0}|[^']{2,}|\\\\[^']{0,})'");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Primary::String, "\"[^\"]*\"");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Primary::Identifier,
                                    "[a-zA-Z_][a-zA-Z0-9_]*");

        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Relational::Equal,
                                    "==");

        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Assignment::Direct,
                                    "=");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Assignment::Addition,
                                    "\\+=");
        TOKEN_HANDLERS.emplace_back(
            TokenTypes::Operator::Assignment::Subtraction, "-=");
        TOKEN_HANDLERS.emplace_back(
            TokenTypes::Operator::Assignment::Multiplication, "\\*=");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Assignment::Division,
                                    "/=");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Assignment::Modulo,
                                    "%=");

        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Arrow::Left, "->");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Arrow::Right, "<-");

        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::ArithmeticUnary::Plus,
                                    "\\+");
        TOKEN_HANDLERS.emplace_back(
            TokenTypes::Operator::ArithmeticUnary::Minus, "-");

        TOKEN_HANDLERS.emplace_back(
            TokenTypes::Operator::Arithmetic::Multiplication, "\\*");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Arithmetic::Division,
                                    "/");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Arithmetic::Modulo,
                                    "%");

        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Relational::Equal,
                                    "!=");
        TOKEN_HANDLERS.emplace_back(
            TokenTypes::Operator::Relational::GreatherThanEqual, ">=");
        TOKEN_HANDLERS.emplace_back(
            TokenTypes::Operator::Relational::GreaterThan, ">");
        TOKEN_HANDLERS.emplace_back(
            TokenTypes::Operator::Relational::LessThanEqual, "<=");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Relational::LessThan,
                                    "<");

        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Logical::Not, "!");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Logical::And,
                                    "\\&\\&");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Operator::Logical::Or,
                                    "\\|\\|");

        TOKEN_HANDLERS.emplace_back(TokenTypes::Delimeter::ParenthesisOpen,
                                    "\\(");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Delimeter::ParenthesisClose,
                                    "\\)");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Delimeter::BraceOpen, "\\{");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Delimeter::BraceClose, "\\}");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Delimeter::BracketOpen, "\\[");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Delimeter::BracketClose, "\\]");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Delimeter::Comma, ",");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Delimeter::Semicolon, ";");
        TOKEN_HANDLERS.emplace_back(TokenTypes::Delimeter::Colon, ":");

        TOKEN_HANDLERS.emplace_back(TokenTypes::Miscellaneous::Whitespace,
                                    "[ \\f\\t]");

        TOKEN_HANDLERS.emplace_back(TokenTypes::Miscellaneous::LineBreak,
                                    "\\n");
    }

    bool initialized_handlers = (init_handlers(), true);

} // namespace Lexer
