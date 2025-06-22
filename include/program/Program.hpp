#pragma once

#include <cstddef>
#include <filesystem>
#include <fstream>
#include <string>

namespace Program
{
    class ProgramFile;

    struct Position
    {
        size_t row, col;
        ProgramFile &program;
    };

    class ProgramFile
    {
      private:
        std::filesystem::path path_;
        bool initialized_ = false;

        std::string source_;

      public:
        ProgramFile(const char *path);

      protected:
        // File Operations
        size_t file_size(std::ifstream &file);
        void read();

      public:
        // Accessors
        const std::filesystem::path &path() const;
        const std::string &source() const;

        Position position_at(size_t row, size_t col);
        void execute();
    };

} // namespace Program
