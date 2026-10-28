#include "extension_interface.hpp"

#include "main.hpp"
#include "nodes/systems/audio_controller.h"
#include "nodes/systems/game_application.h"
#include "nodes/systems/input_controller.h"
#include "nodes/systems/save_controller.h"
#include "nodes/systems/scene_coordinator.h"
#include "nodes/systems/transition_controller.h"
#include "nodes/controllers/camp_ui_scene_controller.h"
#include "nodes/controllers/save_scene_controller.h"
#include "nodes/controllers/shrine_scene_controller.h"
#include "nodes/controllers/title_scene_controller.h"
#include "nodes/debug/content_catalog_scene_controller.h"
#include "util/console.hpp"

#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>

void godot::initialize_static_objects() {
    Logger::get()->start_logging();
}

void godot::teardown_static_objects() {
    Logger::get()->stop_logging();
}

void godot::initialize_extension_module(const godot::ModuleInitializationLevel init_level) {
    if (init_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE)
        return;

    //Main
    godot::ClassDB::register_class<Main>();

    //Systems
    godot::ClassDB::register_class<AudioController>();
    godot::ClassDB::register_class<GameApplication>();
    godot::ClassDB::register_class<InputController>();
    godot::ClassDB::register_class<SaveController>();
    godot::ClassDB::register_class<SceneCoordinator>();
    godot::ClassDB::register_class<TransitionController>();

    //title
    godot::ClassDB::register_class<ShrineSceneController>();
    godot::ClassDB::register_class<CampUISceneController>();
    godot::ClassDB::register_class<SaveSceneController>();
    godot::ClassDB::register_class<TitleSceneController>();

    //Debug
    godot::ClassDB::register_class<ContentCatalogSceneController>();

    initialize_static_objects();
}

void godot::uninitialize_extension_module(const godot::ModuleInitializationLevel init_level) {
    if (init_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE)
        return;
    godot::teardown_static_objects();
}

extern "C" {
GDExtensionBool GDE_EXPORT godot::extension_library_init(GDExtensionInterfaceGetProcAddress addr, GDExtensionClassLibraryPtr lib, GDExtensionInitialization* init) {
    constexpr auto                              init_level = godot::MODULE_INITIALIZATION_LEVEL_SCENE;
    const godot::GDExtensionBinding::InitObject init_obj(addr, lib, init);
    init_obj.register_initializer(initialize_extension_module);
    init_obj.register_terminator(uninitialize_extension_module);
    init_obj.set_minimum_library_initialization_level(init_level);
    return init_obj.init();
}
}