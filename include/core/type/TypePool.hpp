#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace Core
{
    struct InstantiatedType;

    struct TypePool
    {
        std::string &type_to_string(InstantiatedType &type) const;

        template <typename T = InstantiatedType>
        std::enable_if_t<std::is_base_of_v<InstantiatedType, T>, T *>
        add(std::unique_ptr<T> &&type)
        {
            std::string &str = type_to_string(*type);

            if (auto it = pool_.find(str); it != pool_.end())
                return static_cast<T *>(it->second.get());

            auto [it, _] = pool_.try_emplace(str, std::move(type));
            return static_cast<T *>(it->second.get());
        }

      private:
        std::unordered_map<std::string, std::unique_ptr<InstantiatedType>>
            pool_;
    };

    extern TypePool TYPE_POOL;

} // namespace Core
