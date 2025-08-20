#include <cassert>
#include <unordered_map>

#include "ast/expr/LiteralExpr.hpp"
#include "diagnostic/ErrorDiagnostic.hpp"
#include "utils/control.hpp"
#include "utils/style.hpp"

namespace Diagnostic
{
    std::unordered_map<ErrorType, const char *> ERROR_CODES;

    static void initialize_codes()
    {
        ERROR_CODES.reserve(32);

        ERROR_CODES[ErrorTypes::Syntax::UnexpectedToken] =
            "HYC::SYNTAX::UNEXPECTED_TOKEN";
        ERROR_CODES[ErrorTypes::Syntax::MissingImportTarget] =
            "HYC::SYNTAX::MISSING_IMPORT_TARGET";
        ERROR_CODES[ErrorTypes::Syntax::MissingIdentifier] =
            "HYC::SYNTAX:MISSING_IDENTIFIER";
        ERROR_CODES[ErrorTypes::Syntax::MissingValue] =
            "HYC::SYNTAX:MISSING_VALUE";
        ERROR_CODES[ErrorTypes::Syntax::MissingKeyword] =
            "HYC::SYNTAX:MISSING_KEYWORD";
        ERROR_CODES[ErrorTypes::Syntax::MissingOperator] =
            "HYC::SYNTAX:MISSING_OPERATOR";
        ERROR_CODES[ErrorTypes::Syntax::MissingType] =
            "HYC::SYNTAX:MISSING_TYPE";
        ERROR_CODES[ErrorTypes::Syntax::MissingReturnType] =
            "HYC::SYNTAX:MISSING_RETURN_TYPE";
        ERROR_CODES[ErrorTypes::Syntax::MissingMutabilityModifier] =
            "HYC::SYNTAX:MISSING_MUTABILITY_MODIFIER";
        ERROR_CODES[ErrorTypes::Syntax::InvalidFieldSpecifier] =
            "HYC::SYNTAX:INVALID_FIELD_SPECIFIER";
        ERROR_CODES[ErrorTypes::Syntax::FieldDuplcation] =
            "HYC::SYNTAX:FIELD_DUPLICATION";

        ERROR_CODES[ErrorTypes::Type::Mismatch] = "HYC::TYPE:MISMATCH";
        ERROR_CODES[ErrorTypes::Type::UnrecognizedType] =
            "HYC::TYPE::UNRECOGNIZED_TYPE";
        ERROR_CODES[ErrorTypes::Type::InvalidVariableType] =
            "HYC::TYPE::INVALID_VARIABLE_TYPE";
        ERROR_CODES[ErrorTypes::Type::InvalidArgumentType] =
            "HYC::TYPE::INVALID_ARGUMENT_TYPE";
        ERROR_CODES[ErrorTypes::Type::InvalidTypeArgumentType] =
            "HYC::TYPE::INVALID_TYPE_ARGUMENT_TYPE";
        ERROR_CODES[ErrorTypes::Type::InvalidReturnType] =
            "HYC::TYPE::INVALID_RETURN_TYPE";
        ERROR_CODES[ErrorTypes::Type::InvalidInstantiationType] =
            "HYC::TYPE::INVALID_INSTANTIATION_TYPE";

        ERROR_CODES[ErrorTypes::Semantic::NonCallableInvocation] =
            "HYC::SEMANTIC::NONCALLABLE_INVOCATION";
        ERROR_CODES[ErrorTypes::Semantic::UnrecognizedSymbol] =
            "HYC::SEMANTIC::UNRECOGNIZED_SYMBOL";
        ERROR_CODES[ErrorTypes::Semantic::InvalidArgumentCount] =
            "HYC::SEMANTIC::INVALID_ARGUMENT_COUNT";
        ERROR_CODES[ErrorTypes::Semantic::Duplication] =
            "HYC::SEMANTIC::DUPLICATION";
        ERROR_CODES[ErrorTypes::Semantic::InvalidReturnUsage] =
            "HYC::SEMANTIC::INVALID_RETURN_USAGE";
        ERROR_CODES[ErrorTypes::Semantic::IllegalShadowing] =
            "HYC::SEMANTIC::ILLEGAL_SHADOWING";
        ERROR_CODES[ErrorTypes::Semantic::NoViableOperatorOverload] =
            "HYC::SEMANTIC::NO_VIABLE_OPERATOR_OVERLOAD";
        ERROR_CODES[ErrorTypes::Semantic::UnrecognizedField] =
            "HYC::SEMANTIC::UNRECOGNIZED_FIELD";
        ERROR_CODES[ErrorTypes::Semantic::InvalidFieldCount] =
            "HYC::SEMANTIC::INVALID_FIELD_COUNT";
        ERROR_CODES[ErrorTypes::Semantic::IllegalNonArrayElementAccess] =
            "HYC::SEMANTIC::ILLEGAL_NONARRAY_ELEMENT_ACCESS";
        ERROR_CODES[ErrorTypes::Semantic::OutOfRange] =
            "HYC::SEMANTIC::OUT_OF_RANGE";

        ERROR_CODES[ErrorTypes::Runtime::UnresolvedImport] =
            "HYC::RUNTIME::UNRESOLVED_IMPORT";
        ERROR_CODES[ErrorTypes::Runtime::OutOfRange] =
            "HYC::RUNTIME::OUT_OF_RANGE";

        ERROR_CODES[ErrorTypes::Modification::ReadValueModification] =
            "HYC::MODIFICATION:RVALUE_MODIFICATION";
        ERROR_CODES[ErrorTypes::Modification::ImmutableValueModification] =
            "HYC::MODIFICATION:IMMUTABLE_MODIFICATION";

        ERROR_CODES[ErrorTypes::Uninitialization::UninitializedValue] =
            "HYC::UNINITIALIZATION:UNINITIALIZED_VALUE";
        ERROR_CODES[ErrorTypes::Uninitialization::UninitializedImmutable] =
            "HYC::UNINITIALIZATION:UNINITIALIZED_IMMUTABLE";
    }

