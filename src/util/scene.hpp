#pragma once

#include "assert.hpp"
#include "util/conversions.hpp"

#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/window.hpp>

#include <concepts>

namespace core::inline utils {
    namespace scene {
        namespace node {
            template <typename TNode>
                requires std::derived_from<TNode, godot::Node>
            static inline void set_unique_name(TNode* node, const char* name) {
                runtime_assert(node != nullptr);
                node->set_name(name);
                node->set_unique_name_in_owner(true);
            }

            /* Sets the owner of a node and all it's children. */
            template <typename TNodeA, typename TNodeB>
                requires std::derived_from<TNodeA, godot::Node> && std::derived_from<TNodeB, godot::Node>
            static inline void set_owner(TNodeA* node, TNodeB* owner) {
                runtime_assert(node != nullptr && owner != nullptr);
                const int node_child_count = node->get_child_count();
                for (int i = 0; i < node_child_count; ++i) {
                    auto child = node->get_child(i);
                    child->set_owner(owner);
                    set_owner(child, owner);
                }
            }
        }

        namespace tree {
            template <typename TNode>
                requires std::derived_from<TNode, godot::Node>
            static inline godot::SceneTree* get(TNode* node) {
                godot::SceneTree* scene_tree{node->get_tree()};
                return scene_tree;
            }

            template <typename TNode>
                requires std::derived_from<TNode, godot::Node>
            static inline godot::Node* edited_root(TNode* node) {
                godot::Node* edited_root{node->get_tree()->get_edited_scene_root()};
                return edited_root;
            }

            template <typename TNode>
                requires std::derived_from<TNode, godot::Node>
            static inline godot::Node* root_node(TNode* node) {
                godot::SceneTree* scene_tree{tree::get(node)};
                godot::Window*    root_window{scene_tree->get_root()};
                godot::Node*      root_node{gdcast<godot::Node>(root_window)};
                return root_node;
            }
        }

        namespace packer {
            /* PackedScene from godot::Node parameter */
            template <typename TNode>
                requires std::derived_from<TNode, godot::Node>
            static inline godot::PackedScene* pack(TNode* node) {
                node::set_owner<TNode, TNode>(node, node);
                godot::PackedScene* package = memnew(godot::PackedScene);
                package->pack(node);
                return package;
            }
        }
    }

    namespace resource {
        namespace loader {
            static inline godot::ResourceLoader* get() {
                return godot::ResourceLoader::get_singleton();
            }
        }

        namespace saver {
            static inline godot::ResourceSaver* get() {
                return godot::ResourceSaver::get_singleton();
            }
        }

        namespace preload {
            template <typename TObj, typename TScene = godot::PackedScene>
                requires std::derived_from<TScene, godot::Resource> && std::convertible_to<TObj, godot::Object>
            class PackedScene {
            public:
                using Scene = TScene;
                using Object = TObj;

                /** Load and pack from path. */
                explicit PackedScene(
                    const godot::String&             load_resource_path,
                    const godot::String&             load_type_hint = godot::String(),
                    godot::ResourceLoader::CacheMode load_cache_mode = godot::ResourceLoader::CacheMode::CACHE_MODE_REUSE
                ) {
                    godot::ResourceLoader* resource_loader{loader::get()};
                    const bool             resource_exists{resource_loader->exists(load_resource_path)};
                    runtime_assert(resource_exists);

                    if (resource_exists) {
                        m_packed_resource = resource_loader->load(load_resource_path, load_type_hint, load_cache_mode);
                        m_initialized = m_packed_resource.is_valid();
                    }
                }

                /* Pack from existing instance. */
                explicit PackedScene(godot::Node* node) {
                    m_packed_resource = scene::packer::pack(node);
                    m_initialized = m_packed_resource.is_valid();
                }

                [[nodiscard]] auto instantiate() -> Object* {
                    assertion(m_initialized, "Resource instantiation invoked from uninitialized scene loader.");
                    if (!m_initialized) [[unlikely]]
                        return nullptr;

                    Object* obj{gdcast<Object>(m_packed_resource->instantiate())};
                    runtime_assert(obj != nullptr);

                    return obj;
                }

                /** Save this resource to specified path. */
                void save(godot::String& resource_save_path) {
                    if (m_initialized) {
                        auto error = saver::get()->save(m_packed_resource, resource_save_path);
                        assertion(error == godot::Error::OK, "Packed resource save failed.");
                    }
                }

            private:
                godot::Ref<Scene> m_packed_resource{};
                bool              m_initialized{false};
            };
        }
    }
}
