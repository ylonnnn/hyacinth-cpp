#pragma once

#include "core/program/ProgramState.hpp"
#include <bitset>
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
    struct Position;

    struct ProgramFile
    {
        friend ProgramRegistry;

        ProgramRegistry *registry_ = nullptr;

        std::filesystem::path path;
        ProgramStateFlags state;

        std::string source;
        std::vector<std::string_view> source_lines;

        Lexer::Lexer *lexer = nullptr;
        // std::unique_ptr<AST::Program> node;

        // std::unique_ptr<DependencyEnvironment> dependencies;
        // std::unique_ptr<Environment> environment;

        ProgramFile(const std::string &path,
                    ProgramStateFlags state = PFS_MAIN);
        ~ProgramFile();

        size_t file_size(std::ifstream &file);
        void read();

        bool valid() const;
        bool analyzed() const;
        bool interpreted() const;

        Position position_at(size_t row, size_t col, size_t offset);

        // void depend(Environment &dependency);

        Lexer::LexerResult lex();
        Lexer::LexerResult lex(ProgramResult &result);
        // Parser::ProgramParseResult parse(Lexer::Lexer *lexer);
        // Parser::ProgramParseResult parse(ProgramResult &result);
        // Semantic::AnalysisResult
        // analyze(std::unique_ptr<AST::Program> &program);
        // Semantic::AnalysisResult analyze(ProgramResult &result);
        // Interpreter::InterpretationResult
        // interpret(std::unique_ptr<AST::Program> &program);
        // Interpreter::InterpretationResult interpret(ProgramResult &result);

        // Semantic::AnalysisResult lex_parse_analyze();
        // Interpreter::InterpretationResult lex_parse_analyze_interpret();

        void execute();
    };

} // namespace Core