    static bool initialized_codes = (initialize_codes(), true);

    ErrorDiagnostic::ErrorDiagnostic(AST::Node *node, ErrorType error_type,
                                     const std::string &message,
                                     const std::string &submessage)
        : Diagnostic(node, message, submessage), error_type_(error_type)
    {
        construct();
    }

    ErrorType ErrorDiagnostic::error_type() { return error_type_; }

    const char *ErrorDiagnostic::error_type_to_string(ErrorType type)
    {
        auto it = ERROR_CODES.find(type);
        if (it == ERROR_CODES.end())
            Utils::terminate("Unknown error type provided!", EXIT_FAILURE);

        return it->second;
    }

    void ErrorDiagnostic::construct()
    {
        constructed_ += std::string("\n\n") + ERR_GEN + "Error <" +
                        error_type_to_string(error_type_) + "> " +
                        Utils::Styles::Reset + message_ + "\n\n";

        construct_emphasis((DiagnosticEmphasis){
            .message = submessage_,
            .position = node_->position(),
            .end_position = node_->end_position(),
            .emphasis = ERR_EMPH,
            .trace = ERR_GEN,
            .pointer = ERR_GEN,
        });
    }

    std::unique_ptr<ErrorDiagnostic>
    create_syntax_error(Lexer::Token *token,
                        std::optional<Lexer::TokenType> expected)
    {
        assert(token != nullptr);

        bool expects = !!expected;
        auto node = AST::LiteralExpr(*token);

        return std::make_unique<ErrorDiagnostic>(
            &node, ErrorTypes::Syntax::UnexpectedToken,
            std::string("Unexpected \"") + ERR_GEN + std::string(token->value) +
                Utils::Styles::Reset + "\"." +
                (expects ? std::string(" Expected \"") + ERR_GEN +
                               Lexer::type_to_string(*expected) +
                               Utils::Styles::Reset + "."
                         : ""),
            std::string("Received ") + Lexer::type_to_string(token->type) +
                (expects ? " instead" : "") + ".");
    }

    std::unique_ptr<ErrorDiagnostic> create_unknown_type_error(AST::Type *type)
    {
        assert(type != nullptr);

        return std::make_unique<ErrorDiagnostic>(
            type, ErrorTypes::Type::UnrecognizedType,
            std::string("Unknown type \"") + ERR_GEN + type->to_string() +
                Utils::Styles::Reset + "\" received.",
            "Type received here");
    }

    std::unique_ptr<ErrorDiagnostic>
    create_invalid_entry_count_error(AST::Node *node, ErrorType error_type,
                                     const std::string &entry, size_t expected,
                                     size_t provided)
    {
        assert(node != nullptr);

        auto arg__ = [&](size_t n) -> std::string
        { return entry + (n > 1 ? "s" : ""); };

        return std::make_unique<ErrorDiagnostic>(
            node, error_type,
            std::string("Expects ") + ERR_GEN + std::to_string(expected) +
                Utils::Styles::Reset + " " + arg__(expected) + ". Provided " +
                ERR_GEN + std::to_string(provided) + Utils::Styles::Reset +
                " " + arg__(provided) + ".",
            "");
    }

    std::unique_ptr<ErrorDiagnostic>
    create_invalid_arguments_error(AST::Node *node, size_t expected,
                                   size_t provided)
    {
        return create_invalid_entry_count_error(
            node, ErrorTypes::Semantic::InvalidArgumentCount, "argument",
            expected, provided);
    }

} // namespace Diagnostic
