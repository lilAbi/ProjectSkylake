#include "camp_ui_scene_controller.h"

#include "util/constants.hpp"
#include "util/conversions.hpp"
#include "util/engine.hpp"
#include "util/godot_bindings.hpp"

void CampUISceneController::_bind_methods() {
    // Bind Godot-facing methods, properties, and signals here.
}

void CampUISceneController::get_godot_objects() {
    m_barracks_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::CampUI::BarracksButton));
    m_war_table_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::CampUI::WarTableButton));
    m_forge_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::CampUI::ForgeButton));
    m_campfire_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::CampUI::CampfireButton));
    m_adventure_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::CampUI::AdventureButton));
    m_shrine_button = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::CampUI::ShrineButton));
}

void CampUISceneController::bind_signals() {
    core::connect_signal(m_barracks_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &CampUISceneController::on_barracks_button_pressed));
    core::connect_signal(m_war_table_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &CampUISceneController::on_war_table_button_pressed));
    core::connect_signal(m_forge_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &CampUISceneController::on_forge_button_pressed));
    core::connect_signal(m_campfire_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &CampUISceneController::on_campfire_button_pressed));
    core::connect_signal(m_adventure_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &CampUISceneController::on_adventure_button_pressed));
    core::connect_signal(m_shrine_button, core::labels::signal::BaseButton::pressed, callable_mp(this, &CampUISceneController::on_shrine_button_pressed));
}

void CampUISceneController::_ready() {
    // Initialize scene-dependent state here.
    if (core::Engine::is_editor_active())
        return;
    //grab references to godot objects within the scene
    this->get_godot_objects();
    //bind respective signals to callback functions
    this->bind_signals();
}

void CampUISceneController::on_barracks_button_pressed() {
    m_logger->print("[CampUISceneController] {}() called", __FUNCTION__);
}

void CampUISceneController::on_war_table_button_pressed() {
    m_logger->print("[CampUISceneController] {}() called", __FUNCTION__);
}

void CampUISceneController::on_forge_button_pressed() {
    m_logger->print("[CampUISceneController] {}() called", __FUNCTION__);
}

void CampUISceneController::on_campfire_button_pressed() {
    m_logger->print("[CampUISceneController] {}() called", __FUNCTION__);
}

void CampUISceneController::on_adventure_button_pressed() {
    m_logger->print("[CampUISceneController] {}() called", __FUNCTION__);
}

void CampUISceneController::on_shrine_button_pressed() {
    m_logger->print("[CampUISceneController] {}() called", __FUNCTION__);
}