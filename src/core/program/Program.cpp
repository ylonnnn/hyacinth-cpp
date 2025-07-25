#include <chrono>
#include <fstream>
#include <iostream>
#include <string>

#include "core/program/Program.hpp"
#include "lexer/Lexer.hpp"
#include "parser/Parser.hpp"
#include "semantic/analyzer/Analyzer.hpp"
#include "utils/control.hpp"
#include "utils/style.hpp"

namespace Core
{

    ProgramFile::ProgramFile(const char *path)
        : path_(std::filesystem::absolute(path))
    {
        std::cout << path_.c_str() << "\n";

        read();
    }

    size_t ProgramFile::file_size(std::ifstream &file)
    {
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        file.seekg(0);

        return size;
    }

    void ProgramFile::read()
    {
        std::ifstream file(path_);

        if (!file)
        {
            Utils::terminate(("Failed to open file: " + path_.string()).c_str(),
                             EXIT_FAILURE);
        }

        // source_ = std::string(std::istreambuf_iterator<char>(file),
        //                       std::istreambuf_iterator<char>());

        size_t size = file_size(file);

        source_.resize(size);
        file.read(&source_[0], size);
    }

    const std::filesystem::path &ProgramFile::path() const { return path_; }

    const std::string &ProgramFile::source() const { return source_; }

    // Semantic::ScopeStack &ProgramFile::scope_stack() { return scope_stack_; }

    Position ProgramFile::position_at(size_t row, size_t col)
    {
        return (Position){
            .row = row,
            .col = col,
            .program = *this,
        };
    }

    void ProgramFile::execute()
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto succeed = true;

        auto result = Result<void *>{ResultStatus::Success, nullptr, {}};

        // Lexical Analysis
        Lexer::Lexer lexer(*this);
        lexer.tokenize();

        auto lexer_end = std::chrono::high_resolution_clock::now();

        // Parsing
        Parser::Parser parser(*this, lexer);
        Parser::ProgramParseResult parse_result = parser.parse();

        if (parse_result.status != Core::ResultStatus::Success)
            succeed = false;

        result.adapt(std::move(parse_result.diagnostics));

        auto parser_end = std::chrono::high_resolution_clock::now();

        // std::cout << *parse_result.data << "\n";

        // Semantic Analysis
        if (succeed)
        {
            Semantic::Analyzer analyzer(*this);
            Semantic::AnalysisResult semantic_result =
                analyzer.analyze(*parse_result.data);

            if (semantic_result.status != Core::ResultStatus::Success)
                succeed = false;

            result.adapt(std::move(semantic_result.diagnostics));
        }

        auto semantic_analysis_end = std::chrono::high_resolution_clock::now();

        // After Execution

        // Time Elapsed
        auto end = std::chrono::high_resolution_clock::now();
        auto microseconds =
            std::chrono::duration_cast<std::chrono::microseconds>(end - start);

        std::cout << "[Lexer] "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         lexer_end - start)
                         .count()
                  << "\n";

        std::cout << "[Parser] "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         parser_end - lexer_end)
                         .count()
                  << "\n";

        std::cout << "[Semantic] "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         semantic_analysis_end - parser_end)
                         .count()
                  << "\n";

        // Diagnostics
        for (auto &diagnostic : result.diagnostics)
            diagnostic->report();

        Utils::TextStyle color =
            succeed ? Utils::Colors::Green : Utils::Colors::Red;

        std::cout << "\n\n"
                  << Utils::tab(3) << color << "[" << (succeed ? "/" : "X")
                  << "]" << Utils::Styles::Reset << " Program Executed ("
                  << color << static_cast<double>(microseconds.count()) / 1'000
                  << "ms" << Utils::Styles::Reset << ")\n\n";
    }

} // namespace Core
