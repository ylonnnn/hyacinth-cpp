#pragma once

#include <type_traits>

#include "ast/Node.hpp"

namespace Visitor
{
    template <typename T,
              typename = std::enable_if_t<std::is_base_of_v<AST::Node, T>>>
    struct InterpreterImpl
    {
        static void interpret(class Interpreter &interpreter, T &node);
    };

    class Interpreter
    {
      protected:
        

      public:
        template <typename T,
                  typename = std::enable_if_t<std::is_base_of_v<AST::Node, T>>>
        void interpret(T &node);
    };

} // namespace Visitor
