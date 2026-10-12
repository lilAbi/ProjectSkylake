#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>

class SaveController : public godot::Node {
    GDCLASS(SaveController, godot::Node)
public:
    SaveController() = default;
    ~SaveController() override = default;

protected:
    static void _bind_methods();

public:
    void _ready() override;
};
