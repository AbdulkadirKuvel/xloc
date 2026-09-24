#pragma once
#ifndef COLLECTOR_HPP
#define COLLECTOR_HPP

#include <filesystem>
#include <types.hpp>
#include <lexer.hpp>
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

    router get_analyzer(std::string_view) noexcept;

    [[nodiscard]] std::map<std::string, xloc::types::FileStats> gather_files_stats(std::span<const fs::path>, const xloc::types::Config &);

    static const std::unordered_map<std::string_view, router> analyzers = {
        {".c", xloc::lexer::file_analyzer_c},
        {".cpp", xloc::lexer::file_analyzer_c},
        {".cxx", xloc::lexer::file_analyzer_c},
        {".tcc", xloc::lexer::file_analyzer_c},
        {".txx", xloc::lexer::file_analyzer_c},
        {".tpp", xloc::lexer::file_analyzer_c},
        {".cc", xloc::lexer::file_analyzer_c},
        {".h", xloc::lexer::file_analyzer_c},
        {".hpp", xloc::lexer::file_analyzer_c},
        {".cs", xloc::lexer::file_analyzer_c},
        {".java", xloc::lexer::file_analyzer_c},
        {".js", xloc::lexer::file_analyzer_c},
        {".jsx", xloc::lexer::file_analyzer_c},
        {".ts", xloc::lexer::file_analyzer_c},
        {".tsx", xloc::lexer::file_analyzer_c},
        {".go", xloc::lexer::file_analyzer_c},
        {".rs", xloc::lexer::file_analyzer_c},
        {".swift", xloc::lexer::file_analyzer_c},
        {".kt", xloc::lexer::file_analyzer_c},

        {".py", xloc::lexer::file_analyzer_py},
        {".pyx", xloc::lexer::file_analyzer_py},
        {".pyw", xloc::lexer::file_analyzer_py},

        {".xml", xloc::lexer::file_analyzer_xml},
        {".svg", xloc::lexer::file_analyzer_xml},
        {".html", xloc::lexer::file_analyzer_xml},
    };
}

#endif