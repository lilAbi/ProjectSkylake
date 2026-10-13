#include "scene_coordinator.h"

#include "nodes/title/save_scene_controller.h"
#include "nodes/title/title_scene_controller.h"
#include "util/constants.hpp"
#include "util/engine.hpp"
#include "util/godot_bindings.hpp"
#include "util/scene.hpp"

void SceneCoordinator::_bind_methods() {
    // Bind Godot-facing methods, properties, and signals here.
}

void SceneCoordinator::get_godot_objects() {
    m_screen_root = core::gdcast<godot::Control>(this->get_node_or_null(core::labels::node::Main::ScreenRoot));
    m_activity_root = core::gdcast<godot::Control>(this->get_node_or_null(core::labels::node::Main::ActivityRoot));
}

void SceneCoordinator::bind_signals() {

}

void SceneCoordinator::_ready() {
    // Initialize scene-dependent state here.
    if (core::Engine::is_editor_active()) return;
    //grab references to godot objects within the scene
    this->get_godot_objects();
    //bind respective signals to callback functions
    this->bind_signals();

    this->display_title_screen();
}

void SceneCoordinator::display_title_screen() {
    m_logger->print("[SceneCoordinator] {} called", __FUNCTION__);
    this->clear_screen();
    //load & initialize the scene
    core::utils::resource::preload::PackedScene<TitleSceneController> title_scene{core::labels::path::scene::ui::title::title};
    const auto title_scene_controller = title_scene.instantiate();
    //bind "new_game_requested" signal
    core::connect_signal(title_scene_controller, core::labels::signal::TitleSceneController::new_game_requested, callable_mp(this, &SceneCoordinator::display_new_game_screen));
    //bind "load_game_requested" signal
    core::connect_signal(title_scene_controller, core::labels::signal::TitleSceneController::load_game_requested, callable_mp(this, &SceneCoordinator::display_load_game_screen));
    this->mount_screen(title_scene_controller);
}

void SceneCoordinator::display_camp_screen() {

}

void SceneCoordinator::display_battle_screen() {

}

void SceneCoordinator::display_new_game_screen() {
    m_logger->print("[SceneCoordinator] {} called", __FUNCTION__);
}

void SceneCoordinator::display_load_game_screen() {
    m_logger->print("[SceneCoordinator] {} called", __FUNCTION__);
    this->clear_screen();
    //load & initialize the scene
    core::utils::resource::preload::PackedScene<SaveSceneController> save_scene{core::labels::path::scene::ui::title::save};
    const auto save_scene_controller = save_scene.instantiate();
    //bind "game_title_requested" signal
    core::connect_signal(save_scene_controller, core::labels::signal::SaveSceneController::game_title_requested, callable_mp(this, &SceneCoordinator::display_title_screen));
    this->mount_screen(save_scene_controller);
}

void SceneCoordinator::mount_screen(godot::Node* screen) {
    runtime_assert(m_screen_root != nullptr);
    runtime_assert(screen != nullptr);

    // A scene being mounted should not already belong to another parent.
    runtime_assert(screen->get_parent() == nullptr);

    // Navigation code should clear the previous screen first.
    runtime_assert(m_current_screen == nullptr);

    m_screen_root->add_child(screen);
    m_current_screen = screen;
}

void SceneCoordinator::mount_activity(godot::Node* activity) {
    runtime_assert(m_activity_root != nullptr);
    runtime_assert(activity != nullptr);

    runtime_assert(activity->get_parent() == nullptr);
    runtime_assert(m_current_activity == nullptr);

    m_activity_root->add_child(activity);
    m_current_activity = activity;
}

void SceneCoordinator::clear_screen() {
    if (m_current_screen == nullptr) {
        return;
    }

    runtime_assert(m_screen_root != nullptr);
    runtime_assert(m_current_screen->get_parent() == m_screen_root);

    m_screen_root->remove_child(m_current_screen);
    m_current_screen->queue_free();

    m_current_screen = nullptr;
}

void SceneCoordinator::clear_activity() {
    if (m_current_activity == nullptr) {
        return;
    }

    runtime_assert(m_activity_root != nullptr);
    runtime_assert(m_current_activity->get_parent() == m_activity_root);

    m_activity_root->remove_child(m_current_activity);
    m_current_activity->queue_free();

    m_current_activity = nullptr;
}