#pragma once

#include <filesystem>
#include <vector>

namespace AST
{
    struct Program;
} // namespace AST

namespace Lexer
{
    class Lexer;
    struct LexerResult;
}; // namespace Lexer

namespace Parser
{
    struct ProgramParseResult;
}

namespace Semantic
{
    struct AnalysisResult;
}

namespace Interpreter
{
    struct InterpretationResult;
}

namespace Core
{
    class ProgramFile;
    class ProgramRegistry;

    class Environment;
    class DependencyEnvironment;

    struct ProgramResult;
    struct Position
    {
        size_t row, col;
        ProgramFile &program;
    };

    class ProgramFile
    {
        friend ProgramRegistry;

      private:
        ProgramRegistry *registry_ = nullptr;

        bool is_main_ = true;

        std::filesystem::path path_;
        bool is_valid_ = false,
             // Phase States
            analyzed_ = false, interpreted_ = false;

        std::string source_;
        std::vector<std::string_view> lines_;

        Lexer::Lexer *lexer_ = nullptr;
        std::unique_ptr<AST::Program> node_;

        std::unique_ptr<DependencyEnvironment> dependencies_;
        std::unique_ptr<Environment> environment_;

      public:
        ProgramFile(const char *path, bool is_main = true);
        ~ProgramFile();

      protected:
        size_t file_size(std::ifstream &file);
        void read();

      public:
        ProgramRegistry *registry();

        const std::filesystem::path &path() const;
        const std::string &source() const;

        std::vector<std::string_view> &lines();
        const std::vector<std::string_view> &lines() const;

        std::unique_ptr<AST::Program> &node();
        const std::unique_ptr<AST::Program> &node() const;

        DependencyEnvironment &dependencies();
        Environment &environment();

        bool valid() const;
        bool analyzed() const;
        bool interpreted() const;

        Position position_at(size_t row, size_t col);

        void depend(Environment &dependency);

        Lexer::LexerResult lex();
        Lexer::LexerResult lex(ProgramResult &result);
        Parser::ProgramParseResult parse(Lexer::Lexer *lexer);
        Parser::ProgramParseResult parse(ProgramResult &result);
        Semantic::AnalysisResult
        analyze(std::unique_ptr<AST::Program> &program);
        Semantic::AnalysisResult analyze(ProgramResult &result);
        Interpreter::InterpretationResult
        interpret(std::unique_ptr<AST::Program> &program);
        Interpreter::InterpretationResult interpret(ProgramResult &result);

        Semantic::AnalysisResult lex_parse_analyze();
        Interpreter::InterpretationResult lex_parse_analyze_interpret();

        void execute();
    };

} // namespace Core
