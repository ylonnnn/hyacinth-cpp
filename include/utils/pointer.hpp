#pragma once

#include <memory>
#include <type_traits>

namespace utils
{
    template <typename T, typename B>
    std::enable_if_t<std::is_base_of_v<B, T>, std::unique_ptr<T>>
    dynamic_ptr_cast(std::unique_ptr<B> &base)
    {
        if (base == nullptr)
            return nullptr;

        auto ptr = dynamic_cast<T *>(base.get());
        if (ptr == nullptr)
            return nullptr;

        auto n_ptr = std::unique_ptr<T>(ptr);

        auto _ = base.release();
        (void)_;

        return n_ptr;
    }

} // namespace utils
