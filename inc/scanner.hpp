#pragma once
#ifndef SCANNER_HPP
#define SCANNER_HPP

#include <filesystem>
#include <vector>
#include <types.hpp>

namespace fs = std::filesystem;

namespace xloc::scanner
{
    namespace internal
    {
        [[nodiscard]] inline bool in_list(const std::string &, const std::unordered_set<std::string> &) noexcept;
    }

    [[nodiscard]] std::vector<fs::path> scan(const xloc::types::Config &);
    [[nodiscard]] inline bool should_ignore_entry(const fs::path &, const xloc::types::Config &);
    [[nodiscard]] std::vector<fs::path> list_files(const fs::path &, const xloc::types::Config &);
    [[nodiscard]] std::vector<fs::path> list_files_recursive(const fs::path &, const xloc::types::Config &);
}

#endif