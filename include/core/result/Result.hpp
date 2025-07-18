#pragma once

#include "diagnostic/Diagnostic.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "diagnostic/NoteDiagnostic.hpp"
#include "diagnostic/WarningDiagnostic.hpp"
#include <iostream>

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

        virtual void
        error(std::unique_ptr<Diagnostic::ErrorDiagnostic> diagnostic)
        {
            status = ResultStatus::Fail;
            diagnostics.push_back(std::move(diagnostic));
        }

        virtual void error(AST::Node *node, Diagnostic::ErrorType error_type,
                           const std::string & message,
                           const std::string & submessage)
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
                          const std::string & message, const std::string & submessage)
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
                          const std::string & message, const std::string & submessage)
        {
            diagnostics.push_back(std::make_unique<Diagnostic::NoteDiagnostic>(
                node, note_type, message, submessage));
        }
    };

} // namespace Core
