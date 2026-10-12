#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>

class SceneCoordinator : public godot::Node {
    GDCLASS(SceneCoordinator, godot::Node)
public:
    SceneCoordinator() = default;
    ~SceneCoordinator() override = default;

protected:
    static void _bind_methods();

public:
    void _ready() override;

    

private:
    godot::Node* m_screen_root = nullptr;
    godot::Node* m_current_scene = nullptr;
};
