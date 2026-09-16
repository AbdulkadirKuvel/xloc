#pragma once

#include <filesystem>
#include <unordered_set>

namespace fs = std::filesystem;

namespace types
{
    constexpr std::string_view version = "v1.0";
    struct Error
    {
        std::string title = "";
        std::string message = "";
    };

    struct Config
    {
        std::unordered_set<std::string> whitelist = {
            ".py", ".java", ".c", ".cpp", ".h", ".hpp", ".cs",
            ".html", ".css", ".js", ".rb", ".go", ".asm", ".v",
            "makefile", "Makefile"};

        std::unordered_set<std::string> blacklist = {
            ".git", "bin", "out", "obj", "lib", "node_modules", ".vs", "venv", ".venv", "__pycache__"};

        Error error_info;

        fs::path path = ".";
        fs::path config_path;
        fs::path output_path;

        bool enable_color = true;
        bool help_requested = false;
        bool error_requested = false;
        bool version_requested = false;
        bool single_file = false;
        bool recursive = false;
        bool json_form = false;
        bool md_form = false;
        bool table_form = true;
        bool quiet = false;
    };
    struct FileStats
    {
        size_t file_count = 0;

        size_t code_line = 0;
        size_t comment_line = 0;
        size_t blank_line = 0;
        size_t total_line = 0;
    };
}