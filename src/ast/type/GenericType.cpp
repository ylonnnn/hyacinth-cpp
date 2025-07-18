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
            end_pos_ = arguments_.back()->end_pos();
    }

    Type &GenericType::constructor() { return *constructor_; }

    std::vector<std::unique_ptr<Type>> &GenericType::arguments()
    {
        return arguments_;
    }

    void GenericType::set_end_pos(size_t end_pos) { end_pos_ = end_pos; }

    std::string GenericType::to_string() const
    {
        std::string str(value_.value);

        str += "[";

        for (auto &argument : arguments_)
            str += argument->to_string() +
                   (arguments_.back() == argument ? "" : ", ");

        return str += "]";
    }

    void GenericType::print(std::ostream &os, uint8_t tab) const
    {
        std::string indentation = Utils::tab(tab - 1, 4),
                    inner_indentation = Utils::tab(tab, 4);
        os << "GenericType {";

        os << "\n" << inner_indentation << "constructor: ";
        constructor_->print(os, tab + 1);

        os << "\n" << inner_indentation << "arguments:  {\n";

        for (const auto &argument : arguments_)
        {
            std::string inner_indentation = Utils::tab(tab + 1, 4);

            os << inner_indentation;
            argument->print(os, tab + 2);
            os << "\n";
        }

        os << inner_indentation << "}";

        os << "\n" << indentation << "}";
    }

} // namespace AST
