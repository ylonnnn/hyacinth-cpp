#include "core/type/builtin/Str.hpp"
#include "core/type/TypePool.hpp"

namespace Core
{
    StrType::StrType(Environment &environment) : BaseType(environment, "str") {}

    void StrType::default_operations() {}

    StrType::T *
    StrType::create_instance(std::vector<GenericArgument> &&arguments,
                             Core::PositionRange *range)
    {
        return TYPE_POOL.add(
            std::make_unique<T>(*this, std::move(arguments), range));
    }

    StrType::Signal
    StrType::assignable(const std::vector<GenericArgument> &arguments,
                        Value *value, TypeResult &result) const
    {
        if (value == nullptr)
            return Mismatch;

        // Default (std::string)
        if (auto ptr = std::get_if<std::string>(get_rvalue(*value).value.get()))
            return Assignable;

        // TODO: Assignability for other numeric types within the Value variant

        return Mismatch;
    }

    StrType::T *StrType::infer(Value &value)
    {
        return create_instance({}, value.range);
    }

    std::unique_ptr<Diagnostic::Diagnostic>
    StrType::make_suggestion(const std::vector<GenericArgument> &arguments,
                             Value *value) const
    {
        // TODO: StrType::make_suggestion()
        return nullptr;
    }

} // namespace Core
