#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "core/environment/Environment.hpp"
#include "core/program/DependencyEnvironment.hpp"
#include "core/program/Program.hpp"
#include "core/result/Result.hpp"
#include "interpreter/Interpreter.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "semantic/analyzer/Analyzer.hpp"
#include "utils/control.hpp"
#include "utils/style.hpp"

namespace Core
{
    namespace fs = std::filesystem;

    constexpr auto FILE_EXTENSION = ".hyc";

    struct ProgramResult : Result<std::unique_ptr<AST::Program>>
    {
        Lexer::Lexer *lexer = nullptr;

        using Result<std::unique_ptr<AST::Program>>::Result;
    };

    ProgramFile::ProgramFile(const char *path, bool is_main)
        : is_main_(is_main),
          dependencies_(std::make_unique<DependencyEnvironment>()),
          environment_(std::make_unique<Environment>(dependencies_.get()))
    {
        try
        {
            path_ = fs::canonical(path);
            is_valid_ = true;
        }
        catch (const fs::filesystem_error &err)
        {
            // For explicitness
            is_valid_ = false;

            Utils::terminate(
                (std::string("Unknown file path provided: ") + path).c_str(),
                EXIT_SUCCESS);
        }

        lines_.reserve(64);

        read();

        lexer_ = new Lexer::Lexer(*this);
    }

    ProgramFile::~ProgramFile() { delete lexer_; }

    size_t ProgramFile::file_size(std::ifstream &file)
    {
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        file.seekg(0);

        return size;
    }

    void ProgramFile::read()
    {
        if (!fs::is_regular_file(path_))
        {
            Utils::terminate(
                (std::string("Target be a file: ") + path_.string()).c_str(),
                EXIT_SUCCESS);
        }
        else if (path_.extension() != FILE_EXTENSION)
            Utils::terminate(
                (std::string("Target be a Hyacinth file: ") + path_.string())
                    .c_str(),
                EXIT_SUCCESS);

        std::ifstream file(path_);

        if (!file)
        {
            Utils::terminate(("Failed to open file: " + path_.string()).c_str(),
                             EXIT_SUCCESS);
        }

        size_t size = file_size(file);

        source_.resize(size);
        file.read(source_.data(), size);

        size_t cursor = 0, line_start = cursor;
        while (cursor < size)
        {
            char &ch = source_[cursor];
            if (ch == '\n')
            {
                lines_.emplace_back(source_.data() + line_start,
                                    cursor - line_start);

                line_start = cursor + 1;
            }

            cursor++;
        }

        if (line_start != cursor)
            lines_.emplace_back(source_.data() + line_start,
                                cursor - line_start);

        file.close();
    }

    ProgramRegistry *ProgramFile::registry() { return registry_; }

    const fs::path &ProgramFile::path() const { return path_; }

    const std::string &ProgramFile::source() const { return source_; }

    std::vector<std::string_view> &ProgramFile::lines() { return lines_; }

    const std::vector<std::string_view> &ProgramFile::lines() const
    {
        return lines_;
    }

    std::unique_ptr<AST::Program> &ProgramFile::node() { return node_; }

    const std::unique_ptr<AST::Program> &ProgramFile::node() const
    {
        return node_;
    }

    DependencyEnvironment &ProgramFile::dependencies()
    {
        return *dependencies_;
    }

    Environment &ProgramFile::environment() { return *environment_; }

    bool ProgramFile::valid() const { return is_valid_; }

    bool ProgramFile::analyzed() const { return analyzed_; }

    bool ProgramFile::interpreted() const { return interpreted_; }

    Position ProgramFile::position_at(size_t row, size_t col)
    {
        return (Position){
            .row = row,
            .col = col,
            .program = *this,
        };
    }

    void ProgramFile::depend(Environment &dependency)
    {
        dependencies_->dependencies().push_back(&dependency);
    }

    Lexer::LexerResult ProgramFile::lex()
    {
        auto lexer_start = std::chrono::high_resolution_clock::now();

        Lexer::LexerResult lex_result = lexer_->tokenize();

        std::cout << "[Lexer] "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         std::chrono::high_resolution_clock::now() -
                         lexer_start)
                         .count()
                  << "\n";

