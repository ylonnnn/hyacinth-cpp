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
    class NodeCollection : public Node
    {
      protected:
        std::vector<std::unique_ptr<T>> collection_;

      public:
        NodeCollection(Core::Position &position,
                       std::vector<std::unique_ptr<T>> collection)
            : Node(position), collection_(std::move(collection))
        {
        }

        virtual ~NodeCollection() override = default;

      public:
        virtual void print(std::ostream &os, uint8_t tab) const override
        {
            std::string indentation = Utils::tab(tab - 1),
                        tabs = Utils::tab(tab);
            os << indentation << "NodeCollection {";

            for (const auto &node : collection_)
                node->print(os, tab + 1);

            os << "\n" << indentation << "}";
        }

        auto &collection() { return collection_; }
    };

} // namespace AST
