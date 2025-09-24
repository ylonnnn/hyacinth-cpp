#pragma once

#include <unordered_map>

#include "ast/Node.hpp"
#include "diagnostic/Diagnostic.hpp"

namespace Diagnostic
{
    constexpr auto NOTE_GEN = utils::Colors::Blue,
                   NOTE_EMPH = utils::Colors::BrightBlue;

    enum class NoteType
    {
        Declaration = 1,
        Definition,
        Usage,
        Suggestion,
    };

    extern std::unordered_map<NoteType, const char *> NOTE_CODES;

    class NoteDiagnostic : public Diagnostic
    {
      private:
        NoteType note_type_;

      public:
        NoteDiagnostic(AST::Node *node, NoteType note_type,
                       const std::string & message, const std::string & submessage);

        NoteType note_type();

        const char *note_type_to_string(NoteType type);

        void construct() override;
    };

} // namespace Diagnostic
