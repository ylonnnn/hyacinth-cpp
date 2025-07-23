#pragma once

#include <iostream>

#include "diagnostic/Diagnostic.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "diagnostic/NoteDiagnostic.hpp"
#include "diagnostic/WarningDiagnostic.hpp"

namespace Core
{
    enum class ResultStatus
    {
        Success,
        Fail,
    };

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

        virtual void
        error(std::unique_ptr<Diagnostic::ErrorDiagnostic> diagnostic)
        {
            status = ResultStatus::Fail;
            diagnostics.push_back(std::move(diagnostic));
        }

        virtual void error(AST::Node *node, Diagnostic::ErrorType error_type,
                           const std::string &message,
                           const std::string &submessage)
        {
            status = ResultStatus::Fail;
            diagnostics.push_back(std::make_unique<Diagnostic::ErrorDiagnostic>(
                node, error_type, message, submessage));
        }

        virtual void
        warn(std::unique_ptr<Diagnostic::WarningDiagnostic> diagnostic)
        {

            diagnostics.push_back(std::move(diagnostic));
        }

        virtual void warn(AST::Node *node, Diagnostic::WarningType warn_type,
                          const std::string &message,
                          const std::string &submessage)
        {
            diagnostics.push_back(
                std::make_unique<Diagnostic::WarningDiagnostic>(
                    node, warn_type, message, submessage));
        }

        virtual void
        note(std::unique_ptr<Diagnostic::NoteDiagnostic> diagnostic)
        {

            diagnostics.push_back(std::move(diagnostic));
        }

        virtual void note(AST::Node *node, Diagnostic::NoteType note_type,
                          const std::string &message,
                          const std::string &submessage)
        {
            diagnostics.push_back(std::make_unique<Diagnostic::NoteDiagnostic>(
                node, note_type, message, submessage));
        }
    };

} // namespace Core
