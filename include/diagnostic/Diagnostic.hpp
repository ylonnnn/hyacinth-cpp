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
        const std::string &message;

        Core::Position &position;
        Core::Position &end_position;

        Utils::TextStyle emphasis;
        Utils::TextStyle trace;
        Utils::TextStyle pointer;
    };

    class Diagnostic
    {
      protected:
        AST::Node *node_ = nullptr;
        const std::string & message_;
        const std::string & submessage_;

        std::vector<std::unique_ptr<Diagnostic>> details_;

        std::string constructed_;

      public:
        Diagnostic(AST::Node *node,const std::string &message,
                   const std::string &submessage);

      public:
        AST::Node &node();

        void add_detail(std::unique_ptr<Diagnostic> detail);

        void construct_emphasis(DiagnosticEmphasis options);
        void report() const;

        virtual void construct() = 0;
    };

    using DiagnosticList = std::vector<std::unique_ptr<Diagnostic>>;

} // namespace Diagnostic
