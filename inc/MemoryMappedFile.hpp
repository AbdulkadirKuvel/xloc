#pragma once

#include <string_view>
#include <filesystem>
#include <cstddef>
#include <memory>

namespace fs = std::filesystem;

namespace xloc::io
{
    class MemoryMappedFile
    {
    public:
        explicit MemoryMappedFile(const fs::path &);
        ~MemoryMappedFile() noexcept;

        MemoryMappedFile(const MemoryMappedFile &) = delete;
        MemoryMappedFile &operator=(const MemoryMappedFile &) = delete;

        MemoryMappedFile(MemoryMappedFile &&) noexcept;
        MemoryMappedFile &operator=(MemoryMappedFile &&) noexcept;

        [[nodiscard]] std::string_view data() const noexcept;
        [[nodiscard]] std::size_t size() const noexcept;
        [[nodiscard]] bool empty() const noexcept;

    private:
        void map_file();
        void unmap_file() noexcept;

        fs::path filepath_;
        void *mapped_data_ = nullptr;
        std::size_t size_ = 0;

        struct PlatformNativeData;
        std::unique_ptr<PlatformNativeData> pimpl_;
    };
}