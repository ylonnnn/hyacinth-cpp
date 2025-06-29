#pragma once

#include <memory>
#include <unordered_map>

#include "ast/Node.hpp"
#include "diagnostic/Diagnostic.hpp"

namespace Diagnostic
{
    constexpr auto WARN_GEN = Utils::Colors::Yellow,
                   WARN_EMPH = Utils::Colors::BrightYellow;

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
        WarningDiagnostic(std::unique_ptr<AST::Node> node,
                          WarningType warn_type, std::string message,
                          std::string emphasis_message);

        WarningType warn_type();

        const char *warn_type_to_string(WarningType type);
        void report() override;
    };

} // namespace Diagnostic
