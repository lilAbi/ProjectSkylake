#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>

class AudioController : public godot::Node {
    GDCLASS(AudioController, godot::Node)
public:
    AudioController() = default;
    ~AudioController() override = default;

protected:
    static void _bind_methods();

public:
    void _ready() override;
};
