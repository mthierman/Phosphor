#pragma once

#include <Windows.h>
#include <ShObjIdl.h>
#include <wil/com.h>
#include <glow/glow.hxx>

struct Phosphor final : public glow::Window
{
    auto WndProc(::UINT uMsg, ::WPARAM wParam, ::LPARAM lParam) -> ::LRESULT override;
    auto on_create(::WPARAM wParam, ::LPARAM lParam) -> int;
    auto on_setting_change(::WPARAM wParam, ::LPARAM lParam) -> int;

    auto check(::HRESULT hr) -> void;

    glow::CoInitialize m_coInit;
    wil::com_ptr_t<IDesktopWallpaper> m_wallpaper;
    bool m_isDark;
};
