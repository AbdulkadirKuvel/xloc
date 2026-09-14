#include <parser.hpp>
#include <utils.hpp>
#include <config.hpp>
#include <print>
#include <format>
#include <ranges>

namespace parser
{
    types::Config parse_commands(int argc, char *argv[])
    {
        types::Config config;

        for (int i = 1; i < argc; ++i)
        {
            std::string_view arg = argv[i];
            if (arg == "-h" || arg == "--help")
            {
                config.help_requested = true;
                return config;
            }
            else if (arg == "-v" || arg == "--version")
            {
                config.version_requested = true;
                return config;
            }
            else if (arg == "-c" || arg == "--config")
            {
                if (i + 1 >= argc || argv[i + 1][0] == '-')
                {
                    config.error_requested = true;
                    config.error_info.title = "Missing File Path Error";
                    config.error_info.message = "-c/--config flag requires a file path.";
                    return config;
                }

                fs::path config_path = argv[i + 1];
                config_reader::apply_json_config(config_path, config);

                if (config.error_requested)
                    return config;

                break;
            }
        }

        for (int i = 1; i < argc; ++i)
        {
            std::string_view arg = argv[i];

            if (arg == "-r" || arg == "--recursive")
                config.recursive = true;

            else if (arg == "-j" || arg == "--json")
                config.json_form = true;
            
            else if (arg == "-m" || arg == "--markdown")
                config.md_form = true;
            
            else if (arg == "-q" || arg == "--quiet")
                config.quiet = true; // TODO: This feat has not been implemented.
            
            else if (arg == "-c" || arg == "--config")
            {
                i++;
                continue;
            }
            else if (arg == "-o" || arg == "--output")
            {
                if (i + 1 >= argc || argv[i + 1][0] == '-')
                {
                    config.error_requested = true;
                    config.error_info.title = "Missing File Path Error";
                    config.error_info.message = "-o/--output flag requires a file path.";
                    return config;
                }

                config.output_path = argv[++i];
            }
            else if (!arg.starts_with('-'))
            {
                fs::path target(arg);
                if (fs::is_regular_file(target))
                {
                    // This is just one file
                    config.single_file = true;
                }
                else if (!fs::is_directory(target))
                {
                    config.error_requested = true;
                    config.error_info.title = "Not a Directory Error";
                    config.error_info.message = std::format("Given path '{}' is not a directory.", target.generic_string());
                    return config;
                }
                config.path = target;
            }
        }

        config.enable_color = utils::supports_color();
        return config;
    }
}
