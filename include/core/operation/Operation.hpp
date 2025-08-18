#pragma once

#include <functional>
#include <optional>

#include "core/value/Value.hpp"
#include "lexer/Token.hpp"

namespace Core
{
    class Type;

    namespace Operation
    {
        struct Operand
        {
            Type *type = nullptr;
            std::shared_ptr<Value> value;
        };

        struct Assignee : Operand
        {
        };

        using BinaryHandler = std::function<std::optional<Assignee>(
            const Operand &, const Operand &)>;

        struct BinarySignature
        {
            Lexer::TokenType op;
            Type *left = nullptr;
            Type *right = nullptr;
        };

        struct Binary : BinarySignature
        {
            bool commutative;
            BinaryHandler handler;
        };

        struct BinarySignatureHash
        {
            size_t operator()(const BinarySignature &signature) const;
        };

        struct BinarySignatureEq
        {
            bool operator()(const BinarySignature &a,
                            const BinarySignature &b) const;
        };

        using BinaryOverloadMap =
            std::unordered_map<BinarySignature, Binary, BinarySignatureHash,
                               BinarySignatureEq>;

        static BinaryOverloadMap binary_operations_;

        std::pair<const Binary *, bool>
        get_binary_overload(const BinarySignature &signature);

        void overload_binary(Binary &&operation);
    } // namespace Operation

} // namespace Core
