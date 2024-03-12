#include "utility.hxx"

namespace Phosphor
{
auto check(HRESULT hr) -> void { std::println("{}", std::system_category().message(hr)); }
} // namespace Phosphor
