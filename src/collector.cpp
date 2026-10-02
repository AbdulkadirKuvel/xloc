#include <collector.hpp>
#include <formatter.hpp>
#include <lexer.hpp>
#include <MemoryMappedFile.hpp>
#include <algorithm>
#include <cctype>
#include <format>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <iostream>
#include <print>
#include <string>
#include <string_view>

namespace xloc::analysis
{

    constexpr std::uint64_t hash_ext64(std::string_view ext) noexcept
    {
        std::uint64_t val = 0;
        const std::size_t len = ext.size() > 8 ? 8 : ext.size();
        for (std::size_t i = 0; i < len; ++i)
            val = (val << 8) | static_cast<std::uint8_t>(ext[i]);

        return val;
    }

    inline router get_analyzer(std::string_view ext) noexcept
    {
        if (ext.empty() || ext.size() > 5)
            return nullptr;

        switch (hash_ext64(ext))
        {
        case hash_ext64(".c"):
        case hash_ext64(".cpp"):
        case hash_ext64(".tcc"):
        case hash_ext64(".txx"):
        case hash_ext64(".tpp"):
        case hash_ext64(".h"):
        case hash_ext64(".hpp"):
        case hash_ext64(".cc"):
        case hash_ext64(".cs"):
        case hash_ext64(".js"):
        case hash_ext64(".ts"):
        case hash_ext64(".go"):
        case hash_ext64(".rs"):
            return xloc::lexer::file_analyzer_c;
            
        case hash_ext64(".py"):
        case hash_ext64(".pyx"):
        case hash_ext64(".pyw"):
            return xloc::lexer::file_analyzer_py;

        case hash_ext64(".xml"):
        case hash_ext64(".html"):
        case hash_ext64(".svg"):
            return xloc::lexer::file_analyzer_xml;

        default:
            return nullptr;
        }
    }

    std::map<std::string, xloc::types::FileStats> gather_files_stats(std::span<const fs::path> files, const xloc::types::Config &config)
    {
        std::unordered_map<std::string, xloc::types::FileStats> gathered_stats;

        for (const auto &filepath : files)
        {
            auto ext_path = filepath.extension();
            if (ext_path.empty())
                ext_path = filepath.filename();

            std::string ext = ext_path.string();
            std::ranges::transform(ext, ext.begin(), [](unsigned char c)
                                   { return std::tolower(c); });
            auto lexer_function = get_analyzer(ext);

            if (!lexer_function)
                continue;

            try
            {
                xloc::io::MemoryMappedFile mmap_file(filepath);

                if (mmap_file.empty())
                {
                    gathered_stats[ext].file_count += 1;
                    continue;
                }

                xloc::types::FileStats file_stats;
                file_stats.file_count = 1;

                lexer_function(mmap_file.data(), file_stats);

                gathered_stats[ext] += file_stats;
            }
            catch (const std::system_error &error)
            {
                xloc::fmt::print_warning(
                    xloc::types::Error{
                        .title = "IO Error",
                        .message = std::format("File {} skipped due to io error.\n Code: {}", filepath.string(), error.code().value())},
                    config);
            }
            catch (...) // TODO: Maybe delete this?
            {
                xloc::fmt::print_error(
                    xloc::types::Error{
                        .title = "Lexer Error",
                        .message = std::format("Lexer exception on file: {}.", filepath.string())},
                    config);
            }
        }
        return {gathered_stats.begin(), gathered_stats.end()};
    }
}