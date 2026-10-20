#pragma once

#include "util/console.hpp"

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>

class SceneCoordinator : public godot::Node {
    GDCLASS(SceneCoordinator, godot::Node)
public:
    SceneCoordinator() = default;
    ~SceneCoordinator() override = default;

protected:
    static void _bind_methods();

private:
    inline void get_godot_objects();
    inline void bind_signals();

public:
    void _ready() override;

private:
    void display_title_scene();
    void display_camp_scene();
    void display_battle_scene();
    void display_shrine_scene();
    void display_new_game_scene();
    void display_load_game_scene();

    void mount_scene(godot::Node* root);
    void mount_activity(godot::Node* root);

    void clear_scene();
    void clear_activity();

private:
    Logger*      m_logger = Logger::get();
    godot::Node* m_screen_root = nullptr;
    godot::Node* m_activity_root = nullptr;
    godot::Node* m_current_scene = nullptr;
    godot::Node* m_current_activity = nullptr;
    bool         m_is_transitioning = false;
};
