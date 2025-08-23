#pragma once

#include "ast/block/LibBlock.hpp"
#include "core/environment/LibEnvironment.hpp"
#include "core/symbol/Symbol.hpp"

namespace Core
{
    struct LibSymbol : public Symbol
    {
        AST::LibBlock *node = nullptr;
        AST::LibBlock *definition = nullptr;

        Core::LibEnvironment *environment = nullptr;

        LibSymbol(std::string_view name, SymbolAccessibility accessibility,
                  Core::Position &declared_at, AST::LibBlock *node = nullptr);

        void define(Core::Position *position) override;

        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace Core
