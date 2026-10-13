#pragma once

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <memory>

class GameApplication : public godot::Node {
    GDCLASS(GameApplication, godot::Node)
public:
    GameApplication() = default;
    ~GameApplication() override = default;

protected:
    static void _bind_methods();

public:
    void _enter_tree() override;
    void _exit_tree() override;
    void _ready() override;

private:
    

};
