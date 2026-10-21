#pragma once

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/core/class_db.hpp>

class ShrineSceneController : public godot::Control {
    GDCLASS(ShrineSceneController, godot::Control)
public:
    ShrineSceneController() = default;
    ~ShrineSceneController() override = default;
protected:
    static void _bind_methods();

private:
    inline void get_godot_objects();
    inline void bind_signals();

public:
    void _ready() override;
};
