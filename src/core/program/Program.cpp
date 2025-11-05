#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "analyzer/Analyzer.hpp"
#include "core/environment/Environment.hpp"
#include "core/position/Position.hpp"
#include "core/program/DependencyEnvironment.hpp"
#include "core/program/Program.hpp"
#include "core/program/ProgramState.hpp"
#include "core/result/Result.hpp"
#include "diagnostic/reporter/CLIReporter.hpp"
#include "lexer/Lexer.hpp"
// #include "parser/Parser.hpp"
// #include "analyzer/Analyzer.hpp"
// #include "interpreter/Interpreter.hpp"
#include "parser/Parser.hpp"
#include "utils/control.hpp"
#include "utils/style.hpp"

namespace Core
{
    namespace fs = std::filesystem;

    constexpr auto FILE_EXT = ".hyc";

    struct ProgramResult : Result<std::unique_ptr<AST::Program>>
    {
        using Result<std::unique_ptr<AST::Program>>::Result;
    };

    Program::Program(const std::string &path, ProgramState state)
        : state(state), dependencies(std::make_unique<DependencyEnvironment>()),
          environment(std::make_unique<Environment>(dependencies.get()))
    {
        if (!fs::exists(path))
            utils::terminate("Unknown file provided: " + path);

        this->path = fs::absolute(path);
        this->state |= PFS_VALID;

        source_lines.reserve(64);

        read();
    }

    Program::~Program() { delete lexer; }

    size_t Program::file_size(std::ifstream &file)
    {
        file.seekg(0, std::ios::end);
        size_t size = file.tellg();
        file.seekg(0);

        return size;
    }

    void Program::read()
    {
        auto path_str = path.string();

        if (!fs::is_regular_file(path))
            utils::terminate("Target must be a file: " + path_str);
        else if (path.extension() != FILE_EXT)
            utils::terminate("Target must be a Hyacinth file: " + path_str);

        std::ifstream file(path);

        if (!file)
            utils::terminate("Failed to open file: " + path_str);

        auto size = std::filesystem::file_size(path);

        source.resize(size);
        file.read(source.data(), size);

        size_t cursor = 0, line_start = cursor;
        while (cursor < size)
        {
            char &ch = source[cursor];
            if (ch == '\n')
            {
                source_lines.emplace_back(source.data() + line_start,
                                          cursor - line_start);

                line_start = cursor + 1;
            }

            cursor++;
        }

        if (line_start != cursor)
            source_lines.emplace_back(source.data() + line_start,
                                      cursor - line_start);

        // For EOF
        source_lines.push_back("");

        file.close();
    }

    bool Program::valid() const { return state & PFS_VALID; }

    bool Program::analyzed() const { return state & PFS_ANALYZED; }

    bool Program::interpreted() const { return state & PFS_INTERPRETED; }

    Position Program::position_at(size_t row, size_t col, size_t offset)
    {
        return (Position){
            .row = row,
            .col = col,
            .offset = offset,
            .program = std::ref(*this),
        };
    }

    // void Program::depend(Environment &dependency)
    // {
    //     dependencies_->dependencies().push_back(&dependency);
    // }

    Lexer::LexerResult Program::lex()
    {
        auto lexer_start = std::chrono::high_resolution_clock::now();

        lexer = new Lexer::Lexer(*this);
        Lexer::LexerResult lex_result = lexer->tokenize();

        std::cout << "[Lexer] "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         std::chrono::high_resolution_clock::now() -
                         lexer_start)
                         .count()
                  << "\n";

