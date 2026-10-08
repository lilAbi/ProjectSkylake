#include <type_traits>

#include <gdextension_interface.h>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/core/memory.hpp>
#include <godot_cpp/variant/string_name.hpp>

#include "api/extension_interface.hpp"
#include "main.hpp"
#include "singletons/console.hpp"
#include "util/engine.hpp"


static inline rl::console* console_singleton{ nullptr };

void godot::initialize_static_objects() {
    console_singleton = memnew(rl::console);
    rl::engine::get()->register_singleton("Console", rl::console::get());
}

void godot::teardown_static_objects() {
    rl::engine::get()->unregister_singleton("Console");
    memdelete(console_singleton);
}

void godot::initialize_extension_module(godot::ModuleInitializationLevel init_level) {
    if (init_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) return;
    godot::ClassDB::register_class<Main>();
    godot::ClassDB::register_class<rl::console>();
    initialize_static_objects();
}

void godot::uninitialize_extension_module(godot::ModuleInitializationLevel init_level) {
    if (init_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) return;
    godot::teardown_static_objects();
}

extern "C" {
GDExtensionBool GDE_EXPORT godot::extension_library_init(GDExtensionInterfaceGetProcAddress addr, GDExtensionClassLibraryPtr lib, GDExtensionInitialization* init) {
    constexpr auto init_level = godot::MODULE_INITIALIZATION_LEVEL_SCENE;
    const godot::GDExtensionBinding::InitObject init_obj(addr, lib, init);
    init_obj.register_initializer(initialize_extension_module);
    init_obj.register_terminator(uninitialize_extension_module);
    init_obj.set_minimum_library_initialization_level(init_level);
    return init_obj.init();
}
}