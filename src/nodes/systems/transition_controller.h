#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>

class TransitionController : public godot::Node {
    GDCLASS(TransitionController, godot::Node)
public:
    TransitionController() = default;
    ~TransitionController() override = default;

protected:
    static void _bind_methods();

public:
    void _ready() override;
};
