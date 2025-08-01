#include "core/program/Program.hpp"
#include "core/program/ProgramRegistry.hpp"
#include <iostream>

void test(Core::ProgramFile &program)
{
    namespace fs = std::filesystem;

    auto &src_path = program.path();
    fs::path rel_path = "../tests/functions.hyc",
             target_path = src_path.parent_path() / rel_path;

    fs::path absolute_path = fs::absolute(target_path);
    std::cout << absolute_path.c_str() << "\n";

    std::cout << fs::weakly_canonical(target_path) << "\n";
}

void execute_file(const char *file)
{
    Core::ProgramFile program(file);
    Core::ProgramRegistry registry(program);

    program.execute();
}

int main(int argc, char **argv)
{
    // No file specified
    // if (argc < 2)
    // {
    //     std::cerr << "Please specify the file to execute!\n";
    //     return 1;
    // }

    const char *file = argc < 2 ? "hyc-examples/sample.hyc" : argv[1];

    execute_file(file);

    return 0;
}
