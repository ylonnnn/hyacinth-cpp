#include <algorithm>
#include <iostream>
#include <string>

#include "core/value/Value.hpp"
#include "lexer/Token.hpp"
#include "utils/value.hpp"

namespace Utils
{
    Core::Value parse_val(Lexer::Token &token)
    {
        using Lexer::TokenTypes::Primary;

        auto ptr = std::get_if<Primary>(&token.type);
        if (ptr == nullptr)
            return Core::null{};

        std::string value = std::string(token.value);

        switch (*ptr)
        {
            case Primary::Int:
            {
                value.erase(std::remove(value.begin(), value.end(), '_'),
                            value.end());

                if (token.value[0] == '-')
                    return Core::h_int(std::stoll(value), true);
                else
                {
                    auto val = std::stoll(value);
                    return Core::h_int(val, !(val >= 0 && val > INT64_MAX));
                }
            }

            case Primary::Float:
            {
                value.erase(std::remove(value.begin(), value.end(), '_'),
                            value.end());

                return std::stod(value);
            }

            case Primary::Boolean:
                return value == "true" ? true : false;

            case Primary::Character:
                return value[1];

            case Primary::String:
                return value.substr(1, value.size() - 2);

            default:
            {
                std::cout << "invalid: " << token.value << "\n";
                return Core::null{};
            }
        }
    }

} // namespace Utils
