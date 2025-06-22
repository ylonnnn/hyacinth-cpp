#pragma once

#include <memory>
#include <string>
#include <vector>

#include "ast/Node.hpp"
#include "utils/style.hpp"

namespace Diagnostic
{
    struct DiagnosticEmphasis
    {
        std::string message;

        Program::Position &position;
        size_t length;

        Utils::TextStyle emphasis;
        Utils::TextStyle trace;
        Utils::TextStyle pointer;
    };

    class Diagnostic
    {
      protected:
        std::unique_ptr<AST::Node> node_;
        std::string message_;

        std::string emphasis_message_;

        std::vector<std::unique_ptr<Diagnostic>> details;

      public:
        Diagnostic(std::unique_ptr<AST::Node> node, std::string &&message,
                   std::string &&emphasis_message);

      public:
        AST::Node &node();

        void emphasize_position(DiagnosticEmphasis &&options);

        virtual void report() = 0;
    };

} // namespace Diagnostic
