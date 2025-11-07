#include "core/type/builtin/Char.hpp"

namespace Core
{
    CharType::CharType(Environment &environment) : BaseType(environment, "char")
    {
    }

    void CharType::default_operations() {}

    CharType::T *
    CharType::create_instance(std::vector<GenericArgument> &&arguments,
                              Core::PositionRange *range)
    {
        // TODO: CharType::create_instance
        return nullptr;
    }

    CharType::Signal
    CharType::assignable(const std::vector<GenericArgument> &arguments,
                         Value *value, TypeResult &result) const
    {
        // TODO: CharType::assignable()
        return 0;
    }

    CharType::T *CharType::infer(Value &value)
    {
        // TODO: CharType::infer()
        return nullptr;
    }

    std::unique_ptr<Diagnostic::Diagnostic>
    CharType::make_suggestion(const std::vector<GenericArgument> &arguments,
                              Value *value) const
    {
        // TODO: CharType::make_suggestion()
        return nullptr;
    }

} // namespace Core
