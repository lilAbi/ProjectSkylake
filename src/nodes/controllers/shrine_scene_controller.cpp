#include "shrine_scene_controller.h"

#include "util/constants.hpp"
#include "util/conversions.hpp"
#include "util/engine.hpp"
#include "util/godot_bindings.hpp"

void ShrineSceneController::_bind_methods() {
    // Bind Godot-facing methods, properties, and signals here.
    core::add_signal<ShrineSceneController>(godot::MethodInfo(core::labels::signal::ShrineSceneController::camp_ui_scene_requested));
}

void ShrineSceneController::get_godot_objects() {
    //core::labels::node::Shrine::BackButton
    m_back_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::Shrine::BackButton));
}

void ShrineSceneController::bind_signals() {
    core::connect_signal(m_back_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &ShrineSceneController::on_back_button_pressed));
}

void ShrineSceneController::_ready() {
    // Initialize scene-dependent state here.
    if (core::Engine::is_editor_active())
        return;
    //grab references to godot objects within the scene
    this->get_godot_objects();
    //bind respective signals to callback functions
    this->bind_signals();
}

void ShrineSceneController::on_back_button_pressed() {
    m_logger->print("[ShrineSceneController] {}() called", __FUNCTION__);
    this->emit_signal(core::labels::signal::ShrineSceneController::camp_ui_scene_requested);
}