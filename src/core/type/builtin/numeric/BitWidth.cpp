#include "core/type/builtin/numeric/BitWidth.hpp"
#include "core/type/TypePool.hpp"

namespace Core
{
    BitWidthType::BitWidthType(Environment &environment)
        : BaseType(environment, "bw"), min_(1), max_(64)
    {
    }

    BitWidthType::T *
    BitWidthType::create_instance(std::vector<GenericArgument> &&arguments)
    {
        return TYPE_POOL.add(std::make_unique<T>(*this, std::move(arguments)));
    }

    BitWidthType::Signal
    BitWidthType::assignable(const std::vector<GenericArgument> &arguments,
                             Value *value, TypeResult &result) const
    {
        auto error = [this, &result, value]() -> auto
        {
            return result.error(Core::PositionRange(*value->range),
                                Diagnostic::ErrorType::TypeMismatch,
                                "expected value of type '" + std::string(name) +
                                    "', received '{}'");
        };

        auto ptr = std::get_if<integer>(value->value.get());
        if (value == nullptr || ptr == nullptr)
        {
            error();
            return Mismatch;
        }

        integer &i_val = *ptr;
        auto val = i_val.as<uint64_t>();

        if (val < min_ || val > max_)
        {
            // TODO: Use make_suggestion()
            error()->add_detail(
                Diagnostic::DiagnosticSeverity::Note,
                static_cast<uint32_t>(Diagnostic::NoteType::Suggestion),
                Core::PositionRange(*value->range),
                "values within {min} to {max} are the only values expected.");

            return Mismatch;
        }

        return Assignable;
    }

} // namespace Core
