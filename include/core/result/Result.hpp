#pragma once

#include <iostream>

#include "diagnostic/Diagnostic.hpp"
#include "diagnostic/ErrorType.hpp"
#include "diagnostic/NoteType.hpp"
#include "diagnostic/WarningType.hpp"

namespace Core
{
    enum class ResultStatus
    {
        Success,
        Fail,
    };

    std::ostream &operator<<(std::ostream &os, ResultStatus status);

    template <typename T> struct Result
    {
        ResultStatus status;
        T data;
        Diagnostic::DiagnosticList diagnostics;

        Result(ResultStatus status, const T &data,
               Diagnostic::DiagnosticList &&diagnostics)
            : status(status), data(data), diagnostics(std::move(diagnostics))
        {
        }

        Result(ResultStatus status, T &&data,
               Diagnostic::DiagnosticList &&diagnostics)
            : status(status), data(std::move(data)),
              diagnostics(std::move(diagnostics))
        {
        }

        virtual void adapt(Diagnostic::DiagnosticList &&diagnostics)
        {
            this->diagnostics.insert(
                this->diagnostics.end(),
                std::make_move_iterator(diagnostics.begin()),
                std::make_move_iterator(diagnostics.end()));
        }

        virtual void adapt(ResultStatus status,
                           Diagnostic::DiagnosticList &&diagnostics)
        {
            if (status == Core::ResultStatus::Fail)
                this->status = status;

            adapt(std::move(diagnostics));
        }

        template <typename U>
        void adapt(ResultStatus status,
                   Diagnostic::DiagnosticList &&diagnostics, U &&data)
        {
            adapt(status, std::move(diagnostics));
            this->data = std::forward<U>(data);
        }

        virtual Diagnostic::Diagnostic *
        error(std::unique_ptr<Diagnostic::Diagnostic> &&diagnostic)
        {
            status = ResultStatus::Fail;
            diagnostics.push_back(std::move(diagnostic));

            return diagnostics.back().get();
        }

        virtual Diagnostic::Diagnostic *
        error(Diagnostic::Diagnostic::PosRange range,
              Diagnostic::ErrorType type, std::string &&message)
        {
            status = ResultStatus::Fail;
            diagnostics.push_back(std::make_unique<Diagnostic::Diagnostic>(
                Diagnostic::DiagnosticSeverity::Error,
                static_cast<uint32_t>(type), range, std::move(message)));

            return diagnostics.back().get();
        }

        virtual Diagnostic::Diagnostic *
        warn(std::unique_ptr<Diagnostic::Diagnostic> &&diagnostic)
        {
            diagnostics.push_back(std::move(diagnostic));
            return diagnostics.back().get();
        }

        virtual Diagnostic::Diagnostic *
        warn(Diagnostic::Diagnostic::PosRange range,
             Diagnostic::WarningType type, std::string &&message)
        {
            diagnostics.push_back(std::make_unique<Diagnostic::Diagnostic>(
                Diagnostic::DiagnosticSeverity::Warning,
                static_cast<uint32_t>(type), range, std::move(message)));

            return diagnostics.back().get();
        }

        virtual Diagnostic::Diagnostic *
        note(std::unique_ptr<Diagnostic::Diagnostic> &&diagnostic)
        {
            diagnostics.push_back(std::move(diagnostic));
            return diagnostics.back().get();
        }

        virtual Diagnostic::Diagnostic *
        note(Diagnostic::Diagnostic::PosRange range, Diagnostic::NoteType type,
             std::string &&message)
        {
            diagnostics.push_back(std::make_unique<Diagnostic::Diagnostic>(
                Diagnostic::DiagnosticSeverity::Note,
                static_cast<uint32_t>(type), range, std::move(message)));

            return diagnostics.back().get();
        }
    };

} // namespace Core
