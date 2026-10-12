#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

#include <godot_cpp/classes/object.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/object.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/variant/callable_method_pointer.hpp>
#include <godot_cpp/variant/string_name.hpp>

// C++20. Use the registration helpers inside a registered class's
// _bind_methods(), never from global/static object constructors.
namespace core::inline utils {

    template <auto Function>
        requires std::is_member_function_pointer_v<decltype(Function)>
    struct Method {
        // Parameter NAMES belong in D_METHOD(...).
        // Only explicitly supplied DEFAULT VALUES follow the member pointer.
        template <typename... DefaultArgs>
        static godot::MethodBind* bind(const godot::MethodDefinition& definition, DefaultArgs&&... defaults) {
            return godot::ClassDB::bind_method(definition, Function, std::forward<DefaultArgs>(defaults)...);
        }
    };

    template <typename TClass, auto Function>
        requires(
            std::is_base_of_v<godot::Object, TClass> &&
            std::is_pointer_v<decltype(Function)> &&
            std::is_function_v<std::remove_pointer_t<decltype(Function)>>
        )
    struct StaticMethod {
        // TClass must be a registered GDExtension class. Function may be a
        // free function or a static member function with Godot-compatible types.
        template <typename... DefaultArgs>
        static godot::MethodBind* bind(const godot::MethodDefinition& definition, DefaultArgs&&... defaults) {
            return godot::ClassDB::bind_static_method(
                TClass::get_class_static(),
                definition,
                Function,
                std::forward<DefaultArgs>(defaults)...
            );
        }
    };

    template <typename TClass, auto Getter, auto Setter>
        requires(
            std::is_base_of_v<godot::Object, TClass> &&
            std::is_member_function_pointer_v<decltype(Getter)> &&
            std::is_member_function_pointer_v<decltype(Setter)>
        )
    struct NodeProperty {
        // For ordinary non-indexed read/write properties. This also works on
        // Object/Resource subclasses; the name follows the original helper.
        // Binds the getter and setter itself: do not bind them separately.
        // Supply accurate PropertyInfo, including hints/usage/class metadata.
        static void add(
            const godot::PropertyInfo& info,
            const godot::StringName&   getter_name,
            const godot::StringName&   setter_name
        ) {
            static_assert(std::is_invocable_v<decltype(Getter), TClass*>, "The getter must be callable on TClass with no arguments.");

            using value_t = std::remove_cvref_t<std::invoke_result_t<decltype(Getter), TClass*>>;
            static_assert(!std::is_void_v<value_t>, "The getter must return a value.");
            static_assert(std::is_invocable_v<decltype(Setter), TClass*, value_t>, "The setter must accept the getter's value type.");
            static_assert(std::is_void_v<std::invoke_result_t<decltype(Setter), TClass*, value_t>>, "The setter must return void.");

            if (Method<Getter>::bind(godot::D_METHOD(getter_name)) == nullptr) {
                return; // ClassDB already reports the registration error.
            }
            if (Method<Setter>::bind(godot::D_METHOD(setter_name, "value")) == nullptr) {
                return;
            }

            godot::ClassDB::add_property(TClass::get_class_static(), info, setter_name, getter_name);
        }
    };

    template <typename TClass>
        requires std::is_base_of_v<godot::Object, TClass>
    void add_signal(const godot::MethodInfo& info) {
        godot::ClassDB::add_signal(TClass::get_class_static(), info);
    }

    // Call at runtime with a LIVE sender and a valid Callable. The sender is
    // borrowed for this call only; no sender/receiver pointers or metadata are
    // retained by these helpers. A nullptr check cannot detect a dangling pointer.
    //
    // Pass Callable(receiver, "bound_method") for name-based dispatch, or
    // callable_mp(receiver, &Receiver::callback) for a C++ member callback.
    // The latter does not require ClassDB method registration.
    // Signal/callback argument compatibility is still validated by Godot's
    // dispatch machinery; this wrapper does not promise a typed signal interface.
    // Duplicate connections and connection flags retain Object::connect semantics.
    [[discard]] inline godot::Error connect_signal(
        godot::Object*           sender,
        const godot::StringName& signal_name,
        const godot::Callable&   callback,
        std::uint32_t            flags = 0
    ) {
        if (sender == nullptr) {
            return godot::ERR_INVALID_PARAMETER;
        }
        if (!sender->has_signal(signal_name)) {
            return godot::ERR_DOES_NOT_EXIST;
        }
        if (!callback.is_valid()) {
            return godot::ERR_INVALID_PARAMETER;
        }
        return sender->connect(signal_name, callback, flags);
    }

} // namespace core::inline utils
