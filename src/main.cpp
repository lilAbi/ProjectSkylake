#include "main.hpp"

#include "util/constants.hpp"
#include "util/conversions.hpp"
#include "util/engine.hpp"

void Main::_bind_methods() {

}

void Main::get_godot_objects() {
    m_game_application = core::gdcast<GameApplication>(this->get_node_or_null(core::labels::node::Main::GameApplication));
}

void Main::bind_signals() {

}

void Main::_ready() {
    if (core::Engine::is_editor_active()) return;
    m_logger->print("[Main] {} called", __FUNCTION__);
    //grab references to godot objects within the scene
    this->get_godot_objects();
    //bind respective signals to callback functions
    this->bind_signals();
    m_game_application->start_game();
    m_logger->print("[Main] Starting Game...", __FUNCTION__);
}