        return lex_result;
    }

    Lexer::LexerResult ProgramFile::lex(ProgramResult &result)
    {
        Lexer::LexerResult lex_result = lex();

        result.lexer = lex_result.lexer;
        result.adapt(lex_result.status, std::move(lex_result.diagnostics));

        return lex_result;
    }

    Parser::ProgramParseResult ProgramFile::parse(Lexer::Lexer *lexer)
    {
        auto parser_start = std::chrono::high_resolution_clock::now();

        Parser::Parser parser(*this, *lexer);
        Parser::ProgramParseResult parse_result = parser.parse();

        std::cout << "[Parser] "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         std::chrono::high_resolution_clock::now() -
                         parser_start)
                         .count()
                  << "\n";

        return parse_result;
    }

    Parser::ProgramParseResult ProgramFile::parse(ProgramResult &result)
    {
        Parser::ProgramParseResult parse_result = parse(result.lexer);
        result.adapt(parse_result.status, std::move(parse_result.diagnostics),
                     std::move(parse_result.data));

        return parse_result;
    }

    Semantic::AnalysisResult
    ProgramFile::analyze(std::unique_ptr<AST::Program> &program)
    {
        if (analyzed_)
            return {nullptr, Core::ResultStatus::Success, nullptr, {}};

        auto start = std::chrono::high_resolution_clock::now();

        Semantic::Analyzer analyzer(*this);
        Semantic::AnalysisResult analysis_result = analyzer.analyze(*program);

        analyzed_ = true;

        std::cout << "[SemanticAnalysis] "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         std::chrono::high_resolution_clock::now() - start)
                         .count()
                  << "\n";

        return analysis_result;
    }

    Semantic::AnalysisResult ProgramFile::analyze(ProgramResult &result)
    {
        Semantic::AnalysisResult analysis_result = analyze(result.data);

        if (analyzed_)
            return analysis_result;

        result.adapt(analysis_result.status,
                     std::move(analysis_result.diagnostics));

        return analysis_result;
    }

    Interpreter::InterpretationResult
    ProgramFile::interpret(std::unique_ptr<AST::Program> &program)
    {
        if (interpreted_)
            return {Core::ResultStatus::Success, nullptr, {}};

        auto start = std::chrono::high_resolution_clock::now();

        Interpreter::Interpreter interpreter(*this);
        Interpreter::InterpretationResult interpretation_result =
            interpreter.interpret(*program);

        interpreted_ = true;

        std::cout << "[Interpretation] "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         std::chrono::high_resolution_clock::now() - start)
                         .count()
                  << "\n";

        return interpretation_result;
    }

    Interpreter::InterpretationResult
    ProgramFile::interpret(ProgramResult &result)
    {
        Interpreter::InterpretationResult interpretation_result =
            interpret(result.data);

        if (interpreted_)
            return interpretation_result;

        result.adapt(interpretation_result.status,
                     std::move(interpretation_result.diagnostics));

        return interpretation_result;
    }

    Semantic::AnalysisResult ProgramFile::lex_parse_analyze()
    {
        Semantic::AnalysisResult result = {
            nullptr, Core::ResultStatus::Success, nullptr, {}};

        Lexer::LexerResult lex_result = lex();
        result.adapt(lex_result.status, std::move(lex_result.diagnostics));

        if (result.status == Core::ResultStatus::Fail)
            return result;

        Parser::ProgramParseResult parse_result = parse(lex_result.lexer);

        node_ = std::move(parse_result.data);
        result.adapt(parse_result.status, std::move(parse_result.diagnostics));

        if (result.status == Core::ResultStatus::Fail)
            return result;

        Semantic::AnalysisResult analysis_result = analyze(node_);
        result.adapt(analysis_result.status,
                     std::move(analysis_result.diagnostics));

        return result;
    }

    Interpreter::InterpretationResult ProgramFile::lex_parse_analyze_interpret()
    {
        lex_parse_analyze();

        return interpret(node_);
    }

    void ProgramFile::execute()
    {
        if (!is_valid_)
            return;

        auto start = std::chrono::high_resolution_clock::now();
        auto succeeded = true;

        ProgramResult result = {ResultStatus::Success, nullptr, {}};
        result.diagnostics.reserve(32);

        // Lexical Analysis
        succeeded = lex(result).status == Core::ResultStatus::Success;

        // Parsing
        if (succeeded)
            succeeded = parse(result).status == Core::ResultStatus::Success;

        node_ = std::move(result.data);

        // Semantic Analysis
        if (succeeded)
        {
            Semantic::AnalysisResult analysis_result = analyze(node_);
            result.adapt(analysis_result.status,
                         std::move(analysis_result.diagnostics));

            succeeded = result.status == Core::ResultStatus::Success;
            analyzed_ = true;
        }

        // Interpretation
        if (succeeded)
        {
            Interpreter::InterpretationResult interpretation_result =
                interpret(node_);
            result.adapt(interpretation_result.status,
                         std::move(interpretation_result.diagnostics));

            succeeded = result.status == Core::ResultStatus::Success;
            interpreted_ = true;
        }

        // After Execution
        // Time Elapsed
        auto microseconds =
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start);

        // TEMP
        if (node_ != nullptr)
            std::cout << *node_ << "\n";

        dependencies_->display_symbol_table(std::cout, 1);
        std::cout << "\n";

        environment_->display_symbol_table(std::cout, 1);
        std::cout << "\n";

        // Diagnostics
        for (auto &diagnostic : result.diagnostics)
            diagnostic->report();

        if (!is_main_)
            return;

        Utils::TextStyle color =
            succeeded ? Utils::Colors::Green : Utils::Colors::Red;

        std::cout << "\n\n"
                  << Utils::tab(3) << color << "[" << (succeeded ? "/" : "X")
                  << "]" << Utils::Styles::Reset << " Program Executed ("
                  << color << static_cast<double>(microseconds.count()) / 1'000
                  << "ms" << Utils::Styles::Reset << ")\n\n";
    }

} // namespace Core
