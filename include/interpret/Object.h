#pragma once
#include "ObjectType.h"

#include <string>
#include <variant>

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

class Object
{
    using Variant = std::variant<double, std::string, bool, nullptr_t>;

private:
    Variant value_ = nullptr;
    ObjectType type_ = ObjectType::Null;

public:
    Object() = default;

    Object(std::nullptr_t)
        : value_(nullptr)
    {}

    Object(bool value)
        : value_(value)
        , type_(ObjectType::Bool)
    {}

    Object(double value)
        : value_(value)
        , type_(ObjectType::Double)
    {}

    Object(const std::string& value)
        : value_(value)
        , type_(ObjectType::String)
    {}

    template <typename T>
    requires std::is_convertible_v<T, double>
    Object(T value)
        : Object(static_cast<double>(value))
    {}

    template <typename T>
    requires std::is_convertible_v<T, std::string>
    Object(const T& value)
        : Object(static_cast<std::string>(value))
    {}

public:
    ObjectType Type() const;
    std::string TypeName() const;

    bool IsNull() const noexcept;
    bool IsNumeric() const noexcept;
    bool IsString() const noexcept;

private:
    bool ToBool() const;
    double ToDouble() const;

public:
    std::string ToString() const;

    friend bool operator!(const Object& rhs);
    friend Object operator-(const Object& rhs);

    friend std::ostream& operator<<(std::ostream& os, const Object& obj);

    friend Object operator+(const Object& lhs, const Object& rhs);
    friend Object operator-(const Object& lhs, const Object& rhs);
    friend Object operator*(const Object& lhs, const Object& rhs);
    friend Object operator/(const Object& lhs, const Object& rhs);

    friend bool operator==(const Object& lhs, const Object& rhs);
    friend bool operator!=(const Object& lhs, const Object& rhs);
    friend bool operator>=(const Object& lhs, const Object& rhs);
    friend bool operator<=(const Object& lhs, const Object& rhs);

    friend bool operator<(const Object& lhs, const Object& rhs);
    friend bool operator>(const Object& lhs, const Object& rhs);

    friend Object operator&&(const Object& lhs, const Object& rhs);
    friend Object operator||(const Object& lhs, const Object& rhs);
};

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core