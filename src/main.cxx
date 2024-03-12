#include <glow/glow.hxx>
#include "window.hxx"

auto main() -> int
{
    glow::CoInitialize coInit;
    glow::App app;
    Window window;
    window.create_window();

    return app();
}
