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

                    if (token.value[0] == '-')
                        result = int64_t(std::stoll(value));
                    else
                        result = uint64_t(std::stoull(value));

                    break;
                }

                case Primary::Float:
                {
                    value.erase(std::remove(value.begin(), value.end(), '_'),
                                value.end());

                    char &ext = value.back();
                    if (ext == 'f')
                        value.resize(value.size() - 1);

                    result = std::stod(value);

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
                    result = value.substr(1, value.size() - 2);

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
