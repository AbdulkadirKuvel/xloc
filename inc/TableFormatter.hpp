#include <IReportFormatter.hpp>
#include <print>
#include <types.hpp>

constexpr std::string_view str = "│ {:<10} │ {:>7} │ {:>9} │ {:>9} │ {:>9} │ {:>9} │";

class TableFormatter : public IReportFormatter
{
public:
    void format(std::ostream &os, const std::map<std::string, xloc::types::FileStats> &stats) const override
    {
        constexpr std::string_view upmost_border = "┌────────────┬─────────┬───────────┬───────────┬───────────┬───────────┐";
        constexpr std::string_view middle_border = "├────────────┼─────────┼───────────┼───────────┼───────────┼───────────┤";
        constexpr std::string_view bottom_border = "└────────────┴─────────┴───────────┴───────────┴───────────┴───────────┘";

        std::println(os, "{}", upmost_border);
        std::println(os, str, "Language", "Files", "Lines", "Code", "Comment", "Blank");
        std::println(os, "{}", middle_border);

        xloc::types::FileStats total{};

        for (auto &[ext, stats] : stats)
        {
            total.file_count += stats.file_count;
            total.total_line += stats.total_line;
            total.comment_line += stats.comment_line;
            total.code_line += stats.code_line;
            total.blank_line += stats.blank_line;

            std::println(os, str,
                         ext, stats.file_count, stats.total_line,
                         stats.code_line, stats.comment_line, stats.blank_line);
        }

        std::println(os, "{}", middle_border);
        std::println(os, str,
                     "Total", total.file_count, total.total_line,
                     total.code_line, total.comment_line, total.blank_line);

        std::println(os, "{}", bottom_border);
        
    }
};