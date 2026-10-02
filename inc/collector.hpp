#pragma once
#ifndef COLLECTOR_HPP
#define COLLECTOR_HPP

#include <filesystem>
#include <types.hpp>
#include <lexer.hpp>
#include <cstdint>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <map>
#include <span>

namespace fs = std::filesystem;

// Will implement these languages
// const std::unordered_set<std::string> asm_style = {
//     ".asm", ".s", ".lisp"};

// const std::unordered_set<std::string> sql_style = {
//     ".lua", ".sql"};

// const std::unordered_set<std::string> shell_style = {
//     "makefile", ".yml", ".sh"};

// const std::string ruby = ".rb";

namespace xloc::analysis
{
    using router = void (*)(std::string_view, xloc::types::FileStats &);

    /// @brief Turns extension string into 32-bit integer id in compile time
    /// @param `ext` the string to turn into 32-bit integer
    /// @return `val` the 32-bit integer resembling the extension.
    [[nodiscard]] constexpr std::uint64_t hash_ext64(std::string_view) noexcept;
    
    /// @brief Chooses the analyzer according to file extension
    /// @param `ext` the string resembling the file extension
    /// @return `analyzer` the analyzer for the file
    [[nodiscard]] inline router get_analyzer(std::string_view) noexcept;

    /// @brief Gathers stats for every extension
    /// @param `files` paths of the files to analyze
    /// @param `config` the configuration struct
    /// @return `gathered_stats` the stats value
    [[nodiscard]] std::map<std::string, xloc::types::FileStats> gather_files_stats(std::span<const fs::path>, const xloc::types::Config &);

} // xloc::analysis
#endif