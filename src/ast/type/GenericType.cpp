#include "ast/type/GenericType.hpp"

namespace AST
{
    GenericType::GenericType(std::unique_ptr<Type> constructor,
                             std::vector<std::unique_ptr<Type>> arguments)
        : Type(::Program::Position(constructor->position())),
          constructor_(std::move(constructor)), arguments_(std::move(arguments))
    {
    }

    Type &GenericType::constructor() { return *constructor_; }

    std::vector<std::unique_ptr<Type>> &GenericType::arguments()
    {
        return arguments_;
    }

    void GenericType::set_end_pos(size_t end_pos) { end_pos_ = end_pos; }

    void GenericType::print([[maybe_unused]] std::ostream &os,
                            [[maybe_unused]] uint8_t tab) const
    {
    }

} // namespace AST
