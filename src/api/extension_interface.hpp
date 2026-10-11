#pragma once
#include "../util/console.hpp"
#include "godot_cpp/godot.hpp"
#include "godot_cpp/core/defs.hpp"

namespace godot {

    void initialize_static_objects();
    void teardown_static_objects();
    void initialize_extension_module(ModuleInitializationLevel init_level);
    void uninitialize_extension_module(ModuleInitializationLevel init_level);

    extern "C" {
    GDExtensionBool GDE_EXPORT extension_library_init(GDExtensionInterfaceGetProcAddress addr, GDExtensionClassLibraryPtr lib, GDExtensionInitialization* init);
    }

    static inline Console<godot::RichTextLabel>* g_console_singleton{nullptr};
}
