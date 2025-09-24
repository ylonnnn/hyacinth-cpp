#include "diagnostic/NoteDiagnostic.hpp"
#include "utils/control.hpp"
#include "utils/style.hpp"

namespace Diagnostic
{
    std::unordered_map<NoteType, const char *> NOTE_CODES;

    static void initialize_codes()
    {
        NOTE_CODES.reserve(32);

        NOTE_CODES[NoteType::Declaration] = "HYC::DEFINITION";
        NOTE_CODES[NoteType::Definition] = "HYC::DEFINITION";
        NOTE_CODES[NoteType::Usage] = "HYC::USAGE";
        NOTE_CODES[NoteType::Suggestion] = "HYC::SUGGESTION";
    }

    static bool initialized_codes = (initialize_codes(), true);

    NoteDiagnostic::NoteDiagnostic(AST::Node *node, NoteType note_type,
                                   const std::string &message,
                                   const std::string &submessage)
        : Diagnostic(node, message, submessage), note_type_(note_type)
    {
        construct();
    }

    NoteType NoteDiagnostic::note_type() { return note_type_; }

    const char *NoteDiagnostic::note_type_to_string(NoteType type)
    {
        auto it = NOTE_CODES.find(type);
        if (it == NOTE_CODES.end())
            utils::terminate("Unknown note type provided!", EXIT_FAILURE);

        return it->second;
    }

    void NoteDiagnostic::construct()
    {
        constructed_ += std::string("\n\n") + NOTE_GEN + "Note <" +
                        note_type_to_string(note_type_) + "> " +
                        utils::Styles::Reset + message_ + "\n\n";

        construct_emphasis((DiagnosticEmphasis){
            .message = submessage_,
            .position = node_->position(),
            .end_position = node_->end_position(),
            .emphasis = NOTE_EMPH,
            .trace = NOTE_GEN,
            .pointer = NOTE_GEN,
        });
    }

} // namespace Diagnostic
