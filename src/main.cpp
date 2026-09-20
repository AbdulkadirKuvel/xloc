#include <parser.hpp>
#include <formatter.hpp>
#include <benchmark.hpp>
#include <scanner.hpp>
#include <collector.hpp>

#include <filesystem>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    const auto &config = xloc::parser::parse_commands(argc, argv); 

    if (config.help_requested)
    {
        xloc::fmt::print_help();
        return 0;
    }
    else if (config.version_requested)
    {
        xloc::fmt::print_version(config);
        return 0;
    }
    else if (config.error_requested)
    {
        xloc::fmt::print_error(config.error_info, config);
        return 0;
    }

    // clang-format off
    const auto paths = xloc::benchmark::measure_step("scanning files", config, [&]
    { 
        return xloc::scanner::scan(config); 
    });
    
    const auto stats = xloc::benchmark::measure_step("analyzing files", config, [&]
    {
        return xloc::analysis::gather_files_stats(paths, config);
    });
    // clang-format on

    xloc::fmt::report_files_stats(stats, config);

    return 0;
}
