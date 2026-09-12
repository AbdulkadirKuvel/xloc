#pragma once

#include <IReportFormatter.hpp>
#include <ext/json.hpp>
#include <ostream>
#include <types.hpp>

class JsonFormatter : public IReportFormatter
{
public:
    void format(std::ostream &os, const std::map<std::string, types::FileStats> &stats) const override
    {
        using json = nlohmann::json;

        json root = json::object();
        json files_array = json::array();

        for (const auto& [ext, stat] : stats)
        {
            files_array.push_back({
                {"extension", ext},
                {"file_count", stat.file_count},
                {"total_lines", stat.total_line},
                {"code_lines", stat.code_line},
                {"comment_lines", stat.comment_line},
                {"blank_lines", stat.blank_line}
            });
        }

        root["files"] = std::move(files_array);

        os << root.dump(4) << '\n';
    }
};