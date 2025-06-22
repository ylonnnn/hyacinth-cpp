#include <fstream>
#include <iostream>
#include <string>

#include "lexer/Lexer.hpp"
#include "lexer/Token.hpp"
#include "parser/Parser.hpp"
#include "program/Program.hpp"
#include "utils/control.hpp"

namespace Program
{

    ProgramFile::ProgramFile(const char *path)
        : path_(std::filesystem::absolute(path))
    {
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
        read();

        Lexer::Lexer lexer(*this);

        lexer.tokenize();

        while (!lexer.eof())
            std::cout << *lexer.next() << "\n";

        std::cout << std::endl;

        lexer.rewind();

        Parser::Parser parser(*this, lexer);
        parser.parse();
    }

} // namespace Program
