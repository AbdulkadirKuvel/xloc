#include <MemoryMappedFile.hpp>

#include <iostream>
#include <stdexcept>
#include <system_error>
#include <utility>

#if defined(_WIN32)
#include <windows.h>
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#else
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#endif

namespace xloc::io
{
    struct MemoryMappedFile::PlatformNativeData
    {
#if defined(_WIN32)
        HANDLE file_handle = INVALID_HANDLE_VALUE;
        HANDLE mapping_handle = nullptr;
#else
        int fd = -1;
#endif
    };

    MemoryMappedFile::MemoryMappedFile(const fs::path &filepath) : filepath_(filepath), pimpl_(std::make_unique<PlatformNativeData>())
    {
        map_file();
    }

    MemoryMappedFile::~MemoryMappedFile() noexcept
    {
        unmap_file();
    }

    MemoryMappedFile::MemoryMappedFile(MemoryMappedFile &&other) noexcept
        : filepath_(std::move(other.filepath_)),
          mapped_data_(std::exchange(other.mapped_data_, nullptr)),
          size_(std::exchange(other.size_, 0)),
          pimpl_(std::move(other.pimpl_))
    {
    }

    MemoryMappedFile &MemoryMappedFile::operator=(MemoryMappedFile &&other) noexcept
    {
        if (this != &other)
        {
            unmap_file();
            filepath_ = std::move(other.filepath_);
            mapped_data_ = std::exchange(other.mapped_data_, nullptr);
            size_ = std::exchange(other.size_, 0);
            pimpl_ = std::move(other.pimpl_);
        }

        return *this;
    }

    std::string_view MemoryMappedFile::data() const noexcept
    {
        if (!mapped_data_ || size_ == 0)
            return {};
        return {static_cast<const char *>(mapped_data_), size_};
    }

    std::size_t MemoryMappedFile::size() const noexcept
    {
        return size_;
    }

    bool MemoryMappedFile::empty() const noexcept
    {
        return size_ == 0;
    }

#if defined(_WIN32)

    void MemoryMappedFile::map_file()
    {
        pimpl_->file_handle = CreateFileW(
            filepath_.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

        if (pimpl_->file_handle == INVALID_HANDLE_VALUE)
        {
            throw std::system_error(GetLastError(), std::system_category(), std::format("Failed to open file: {}", filepath_.string()));
        }

        LARGE_INTEGER file_size;
        if (!GetFileSizeEx(pimpl_->file_handle, &file_size))
        {
            unmap_file();
            throw std::system_error(GetLastError(), std::system_category(), std::format("Failed to get file size: {}", filepath_.string()));
        }
        size_ = static_cast<std::size_t>(file_size.QuadPart);

        if (size_ == 0)
            return;

        pimpl_->mapping_handle = CreateFileMappingW(pimpl_->file_handle, nullptr, PAGE_READONLY, 0, 0, nullptr);
        if (!pimpl_->mapping_handle)
        {
            unmap_file();
            throw std::system_error(GetLastError(), std::system_category(), std::format("Failed to create file mapping: {}", filepath_.string()));
        }

        mapped_data_ = MapViewOfFile(pimpl_->mapping_handle, FILE_MAP_READ, 0, 0, 0);
        if (!mapped_data_)
        {
            unmap_file();
            throw std::system_error(GetLastError(), std::system_category(), std::format("Failed to map view of file: {}", filepath_.string()));
        }
    }

    void MemoryMappedFile::unmap_file() noexcept
    {
        if (mapped_data_)
        {
            UnmapViewOfFile(mapped_data_);
            mapped_data_ = nullptr;
        }
        if (pimpl_ && pimpl_->mapping_handle)
        {
            CloseHandle(pimpl_->mapping_handle);
            pimpl_->mapping_handle = nullptr;
        }
        if (pimpl_ && pimpl_->file_handle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(pimpl_->file_handle);
            pimpl_->file_handle = INVALID_HANDLE_VALUE;
        }
    }

#else
    void MemoryMappedFile::map_file()
    {
        pimpl_->fd = open(filepath_.c_str(), O_RDONLY);
        if (pimpl_->fd == -1)
        {
            throw std::system_error(errno, std::generic_category(), std::format("Failed to open file: {}", filepath_.string()));
        }

        struct stat sb;
        if (fstat(pimpl_->fd, &sb) == -1)
        {
            unmap_file();
            throw std::system_error(errno, std::generic_category(), std::format("Failed to get file size: {}", filepath_.string()));
        }

        size_ = static_cast<std::size_t>(sb.st_size);

        if (size_ == 0)
            return;

        mapped_data_ = mmap(nullptr, size_, PROT_READ, MAP_PRIVATE, pimpl_->fd, 0);

        if (mapped_data_ == MAP_FAILED)
        {
            mapped_data_ = nullptr;
            unmap_file();
            throw std::system_error(errno, std::generic_category(), std::format("Failed to map memory: {}", filepath_.string()));
        }
    }

    void MemoryMappedFile::unmap_file() noexcept
    {
        if (mapped_data_)
        {
            munmap(mapped_data_, size_);
            mapped_data_ = nullptr;
        }
        if (pimpl_ && pimpl_->fd != -1)
        {
            close(pimpl_->fd);
            pimpl_->fd = -1;
        }
    }

#endif
}