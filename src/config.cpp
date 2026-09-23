#include <config.hpp>
#include <types.hpp>
#include <ext/json.hpp>
#include <fstream>
#include <print>

using json = nlohmann::json;

namespace xloc::config
{
    NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(JsonDTO, recursive, blacklist, whitelist);
    
    /// @brief Special file that nlohmann will automatically call.
    /// @param j 
    /// @param dto 
    void from_json(const json &j, JsonDTO &dto)
    {
        if (j.contains("recursive") && j["recursive"].is_boolean())
            dto.recursive = j["recursive"].get<bool>();

        if (j.contains("blacklist") && j["blacklist"].is_array())
            dto.blacklist = j["blacklist"].get<std::vector<std::string>>();

        if (j.contains("whitelist") && j["whitelist"].is_array())
            dto.whitelist = j["whitelist"].get<std::vector<std::string>>();
    }
    
    /// @brief reads the config file and turns it into usable xloc::types::Config.
    /// @param filepath path of the config file.
    /// @param app_config xloc::types::Config
    void apply_json_config(const fs::path &filepath, xloc::types::Config &app_config)
    {
        std::ifstream file(filepath);

        if (!file.is_open())
        {
            app_config.error_requested = true;
            app_config.error_info.title = "Unable to Open File";
            app_config.error_info.message = std::format("Could not open config file: {}", filepath.string());
            return;
        }

        try
        {
            json j;
            file >> j;

            // from_json function will be automatically called here.
            JsonDTO dto = j.template get<JsonDTO>();

            if (dto.recursive.has_value())
                app_config.recursive = dto.recursive.value();

            for (const auto& item : dto.blacklist)
                app_config.blacklist.insert(item);

            for (const auto& item : dto.whitelist)
                app_config.whitelist.insert(item);
            
        }
        catch (const json::parse_error &e)
        {
            std::println("[Error] JSON parse error in config file: {}", e.what());
            exit(EXIT_FAILURE);
        }
        catch (const json::type_error &e)
        {
            std::println("[Error] JSON type error in config file: {}", e.what());
            exit(EXIT_FAILURE);
        }
    }

}