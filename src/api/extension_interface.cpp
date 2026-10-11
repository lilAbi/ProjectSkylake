#include "extension_interface.hpp"

#include "../util/console.hpp"
#include "main.hpp"

#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>

void godot::initialize_static_objects() {
    g_console_singleton = memnew(console);
    godot::Engine::get_singleton()->register_singleton("Console", console::get());
}

void godot::teardown_static_objects() {
    godot::Engine::get_singleton()->unregister_singleton("Console");
    memdelete(g_console_singleton);
}

void godot::initialize_extension_module(const godot::ModuleInitializationLevel init_level) {
    if (init_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE)
        return;
    godot::ClassDB::register_class<Main>();
    godot::ClassDB::register_class<console>();
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