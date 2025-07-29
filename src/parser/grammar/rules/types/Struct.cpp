#include "parser/grammar/rules/types/Struct.hpp"
#include "ast/expr/LiteralExpr.hpp"
#include "ast/stmt/types/struct/StructDefStmt.hpp"
#include "parser/grammar/GrammarRule.hpp"
#include "parser/grammar/common/Common.hpp"
#include "parser/grammar/rules/Hyacinth.hpp"

namespace Parser
{
    using namespace Lexer::TokenTypes;

    StructDefinition::StructDefinition() : GrammarRule(Hyacinth::STRUCT) {}

    static Lexer::Token *parse_name(Parser &parser, ParseResult &result)
    {
        auto &lexer = parser.lexer();

        if (!parser.expect(Primary::Identifier, false))
        {
            auto node = AST::LiteralExpr(*lexer.peek());
            result.force_error(
                &node, Diagnostic::ErrorTypes::Syntax::MissingIdentifier,
                std::string("Missing struct") + Diagnostic::ERR_GEN +
                    "IDENTIFIER" + Utils::Styles::Reset +
                    " after function keyword.",
                "Missing identifier here");

            return nullptr;
        }
        else
            return lexer.next();
    }

    static std::unordered_map<std::string_view, AST::StructField>
    parse_fields(Parser &parser, ParseResult &result)
    {
        auto &lexer = parser.lexer();

        std::unordered_map<std::string_view, AST::StructField> fields;
        fields.reserve(8);

        Lexer::TokenType closing = Delimeter::BraceClose;

        auto &identifier = Common::IdentifierInitialization;
        auto expect_field = true;

        while (!parser.expect(closing, false))
        {
            if (expect_field)
            {
                ParseResult i_res = identifier.parse(parser);
                result.adapt(i_res.status, std::move(i_res.diagnostics));

                if (i_res.data == nullptr)
                    break;

                if (auto ptr =
                        dynamic_cast<IdentifierInitNode *>(i_res.data.get()))
                {
                    Lexer::Token &name = ptr->name();

                    auto [_, inserted] = fields.try_emplace(
                        name.value,
                        AST::StructField{name, ptr->mut_state(),
                                         std::move(ptr->type_ptr())});

                    // Duplication
                    if (!inserted)
                    {
                        result.error(
                            ptr,
                            Diagnostic::ErrorTypes::Syntax::FieldDuplcation,
                            std::string("Field \"") + Diagnostic::ERR_GEN +
                                std::string(name.value) + Utils::Styles::Reset +
                                "\" has already been defined.",
                            "Field is being duplicated");

                        break;
                    }
                }

                expect_field = false;
            }

            if (parser.expect(Delimeter::Semicolon, false))
            {
                lexer.next();
                expect_field = true;

                // Closing Brace Lookahead
                if (parser.expect(closing, false))
                    break;

                continue;
            }

            break;
        }

        // if (auto diagnostic = parser.expect_or_error(closing, false))
        // {
        //     parser.panic();

        //     result.status = Core::ResultStatus::Fail;
        //     result.diagnostics.push_back(std::move(diagnostic));
        // }

        // else
        //     lexer.next();

        return fields;
    }

    static size_t parse_closing(Parser &parser, ParseResult &result,
                                Lexer::TokenType expected)
    {
        auto &lexer = parser.lexer();

        if (auto diagnostic = parser.expect_or_error(expected, false))
        {
            parser.panic();
            result.adapt(Core::ResultStatus::Fail,
                         Diagnostic::DiagnosticList{
                             std::make_move_iterator(&diagnostic),
                             std::make_move_iterator(&diagnostic + 1)});

            return SIZE_MAX;
        }

        Lexer::Token *close = lexer.next();

        return close->position.col + close->value.size();
    }

    ParseResult StructDefinition::parse(Parser &parser)
    {
        // struct NAME;
        // struct NAME { (FIELD_NAME : FIELD_TYPE)* }

        auto &lexer = parser.lexer();
        ParseResult result = {parser, Core::ResultStatus::Success, nullptr, {}};

        // struct (Keyword)
        if (auto diagnostic = parser.expect_or_error(token_type_, false))
            result.diagnostics.push_back(std::move(diagnostic));
        else
            lexer.next();

        // NAME (Identifier)
        Lexer::Token *name = parse_name(parser, result);
        if (name == nullptr)
            return result;

        // { (BraceOpen)
        if (parser.expect(Delimeter::BraceOpen, false))
        {
            lexer.next();

            // (FIELD_NAME : FIELD_TYPE)*
            std::unordered_map<std::string_view, AST::StructField> fields =
                parse_fields(parser, result);

            // } (BraceClose)
            size_t st_end_pos =
                parse_closing(parser, result, Delimeter::BraceClose);
            if (st_end_pos)
                st_end_pos = name->position.col + name->value.size();

            auto node = std::make_unique<AST::StructDefinitionStmt>(
                *name, std::move(fields));

            node->set_end_pos(st_end_pos);
            result.data = std::move(node);

            return result;
        }

        // ; (Terminator)
        else
        {
            ParseResult t_res = Common::Terminator.parse(parser);
            result.adapt(t_res.status, std::move(t_res.diagnostics));

            if (t_res.status == Core::ResultStatus::Fail)
                return result;

            result.data = std::make_unique<AST::StructDeclarationStmt>(*name);
        }

        return result;
    }

} // namespace Parser
