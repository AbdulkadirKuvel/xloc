#pragma once
#ifndef UTILS_HPP
#define UTILS_HPP

#include <string_view>
#include <filesystem>
#include <algorithm>
#include <format>
#include <ranges>
#include <fstream>

#include <cstdlib>

#if defined(_WIN32)
#include <io.h>
#define ISATTY _isatty
#define FILENO _fileno
#else
#include <unistd.h>
#define ISATTY isatty
#define FILENO fileno
#endif

namespace fs = std::filesystem;

namespace utils
{
    [[nodiscard]] inline bool supports_color() noexcept
    {
        static const bool color_enabled = []()
        {
            const bool is_terminal = ISATTY(FILENO(stdout)) != 0;
            if (!is_terminal)
                return false;

            const char *no_color = std::getenv("NO_COLOR");
            const bool no_color_set = (no_color != nullptr && !std::string_view(no_color).empty());

            return !no_color_set;
        }();

        return color_enabled;
    }

    inline std::string_view sv_trim(std::string_view sv)
    {
        sv.remove_prefix(std::min(sv.find_first_not_of(" \t\r\n"), sv.size()));
        sv.remove_suffix(sv.size() - std::min(sv.find_last_not_of(" \t\r\n") + 1, sv.size()));

        return sv;
    }

    inline std::string_view sv_unquote(std::string_view sv)
    {
        if (sv.size() >= 2 && ((sv.front() == '"' && sv.back() == '"') ||
                               (sv.front() == '\'' && sv.back() == '\'')))
        {
            return sv.substr(1, sv.size() - 2);
        }
        return sv;
    }

    std::string read_file_into_buffer(const fs::path &, std::error_code &);
}

#endif