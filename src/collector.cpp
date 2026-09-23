#include <collector.hpp>
#include <formatter.hpp>
#include <format>
#include <lexer.hpp>
#include <MemoryMappedFile.hpp>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <iostream>
#include <print>

namespace xloc::analysis
{
    router get_analyzer(const std::string ext)
    {
        if (c_style.contains(ext))
            return xloc::lexer::file_analyzer_c;

        if (py_style.contains(ext))
            return xloc::lexer::file_analyzer_py;

        if (xml_style.contains(ext))
            return xloc::lexer::file_analyzer_xml;

        return nullptr;
    }

    std::map<std::string, xloc::types::FileStats> gather_files_stats(std::span<const fs::path> files, const xloc::types::Config &config)
    {
        std::unordered_map<std::string, xloc::types::FileStats> gathered_stats;

        for (const auto &filepath : files)
        {
            const auto ext_path = filepath.extension();
            if (ext_path.empty())
                continue;

            std::string ext = ext_path.string();
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