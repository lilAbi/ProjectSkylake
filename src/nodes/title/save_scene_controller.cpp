#include "save_scene_controller.h"

#include "util/constants.hpp"
#include "util/conversions.hpp"
#include "util/engine.hpp"
#include "util/godot_bindings.hpp"

void SaveSceneController::_bind_methods() {
    // Bind Godot-facing methods, properties, and signals here.
    core::add_signal<SaveSceneController>(godot::MethodInfo(core::labels::signal::SaveSceneController::game_title_requested));
}

void SaveSceneController::get_godot_objects() {
    m_back_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::Save::BackButton));
}

void SaveSceneController::bind_signals() {
    core::connect_signal(m_back_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &SaveSceneController::on_back_button_pressed));
}

void SaveSceneController::_ready() {
    // Initialize scene-dependent state here.
    if (core::Engine::is_editor_active()) return;
    //grab references to godot objects within the scene
    this->get_godot_objects();
    //bind respective signals to callback functions
    this->bind_signals();
}

void SaveSceneController::on_back_button_pressed() {
    m_logger->print("[SaveSceneController] {} called", __FUNCTION__);
    this->emit_signal(core::labels::signal::SaveSceneController::game_title_requested);
}