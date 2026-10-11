#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/main_loop.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

#include "util/engine.hpp"

namespace core::inline utils {

    godot::Engine* Engine::get_engine() {
        godot::Engine* godot_engine{godot::Engine::get_singleton()};
        return godot_engine;
    }

    godot::MainLoop* Engine::get_main_loop() {
        const auto godot_engine{Engine::get_engine()};
        return godot_engine->get_main_loop();
    }

    godot::SceneTree* Engine::get_scene_tree() {
        const auto loop{Engine::get_main_loop()};
        return godot::Object::cast_to<godot::SceneTree>(loop);
    }

    godot::Window* Engine::get_root_window() {
        const auto tree{Engine::get_scene_tree()};
        return tree->get_root();
    }

    godot::Node* Engine::get_root_node() {
        return godot::Object::cast_to<godot::Node>(Engine::get_root_window());
    }

    void Engine::set_fps(const int32_t fps) {
        const auto godot_engine{Engine::get_engine()};
        return godot_engine->set_max_fps(fps);
    }

    int32_t Engine::get_max_fps() {
        const auto godot_engine{Engine::get_engine()};
        return godot_engine->get_max_fps();
    }

    bool Engine::is_editor_active() {
        const auto godot_engine{Engine::get_engine()};
        return godot_engine->is_editor_hint();
    }
}
