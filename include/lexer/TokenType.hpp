#include <cstddef>

namespace Lexer
{
    enum class TokenType
    {
        // ==== ILLEGAL ====
        Illegal,

        // ==== RESERVED ====

        // ==== RESERVED::DECLARATION ====
        Unit,
        Struct,
        Fn,
        Var,

        // ==== RESERVED::QUALIFIER ====
        Mut,

        // ==== RESERVED::CONTROL ====
        Return,

        // ==== RESERVED::ACCESSIBILITY ====
        Pub,
        Prot,

        // ==== RESERVED::OTHER ====
        Import,

        // ==== PRIMARY ====
        Identifier,
        Int,
        Float,
        Char,
        Bool,
        String,
        Null,

        // ==== OPERATORS ====

        // ==== OPERATORS:ARITHMETIC ====
        Plus,    // +
        Minus,   // -
        Star,    // *
        Slash,   // /
        Percent, // %

        // ==== OPERATORS::ARROW ====
        LessMinus,    // <-
        MinusGreater, // ->

        // ===== OPERATORS:COMPARISON =====
        EqualEqual,   // ==
        BangEqual,    // !=
        Less,         // <
        LessEqual,    // <=
        Greater,      // >
        GreaterEqual, // >=

        // ===== OPERATORS:LOGICAL =====
        Bang,               // !
        AmpersandAmpersand, // &&
        PipePipe,           // ||

        // ===== OPERATORS:BITWISE =====
        Ampersand,      // &
        Pipe,           // |
        Caret,          // ^
        Tilde,          // ~
        LessLess,       // <<
        GreaterGreater, // >>

        // ==== OPERATORS::UNARY ====
        PlusPlus,   // ++
        MinusMinus, // --
        CaretCaret, // ^^ (Exponent)

        // ===== OPERATORS:ASSIGNMENT =====
        Equal,        // =
        PlusEqual,    // +=
        MinusEqual,   // -=
        StarEqual,    // *=
        SlashEqual,   // /=
        PercentEqual, // %=

        // ===== DELIMITERS =====
        LeftParen,        // (
        RightParen,       // )
        LeftBrace,        // {
        RightBrace,       // }
        LeftBracket,      // [
        RightBracket,     // ]
        Comma,            // ,
        Dot,              // .
        DoubleDot,        // ..
        Semicolon,        // ;
        Colon,            // :
        DoubleColon,      // ::
        Question,         // ?
        QuestionQuestion, // ??
        QuestionDot,      // ?.

        // ==== MISC ====
        EndOfFile, // EOF

        // Count
        TokenTypeCount,
    };

    static constexpr const char *TYPE_NAMES[] = {
        // ==== ILLEGAL ====
        "Illegal",

        // ==== RESERVED ====

        // ==== RESERVED::DECLARATION ====
        "Unit",
        "Struct",
        "Fn",
        "Var",

        // ==== RESERVED::QUALIFIER ====
        "Mut",

        // ==== RESERVED::CONTROL ====
        "Return",

        // ==== RESERVED::ACCESSIBILITY ====
        "Pub",
        "Prot",

        // ==== RESERVED::OTHER ====
        "Import",

        // ==== PRIMARY ====
        "Identifier",
        "Int",
        "Float",
        "Char",
        "Bool",
        "String",
        "Null",

        // ==== OPERATORS ====

        // ==== OPERATORS:ARITHMETIC ====
        "Plus",    // +
        "Minus",   // -
        "Star",    // *
        "Slash",   // /
        "Percent", // %

        // ==== OPERATORS::ARROW ====
        "LessMinus",    // <-
        "MinusGreater", // ->

        // ===== OPERATORS:COMPARISON =====
        "EqualEqual",   // ==
        "BangEqual",    // !=
        "Less",         // <
        "LessEqual",    // <=
        "Greater",      // >
        "GreaterEqual", // >=

        // ===== OPERATORS:LOGICAL =====
        "Bang",               // !
        "AmpersandAmpersand", // &&
        "PipePipe",           // ||

        // ===== OPERATORS:BITWISE =====
        "Ampersand",      // &
        "Pipe",           // |
        "Caret",          // ^
        "Tilde",          // ~
        "LessLess",       // <<
        "GreaterGreater", // >>

        // ==== OPERATORS::UNARY ====
        "PlusPlus",   // ++
        "MinusMinus", // --
        "CaretCaret", // ^^ (Exponent)

        // ===== OPERATORS:ASSIGNMENT =====
        "Equal",        // =
        "PlusEqual",    // +=
        "MinusEqual",   // -=
        "StarEqual",    // *=
        "SlashEqual",   // /=
        "PercentEqual", // %=

        // ===== DELIMITERS =====
        "LeftParen",        // (
        "RightParen",       // )
        "LeftBrace",        // {
        "RightBrace",       // {}
        "LeftBracket",      // [
        "RightBracket",     // ]
        "Comma",            // ,
        "Dot",              // .
        "DoubleDot",        // ..
        "Semicolon",        // ;
        "Colon",            // :
        "DoubleColon",      // ::
        "Question",         // ?
        "QuestionDot",      // ?.
        "QuestionQuestion", // ??

        // ==== MISC ====
        "EndOfFile", // EOF
    };

