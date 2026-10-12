#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>

class InputController : public godot::Node {
    GDCLASS(InputController, godot::Node)
public:
    InputController() = default;
    ~InputController() override = default;

protected:
    static void _bind_methods();

public:
    void _ready() override;
};
