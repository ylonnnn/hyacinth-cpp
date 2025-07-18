#include <iostream>
#include <string>

#include "core/value/Value.hpp"
#include "lexer/Token.hpp"

namespace Utils
{
    Core::Value parse_val(Lexer::Token &token)
    {
        using Lexer::TokenTypes::Primary;

        Core::Value result = Core::null{};
        std::string value = std::string(token.value);

        if (auto ptr = std::get_if<Primary>(&token.type))
        {
            switch (*ptr)
            {
                case Primary::Int:
                {
                    value.erase(std::remove(value.begin(), value.end(), '_'),
                                value.end());

                    // std::cout << value << "\n";
                    // std::cout << std::stoll(value) << "\n";
                    // std::cout << std::stoull(value) << "\n";

                    if (token.value[0] == '-')
                        result = int64_t(std::stoll(value));
                    else
                        result = uint64_t(std::stoull(value));

                    break;
                }

                case Primary::Float:
                {
                    std::cout << token.value << "\n";
                    break;
                }

                case Primary::Boolean:
                {
                    std::cout << token.value << "\n";
                    break;
                }

                case Primary::Character:
                {
                    std::cout << token.value << "\n";
                    break;
                }

                case Primary::String:
                {
                    std::cout << token.value << "\n";
                    break;
                }

                default:
                    std::cout << "invalid: " << token.value << "\n";
                    break;
            }
        }

        return result;
    }

} // namespace Utils
