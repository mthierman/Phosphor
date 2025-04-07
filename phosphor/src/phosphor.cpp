#include "phosphor.hpp"
#include <filesystem>
#include <glaze/glaze.hpp>
#include <pane/pane.hxx>

namespace phosphor {
auto system_dark_mode() -> bool { return pane::color(winrt::UIColorType::Background).is_dark(); }
} // namespace phosphor
