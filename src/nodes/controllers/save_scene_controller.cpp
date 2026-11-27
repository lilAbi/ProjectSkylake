#include "save_scene_controller.h"

#include "game/app/game_runtime.h"

#include "util/constants.hpp"
#include "util/conversions.hpp"
#include "util/engine.hpp"
#include "util/godot_bindings.hpp"

void SaveSceneController::_bind_methods() {
    // Bind Godot-facing methods, properties, and signals here.
    core::add_signal<SaveSceneController>(godot::MethodInfo(core::labels::signal::SaveSceneController::game_title_requested));
    core::add_signal<SaveSceneController>(godot::MethodInfo(core::labels::signal::SaveSceneController::start_game_requested));
}

void SaveSceneController::get_godot_objects() {
    m_slot_1_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::Save::Slot1Button));
    m_slot_2_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::Save::Slot2Button));
    m_slot_3_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::Save::Slot3Button));
    m_back_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::Save::BackButton));
}

void SaveSceneController::bind_signals() {
    core::connect_signal(m_slot_1_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &SaveSceneController::on_slot_1_pressed));
    core::connect_signal(m_slot_2_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &SaveSceneController::on_slot_2_pressed));
    core::connect_signal(m_slot_3_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &SaveSceneController::on_slot_3_pressed));
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

void SaveSceneController::configure(GameRuntime& runtime) {
    m_logger->print("[SaveSceneController] {}() called", __FUNCTION__);
    m_runtime = &runtime;
}

void SaveSceneController::load_slot(const std::int32_t slot) {
    m_logger->print("[SaveSceneController] {}() called", __FUNCTION__);
    /*
    const auto loaded = m_runtime->load_profile(SaveSlot{slot});
    if (const auto* error = std::get_if<Error>(&loaded)) {
        if (m_status) {
            m_status->set_text(godot::String::utf8(error->m_message.c_str()));
        }
        return;
    }
    */
    this->emit_signal(core::labels::signal::SaveSceneController::start_game_requested);
}

void SaveSceneController::on_slot_1_pressed() {
    m_logger->print("[SaveSceneController] {}() called", __FUNCTION__);
    this->load_slot(1);
}

void SaveSceneController::on_slot_2_pressed() {
    m_logger->print("[SaveSceneController] {}() called", __FUNCTION__);
    this->load_slot(2);
}

void SaveSceneController::on_slot_3_pressed() {
    m_logger->print("[SaveSceneController] {}() called", __FUNCTION__);
    this->load_slot(3);
}

void SaveSceneController::on_back_button_pressed() {
    m_logger->print("[SaveSceneController] {}() called", __FUNCTION__);
    this->emit_signal(core::labels::signal::SaveSceneController::game_title_requested);
}