    static constexpr const char *TYPE_STRS[] = {
        // ==== ILLEGAL ====
        "Illegal",

        // ==== RESERVED::DECLARATION ====
        "unit",
        "struct",
        "fn",
        "var",

        // ==== RESERVED::QUALIFIER ====
        "mut",

        // ==== RESERVED::CONTROL ====
        "return",

        // ==== RESERVED::ACCESSIBILITY ====
        "pub",
        "prot",

        // ==== RESERVED::OTHER ====
        "import",

        // ==== PRIMARY ====
        "identifier",
        "int",
        "float",
        "char",
        "bool",
        "string",
        "null",

        // ==== OPERATORS:ARITHMETIC ====
        "+", // Plus
        "-", // Minus
        "*", // Star
        "/", // Slash
        "%", // Percent

        // ==== OPERATORS::ARROW ====
        "<-", // LessMinus
        "->", // MinusGreater

        // ===== OPERATORS:COMPARISON =====
        "==", // EqualEqual
        "!=", // BangEqual
        "<",  // Less
        "<=", // LessEqual
        ">",  // Greater
        ">=", // GreaterEqual

        // ===== OPERATORS:LOGICAL =====
        "!",  // Bang
        "&&", // AmpersandAmpersand
        "||", // PipePipe

        // ===== OPERATORS:BITWISE =====
        "&",  // Ampersand
        "|",  // Pipe
        "^",  // Caret
        "~",  // Tilde
        "<<", // LessLess
        ">>", // GreaterGreater

        // ==== OPERATORS::UNARY ====
        "++", // PlusPlus
        "--", // MinusMinus
        "^^", // CaretCaret

        // ===== OPERATORS:ASSIGNMENT =====
        "=",  // Equal
        "+=", // PlusEqual
        "-=", // MinusEqual
        "*=", // StarEqual
        "/=", // SlashEqual
        "%=", // PercentEqual

        // ===== DELIMITERS =====
        "(",  // LeftParen
        ")",  // RightParen
        "{",  // LeftBrace
        "}",  // RightBrace
        "[",  // LeftBracket
        "]",  // RightBracket
        ",",  // Comma
        ".",  // Dot
        "..", // DoubleDot
        ";",  // Semicolon
        ":",  // Colon
        "::", // DoubleColon
        "?",  // Question
        "?.", // QuestionDot
        "??", // QuestionQuestion

        // ==== MISC ====
        "EOF", // EndOfFile
    };

    static inline constexpr const char *type_to_name(TokenType type)
    {
        return TYPE_NAMES[static_cast<size_t>(type)];
    }

    static inline const char *type_to_string(TokenType type)
    {
        return TYPE_STRS[static_cast<size_t>(type)];
    }

} // namespace Lexer
