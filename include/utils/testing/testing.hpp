#pragma once

#include <filesystem>
#include <vector>

#define __TESTING__

namespace utils::testing
{
    std::vector<std::filesystem::path>
    files_from(const std::filesystem::path &path);

    void begin_tests(const std::filesystem::path &path);

} // namespace utils::testing
