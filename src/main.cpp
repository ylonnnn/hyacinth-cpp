#include <filesystem>
#include <iostream>
#include <string>

#include "program/Program.hpp"

void run_tests(const char *path)
{
    namespace fs = std::filesystem;

    fs::path f_path = path;

    if (!fs::exists(f_path) || !fs::is_directory(f_path))
        return;

    for (const auto &entry : fs::directory_iterator(f_path))
        if (fs::is_regular_file(entry.status()))
        {
            Program::ProgramFile program(entry.path().c_str());

            std::cout << std::string(16, '=') << " TEST "
                      << std::string(16, '=') << "\n";
            std::cout << "For: " << entry.path() << "\n";
            program.execute();
            std::cout << std::string(16, '=') << " TEST "
                      << std::string(16, '=') << "\n";
        }
        else if (fs::is_directory(entry.status()))
            run_tests(entry.path().c_str());
}

void run_specific_tests() { run_tests("hyc-examples/tests/variables/"); }

int main()
{
    run_specific_tests();

    return 0;
}
