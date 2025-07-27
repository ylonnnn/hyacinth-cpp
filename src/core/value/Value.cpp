#include "core/value/Value.hpp"
#include "ast/stmt/function/FunctionDefStmt.hpp"
#include "core/symbol/FunctionSymbol.hpp"

namespace Core
{
    std::ostream &operator<<(std::ostream &os, const Value &value)
    {
        return std::visit(
            [&](const auto &val) -> std::ostream &
            {
                using T = std::decay_t<decltype(val)>;

                if constexpr (std::is_convertible_v<T, std::string>)
                    return os << std::string(val);
                else
                    return os << std::to_string(val);
            },
            value);
    }

    null::operator std::string() const { return "null"; }

    Value *object::get(const std::string &key)
    {
        auto it = value_.find(key);
        if (it == value_.end())
            return nullptr;

        return &it->second;
    }

    void object::set(const std::string &key, Value value)
    {
        value_.insert_or_assign(key, std::move(value));
    }

    object::operator std::string() const
    {
        // TODO: object operator std::string
        return "";
    }

    callable::callable(FunctionSymbol *value) : value_(value) {}

    Value callable::call(std::vector<Value> args)
    {
        AST::FunctionDeclarationStmt *stmt = value_->node;
        if (stmt == nullptr)
            return null{};

        if (stmt->is_definition())
            if (auto ptr = static_cast<AST::FunctionDefinitionStmt *>(stmt))
            {
                // TODO: Call Function Symbol
                // ptr->body();
            }

        return null{};
    }

    callable::operator std::string() const
    {
        // TODO: callable operator std::string
        return "";
    }

} // namespace Core
