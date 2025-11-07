#include "core/type/builtin/Char.hpp"
#include "core/type/TypePool.hpp"

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
        return TYPE_POOL.add(
            std::make_unique<T>(*this, std::move(arguments), range));
    }

    CharType::Signal
    CharType::assignable(const std::vector<GenericArgument> &arguments,
                         Value *value, TypeResult &result) const
    {
        // Default (Core::character)
        if (auto ptr = std::get_if<character>(value->value.get()))
            return Assignable;

        // TODO: Assignability for other numeric types within the Value variant

        return Mismatch;
    }

    CharType::T *CharType::infer(Value &value)
    {
        return create_instance({}, value.range);
    }

    std::unique_ptr<Diagnostic::Diagnostic>
    CharType::make_suggestion(const std::vector<GenericArgument> &arguments,
                              Value *value) const
    {
        // TODO: CharType::make_suggestion()
        return nullptr;
    }

} // namespace Core
