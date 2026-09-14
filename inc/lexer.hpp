#pragma once
#ifndef LEXER_HPP
#define LEXER_HPP

#include <filesystem>
#include <types.hpp>
#include <utils.hpp>

namespace lexer
{
    enum DFA_Machine
    {
        IN_CODE,          // start
        IN_SINGLE_STRING, // one character string (')
        IN_DOUBLE_STRING, // two characters string (")
        IN_MULTI_COMMENT, // in multiline comment (/*) or (""")
    };

    struct LineMetrics {
        size_t total = 0;
        size_t blank = 0;
    };

    
    [[nodiscard]] inline LineMetrics analyze_lines(std::string_view);
    
    void file_analyzer_c(std::string_view, types::FileStats &);
    void file_analyzer_py(std::string_view, types::FileStats &);
    void file_analyzer_xml(std::string_view, types::FileStats &);
}
#endif