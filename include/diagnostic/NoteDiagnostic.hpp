#pragma once

#include <memory>
#include <unordered_map>

#include "ast/Node.hpp"
#include "diagnostic/Diagnostic.hpp"

namespace Diagnostic
{
    enum class NoteType
    {
        Definition = 1,
        Usage,
        Suggestion,
    };

    extern std::unordered_map<NoteType, const char *> NOTE_CODES;

    class NoteDiagnostic : public Diagnostic
    {
      private:
        NoteType note_type_;

      public:
        NoteDiagnostic(std::unique_ptr<AST::Node> node, NoteType note_type,
                       std::string &&message, std::string &&emphasis_message);

        NoteType note_type();

        const char *note_type_to_string(NoteType type);
        void report() override;
    };

} // namespace Diagnostic
