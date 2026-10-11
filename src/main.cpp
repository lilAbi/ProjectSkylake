#include "main.hpp"
#include "util/engine.hpp"

Main::Main() {
}

void Main::_bind_methods() {
}

void Main::_ready() {
}

void Main::_physics_process(double delta) {
    if (core::Engine::is_editor_active())
        return;
}
