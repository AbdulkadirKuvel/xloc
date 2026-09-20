#pragma once
#ifndef COLLECTOR_HPP
#define COLLECTOR_HPP

#include <filesystem>
#include <types.hpp>
#include <unordered_set>
#include <vector>
#include <map>
#include <span>

namespace fs = std::filesystem;

const std::unordered_set<std::string> c_style = {
    ".c",
    ".cpp",
    ".cxx",
    ".cc",
    ".h",
    ".hpp",
    ".cs",
    ".java",
    ".js",
    ".jsx",
    ".ts",
    ".tsx",
    ".go",
    ".rs",
    ".swift",
    ".kt",
};

const std::unordered_set<std::string> py_style = {
    ".py", ".pyx", ".pyw"};

const std::unordered_set<std::string> asm_style = {
    ".asm", ".s", ".lisp"};

const std::unordered_set<std::string> sql_style = {
    ".lua", ".sql"};

const std::unordered_set<std::string> shell_style = {
    "makefile", ".yml", ".sh"};

const std::unordered_set<std::string> xml_style = {
    ".xml", ".html", ".svg"};

const std::string ruby = ".rb";

namespace xloc::analysis
{
    using router = void (*)(std::string_view, xloc::types::FileStats &);

    router get_analyzer(const std::string);

    [[nodiscard]] std::map<std::string, xloc::types::FileStats> gather_files_stats(std::span<const fs::path>, const xloc::types::Config&);

}

#endif