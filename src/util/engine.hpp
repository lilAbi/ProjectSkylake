#pragma once

#include <cstdint>

namespace godot {
    class Engine;
    class MainLoop;
    class Node;
    class SceneTree;
    class Window;
}

namespace core::inline utils {
    struct Engine {
        static godot::Engine*    get_engine();
        static godot::MainLoop*  get_main_loop();
        static godot::SceneTree* get_scene_tree();
        static godot::Window*    get_root_window();
        static godot::Node*      get_root_node();
        static void              set_fps(int32_t fps);
        static int32_t           get_max_fps();
        static bool              is_editor_active();
        static bool              is_in_debug_mode();
    };
}
