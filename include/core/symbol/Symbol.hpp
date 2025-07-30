#pragma once

#include "ast/Node.hpp"
#include "core/program/Program.hpp"

namespace Core
{
    struct Symbol
    {
        std::string_view name;
        Core::Position declared_at;
        Core::Position *defined_at = nullptr;
        AST::Node *node = nullptr;

        Symbol(std::string_view name, Core::Position declared_at,
               AST::Node *node = nullptr);

        virtual ~Symbol() = default;

        virtual void define(Core::Position *position);
    };

} // namespace Core
