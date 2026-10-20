#include "game_runtime.h"

Result<void> GameRuntime::initialize() {
    m_logger->print("[GameRuntime] {} called", __FUNCTION__);
    //initialization check
    //load content_catalog from content_data
    //initialize "ContentCatalog" with pre-defined definitions
    return std::monostate{};
}

Result<void> GameRuntime::new_game(const NewGameRequest& new_game_request) {
    m_logger->print("[GameRuntime] {} called", __FUNCTION__);
    //initialize the state to support a "new game"
    return std::monostate{};
}

Result<void> GameRuntime::load_profile(SaveSlot) {
    m_logger->print("[GameRuntime] {} called", __FUNCTION__);
    //load previous saved game file
    return std::monostate{};
}

Result<void> GameRuntime::shutdown() {
    m_logger->print("[GameRuntime] {} called", __FUNCTION__);
    //safely de-construct objects
    return std::monostate{};
}