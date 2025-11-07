#include <algorithm>
#include <iostream>
#include <string>

#include "core/type/Type.hpp"
#include "core/value/ValuePool.hpp"
#include "lexer/Token.hpp"
#include "utils/char.hpp"
#include "utils/value.hpp"

namespace utils
{
    std::unique_ptr<Core::Value::T> parse_val_t(Lexer::Token &token)
    {
        using Lexer::TokenType;

        std::string value(token.value);

        switch (token.type)
        {
            case TokenType::Int:
            {
                value.erase(std::remove(value.begin(), value.end(), '_'),
                            value.end());

                auto val = std::stoull(value);

                return std::make_unique<Core::Value::T>(
                    Core::integer(val, token.value[0] == '-'));
            }

            case TokenType::Float:
            {
                value.erase(std::remove(value.begin(), value.end(), '_'),
                            value.end());

                return std::make_unique<Core::Value::T>(
                    double(std::stod(value)));
            }

            case TokenType::Bool:
                return std::make_unique<Core::Value::T>(
                    value.at(0) == 't' ? true : false);

            case TokenType::Char:
                return std::make_unique<Core::Value::T>(Core::character(
                    utils::utf8_to_utf32(value.substr(1, value.size() - 2))));

            case TokenType::String:
                return std::make_unique<Core::Value::T>(
                    value.substr(1, value.size() - 2));

            default:
            {
                std::cout << "invalid: " << token.value << "\n";
                return std::make_unique<Core::Value::T>(Core::null{});
            }
        }
    }

    Core::Value *parse_val(Lexer::Token &token)
    {
        Core::PositionRange &range = token.range;
        std::unique_ptr<Core::Value::T> val = parse_val_t(token);

        Core::Value *value = Core::create_value(
            std::move(val), nullptr, Core::ValueType::RValue, &range);

        value->type = Core::BaseType::infer(
            *range.start().program.get().environment, *value);

        return value;
    }

} // namespace utils
