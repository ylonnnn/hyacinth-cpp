#include "diagnostic/NoteDiagnostic.hpp"
#include "utils/control.hpp"
#include "utils/style.hpp"

namespace Diagnostic
{
    std::unordered_map<NoteType, const char *> NOTE_CODES;

    static void initialize_codes()
    {
        NOTE_CODES.reserve(32);

        NOTE_CODES[NoteType::Definition] = "HYC::DEFINITION";
        NOTE_CODES[NoteType::Usage] = "HYC::USAGE";
        NOTE_CODES[NoteType::Suggestion] = "HYC::SUGGESTION";
    }

    static bool initialized_codes = (initialize_codes(), true);

    NoteDiagnostic::NoteDiagnostic(std::unique_ptr<AST::Node> node,
                                   NoteType note_type, std::string message,
                                   std::string emphasis_message)
        : Diagnostic(std::move(node), std::move(message),
                     std::move(emphasis_message)),
          note_type_(note_type)
    {
    }

    NoteType NoteDiagnostic::note_type() { return note_type_; }

    const char *NoteDiagnostic::note_type_to_string(NoteType type)
    {
        auto it = NOTE_CODES.find(type);
        if (it == NOTE_CODES.end())
            Utils::terminate("Unknown error type provided!", EXIT_FAILURE);

        return it->second;
    }

    void NoteDiagnostic::report()
    {
        const ::Program::Position &position = node_->position();

        std::cout << "\n\n";

        std::cout << NOTE_GEN << "Note <" << note_type_to_string(note_type_)
                  << "> " << Utils::Styles::Reset << message_ << "\n\n";

        emphasize_position((DiagnosticEmphasis){
            .message = emphasis_message_,
            .position = const_cast<::Program::Position &>(position),
            .length = node_->end_pos(),
            .emphasis = NOTE_EMPH,
            .trace = NOTE_GEN,
            .pointer = NOTE_GEN,
        });

        for (std::unique_ptr<Diagnostic> &detail : details)
        {
            detail->report();
        }
    }

} // namespace Diagnostic