        return lex_result;
    }

    Lexer::LexerResult Program::lex(ProgramResult &result)
    {
        Lexer::LexerResult lex_result = lex();
        result.adapt(lex_result.status, std::move(lex_result.diagnostics));

        return lex_result;
    }

    Parser::ProgramParseResult Program::parse()
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

    Parser::ProgramParseResult Program::parse(ProgramResult &result)
    {
        Parser::ProgramParseResult parse_result = parse();
        result.adapt(parse_result.status, std::move(parse_result.diagnostics),
                     std::move(parse_result.data));

        return parse_result;
    }

    Semantic::AnalysisResult
    Program::analyze(std::unique_ptr<AST::Program> &program)
    {
        if (analyzed())
            return {nullptr, Core::ResultStatus::Success, nullptr, {}};

        auto start = std::chrono::high_resolution_clock::now();

        Semantic::Analyzer analyzer(*this);
        Semantic::AnalysisResult analysis_result = analyzer.analyze(*program);

        state |= PFS_ANALYZED;

        std::cout << "[SemanticAnalysis] "
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         std::chrono::high_resolution_clock::now() - start)
                         .count()
                  << "\n";

        return analysis_result;
    }

    Semantic::AnalysisResult Program::analyze(ProgramResult &result)
    {
        bool is_analyzed = analyzed();
        Semantic::AnalysisResult analysis_result = analyze(result.data);

        if (is_analyzed)
            return analysis_result;

        result.adapt(analysis_result.status,
                     std::move(analysis_result.diagnostics));

        return analysis_result;
    }

    // Interpreter::InterpretationResult
    // Program::interpret(std::unique_ptr<AST::Program> &program)
    // {
    //     if (interpreted_)
    //         return {Core::ResultStatus::Success, nullptr, {}};

    //     auto start = std::chrono::high_resolution_clock::now();

    //     Interpreter::Interpreter interpreter(*this);
    //     Interpreter::InterpretationResult interpretation_result =
    //         interpreter.interpret(*program);

    //     interpreted_ = true;

    //     std::cout << "[Interpretation] "
    //               << std::chrono::duration_cast<std::chrono::microseconds>(
    //                      std::chrono::high_resolution_clock::now() - start)
    //                      .count()
    //               << "\n";

    //     return interpretation_result;
    // }

    // Interpreter::InterpretationResult
    // Program::interpret(ProgramResult &result)
    // {
    //     Interpreter::InterpretationResult interpretation_result =
    //         interpret(result.data);

    //     if (interpreted_)
    //         return interpretation_result;

    //     result.adapt(interpretation_result.status,
    //                  std::move(interpretation_result.diagnostics));

    //     return interpretation_result;
    // }

    // Semantic::AnalysisResult Program::lex_parse_analyze()
    // {
    //     Semantic::AnalysisResult result = {
    //         nullptr, Core::ResultStatus::Success, nullptr, {}};

    //     Lexer::LexerResult lex_result = lex();
    //     result.adapt(lex_result.status, std::move(lex_result.diagnostics));

    //     if (result.status == Core::ResultStatus::Fail)
    //         return result;

    //     Parser::ProgramParseResult parse_result = parse(lex_result.lexer);

    //     node_ = std::move(parse_result.data);
    //     result.adapt(parse_result.status,
    //     std::move(parse_result.diagnostics));

    //     if (result.status == Core::ResultStatus::Fail)
    //         return result;

    //     Semantic::AnalysisResult analysis_result = analyze(node_);
    //     result.adapt(analysis_result.status,
    //                  std::move(analysis_result.diagnostics));

    //     return result;
    // }

    // Interpreter::InterpretationResult
    // Program::lex_parse_analyze_interpret()
    // {
    //     lex_parse_analyze();

    //     return interpret(node_);
    // }

    void Program::execute()
    {
        if (!valid())
            return;

        auto start = std::chrono::high_resolution_clock::now();
        auto succeeded = true;

        ProgramResult result{ResultStatus::Success, nullptr, {}};
        // result.diagnostics.reserve(32);

        // Lexical Analysis
        succeeded = lex(result).status == Core::ResultStatus::Success;

        for (const auto &token : lexer->tokens)
        {
            std::cout << token << "\n";
        }

        // Parsing
        if (succeeded)
            succeeded = parse(result).status == Core::ResultStatus::Success;

        node = std::move(result.data);

        // Semantic Analysis
        if (succeeded)
        {
            Semantic::AnalysisResult analysis_result = analyze(node);
            result.adapt(analysis_result.status,
                         std::move(analysis_result.diagnostics));

            succeeded = result.status == Core::ResultStatus::Success;
        }

        // if (succeeded)
        // {
        //     Semantic::AnalysisResult analysis_result = analyze(node_);
        //     result.adapt(analysis_result.status,
        //                  std::move(analysis_result.diagnostics));

        //     succeeded = result.status == Core::ResultStatus::Success;
        //     analyzed_ = true;
        // }

        // // Interpretation
        // if (succeeded)
        // {
        //     Interpreter::InterpretationResult interpretation_result =
        //         interpret(node_);
        //     result.adapt(interpretation_result.status,
        //                  std::move(interpretation_result.diagnostics));

        //     succeeded = result.status == Core::ResultStatus::Success;
        //     interpreted_ = true;
        // }

        // After Execution
        // Time Elapsed
        auto microseconds =
            std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::high_resolution_clock::now() - start);

        // TEMP
        if (node != nullptr)
            std::cout << *node << "\n";

        // dependencies_->display_symbol_table(std::cout, 1);
        // std::cout << "\n";

        // environment_->display_symbol_table(std::cout, 1);
        // std::cout << "\n";

        // Diagnostics

        for (auto &d : result.diagnostics)
        {
            std::cout << d->range.start() << "\n";
        }

        Diagnostic::CLIReporter reporter(std::move(result.diagnostics));
        Diagnostic::DiagnosticReportStatusResult status = reporter.report();

        if (!(state & PFS_MAIN))
            return;

        utils::TextStyle color =
            succeeded ? utils::Colors::Green : utils::Colors::Red;

        std::cout
            <<
            // [+] N, [~] N, [!] N
            utils::tab(3)

            // [+] N
            << utils::Colors::BrightBlack << "[" << reporter.note_color << "+"
            << utils::Colors::BrightBlack << "] " << reporter.note_color
            << status[static_cast<size_t>(Diagnostic::DiagnosticSeverity::Note)]
            << utils::Styles::Reset
            << " | "

            // [~] N
            << utils::Colors::BrightBlack << "[" << reporter.warning_color
            << "~" << utils::Colors::BrightBlack << "] "
            << reporter.warning_color
            << status[static_cast<size_t>(
                   Diagnostic::DiagnosticSeverity::Warning)]
            << utils::Styles::Reset
            << " | "

            // [!] N
            << utils::Colors::BrightBlack << "[" << reporter.error_color << "!"
            << utils::Colors::BrightBlack << "] " << reporter.error_color
            << status[static_cast<size_t>(
                   Diagnostic::DiagnosticSeverity::Error)]

            << "\n"

            // (/|X) Program Executed (N.NNNNms)
            << utils::tab(3) << color << "[" << (succeeded ? "/" : "X") << "]"
            << utils::Styles::Reset << " Program Executed (" << color
            << static_cast<double>(microseconds.count()) / 1'000 << "ms"
            << utils::Styles::Reset << ")\n\n";
    }

} // namespace Core
