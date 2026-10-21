#include "main.hpp"
#include "util/engine.hpp"

void Main::_bind_methods() {

}

void Main::get_godot_objects() {

}

void Main::bind_signals() {

}

void Main::_ready() {
    if (core::Engine::is_editor_active())
        return;
    m_logger->print("[Main] {} called", __FUNCTION__);
    //todo: maybe fire signal to load the title scene here
}
