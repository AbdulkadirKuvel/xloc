#include <lexer.hpp>
#include <string_view>
#include <cstddef>

namespace lexer
{
    namespace detail
    {
        // Boşluk karakteri kontrolü (ASCII inline optimization)
        constexpr bool is_space(char c) noexcept
        {
            return c == ' ' || c == '\t' || c == '\r';
        }
    }

    /// @brief C-Style diller (C, C++, Java, JS, C#, Go, Rust) için tek geçişli (Single-Pass) analizör.
    void file_analyzer_c(std::string_view content, types::FileStats &stats) noexcept
    {
        if (content.empty()) return;

        const char* const ptr = content.data();
        const std::size_t len = content.size();

        bool has_code = false;
        bool has_comment = false;
        
        enum class State : std::uint8_t {
            IN_CODE,
            IN_SINGLE_COMMENT,
            IN_MULTI_COMMENT,
            IN_SINGLE_STRING,
            IN_DOUBLE_STRING
        };

        State state = State::IN_CODE;

        for (std::size_t i = 0; i < len; ++i)
        {
            const char c = ptr[i];

            // Satır Sonu (Line Ending) İşleme
            if (c == '\n')
            {
                stats.total_line++;

                if (!has_code && !has_comment)
                {
                    stats.blank_line++;
                }
                else if (has_code)
                {
                    stats.code_line++;
                }
                else
                {
                    stats.comment_line++;
                }

                // Bir sonraki satır için durum sıfırlama
                has_code = false;
                
                if (state == State::IN_SINGLE_COMMENT)
                {
                    state = State::IN_CODE;
                    has_comment = false;
                }
                else if (state == State::IN_MULTI_COMMENT)
                {
                    // Çok satırlı yorum devam ediyorsa sonraki satır da yorum sayılır
                    has_comment = true;
                }
                else
                {
                    has_comment = false;
                }

                continue;
            }

            // Durum Makinesi (State Machine)
            switch (state)
            {
            case State::IN_CODE:
            {
                if (detail::is_space(c)) continue;

                if (c == '/' && (i + 1 < len))
                {
                    const char next = ptr[i + 1];
                    if (next == '/')
                    {
                        has_comment = true;
                        state = State::IN_SINGLE_COMMENT;
                        ++i; // '/' karakterini atla
                    }
                    else if (next == '*')
                    {
                        has_comment = true;
                        state = State::IN_MULTI_COMMENT;
                        ++i; // '*' karakterini atla
                    }
                    else
                    {
                        has_code = true;
                    }
                }
                else if (c == '"')
                {
                    has_code = true;
                    state = State::IN_DOUBLE_STRING;
                }
                else if (c == '\'')
                {
                    has_code = true;
                    state = State::IN_SINGLE_STRING;
                }
                else
                {
                    has_code = true;
                }
                break;
            }
            case State::IN_SINGLE_COMMENT:
                // Satır sonuna kadar tüm karakterler yutulur
                break;

            case State::IN_MULTI_COMMENT:
            {
                has_comment = true;
                if (c == '*' && (i + 1 < len) && ptr[i + 1] == '/')
                {
                    state = State::IN_CODE;
                    ++i; // '/' karakterini atla
                }
                break;
            }
            case State::IN_SINGLE_STRING:
            {
                has_code = true;
                if (c == '\\')
                {
                    ++i; // Escape karakterini yut (\' engellemesi)
                }
                else if (c == '\'')
                {
                    state = State::IN_CODE;
                }
                break;
            }
            case State::IN_DOUBLE_STRING:
            {
                has_code = true;
                if (c == '\\')
                {
                    ++i; // Escape karakterini yut (\" engellemesi)
                }
                else if (c == '"')
                {
                    state = State::IN_CODE;
                }
                break;
            }
            }
        }

        // Son satırda '\n' olmaması durumu (EOF Handling)
        if (len > 0)
        {
            stats.total_line++;
            if (!has_code && !has_comment)
            {
                stats.blank_line++;
            }
            else if (has_code)
            {
                stats.code_line++;
            }
            else
            {
                stats.comment_line++;
            }
        }
    }

