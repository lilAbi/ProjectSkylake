#include "title_scene_controller.h"

#include "util/conversions.hpp"
#include "util/engine.hpp"
#include "util/godot_bindings.hpp"
#include "util/constants.hpp"


void TitleSceneController::_bind_methods() {
    // Bind Godot-facing methods, properties, and signals here.
    core::add_signal<TitleSceneController>(godot::MethodInfo(core::labels::signal::TitleSceneController::new_game_requested));
    core::add_signal<TitleSceneController>(godot::MethodInfo(core::labels::signal::TitleSceneController::load_game_requested));
}

void TitleSceneController::get_godot_objects() {
    m_continue_game_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::Title::ContinueButton));
    m_new_game_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::Title::NewGameButton));
    m_settings_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::Title::SettingsButton));
    m_quit_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::Title::QuitButton));

}

void TitleSceneController::bind_signals() {
    core::connect_signal(m_continue_game_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &TitleSceneController::on_new_game_button_pressed));
    core::connect_signal(m_new_game_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &TitleSceneController::on_load_game_button_pressed));
    core::connect_signal(m_settings_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &TitleSceneController::on_settings_button_pressed));
    core::connect_signal(m_quit_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &TitleSceneController::on_quit_button_pressed));
}

void TitleSceneController::_ready() {
    if (core::Engine::is_editor_active()) return;
    //grab references to godot objects within the scene
    this->get_godot_objects();
    //bind respective signals to callback functions
    this->bind_signals();
}

void TitleSceneController::on_new_game_button_pressed() {
    m_logger->print("[TitleSceneController] {} called", __FUNCTION__);
    this->emit_signal(core::labels::signal::TitleSceneController::new_game_requested);
}

void TitleSceneController::on_load_game_button_pressed() {
    m_logger->print("[TitleSceneController] {} called", __FUNCTION__);
    this->emit_signal(core::labels::signal::TitleSceneController::load_game_requested);
}

void TitleSceneController::on_settings_button_pressed() {
    m_logger->print("[TitleSceneController] {} called", __FUNCTION__);
}

void TitleSceneController::on_quit_button_pressed() {
    m_logger->print("[TitleSceneController] {} called", __FUNCTION__);
}