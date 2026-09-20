#pragma once
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <types.hpp>
#include <filesystem>
#include <vector>
#include <string>
#include <optional>

namespace fs = std::filesystem;

namespace xloc::config
{
    struct JsonDTO
    {
        std::optional<bool> recursive;
        std::vector<std::string> blacklist;
        std::vector<std::string> whitelist;
    };

    void apply_json_config(const fs::path &, xloc::types::Config &);
}

#endif