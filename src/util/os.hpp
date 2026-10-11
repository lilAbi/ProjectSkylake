#pragma once

#include <godot_cpp/classes/os.hpp>

namespace core::inline utils {
    struct os {
        static inline godot::OS* get() {
            return godot::OS::get_singleton();
        }
    };
}
