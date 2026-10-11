#pragma once

#include <godot_cpp/classes/ref.hpp>
#include "concepts.hpp"
#include <tuple>
#include <type_traits>

namespace core::inline utils {
    template <VariantCompatible T>
    struct variant_traits {
        using raw_type = T;
        using type_info = godot::GetTypeInfo<std::remove_cvref_t<T>>;
        static constexpr inline auto variant_type = static_cast<godot::Variant::Type>(type_info::VARIANT_TYPE);
    };
}
