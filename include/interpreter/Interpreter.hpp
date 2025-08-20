#pragma once

#include "core/result/Result.hpp"
#include "core/symbol/Symbol.hpp"
#include "core/value/Value.hpp"

namespace Interpreter
{
    struct InterpretationResult
        : public Core::Result<std::shared_ptr<Core::Value>>
    {
        // std::shared_ptr<Core::Value> value = nullptr;
        Core::Symbol *symbol = nullptr;

        InterpretationResult(Core::ResultStatus status,
                             const std::shared_ptr<Core::Value> &data,
                             Diagnostic::DiagnosticList diagnostics);
    };

    template <typename T,
              typename = std::enable_if_t<std::is_base_of_v<AST::Node, T>>>
    struct InterpreterImpl
    {
        static InterpretationResult interpret(class Interpreter &interpreter,
                                              T &node);
    };

    class Interpreter
    {
      protected:
        Core::ProgramFile &program_;
        Core::Environment &environment_, *current_env_ = nullptr;

      public:
        Interpreter(Core::ProgramFile &program);

        Core::ProgramFile &program();
        const Core::ProgramFile &program() const;
        Core::Environment &environment();
        const Core::Environment &environment() const;

        Core::Environment *current_env();
        const Core::Environment *current_env() const;
        void set_current_env(Core::Environment &env);

        template <typename T,
                  typename = std::enable_if_t<std::is_base_of_v<AST::Node, T>>>
        InterpretationResult interpret(T &node)
        {
            return InterpreterImpl<T>::interpret(*this, node);
        }
    };

} // namespace Interpreter
