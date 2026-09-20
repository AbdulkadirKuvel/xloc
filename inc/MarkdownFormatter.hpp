#pragma once

#include <IReportFormatter.hpp>
#include <ostream>
#include <print>
#include <types.hpp>

class MarkdownFormatter : public IReportFormatter
{
public:
    void format(std::ostream &os, const std::map<std::string, xloc::types::FileStats> &stats) const override
    {
        std::println(os, "| Language | Files | Lines | Code | Comment | Blank |");
        std::println(os, "| :--- | :---: | :---: | :---: | :---: | :---: |");

        xloc::types::FileStats total_stats{};

        for (const auto& [ext, stat] : stats)
        {
            total_stats.file_count += stat.file_count;
            total_stats.total_line += stat.total_line;
            total_stats.code_line += stat.code_line;
            total_stats.comment_line += stat.comment_line;
            total_stats.blank_line += stat.blank_line;

            std::println(os, "| {} | {} | {} | {} | {} | {} |",
                         ext, stat.file_count, stat.total_line, 
                         stat.code_line, stat.comment_line, stat.blank_line);
        }

        std::println(os, "| **Total** | **{}** | **{}** | **{}** | **{}** | **{}** |",
                     total_stats.file_count, total_stats.total_line,
                     total_stats.code_line, total_stats.comment_line, total_stats.blank_line);
    }
};