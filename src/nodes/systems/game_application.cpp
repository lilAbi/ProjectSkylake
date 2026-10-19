#include "game_application.h"

#include "util/constants.hpp"
#include "util/conversions.hpp"
#include "util/engine.hpp"

void GameApplication::_bind_methods() {
    // Bind Godot-facing methods, properties, and signals here.
}

void GameApplication::get_godot_objects() {
    m_scene_coordinator = core::gdcast<SceneCoordinator>(this->get_node_or_null(core::labels::node::Main::SceneCoordinator));
    m_save_controller = core::gdcast<SaveController>(this->get_node_or_null(core::labels::node::Main::SaveController));
    m_input_controller = core::gdcast<InputController>(this->get_node_or_null(core::labels::node::Main::InputController));
    m_audio_controller = core::gdcast<AudioController>(this->get_node_or_null(core::labels::node::Main::AudioController));
}

void GameApplication::bind_signals() {

}

void GameApplication::_enter_tree() {
    if (core::Engine::is_editor_active()) return;
    m_runtime = std::make_unique<GameRuntime>();
    m_logger->print("GameRuntime object initialized", __FUNCTION__);
}

void GameApplication::_exit_tree() {
    if (core::Engine::is_editor_active()) return;
    m_runtime.reset();
}

void GameApplication::_ready() {
    if (core::Engine::is_editor_active()) return;
    //grab references to godot objects within the scene
    this->get_godot_objects();
    //bind respective signals to callback functions
    this->bind_signals();
    m_logger->print("[GameApplication] {} called", __FUNCTION__);
}