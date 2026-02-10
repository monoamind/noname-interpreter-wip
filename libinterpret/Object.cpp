#include "Object.h"
#include "Error.h"

using namespace Core;

// ---------------------------------------------------------------------------------------------------------------------

ObjectType Object::Type() const
{
    return type_;
}

std::string Object::TypeName() const
{
    using enum ObjectType;

    switch (type_)
    {
        case Bool:   return "Bool";
        case Double: return "Number";
        case String: return "String";
        default:     return "Null";
    }
}

bool Object::IsNull() const noexcept
{
    return type_ == ObjectType::Null;
}

bool Object::IsNumeric() const noexcept
{
    return (type_ == ObjectType::Double || type_ == ObjectType::Bool);
}

bool Object::IsString() const noexcept
{
    return type_ == ObjectType::String;
}

double Object::ToDouble() const
{
    switch (type_)
    {
        case ObjectType::Bool:
            return std::get<bool>(value_);
        case ObjectType::Double:
            return std::get<double>(value_);
        default:
            break;
    }

    throw TypeError("Unsupported type.");
}

bool Object::ToBool() const
{
    switch (type_)
    {
        case ObjectType::Bool:
            return std::get<bool>(value_);
        case ObjectType::Double:
            return std::get<double>(value_);
        case ObjectType::String:
            return std::get<std::string>(value_).empty();
        default:
            return false;
    }
}

std::string Object::ToString() const
{
    switch (type_)
    {
        case ObjectType::Bool:
            return ToBool() ? "True" : "False";
        case ObjectType::Double:
            return std::to_string(ToDouble());
        case ObjectType::String:
            return std::get<std::string>(value_);
        default:
            return "Null";
    }
}

// ---------------------------------------------------------------------------------------------------------------------

Object Core::operator-(const Object& rhs)
{
    using enum ObjectType;

    switch (rhs.Type())
    {
        case Bool:
            return -rhs.ToBool();
        case Double:
            return -rhs.ToDouble();
        default:
            break;
    }

    throw TypeError("Invalid operand type.");
}

bool Core::operator!(const Object& rhs)
{
    using enum ObjectType;

    switch (rhs.Type())
    {
        case Bool:
        case Double:
            return !rhs.ToBool();
        default:
            break;
    }

    throw TypeError("Invalid operand type.");
}

std::ostream& Core::operator<<(std::ostream& os, const Object& obj)
{
    os << obj.ToString();
    return os;
}

Object Core::operator+(const Object& lhs, const Object& rhs)
{
    if (lhs.IsString() && rhs.IsString())
    {
        return lhs.ToString() + rhs.ToString();
    }

    auto x = lhs.ToDouble();
    auto y = rhs.ToDouble();

    return x + y;
}

Object Core::operator-(const Object& lhs, const Object& rhs)
{
    auto x = lhs.ToDouble();
    auto y = rhs.ToDouble();

    return x - y;
}

Object Core::operator*(const Object& lhs, const Object& rhs)
{
    auto x = lhs.ToDouble();
    auto y = rhs.ToDouble();

    return x * y;
}

Object Core::operator/(const Object& lhs, const Object& rhs)
{
    auto x = lhs.ToDouble();
    auto y = rhs.ToDouble();

    return x / y;
}

bool Core::operator==(const Object& lhs, const Object& rhs)
{
    auto x = lhs.ToDouble();
    auto y = rhs.ToDouble();

    return x == y;
}

bool Core::operator!=(const Object& lhs, const Object& rhs)
{
    auto x = lhs.ToDouble();
    auto y = rhs.ToDouble();

    return x != y;
}

bool Core::operator>=(const Object& lhs, const Object& rhs)
{
    using enum ObjectType;

    auto x = lhs.ToDouble();
    auto y = rhs.ToDouble();

    return x >= y;
}

bool Core::operator<=(const Object& lhs, const Object& rhs)
{
    using enum ObjectType;

    auto x = lhs.ToDouble();
    auto y = rhs.ToDouble();

    return x <= y;
}

bool Core::operator>(const Object& lhs, const Object& rhs)
{
    using enum ObjectType;

    auto x = lhs.ToDouble();
    auto y = rhs.ToDouble();

    return x > y;
}

bool Core::operator<(const Object& lhs, const Object& rhs)
{
    using enum ObjectType;

    auto x = lhs.ToDouble();
    auto y = rhs.ToDouble();

    return x < y;
}

Object Core::operator&&(const Object& lhs, const Object& rhs)
{
    using enum ObjectType;

    auto x = lhs.ToDouble();
    auto y = rhs.ToDouble();

    return x && y;
}

Object Core::operator||(const Object& lhs, const Object& rhs)
{
    using enum ObjectType;

    auto x = lhs.ToDouble();
    auto y = rhs.ToDouble();

    return x || y;
}
