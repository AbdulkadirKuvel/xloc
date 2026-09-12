#include <scanner.hpp>
#include <string>
#include <print>
#include <unordered_set>
namespace scanner
{
    namespace internal
    {
        bool in_whitelist(const fs::path &path, const std::unordered_set<std::string> &whitelist)
        {
            const std::string ext = path.extension().string();
            if (!ext.empty() && whitelist.contains(ext))
                return true;

            const std::string filename = path.filename().string();
            return whitelist.contains(filename);
        }

        bool in_blacklist(const fs::path &path, const std::unordered_set<std::string> &blacklist)
        {
            const std::string filename = path.filename().string();
            const std::string ext = path.extension().string();

            return blacklist.contains(filename) || (!ext.empty() && blacklist.contains(ext));
        }
    }
    std::vector<fs::path> scan(types::Config config)
    {
        if (config.single_file)
        {
            std::vector<fs::path> paths;
            paths.push_back(config.path);
            return paths;
        }
        else if (config.recursive)
        {
            return list_files_recursive(config.path, config);
        }
        return list_files(config.path, config);
    }

    std::vector<fs::path> list_files(const fs::path &path, const types::Config &config)
    {
        std::vector<fs::path> paths;
        std::error_code ec;

        auto options = fs::directory_options::skip_permission_denied;
        auto it = fs::directory_iterator(path, options, ec);

        if (ec)
            return paths;

        auto endit = fs::end(it);

        while (it != endit)
        {
            std::error_code entry_ec;
            const auto &entry = *it;

            if (entry.is_symlink(entry_ec))
            {
                it.increment(ec);
                if (ec)
                    ec.clear();

                continue;
            }

            const bool is_reg = entry.is_regular_file(entry_ec);

            if (!entry_ec && is_reg)
            {
                const auto &current_path = entry.path();
                const auto ext = current_path.extension();
                const auto filename = current_path.filename();

                if (it->is_regular_file(ec))
                {
                    if (!internal::in_blacklist(filename, config.blacklist) &&
                        (internal::in_whitelist(ext, config.whitelist) ||
                         internal::in_whitelist(filename, config.whitelist)))
                    {
                        paths.push_back(current_path);
                    }
                }
            }

            it.increment(ec);

            if (ec)
                ec.clear();
        }
        return paths;
    }

    std::vector<fs::path> list_files_recursive(const fs::path &root, const types::Config &config)
    {
        std::vector<fs::path> paths;
        std::vector<fs::path> directories_to_scan;
        directories_to_scan.push_back(root);

        while (!directories_to_scan.empty())
        {
            fs::path current_dir = directories_to_scan.back();
            directories_to_scan.pop_back();

            std::error_code ec;

            auto options = fs::directory_options::skip_permission_denied;
            auto it = fs::directory_iterator(current_dir, options, ec);

            if (ec)
                continue;

            const auto endit = fs::end(it);

            while (it != endit && !ec)
            {
                std::error_code entry_ec;
                const auto &entry = *it;

                if (entry.is_symlink(entry_ec) || entry_ec)
                {
                    it.increment(ec);
                    if (ec)
                        ec.clear();
                    continue;
                }

                bool is_dir = entry.is_directory(entry_ec);
                if (!entry_ec && is_dir)
                {
                    const auto &dir_path = entry.path();
                    if (!internal::in_blacklist(dir_path, config.blacklist))
                    {
                        directories_to_scan.push_back(dir_path);
                    }
                }
                else if (!entry_ec && entry.is_regular_file(entry_ec))
                {
                    const auto &file_path = entry.path();
                    const auto &ext = file_path.extension();
                    if (!internal::in_blacklist(file_path, config.blacklist) &&
                        (internal::in_whitelist(file_path, config.whitelist) || 
                         internal::in_whitelist(ext, config.whitelist)))
                    {
                        paths.push_back(file_path);
                    }
                }

                it.increment(ec);
                if (ec)
                    ec.clear();
            }
        }
        return paths;
    }
}