#pragma once

#include <Windows.h>
#include <ShObjIdl.h>
#include <filesystem>
#include <utility>
#include <wil/com.h>
#include <glow/glow.hxx>

namespace Phosphor
{
struct Window final : public glow::Window
{
    auto WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT override;
    auto on_create(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_setting_change(::WPARAM wParam, ::LPARAM lParam) -> int;

  private:
    wil::com_ptr_t<IDesktopWallpaper> m_wallpaper;
    bool m_mode;
};
} // namespace Phosphor
