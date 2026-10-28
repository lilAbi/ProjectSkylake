#pragma once

#include "nodes/systems/game_application.h"
#include "util/console.hpp"

#include <godot_cpp/classes/node.hpp>

class Main : public godot::Node {
    GDCLASS(Main, godot::Node);

public:
    Main() = default;
    ~Main() override = default;

protected:
    static void _bind_methods();

private:
    inline void get_godot_objects();
    inline void bind_signals();

public:
    void _ready() override;

private:
    Logger*          m_logger = Logger::get();
    GameApplication* m_game_application = nullptr;
};