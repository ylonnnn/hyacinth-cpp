#include <algorithm>
#include <iostream>
#include <string>

#include "core/value/ValuePool.hpp"
#include "lexer/Token.hpp"
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

                return std::make_unique<Core::Value::T>(std::stod(value));
            }

            case TokenType::Bool:
                return std::make_unique<Core::Value::T>(
                    value == "true" ? true : false);

            case TokenType::Char:
                return std::make_unique<Core::Value::T>(value[1]);

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
        return Core::VALUE_POOL.add(std::make_unique<Core::Value>(
            parse_val_t(token),
            /* TODO: Infer type of values */ nullptr));
    }

} // namespace utils
