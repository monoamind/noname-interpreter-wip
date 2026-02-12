#pragma once
#include <charconv>
#include <format>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

namespace Core
{
// ---------------------------------------------------------------------------------------------------------------------

template <class T>
using Ptr = std::shared_ptr<T>;

template <class T>
using PtrVector = std::vector<Ptr<T>>;

template <class T>
using Vector = std::vector<T>;

// ---------------------------------------------------------------------------------------------------------------------
} // namespace Core