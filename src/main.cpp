#include <iostream>

#include "core/program/Program.hpp"

void execute_file(const char *file)
{
    Core::ProgramFile program(file);

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
