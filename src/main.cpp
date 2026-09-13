#include <scanner.hpp>
#include <collector.hpp>
#include <formatter.hpp>
#include <parser.hpp>
#include <IReportFormatter.hpp>
#include <TableFormatter.hpp>
#include <JsonFormatter.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <benchmark.hpp>

int main(int argc, char *argv[])
{
    // clang-format off
    const auto config = benchmark::measure_step("reading configuration", [&]
    { 
        return parser::parse_commands(argc, argv); 
    });
    // clang-format on

    if (config.help_requested)
    {
        formatter::print_help();
        return 0;
    }
    else if (config.version_requested)
    {
        formatter::print_version(types::version);
        return 0;
    }
    else if (config.error_requested)
    {
        formatter::print_error(config.error_info);
        return 0;
    }

    // clang-format off
    const auto paths = benchmark::measure_step("scanning files", [&]
    { 
        return scanner::scan(config); 
    });
    
    const auto stats = benchmark::measure_step("analyzing files", [&]
    {
        return collector::gather_files_stats(paths);
    });
    // clang-format on

    formatter::report_files_stats(stats, config);

    return 0;
}
