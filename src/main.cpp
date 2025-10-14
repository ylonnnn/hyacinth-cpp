#ifndef __TESTING__
// For main execution
#include "core/program/Program.hpp"
#include "core/program/ProgramRegistry.hpp"
#else
// For test executions
#include "utils/testing/testing.hpp"
#endif

void execute_file(const char *file)
{
    Core::Program program(file);
    Core::ProgramRegistry registry(program);

    program.execute();
}

int main(int argc, char **argv)
{
#ifdef __TESTING__
    utils::testing::begin_tests("hyc-examples/tests/parser");
#else
    // No file specified
    // if (argc < 2)
    // {
    //     std::cerr << "Please specify the file to execute!\n";
    //     return 1;
    // }

    const char *file =
        argc < 2 ? "hyc-examples/tests/parser/petal.hyc" : argv[1];
    execute_file(file);
#endif

    return 0;
}
