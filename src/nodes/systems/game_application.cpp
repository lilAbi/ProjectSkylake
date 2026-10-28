#include "game_application.h"

#include "godot_cpp/classes/engine.hpp"
#include "nodes/debug/content_catalog_scene_controller.h"
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
    m_debug_root = core::gdcast<godot::Control>(this->get_node_or_null(core::labels::node::Main::DebugRoot));
}

void GameApplication::bind_signals() {
    
}

void GameApplication::_enter_tree() {
    if (core::Engine::is_editor_active())
        return;
    m_logger->print("[GameApplication] {}() called", __FUNCTION__);
    //create GameRuntime
    m_runtime = std::make_unique<GameRuntime>();
    //initialize the runtime
    m_runtime->initialize();
    m_logger->print("[GameApplication] GameRuntime object initialized");
}

void GameApplication::_exit_tree() {
    if (core::Engine::is_editor_active())
        return;
    m_logger->print("[GameApplication] {}() called", __FUNCTION__);
    m_runtime.reset();
}

void GameApplication::_ready() {
    if (core::Engine::is_editor_active())
        return;
    m_logger->print("[GameApplication] {}() called", __FUNCTION__);
    //grab references to godot objects within the scene
    this->get_godot_objects();
    //bind respective signals to callback functions
    this->bind_signals();
    //pass the Runtime object to the scene coordinator
    m_scene_coordinator->configure(*m_runtime);
    m_logger->print("[GameApplication] Passed GameRuntime object to SceneCoordinator");
}

void GameApplication::start_game() {
    m_logger->print("[GameApplication] {}() called", __FUNCTION__);
    m_logger->print("[GameApplication] Configuring Debug Tools");
    this->configure_debug();
    m_logger->print("[GameApplication] Display Title Scene");
    m_scene_coordinator->display_title_scene();
}

void GameApplication::configure_debug() {
    if (!core::Engine::is_in_debug_mode())
        return;
    auto* browser = core::utils::gdcast<ContentCatalogSceneController>(m_debug_root->get_node_or_null(core::labels::node::Main::ContentCatalog));
    if (browser) {
        browser->configure(m_runtime->get_content());
        m_logger->print("[GameApplication] Configured Debug Tools");
    }
}