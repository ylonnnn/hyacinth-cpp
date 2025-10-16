#include "parser/grammar/rules/types/Struct.hpp"
#include "ast/stmt/types/struct/StructDefStmt.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"
#include "utils/dev.hpp"
#include "utils/pointer.hpp"

namespace Parser
{
    using Lexer::TokenType;

    StructDefinition::StructDefinition()
        : GrammarRule(Hyacinth::STRUCT, GC_GLOBAL | GC_LOCAL)
    {
    }

    void StructDefinition::parse_field(
        Parser &parser, ParseResult &result,
        std::unordered_map<std::string_view, AST::StructField> &fields)
    {
        // mut? FIELD_IDENTIFIER ":" FIELD_TYPE ( "=" DEFAULT_VALUE ) ";"
        auto &lexer = parser.lexer;

        // mut?
        AST::IdentifierMutabilityState mut_state =
            AST::IdentifierMutabilityState::Immutable;
        if (parser.expect(TokenType::Mut, false))
        {
            lexer.consume();
            mut_state = AST::IdentifierMutabilityState::Mutable;
        }

        // FIELD_IDENTIFIER
        Lexer::Token *identifier = nullptr;
        if (auto diagnostic =
                parser.expect_or_error(TokenType::Identifier, false))
            result.error(std::move(diagnostic));
        else
            identifier = lexer.next();

        // :
        if (auto diagnostic = parser.expect_or_error(TokenType::Colon, false))
            result.error(std::move(diagnostic));
        else
            lexer.consume();

        // FIELD_TYPE
        ParseResult t_res = Common::Pratt.parse_base(parser, 0, true);
        result.adapt(t_res.status, std::move(t_res.diagnostics));

        auto type = utils::dynamic_ptr_cast<AST::Type>(t_res.data);
        if (type == nullptr)
            return;

        // =
        std::unique_ptr<AST::Expr> value;
        if (parser.expect(TokenType::Equal, false))
        {
            lexer.consume();

            // DEFAULT_VALUE
            ParseResult v_res = Common::Pratt.parse_base(parser, 0);
            result.adapt(v_res.status, std::move(v_res.diagnostics));

            value = utils::dynamic_ptr_cast<AST::Expr>(v_res.data);
        }

        // ;
        Common::Terminator.parse(parser, result);

        auto [_, inserted] = fields.try_emplace(
            identifier->value,
            AST::StructField(*identifier, mut_state, std::move(type),
                             std::move(value)));

        if (inserted)
            return;

        utils::todo("throw error: duplication of field {}");
    }

    std::unordered_map<std::string_view, AST::StructField>
    StructDefinition::parse_fields(Parser &parser, ParseResult &result)
    {
        std::unordered_map<std::string_view, AST::StructField> fields;
        fields.reserve(8);

        while (!parser.expect(TokenType::RightBrace, false))
        {
            parse_field(parser, result, fields);
            if (result.status == Core::ResultStatus::Fail)
                break;
        }

        return fields;
    }

    ParseResult StructDefinition::parse(Parser &parser)
    {
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};
        parse(parser, result);

        return result;
    }

    void StructDefinition::parse(Parser &parser, ParseResult &result)
    {
        // struct IDENTIFIER ( "{" ( FIELD_IDENTIFIER ":" FIELD_TYPE )* "}" ) |
        // ";"

        auto &lexer = parser.lexer;

        // struct
        lexer.consume();

        // IDENTIFIER
        Lexer::Token *identifier = nullptr;
        if (auto diagnostic =
                parser.expect_or_error(TokenType::Identifier, false))
            result.error(std::move(diagnostic));
        else
            identifier = lexer.next();

        // {
        if (parser.expect(TokenType::LeftBrace, false))
        {
            lexer.consume();

            // ( FIELD_IDENTIFIER ":" FIELD_TYPE )*
            std::unordered_map<std::string_view, AST::StructField> fields =
                parse_fields(parser, result);

            // }
            Core::Position *e_pos = nullptr;
            if (auto diagnostic =
                    parser.expect_or_error(TokenType::RightBrace, false))
                result.error(std::move(diagnostic));
            else
                e_pos = &lexer.next()->range.end;

            auto node = std::make_unique<AST::StructDefinitionStmt>(
                *identifier, std::move(fields));

            node->end_position = e_pos;
            result.data = std::move(node);
        }

        // ;
        else
        {
            ParseResult t_res = Common::Terminator.parse(parser);
            result.adapt(t_res.status, std::move(t_res.diagnostics));

            if (t_res.status == Core::ResultStatus::Fail)
                return;

            result.data =
                std::make_unique<AST::StructDeclarationStmt>(*identifier);
        }
    }

    void StructDefinition::recover(Parser &parser)
    {
        parser.synchronize({TokenType::RightBrace, TokenType::Semicolon});
    }

} // namespace Parser