    /// @brief Python dosyaları için tek geçişli analizör.
    void file_analyzer_py(std::string_view content, types::FileStats &stats) noexcept
    {
        if (content.empty()) return;

        const char* const ptr = content.data();
        const std::size_t len = content.size();

        bool has_code = false;
        bool has_comment = false;

        enum class State : std::uint8_t {
            IN_CODE,
            IN_SINGLE_COMMENT,
            IN_SINGLE_STRING,
            IN_DOUBLE_STRING
        };

        State state = State::IN_CODE;

        for (std::size_t i = 0; i < len; ++i)
        {
            const char c = ptr[i];

            if (c == '\n')
            {
                stats.total_line++;

                if (!has_code && !has_comment)
                {
                    stats.blank_line++;
                }
                else if (has_code)
                {
                    stats.code_line++;
                }
                else
                {
                    stats.comment_line++;
                }

                has_code = false;
                has_comment = false;

                if (state == State::IN_SINGLE_COMMENT)
                {
                    state = State::IN_CODE;
                }

                continue;
            }

            switch (state)
            {
            case State::IN_CODE:
            {
                if (detail::is_space(c)) continue;

                if (c == '#')
                {
                    has_comment = true;
                    state = State::IN_SINGLE_COMMENT;
                }
                else if (c == '"')
                {
                    has_code = true;
                    state = State::IN_DOUBLE_STRING;
                }
                else if (c == '\'')
                {
                    has_code = true;
                    state = State::IN_SINGLE_STRING;
                }
                else
                {
                    has_code = true;
                }
                break;
            }
            case State::IN_SINGLE_COMMENT:
                break;

            case State::IN_SINGLE_STRING:
            {
                has_code = true;
                if (c == '\\')
                {
                    ++i;
                }
                else if (c == '\'')
                {
                    state = State::IN_CODE;
                }
                break;
            }
            case State::IN_DOUBLE_STRING:
            {
                has_code = true;
                if (c == '\\')
                {
                    ++i;
                }
                else if (c == '"')
                {
                    state = State::IN_CODE;
                }
                break;
            }
            }
        }

        if (len > 0)
        {
            stats.total_line++;
            if (!has_code && !has_comment)
            {
                stats.blank_line++;
            }
            else if (has_code)
            {
                stats.code_line++;
            }
            else
            {
                stats.comment_line++;
            }
        }
    }

    /// @brief XML / HTML dosyaları için tek geçişli analizör.
    void file_analyzer_xml(std::string_view content, types::FileStats &stats) noexcept
    {
        if (content.empty()) return;

        const char* const ptr = content.data();
        const std::size_t len = content.size();

        bool has_code = false;
        bool has_comment = false;

        enum class State : std::uint8_t {
            IN_CODE,
            IN_MULTI_COMMENT,
            IN_SINGLE_STRING,
            IN_DOUBLE_STRING
        };

        State state = State::IN_CODE;

        for (std::size_t i = 0; i < len; ++i)
        {
            const char c = ptr[i];

            if (c == '\n')
            {
                stats.total_line++;

                if (!has_code && !has_comment)
                {
                    stats.blank_line++;
                }
                else if (has_code)
                {
                    stats.code_line++;
                }
                else
                {
                    stats.comment_line++;
                }

                has_code = false;
                has_comment = (state == State::IN_MULTI_COMMENT);

                continue;
            }

            switch (state)
            {
            case State::IN_CODE:
            {
                if (detail::is_space(c)) continue;

                if (c == '<' && (i + 3 < len) &&
                    ptr[i + 1] == '!' && ptr[i + 2] == '-' && ptr[i + 3] == '-')
                {
                    has_comment = true;
                    state = State::IN_MULTI_COMMENT;
                    i += 3;
                }
                else if (c == '"')
                {
                    has_code = true;
                    state = State::IN_DOUBLE_STRING;
                }
                else if (c == '\'')
                {
                    has_code = true;
                    state = State::IN_SINGLE_STRING;
                }
                else
                {
                    has_code = true;
                }
                break;
            }
            case State::IN_MULTI_COMMENT:
            {
                has_comment = true;
                if (c == '-' && (i + 2 < len) && ptr[i + 1] == '-' && ptr[i + 2] == '>')
                {
                    state = State::IN_CODE;
                    i += 2;
                }
                break;
            }
            case State::IN_SINGLE_STRING:
            {
                has_code = true;
                if (c == '\'') state = State::IN_CODE;
                break;
            }
            case State::IN_DOUBLE_STRING:
            {
                has_code = true;
                if (c == '"') state = State::IN_CODE;
                break;
            }
            }
        }

        if (len > 0)
        {
            stats.total_line++;
            if (!has_code && !has_comment)
            {
                stats.blank_line++;
            }
            else if (has_code)
            {
                stats.code_line++;
            }
            else
            {
                stats.comment_line++;
            }
        }
    }

} // namespace lexer