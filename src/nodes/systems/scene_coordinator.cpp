#include "scene_coordinator.h"

#include "nodes/controllers/camp_ui_scene_controller.h"
#include "nodes/controllers/save_scene_controller.h"
#include "nodes/controllers/shrine_scene_controller.h"
#include "nodes/controllers/title_scene_controller.h"
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

    this->display_title_scene();
}

void SceneCoordinator::display_title_scene() {
    m_logger->print("[SceneCoordinator] {} called", __FUNCTION__);
    this->clear_scene();
    //load & initialize the scene
    core::utils::resource::preload::PackedScene<TitleSceneController> title_scene{core::labels::path::scene::ui::title::title};
    const auto title_scene_controller = title_scene.instantiate();
    //bind "new_game_requested" signal
    core::connect_signal(title_scene_controller, core::labels::signal::TitleSceneController::new_game_requested, callable_mp(this, &SceneCoordinator::display_camp_scene));
    //bind "load_game_requested" signal
    core::connect_signal(title_scene_controller, core::labels::signal::TitleSceneController::load_game_requested, callable_mp(this, &SceneCoordinator::display_load_game_scene));
    this->mount_scene(title_scene_controller);
}

void SceneCoordinator::display_camp_scene() {
    m_logger->print("[SceneCoordinator] {} called", __FUNCTION__);
    this->clear_scene();
    //load & initialize the scene
    core::utils::resource::preload::PackedScene<CampUISceneController> camp_ui_scene{core::labels::path::scene::ui::camp::camp_ui};
    const auto camp_ui_scene_controller = camp_ui_scene.instantiate();
    //bind "shrine_scene_requested" signal - transition to shrine scene
    core::connect_signal(camp_ui_scene_controller, core::labels::signal::CampUISceneController::shrine_scene_requested, callable_mp(this, &SceneCoordinator::display_shrine_scene));
    this->mount_scene(camp_ui_scene_controller);
}

void SceneCoordinator::display_battle_scene() {
    m_logger->print("[SceneCoordinator] {}() called", __FUNCTION__);
}

void SceneCoordinator::display_shrine_scene() {
    m_logger->print("[SceneCoordinator] {}() called", __FUNCTION__);
    this->clear_scene();
    //load & initialize the scene
    core::utils::resource::preload::PackedScene<ShrineSceneController> shrine_scene{core::labels::path::scene::ui::summon::shrine};
    const auto shrine_scene_controller = shrine_scene.instantiate();
    //bind "camp_ui_scene_requested" signal - transition to camp_ui scene
    core::connect_signal(shrine_scene_controller, core::labels::signal::ShrineSceneController::camp_ui_scene_requested, callable_mp(this, &SceneCoordinator::display_camp_scene));
    this->mount_scene(shrine_scene_controller);
}

void SceneCoordinator::display_new_game_scene() {
    m_logger->print("[SceneCoordinator] {}() called", __FUNCTION__);
}

void SceneCoordinator::display_load_game_scene() {
    m_logger->print("[SceneCoordinator] {}() called", __FUNCTION__);
    this->clear_scene();
    //load & initialize the scene
    core::utils::resource::preload::PackedScene<SaveSceneController> save_scene{core::labels::path::scene::ui::title::save};
    const auto save_scene_controller = save_scene.instantiate();
    //bind "start_game_requested" signal
    core::connect_signal(save_scene_controller, core::labels::signal::SaveSceneController::start_game_requested, callable_mp(this, &SceneCoordinator::display_camp_scene));
    //bind "game_title_requested" signal
    core::connect_signal(save_scene_controller, core::labels::signal::SaveSceneController::game_title_requested, callable_mp(this, &SceneCoordinator::display_title_scene));
    this->mount_scene(save_scene_controller);
}

void SceneCoordinator::mount_scene(godot::Node* root) {
    runtime_assert(m_screen_root != nullptr);
    runtime_assert(root != nullptr);
    runtime_assert(root->get_parent() == nullptr);
    runtime_assert(m_current_scene == nullptr);
    m_screen_root->add_child(root);
    m_current_scene = root;
}

void SceneCoordinator::mount_activity(godot::Node* root) {
    runtime_assert(m_activity_root != nullptr);
    runtime_assert(root != nullptr);
    runtime_assert(root->get_parent() == nullptr);
    runtime_assert(m_current_activity == nullptr);
    m_activity_root->add_child(root);
    m_current_activity = root;
}

void SceneCoordinator::clear_scene() {
    if (m_current_scene == nullptr) {
        return;
    }
    runtime_assert(m_screen_root != nullptr);
    runtime_assert(m_current_scene->get_parent() == m_screen_root);
    m_screen_root->remove_child(m_current_scene);
    m_current_scene->queue_free();
    m_current_scene = nullptr;
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