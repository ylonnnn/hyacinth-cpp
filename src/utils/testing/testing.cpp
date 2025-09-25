#include "utils/testing/testing.hpp"
#include "core/program/Program.hpp"
#include "core/program/ProgramRegistry.hpp"
#include "utils/control.hpp"
#include "utils/style.hpp"

namespace utils::testing
{
    namespace fs = std::filesystem;

    std::vector<fs::path> files_from(const fs::path &path)
    {
        if (!fs::exists(path) || !fs::is_directory(path))
            utils::terminate("Invalid testing path provided! Provided path "
                             "must be a path to a directory of test files.");

        std::vector<fs::path> files;

        for (const auto &entry : fs::directory_iterator(path))
        {
            const fs::path &path = entry.path();
            if (fs::is_directory(path))
            {
                std::vector<fs::path> inner = files_from(path);
                files.insert(files.end(),
                             std::make_move_iterator(inner.begin()),
                             std::make_move_iterator(inner.end()));
            }

            files.push_back(std::move(path));
        }

        return files;
    }

    void begin_tests(const fs::path &path)
    {
        for (const fs::path &file : files_from(path))
        {
            std::cout << utils::Colors::Green << "[TESTING] "
                      << utils::Styles::Reset << file.string() << "...\n";

            Core::ProgramFile program(file.string());
            Core::ProgramRegistry registry(program);

            program.execute();

            std::cout << utils::Colors::Green << "[TESTED] "
                      << utils::Styles::Reset << file.string() << "\n";
        }
    }

} // namespace utils::testing
