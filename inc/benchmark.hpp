#pragma once

#include <chrono>
#include <concepts>
#include <format>
#include <functional>
#include <string_view>
#include <utility>
#include <formatter.hpp>

namespace benchmark
{

    template <typename Func, typename... Args>
    decltype(auto) measure_step(std::string_view step_name, Func &&func, Args &&...args)
    {
        const auto start = std::chrono::high_resolution_clock::now();

        using ResultType = std::invoke_result_t<Func, Args...>;

        if constexpr (std::is_void_v<ResultType>)
        {
            std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
            const auto end = std::chrono::high_resolution_clock::now();

            std::chrono::duration<double, std::milli> duration = end - start;

            formatter::print_info(std::format("Completed {}. Time : {:.3f}ms", step_name, duration.count()));
        }
        else
        {
            decltype(auto) result = std::invoke(std::forward<Func>(func), std::forward<Args>(args)...);
            const auto end = std::chrono::high_resolution_clock::now();
            const std::chrono::duration<double, std::milli> duration = end - start;

            formatter::print_info(std::format("Completed {}. Time: {:.2f}ms", step_name, duration.count()));
            return result;
        }
    }

}