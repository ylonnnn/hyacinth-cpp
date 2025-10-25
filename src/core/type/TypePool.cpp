#include "core/type/TypePool.hpp"
#include <unordered_map>

namespace Core::TypePool
{
    struct type_hasher
    {
        size_t operator()(const InstantiatedType &type) const
        {
            size_t *hash = type.hash();

            if (hash == nullptr) [[unlikely]]
                return SIZE_MAX;

            return *hash;
        }
    };

    struct type_eq
    {
        bool operator()(const InstantiatedType &a,
                        const InstantiatedType &b) const
        {
            std::string *a_str = a.to_string(), *b_str = b.to_string();

            if (a_str == nullptr || b_str == nullptr) [[unlikely]]
                return false;

            return *a_str == *b_str;
        }
    };

    static std::unordered_map<std::string, std::unique_ptr<InstantiatedType>>
        TYPE_POOL;

    InstantiatedType *add(std::unique_ptr<InstantiatedType> &&type)
    {
        auto [it, _] =
            TYPE_POOL.try_emplace(type->to_string(), std::move(type));
        return it->second.get();
    }

} // namespace Core::TypePool
