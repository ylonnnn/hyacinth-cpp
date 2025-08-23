#include "lexer/TokenType.hpp"

namespace Lexer
{
    bool operator==(TokenType &a, TokenType &b)
    {
        if (a.index() != b.index())
            return false;

        return std::visit(
            [](auto &&l, auto &&r) -> bool
            {
                using T = std::decay_t<decltype(l)>;
                using U = std::decay_t<decltype(r)>;

                if constexpr (std::is_same_v<T, U>)
                    return l == r;
                else
                    return false;
            },
            a, b);
    }

} // namespace Lexer
