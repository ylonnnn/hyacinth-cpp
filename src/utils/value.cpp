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
                    return int64_t(std::stoll(value));
                else
                    return uint64_t(std::stoull(value));
            }

            case Primary::Float:
            {
                value.erase(std::remove(value.begin(), value.end(), '_'),
                            value.end());

                char &ext = value.back();
                if (ext == 'f')
                    value.resize(value.size() - 1);

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
