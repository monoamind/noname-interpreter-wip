#pragma once
#include <memory>
#include <string>
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