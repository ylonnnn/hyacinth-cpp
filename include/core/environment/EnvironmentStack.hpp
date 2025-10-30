#pragma once

#include "core/environment/Environment.hpp"

namespace Core
{
    struct EnvironmentStack
    {
        EnvironmentStack();

        void enter(Environment &env);
        Environment *exit();

        Environment *root();
        const Environment *root() const;

        Environment *current();
        const Environment *current() const;

      private:
        std::vector<Environment *> stack_;
    };

} // namespace Core
