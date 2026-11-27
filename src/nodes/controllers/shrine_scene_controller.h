#pragma once

#include "game/core/ids.h"
#include "game/core/types.h"
#include "game/event/event_bus.h"
#include "util/console.hpp"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/core/class_db.hpp>

namespace godot {
    class Label;
}
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
    void          on_back_button_pressed();
    void          on_summon_button_pressed();
    void          on_multi_summon_button_pressed();
    void          on_save_game_button_pressed();
    void          pull(std::int32_t count);
    void          refresh();
    godot::Label* label(const char* unique_name) const;

private:
    Logger*                    m_logger = Logger::get();
    GameRuntime*               m_runtime = nullptr;
    SummoningService*          m_summoning_service = nullptr;
    RosterService*             m_roster_service = nullptr;
    godot::Button*             m_back_button = nullptr;
    godot::Button*             m_summon_button = nullptr;
    godot::Button*             m_multi_summon_button = nullptr;
    godot::Button*             m_save_game_button = nullptr;
    BannerID                   m_banner;
    std::optional<CharacterID> m_last_character;
    Revision                   m_displayed_revision{-1};
    Subscription               m_roster_subscription;
};
