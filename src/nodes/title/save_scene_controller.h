#pragma once

#include "util/console.hpp"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/core/class_db.hpp>

class SaveSceneController : public godot::Control {
    GDCLASS(SaveSceneController, godot::Control)
public:
    SaveSceneController() = default;
    ~SaveSceneController() override = default;

protected:
    static void _bind_methods();

private:
    inline void get_godot_objects();
    inline void bind_signals();

public:
    void _ready() override;

private:
    void on_back_button_pressed();

private:
    Logger*        m_logger = Logger::get();
    godot::Button* m_back_button = nullptr;
};
