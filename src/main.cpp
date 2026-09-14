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
    const auto &config = parser::parse_commands(argc, argv); 

    if (config.help_requested)
    {
        formatter::print_help();
        return 0;
    }
    else if (config.version_requested)
    {
        formatter::print_version(config);
        return 0;
    }
    else if (config.error_requested)
    {
        formatter::print_error(config.error_info, config);
        return 0;
    }

    // clang-format off
    const auto paths = benchmark::measure_step("scanning files", config, [&]
    { 
        return scanner::scan(config); 
    });
    
    const auto stats = benchmark::measure_step("analyzing files", config, [&]
    {
        return collector::gather_files_stats(paths);
    });
    // clang-format on

    formatter::report_files_stats(stats, config);

    return 0;
}
