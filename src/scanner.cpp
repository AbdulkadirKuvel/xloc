#include <scanner.hpp>
#include <string>
#include <print>
#include <unordered_set>
#include <formatter.hpp>
namespace xloc::scanner
{
    namespace internal
    {
        [[nodiscard]] inline bool in_list(const std::string &item, const std::unordered_set<std::string> &list) noexcept
        {
            return list.find(item) != list.end();
        }
    }
    
    std::vector<fs::path> scan(xloc::types::Config config)
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

    std::vector<fs::path> list_files(const fs::path &path, const xloc::types::Config &config)
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
                const auto ext = current_path.extension().string();
                const auto filename = current_path.filename().string();

                if (it->is_regular_file(ec))
                {
                    if (!internal::in_list(filename, config.blacklist) &&
                        (internal::in_list(ext, config.whitelist) ||
                         internal::in_list(filename, config.whitelist)))
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

    inline bool should_ignore_entry(const fs::path &path, const xloc::types::Config &config)
    {
        const std::string filename = path.filename().string();

        if (filename.empty())
            return false;

        if (filename.front() == '.' ||
            filename == "$RECYCLE.BIN" ||
            filename == "System Volume Information")
        {
            return true;
        }

        return internal::in_list(filename, config.blacklist);
    }

    std::vector<fs::path> list_files_recursive(const fs::path &root, const xloc::types::Config &config)
    {
        std::vector<fs::path> paths;
        std::error_code ec;

        constexpr auto options = fs::directory_options::skip_permission_denied;
        fs::recursive_directory_iterator it(root, options, ec);
        fs::recursive_directory_iterator end;

        if (ec)
        {
            xloc::fmt::print_warning(
                xloc::types::Error{
                    .title = "Root Inaccessable",
                    .message = "Could not access the root folder."},
                config);

            return paths;
        }

        while (it != end)
        {
            const auto &entry = *it;
            const auto &path = entry.path();

            if (should_ignore_entry(path, config))
            {
                if (entry.is_directory())
                {
                    it.disable_recursion_pending();
                }

                it.increment(ec);
                if (ec)
                    ec.clear();
                continue;
            }

            if (entry.is_regular_file(ec))
            {
                const auto &filename = path.filename().string();
                const auto &ext = path.extension().string();

                if ((internal::in_list(filename, config.whitelist) ||
                     internal::in_list(ext, config.whitelist)))
                {
                    paths.push_back(path);
                }
            }

            it.increment(ec);
            if (ec)
            {
                xloc::fmt::print_warning(
                    xloc::types::Error{
                        .title = "Filesystem Iteration Warning",
                        .message = "Error reading entry. Possibility: ERROR_SHARING_VIOLATION or broken symlink."},
                    config);

                ec.clear();
            }
        }
        return paths;
    }
}