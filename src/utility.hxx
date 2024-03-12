#pragma once

#include <Windows.h>
#include <print>
#include <system_error>

namespace Phosphor
{
auto check(HRESULT hr) -> void;
} // namespace Phosphor
