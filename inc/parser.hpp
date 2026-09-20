#pragma once
#ifndef PARSER_HPP
#define PARSER_HPP

#include <types.hpp>
#include <formatter.hpp>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>

namespace fs = std::filesystem;

namespace xloc::parser
{
    [[nodiscard]] xloc::types::Config parse_commands(int argc, char *argv[]);
}

#endif