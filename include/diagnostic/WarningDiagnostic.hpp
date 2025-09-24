#pragma once

#include <unordered_map>

#include "ast/Node.hpp"
#include "diagnostic/Diagnostic.hpp"

namespace Diagnostic
{
    constexpr auto WARN_GEN = utils::Colors::Yellow,
                   WARN_EMPH = utils::Colors::BrightYellow;

    enum class WarningType
    {
        Unreachable = 1,
        Unused,
    };

    extern std::unordered_map<WarningType, const char *> WARN_CODES;

    class WarningDiagnostic : public Diagnostic
    {
      private:
        WarningType warn_type_;

      public:
        WarningDiagnostic(AST::Node *node, WarningType warn_type,
                          const std::string &message,
                          const std::string &submessage);

        WarningType warn_type();

        const char *warn_type_to_string(WarningType type);

        void construct() override;
    };

} // namespace Diagnostic
