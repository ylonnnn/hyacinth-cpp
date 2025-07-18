#pragma once

#include <vector>

#include "ast/type/Type.hpp"

namespace AST
{
    class GenericType : public Type
    {
      private:
        std::unique_ptr<Type> constructor_;
        std::vector<std::unique_ptr<Type>> arguments_;

      public:
        GenericType(std::unique_ptr<Type> constructor,
                    std::vector<std::unique_ptr<Type>> arguments);

        Type &constructor();
        std::vector<std::unique_ptr<Type>> &arguments();

        void set_end_pos(size_t end_pos);

        std::string to_string() const override;
        void print(std::ostream &os, uint8_t tab) const override;
    };

} // namespace AST
