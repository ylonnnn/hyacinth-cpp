#include "core/environment/EnvironmentStack.hpp"

namespace Core
{
    EnvironmentStack::EnvironmentStack() { stack_.reserve(8); }

    void EnvironmentStack::enter(Environment &env) { stack_.push_back(&env); }

    Environment *EnvironmentStack::exit()
    {
        if (stack_.empty())
            return nullptr;

        Environment *curr = stack_.back();
        stack_.pop_back();

        return curr;
    }

    Environment *EnvironmentStack::root()
    {
        return stack_.empty() ? nullptr : stack_[0];
    }

    const Environment *EnvironmentStack::root() const
    {
        return const_cast<EnvironmentStack *>(this)->root();
    }

    Environment *EnvironmentStack::current() { return stack_.back(); }

    const Environment *EnvironmentStack::current() const
    {
        return const_cast<EnvironmentStack *>(this)->current();
    }

} // namespace Core
