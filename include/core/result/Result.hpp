#pragma once

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
               Diagnostic::DiagnosticList diagnostics)
            : status(status), data(data), diagnostics(std::move(diagnostics))
        {
        }

        Result(ResultStatus status, T &&data,
               Diagnostic::DiagnosticList diagnostics)
            : status(status), data(std::move(data)),
              diagnostics(std::move(diagnostics))
        {
        }

        virtual void adapt(Diagnostic::DiagnosticList diagnostics)
        {
            this->diagnostics.insert(
                this->diagnostics.end(),
                std::make_move_iterator(diagnostics.begin()),
                std::make_move_iterator(diagnostics.end()));
        }

        virtual void adapt(ResultStatus status,
                           Diagnostic::DiagnosticList diagnostics)
        {
            if (status == Core::ResultStatus::Fail)
                this->status = status;

            adapt(std::move(diagnostics));
        }

        template <typename U>
        void adapt(ResultStatus status, Diagnostic::DiagnosticList diagnostics,
                   U &&data)
        {
            adapt(status, std::move(diagnostics));
            this->data = std::forward<U>(data);
        }

        // virtual void adapt(ResultStatus status,
        //                    Diagnostic::DiagnosticList diagnostics, T &&data)
        // {
        //     adapt(status, std::move(diagnostics));
        //     this->data = std::move(data);
        // }

        // virtual void adapt(ResultStatus status,
        //                    Diagnostic::DiagnosticList diagnostics,
        //                    const T &data)
        // {
        //     adapt(status, std::move(diagnostics));
        //     this->data = data;
        // }

        virtual void error(std::unique_ptr<Diagnostic::Diagnostic> diagnostic)
        {
            status = ResultStatus::Fail;
            diagnostics.push_back(std::move(diagnostic));
        }

        virtual void error(Core::PositionRange &&range,
                           Diagnostic::ErrorType type,
                           const std::string &message)
        {
            status = ResultStatus::Fail;
            diagnostics.push_back(std::make_unique<Diagnostic::Diagnostic>(
                Diagnostic::DiagnosticSeverity::Error, type, std::move(range),
                std::move(message)));
        }

        virtual void warn(std::unique_ptr<Diagnostic::Diagnostic> diagnostic)
        {
            diagnostics.push_back(std::move(diagnostic));
        }

        virtual void warn(Core::PositionRange &&range,
                          Diagnostic::WarningType type,
                          const std::string &message)
        {
            diagnostics.push_back(std::make_unique<Diagnostic::Diagnostic>(
                Diagnostic::DiagnosticSeverity::Warning, type, std::move(range),
                std::move(message)));
        }

        virtual void note(std::unique_ptr<Diagnostic::Diagnostic> diagnostic)
        {

            diagnostics.push_back(std::move(diagnostic));
        }

        virtual void note(Core::PositionRange &&range,
                          Diagnostic::NoteType type, const std::string &message)
        {
            diagnostics.push_back(std::make_unique<Diagnostic::Diagnostic>(
                Diagnostic::DiagnosticSeverity::Note, type, std::move(range),
                std::move(message)));
        }
    };

} // namespace Core
