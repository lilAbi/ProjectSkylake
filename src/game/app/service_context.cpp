#include "service_context.h"

#include "game/event/event_bus.h"
#include "game/state/game_state.h"

Result<void> check_service_mutation(const ServiceContext& context) {
    if (!context.m_events.is_owner_thread()) {
        return Error{
            .m_code = Error_code::invalid_phase,
            .m_message = "Game state must be accessed on the runtime owner thread."
        };
    }
    if (context.m_events.is_shutting_down() || context.m_game_state.m_profile.m_profile_id.empty()) {
        return Error{
            .m_code = Error_code::not_configured,
            .m_message = "No active player profile."
        };
    }
    return std::monostate{};
}

[[nodiscard]] bool notify_on_roster_changed(const ServiceContext& context) noexcept {
    try {
        return context.m_events.post(RosterChanged{.m_revision = context.m_revision}) == Post_result::posted;
    } catch (...) {
        return false;
    }
}