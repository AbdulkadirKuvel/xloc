#pragma once
#include <types.hpp>
#include <string>

namespace xloc::lexer
{
    namespace detail
    {
        [[nodiscard]] constexpr bool is_space(char) noexcept;
    } // namespace detail

    void file_analyzer_c(std::string_view, xloc::types::FileStats &) noexcept;
    void file_analyzer_py(std::string_view, xloc::types::FileStats &) noexcept;
    void file_analyzer_xml(std::string_view, xloc::types::FileStats &) noexcept;

} // namespace xloc::lexer
