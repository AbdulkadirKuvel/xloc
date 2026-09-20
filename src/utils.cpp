#include <utils.hpp>
#include <string_view>
#include <fstream>

#include <cstdlib>

#if defined(_WIN32)
#include <io.h>
#define ISATTY _isatty
#define FILENO _fileno
#else
#include <unistd.h>
#define ISATTY isatty
#define FILENO fileno
#endif

namespace xloc::utility
{
    bool supports_color() noexcept
    {
        static const bool color_enabled = []()
        {
            const bool is_terminal = ISATTY(FILENO(stdout)) != 0;
            if (!is_terminal)
                return false;

            const char *no_color = std::getenv("NO_COLOR");
            const bool no_color_set = (no_color != nullptr && !std::string_view(no_color).empty());

            return !no_color_set;
        }();

        return color_enabled;
    }
}