#include "program/Program.hpp"

int main()
{
    Program::ProgramFile program("hyc-examples/test.hyc");

    program.execute();

    return 0;
}
