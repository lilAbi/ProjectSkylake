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
    void display_title_screen();
    void display_camp_screen();
    void display_battle_screen();
    void display_new_game_screen();
    void display_load_game_screen();

    void mount_screen(godot::Node* screen);
    void mount_activity(godot::Node* activity);

    void clear_screen();
    void clear_activity();

private:
    Logger*      m_logger = Logger::get();
    godot::Node* m_screen_root = nullptr;
    godot::Node* m_activity_root = nullptr;
    godot::Node* m_current_screen = nullptr;
    godot::Node* m_current_activity = nullptr;
    bool         m_is_transitioning = false;
};
