#pragma once

#include "util/console.hpp"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/core/class_db.hpp>

class RosterService;
class SummoningService;
class GameRuntime;

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

public:
    void configure(GameRuntime& runtime);

private:
    void on_back_button_pressed();
    void on_summon_button_pressed();
    void on_save_to_roster_button_pressed();
    void on_reroll_button_pressed();

private:
    Logger*           m_logger = Logger::get();
    godot::Button*    m_back_button = nullptr;
    SummoningService* m_summoning_service = nullptr;
    RosterService*    m_roster_service = nullptr;
};
