#include "title_screen_controller.h"

#include "util/conversions.hpp"
#include "util/engine.hpp"
#include "util/godot_bindings.hpp"
#include "util/constants.hpp"


void TitleScreenController::_bind_methods() {
    // Bind Godot-facing methods, properties, and signals here.
}

void TitleScreenController::get_godot_objects() {
    m_continue_game_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::Title::ContinueButton));
    m_new_game_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::Title::NewGameButton));
    m_settings_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::Title::SettingsButton));
    m_quit_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::Title::QuitButton));

}

void TitleScreenController::bind_signals() {
    core::connect_signal(m_continue_game_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &TitleScreenController::on_continue_game_button_pressed));
    core::connect_signal(m_new_game_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &TitleScreenController::on_load_game_button_pressed));
    core::connect_signal(m_settings_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &TitleScreenController::on_settings_button_pressed));
    core::connect_signal(m_quit_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &TitleScreenController::on_quit_button_pressed));
}

void TitleScreenController::_ready() {
    if (core::Engine::is_editor_active()) return;
    //grab references to godot objects within the scene
    this->get_godot_objects();
    //bind respective signals to callback functions
    this->bind_signals();
}

void TitleScreenController::on_continue_game_button_pressed() {
    m_logger->print("{} called", __FUNCTION__);
}

void TitleScreenController::on_load_game_button_pressed() {
    m_logger->print("{} called", __FUNCTION__);
}

void TitleScreenController::on_settings_button_pressed() {
    m_logger->print("{} called", __FUNCTION__);
}

void TitleScreenController::on_quit_button_pressed() {
    m_logger->print("{} called", __FUNCTION__);
}