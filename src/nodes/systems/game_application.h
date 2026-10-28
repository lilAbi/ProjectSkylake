#pragma once

#include "audio_controller.h"
#include "input_controller.h"
#include "save_controller.h"
#include "scene_coordinator.h"
#include "game/app/game_runtime.h"
#include "util/console.hpp"

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

private:
    inline void get_godot_objects();
    inline void bind_signals();

public:
    void _enter_tree() override;
    void _exit_tree() override;
    void _ready() override;
    //run startup processes
    void start_game();

private:

    void configure_debug();

private:
    Logger*                      m_logger = Logger::get();
    SceneCoordinator*            m_scene_coordinator = nullptr;
    SaveController*              m_save_controller = nullptr;
    AudioController*             m_audio_controller = nullptr;
    InputController*             m_input_controller = nullptr;
    godot::Node*                 m_debug_root = nullptr;
    std::unique_ptr<GameRuntime> m_runtime = nullptr;
};
