#include <IReportFormatter.hpp>
#include <JsonFormatter.hpp>
#include <MarkdownFormatter.hpp>
#include <TableFormatter.hpp>
#include <formatter.hpp>
#include <iostream>
#include <lexer.hpp>
#include <print>
#include <string_view>
#include <fstream>
#include <map>

namespace formatter
{
    namespace
    {
        /**
         * @brief Returns either ANSI escape sequence or empty string depending on the enable_color variable
         * @return ansi_code | ""
         */
        [[nodiscard]] constexpr std::string_view color_code(std::string_view ansi_code, bool enable_color) noexcept
        {
            return enable_color ? ansi_code : "";
        }
    }

    void report_files_stats(const std::map<std::string, types::FileStats> &stats, const types::Config &config)
    {
        std::unique_ptr<IReportFormatter> report_formatter;

        // Output form
        if (config.json_form)
            report_formatter = std::make_unique<JsonFormatter>();
        else if (config.md_form)
            report_formatter = std::make_unique<MarkdownFormatter>();
        else
            report_formatter = std::make_unique<TableFormatter>();

        // Output point
        if (!config.output_path.empty())
        {
            std::ofstream file(config.output_path);
            if (file.is_open())
            {
                report_formatter->format(file, stats);
            }
            else
            {
                types::Error error;
                error.title = "File Error";
                error.message = "Could not create the output file.";
                // TODO
            }
            file.close();
        }
        else
        {
            report_formatter->format(std::cout, stats);
        }
    }

    void print_help()
    {
        std::println("┌───────────────────────────────────────────────────────────────────┐");
        std::println("│                     xloc  <path> [<commands>]                     │");
        std::println("├────────────────────┬──────────────┬───────────────────────────────┤");
        std::println("│      commands      │   argument   │          explanation          │");
        std::println("├────┬───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -h │ --help        │ none         │ show help                     │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -v │ --version     │ none         │ print current version         │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -n │ --no-color    │ none         │ do not show color for output  │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -r │ --recursive   │ none         │ recursive search              │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -j │ --json        │ none         │ format stats as json          │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -m │ --markdown    │ none         │ format stats as markdown      │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -c │ --config      │ <filepath>   │ use config file (json)        │");
        std::println("├────┼───────────────┼──────────────┼───────────────────────────────┤");
        std::println("│ -o │ --output      │ <outputpath> │ log into a file               │");
        std::println("└────┴───────────────┴──────────────┴───────────────────────────────┘");
    }

    void print_version(const types::Config &config)
    {
        const auto yellow = color_code(color::bold_yellow, config.enable_color);
        const auto white = color_code(color::bold_white, config.enable_color);
        const auto reset = color_code(color::reset, config.enable_color);

        std::println("{}xloc{} version: {}{}{}", yellow, reset, white, types::version, reset);
    }

    void print_error(types::Error error, const types::Config &config)
    {
        const auto bold_red = color_code(color::bold_red, config.enable_color);
        const auto red = color_code(color::red, config.enable_color);
        const auto reset = color_code(color::reset, config.enable_color);

        std::println("{}{}{}", bold_red, error.title, reset);
        std::println("{}{}{}", red, error.message, reset);
    }

    void print_warning(types::Error error, const types::Config &config)
    {
        const auto bold_yellow = color_code(color::bold_white, config.enable_color);
        const auto yellow = color_code(color::yellow, config.enable_color);
        const auto reset = color_code(color::reset, config.enable_color);

        std::println("{}{}{}", bold_yellow, error.title, reset);
        std::println("{}{}{}", yellow, error.message, reset);
    }

    void print_info(std::string message, const types::Config &config)
    {
        if (config.quiet)
        {
            return;
        }
        const auto blue = color_code(color::blue, config.enable_color);
        const auto reset = color_code(color::reset, config.enable_color);

        std::println("{}{}{}", blue, message, reset);
    }
}
