#pragma once

#include "util/console.hpp"

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/core/class_db.hpp>

class TitleSceneController : public godot::Control {
    GDCLASS(TitleSceneController, godot::Control)
public:
    TitleSceneController() = default;
    ~TitleSceneController() override = default;

protected:
    static void _bind_methods();

private:
    inline void get_godot_objects();
    inline void bind_signals();

public:
    void _ready() override;

private:
    void on_new_game_button_pressed();
    void on_load_game_button_pressed();
    void on_settings_button_pressed();
    void on_quit_button_pressed();

private:
    Logger*        m_logger = Logger::get();
    godot::Button* m_continue_game_button = nullptr;
    godot::Button* m_new_game_button = nullptr;
    godot::Button* m_settings_button = nullptr;
    godot::Button* m_quit_button = nullptr;
};
