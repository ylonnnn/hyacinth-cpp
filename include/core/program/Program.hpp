#pragma once

#include <filesystem>
#include <vector>

namespace Core
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
        std::vector<std::string_view> lines_;

        Position position_;

      public:
        ProgramFile(const char *path);

      protected:
        size_t file_size(std::ifstream &file);
        void read();

      public:
        const std::filesystem::path &path() const;
        const std::string &source() const;

        std::vector<std::string_view> &lines();

        Position &position();

        Position position_at(size_t row, size_t col);
        void execute();
    };

} // namespace Core
