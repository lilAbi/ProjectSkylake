#pragma once

#include <godot_cpp/classes/node.hpp>

class Main : public godot::Node {
    GDCLASS(Main, godot::Node);
public:
    Main();
    ~Main() override = default;

protected:
    static void _bind_methods();

public:
    void _ready() override;
    void _physics_process(double delta) override;

private:
};