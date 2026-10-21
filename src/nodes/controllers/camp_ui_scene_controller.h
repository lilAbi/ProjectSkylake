#pragma once

#include "util/console.hpp"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/core/class_db.hpp>

class CampUISceneController : public godot::Control {
    GDCLASS(CampUISceneController, godot::Control)
public:
    CampUISceneController() = default;
    ~CampUISceneController() override = default;

protected:
    static void _bind_methods();

private:
    inline void get_godot_objects();
    inline void bind_signals();

public:
    void _ready() override;

private:
    void on_barracks_button_pressed();
    void on_war_table_button_pressed();
    void on_forge_button_pressed();
    void on_campfire_button_pressed();
    void on_adventure_button_pressed();
    void on_shrine_button_pressed();

private:
    Logger*        m_logger = Logger::get();
    godot::Button* m_barracks_button = nullptr;
    godot::Button* m_war_table_button = nullptr;
    godot::Button* m_forge_button = nullptr;
    godot::Button* m_campfire_button = nullptr;
    godot::Button* m_adventure_button = nullptr;
    godot::Button* m_shrine_button = nullptr;
};
