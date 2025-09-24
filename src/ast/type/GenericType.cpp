#include "ast/type/GenericType.hpp"
#include "utils/style.hpp"

namespace AST
{
    GenericType::GenericType(std::unique_ptr<Type> constructor,
                             std::vector<std::unique_ptr<Type>> arguments)
        : Type(constructor->value()), constructor_(std::move(constructor)),
          arguments_(std::move(arguments))
    {
        if (!arguments_.empty())
            set_end_position(arguments_.back()->end_position());
    }

    Type &GenericType::constructor() { return *constructor_; }

    std::vector<std::unique_ptr<Type>> &GenericType::arguments()
    {
        return arguments_;
    }

    std::string GenericType::to_string() const
    {
        std::string str(value_.value);

        str += "<";

        for (auto &argument : arguments_)
            str += argument->to_string() +
                   (arguments_.back() == argument ? "" : ", ");

        return str += ">";
    }

    void GenericType::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = utils::tab(tab - 1, 4),
                    inner_indentation = utils::tab(tab, 4);
        os << "GenericType {";

        os << "\n" << inner_indentation << "constructor: ";
        constructor_->print(os, tab + 1);

        os << "\n" << inner_indentation << "arguments:  {\n";

        for (const auto &argument : arguments_)
        {
            std::string inner_indentation = utils::tab(tab + 1, 4);

            os << inner_indentation;
            argument->print(os, tab + 2);
            os << "\n";
        }

        os << inner_indentation << "}";

        os << "\n" << indentation << "}";
    }

    // bool GenericType::operator==(const Type &other) const
    // {
    //     if (typeid(other) != typeid(GenericType))
    //         return false;

    //     const GenericType &generic = static_cast<const GenericType &>(other);
    //     auto generic_size = generic.arguments_.size();

    //     if (!(*constructor_ == *generic.constructor_) ||
    //         arguments_.size() != generic_size)
    //         return false;

    //     for (size_t i = 0; i < generic_size; i++)
    //     {
    //         auto &arg = generic.arguments_[i], &c_arg = arguments_[i];

    //         if (*arg == *c_arg)
    //             continue;

    //         return false;
    //     }

    //     return true;
    // }

} // namespace AST
