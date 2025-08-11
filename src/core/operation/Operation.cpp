#include "core/operation/Operation.hpp"
#include "core/type/Type.hpp"

namespace Core::Operation
{
    size_t
    BinarySignatureHash::operator()(const BinarySignature &signature) const
    {
        return std::hash<Lexer::TokenType>{}(signature.op) ^
               signature.left->hash() ^ (signature.right->hash() << 1);
    }

    bool BinarySignatureEq::operator()(const BinarySignature &a,
                                       const BinarySignature &b) const
    {
        return (a.op == b.op) && (a.left->hash() == b.left->hash()) &&
               (a.right->hash() == b.right->hash());
    }

    std::pair<const Binary *, bool>
    get_binary_overload(const BinarySignature &signature)
    {
        auto __get = [&](const BinarySignature &signature) -> const Binary *
        {
            auto &[op, left, right] = signature;

            auto it = binary_operations_.find(signature);
            if (it == binary_operations_.end())
            {
                auto l_base = left->base(), r_base = right->base();
                if (l_base == nullptr && r_base == nullptr)
                    return nullptr;

                it = binary_operations_.find(
                    {op, l_base == nullptr ? left : l_base,
                     r_base == nullptr ? right : r_base});
            }

            if (it == binary_operations_.end())
                return nullptr;

            return &it->second;
        };

        auto operation = __get(signature);
        if (operation != nullptr)
            return {operation, false};

        operation = __get(
            BinarySignature{signature.op, signature.right, signature.left});

        return {operation == nullptr || !operation->commutative ? nullptr
                                                                : operation,
                true};
    }

    void overload_binary(Binary &&operation)
    {
        binary_operations_.try_emplace(
            BinarySignature{operation.op, operation.left, operation.right},
            std::move(operation));
    }

} // namespace Core::Operation
