#pragma once

#include <type_traits>
#include <vector>

#include "ast/Node.hpp"
#include "core/program/Program.hpp"
#include "utils/style.hpp"

namespace AST
{
    template <typename T,
              typename = std::enable_if<std::is_base_of<Node, T>::value>>
    struct NodeCollection : Node
    {
        std::vector<std::unique_ptr<T>> collection;

        NodeCollection(Core::Position &position,
                       std::vector<std::unique_ptr<T>> collection)
            : Node(position), collection(std::move(collection))
        {
        }

        virtual ~NodeCollection() override = default;

        virtual void print(std::ostream &os, uint32_t tab) const override
        {
            std::string indentation = utils::tab(tab - 1),
                        tabs = utils::tab(tab);
            os << indentation << "NodeCollection {";

            for (const auto &node : collection)
                node->print(os, tab + 1);

            os << "\n" << indentation << "}";
        }
    };

} // namespace AST
