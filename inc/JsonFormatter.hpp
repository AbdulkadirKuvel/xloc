#include <IReportFormatter.hpp>
#include <ostream>
#include <types.hpp>

class JsonFormatter : public IReportFormatter
{
public:
    void format(std::ostream &os, const std::map<std::string, types::FileStats> &stats) const override
    {
        std::println(os, "{{");
        std::println(os, "  \"files\": [");

        size_t index = 0;
        const size_t total_items = stats.size();

        for (const auto& [ext, stat] : stats)
        {
            const bool is_last = (++index == total_items);

            std::println(os, "    {{");
            std::println(os, "    \"extension\": \"{}\",",ext);
            std::println(os, "    \"file_count\": \"{}\",",stat.file_count);
            std::println(os, "    \"total_lines\": \"{}\",",stat.total_line);
            std::println(os, "    \"code_lines\": \"{}\",",stat.code_line);
            std::println(os, "    \"comment_lines\": \"{}\",",stat.comment_line);
            std::println(os, "    \"blank_lines\": \"{}\"",stat.blank_line);
            std::println(os, "    }}{}", is_last ? "" : ",");
        }
        
        std::println(os, "  ]");
        std::println(os, "}}");
    }
};