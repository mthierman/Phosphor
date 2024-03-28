#include <glow/glow.hxx>
#include "phosphor.hxx"

auto main() -> int
{
    glow::App app;

    Phosphor phosphor;
    phosphor.create_window();

    return app();
}
