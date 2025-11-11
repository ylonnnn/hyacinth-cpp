#include <algorithm>
#include <iostream>
#include <string>

#include "core/type/Type.hpp"
#include "core/value/ValuePool.hpp"
#include "lexer/Token.hpp"
#include "utils/char.hpp"
#include "utils/numeric.hpp"
#include "utils/value.hpp"

namespace utils
{
    double parse_numeric(const std::string &val)
    {
        uint32_t base = 10;
        std::string input = val;

        char pref_ctrl = val[1];

        if (val[0] == '0' && !std::isdigit(pref_ctrl))
        {
            base = pref_ctrl == 'b'   ? 2
                   : pref_ctrl == 'o' ? 8
                   : pref_ctrl == 'x' ? 16
                                      : UINT32_MAX;
        }

        if (base == UINT32_MAX)
            return 0;

        // If the base is not the base of decimal, remove the prefix from the
        // input string
        if (base != 10)
            input = input.substr(2);

        return utils::to_numeric(input, base);
    }

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

                return std::make_unique<Core::Value::T>(
                    Core::integer(static_cast<uint64_t>(parse_numeric(value)),
                                  token.value[0] == '-'));
            }

            case TokenType::Float:
            {
                value.erase(std::remove(value.begin(), value.end(), '_'),
                            value.end());

                return std::make_unique<Core::Value::T>(
                    static_cast<double>(parse_numeric(value)));
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

        Core::ReadValue *value =
            Core::create_value(std::move(val), nullptr, &range);

        value->type = Core::BaseType::infer(
            *range.start().program.get().environment, *value);

        return value;
    }

} // namespace utils
