#include "content_catalog_scene_controller.h"

#include "util/constants.hpp"
#include "util/conversions.hpp"
#include "util/engine.hpp"
#include "util/godot_bindings.hpp"

#include "godot_cpp/classes/viewport.hpp"

namespace {

    godot::String text(std::string_view value) {
        return godot::String::utf8(value.data(), static_cast<int64_t>(value.size()));
    }

    godot::String title(std::string_view value) {
        return text(value).replace("_", " ").capitalize().replace("Hp", "HP");
    }

    godot::String entry_name(const debug::CatalogEntry& entry) {
        if (!entry.m_name.empty())
            return text(entry.m_name);
        const auto separator = entry.m_key.m_id.find('.');
        return title(separator == std::string::npos ? entry.m_key.m_id : entry.m_key.m_id.substr(separator + 1));
    }

    std::size_t index_of(debug::Catalog_kind kind) {
        return static_cast<std::size_t>(kind);
    }

    const debug::CatalogCategory& category(debug::Catalog_kind kind) {
        return debug::catalog_categories()[index_of(kind)];
    }

    std::string child_path(const std::string& parent, const std::string& key) {
        std::string escaped;
        for (char c : key)
            escaped += c == '~' ? "~0" : c == '/' ? "~1"
                                                  : std::string(1, c);
        return parent + "/" + escaped;
    }

}

void ContentCatalogSceneController::_bind_methods() {
    // Bind Godot-facing methods, properties, and signals here.
}

void ContentCatalogSceneController::get_godot_objects() {
    m_categories = core::gdcast<godot::Tree>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::Categories));
    m_entries = core::gdcast<godot::ItemList>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::Entries));
    m_summary = core::gdcast<godot::Label>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::CatalogSummary));
    m_category_title = core::gdcast<godot::Label>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::CategoryTitle));
    m_entry_count = core::gdcast<godot::Label>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::EntryCount));
    m_cpp_type = core::gdcast<godot::Label>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::CppType));
    m_detail_type = core::gdcast<godot::Label>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::DetailType));
    m_detail_name = core::gdcast<godot::Label>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::DetailName));
    m_detail_id = core::gdcast<godot::Label>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::DetailID));
    m_footer = core::gdcast<godot::Label>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::SelectionPath));
    m_back = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::BackButton));
    m_close = core::gdcast<godot::Button>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::CloseButton));
    m_tabs = core::gdcast<godot::TabContainer>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::InspectorTabs));
    m_fields = core::gdcast<godot::RichTextLabel>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::FieldsText));
    m_references = core::gdcast<godot::RichTextLabel>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::ReferencesText));
    m_data = core::gdcast<godot::TextEdit>(this->get_node_or_null(core::labels::node::ContentCatalogBrowser::DataText));
}

void ContentCatalogSceneController::bind_signals() {
    core::connect_signal(m_categories, core::labels::signal::ItemList::item_selected, callable_mp(this, &ContentCatalogSceneController::on_category_selected));
    core::connect_signal(m_entries, core::labels::signal::ItemList::item_selected, callable_mp(this, &ContentCatalogSceneController::on_entry_selected));
    core::connect_signal(m_fields, core::labels::signal::RichTextLabel::meta_clicked, callable_mp(this, &ContentCatalogSceneController::on_reference_clicked));
    core::connect_signal(m_references, core::labels::signal::RichTextLabel::meta_clicked, callable_mp(this, &ContentCatalogSceneController::on_reference_clicked));
    core::connect_signal(m_back, core::labels::signal::BaseButton::pressed, callable_mp(this, &ContentCatalogSceneController::on_back_pressed));
    core::connect_signal(m_close, core::labels::signal::BaseButton::pressed, callable_mp(this, &ContentCatalogSceneController::on_close_pressed));
}

void ContentCatalogSceneController::_ready() {
    // Initialize scene-dependent state here.
    if (core::Engine::is_editor_active())
        return;
    //hide node until shown
    this->hide();
    //disable processing on node if in debug mode
    if (!core::Engine::is_in_debug_mode()) {
        set_process_input(false);
        return;
    }
    //grab references to godot objects within the scene
    this->get_godot_objects();
    //bind respective signals to callback functions
    this->bind_signals();
    //configure properties
    m_categories->set_column_expand(1, false);
    m_categories->set_column_custom_minimum_width(1, 32);
    //build source
    m_ready = true;
    this->rebuild();
}

void ContentCatalogSceneController::_input(const godot::Ref<godot::InputEvent>& event) {
    if (core::Engine::is_editor_active())
        return;
    if (!m_ready || event.is_null())
        return;
    if (event->is_action_pressed("debug_content_catalog")) {
        m_logger->print("[ContentCatalogSceneController] F3 Pressed");
        this->set_visible(!is_visible());
        if (this->is_visible())
            m_categories->grab_focus();
        this->get_viewport()->set_input_as_handled();
    } else if (is_visible() && event->is_action_pressed("ui_cancel")) {
        this->hide();
        this->get_viewport()->set_input_as_handled();
    }
}

void ContentCatalogSceneController::configure(const ContentCatalog& catalog) {
    m_snapshot = debug::inspect_catalog(catalog);
    m_error.clear();
    m_configured = true;
    m_selection.reset();
    m_history.clear();
    if (m_ready)
        rebuild();
}

void ContentCatalogSceneController::configure_error(const std::string& message) {
    m_snapshot = {};
    m_error = message;
    m_configured = false;
    m_selection.reset();
    m_history.clear();
    if (m_ready)
        rebuild();
}

void ContentCatalogSceneController::rebuild() {
    m_rebuilding = true;
    m_categories->clear();
    m_category_items.clear();
    auto* root = m_categories->create_item();
    for (const auto& kind : debug::catalog_categories()) {
        auto*      item = m_categories->create_item(root);
        const auto count = std::ranges::count(m_snapshot.m_entries, kind.m_kind, [](const auto& entry) {
            return entry.m_key.m_kind;
        });
        item->set_text(0, text(kind.m_name));
        item->set_text(1, godot::String::num_int64(count));
        item->set_selectable(1, false);
        item->set_text_alignment(1, godot::HORIZONTAL_ALIGNMENT_RIGHT);
        item->set_metadata(0, static_cast<int64_t>(kind.m_kind));
        m_category_items.push_back(item);
    }
    if (m_configured) {
        m_summary->set_text("v" + text(m_snapshot.m_content_version) + " / " + godot::String::num_uint64(m_snapshot.m_entries.size()) + " definitions / 16 types");
    } else {
        m_summary->set_text("Catalog unavailable");
    }
    m_rebuilding = false;
    this->select_category(debug::Catalog_kind::classes);
}

void ContentCatalogSceneController::select_category(debug::Catalog_kind kind) {
    const auto first = std::ranges::find(
        m_snapshot.m_entries,
        kind,
        [](const auto& entry) {
            return entry.m_key.m_kind;
        }
    );

    if (first != m_snapshot.m_entries.end()) {
        if (!m_selection || m_selection->m_kind != kind)
            this->select_entry(first->m_key);
        return;
    }

    if (m_selection)
        m_history.push_back({*m_selection, m_tabs->get_current_tab()});

    m_selection.reset();
    m_rebuilding = true;
    this->populate_entries(kind);
    m_rebuilding = false;
    m_detail_type->set_text(text(category(kind).m_name).to_upper());
    m_detail_name->set_text(m_configured ? "No definitions" : "Catalog unavailable");
    m_detail_id->set_text("");
    m_fields->clear();
    m_fields->add_text(m_configured ? "This category is empty." : text(m_error.empty() ? "Run the main scene to inspect its initialized catalog." : m_error));
    m_references->clear();
    m_data->set_text("");
    m_tabs->set_tab_title(1, "References");
    m_tabs->set_current_tab(0);
    m_back->set_disabled(m_history.empty());
    m_footer->set_text(text(category(kind).m_name));
}

void ContentCatalogSceneController::select_entry(const debug::CatalogKey& key, int tab, bool remember) {
    const auto* entry = m_snapshot.find(key);
    if (!entry)
        return;
    if (remember && m_selection && *m_selection != key)
        m_history.push_back({*m_selection, m_tabs->get_current_tab()});
    const bool category_changed = !m_selection || m_selection->m_kind != key.m_kind;
    m_selection = key;
    m_rebuilding = true;
    if (category_changed)
        this->populate_entries(key.m_kind);
    for (std::size_t i = 0; i < m_entry_indices.size(); ++i) {
        if (m_snapshot.m_entries[m_entry_indices[i]].m_key == key)
            m_entries->select(static_cast<int32_t>(i));
    }
    m_rebuilding = false;
    m_tabs->set_current_tab(tab);
    m_back->set_disabled(m_history.empty());
    this->render_entry(*entry);
}

void ContentCatalogSceneController::populate_entries(debug::Catalog_kind kind) {
    m_entries->clear();
    m_entry_indices.clear();
    for (std::size_t i = 0; i < m_snapshot.m_entries.size(); ++i) {
        const auto& entry = m_snapshot.m_entries[i];
        if (entry.m_key.m_kind != kind)
            continue;
        m_entry_indices.push_back(i);
        const auto item = m_entries->add_item(entry_name(entry) + "\n" + text(entry.m_key.m_id));
        m_entries->set_item_tooltip(item, text(entry.m_key.m_id));
    }
    m_category_title->set_text(text(category(kind).m_name));
    m_entry_count->set_text(godot::String::num_uint64(m_entry_indices.size()));
    m_cpp_type->set_text(text(category(kind).m_cpp_type));
    m_category_items[index_of(kind)]->select(0);
}

void ContentCatalogSceneController::render_entry(const debug::CatalogEntry& entry) {
    m_detail_type->set_text(text(category(entry.m_key.m_kind).m_name).to_upper());
    m_detail_name->set_text(entry_name(entry));
    m_detail_id->set_text(text(entry.m_key.m_id));
    m_footer->set_text(text(category(entry.m_key.m_kind).m_name) + " / " + text(entry.m_key.m_id));
    this->render_fields(entry);
    this->render_references(entry);
    // Preserve signed/unsigned 64-bit integers; do not round-trip through Variant.
    m_data->set_text(text(entry.m_fields.dump(2)));
    m_data->set_v_scroll(0);
    m_fields->scroll_to_line(0);
    m_references->scroll_to_line(0);
}

void ContentCatalogSceneController::render_fields(const debug::CatalogEntry& entry) {
    m_fields->clear();
    // add_text() treats authored names and strings literally; no BBCode injection.
    auto properties = nlohmann::ordered_json::object();
    for (const auto& [key, value] : entry.m_fields.items()) {
        if (key != "id" && key != "name" && value.is_primitive())
            properties[key] = value;
    }
    if (entry.m_key.m_kind == debug::Catalog_kind::resources)
        properties = {{"id", entry.m_key.m_id}, {"name", entry.m_name}};
    if (!properties.empty()) {
        this->append_heading(*m_fields, "Properties");
        this->append_value(*m_fields, entry, properties, "");
        m_fields->add_text("\n\n");
    }
    for (const auto& [key, value] : entry.m_fields.items()) {
        if (value.is_primitive())
            continue;
        this->append_heading(*m_fields, title(key) + (value.is_array() ? "  (" + godot::String::num_uint64(value.size()) + ")" : ""));
        if (key == "base_stats" || key == "stats") {
            m_fields->push_table(4);
            for (int i = 0; i < 4; ++i)
                m_fields->set_table_column_expand(i, true);
            for (const auto& [stat, amount] : value.items()) {
                m_fields->push_cell();
                m_fields->push_color(get_theme_color("muted", "ContentCatalog"));
                m_fields->push_font_size(13);
                m_fields->add_text(title(stat) + "\n");
                m_fields->pop();
                m_fields->pop();
                m_fields->push_font_size(24);
                m_fields->add_text(text(amount.dump()));
                m_fields->pop();
                m_fields->pop();
            }
            m_fields->pop();
        } else {
            this->append_value(*m_fields, entry, value, child_path("", key));
        }
        m_fields->add_text("\n\n");
    }
}

void ContentCatalogSceneController::render_references(const debug::CatalogEntry& entry) {
    m_references->clear();
    std::map<debug::CatalogKey, std::vector<std::string>> outgoing;
    std::map<debug::CatalogKey, std::vector<std::string>> incoming;
    for (const auto& reference : entry.m_references)
        outgoing[reference.m_target].push_back(reference.m_path);
    if (const auto it = m_snapshot.m_used_by.find(entry.m_key); it != m_snapshot.m_used_by.end())
        for (const auto& use : it->second)
            incoming[use.m_source].push_back(use.m_path);
    const auto section = [&](const char* name, const auto& references) {
        append_heading(*m_references, text(name) + "  (" + godot::String::num_uint64(references.size()) + ")");
        if (references.empty())
            m_references->add_text("None\n");
        for (const auto& [key, paths] : references) {
            append_link(*m_references, key);
            m_references->add_text("\n");
            m_references->push_color(get_theme_color("muted", "ContentCatalog"));
            m_references->push_font_size(13);
            for (const auto& path : paths)
                m_references->add_text(text(path) + "\n");
            m_references->pop();
            m_references->pop();
            m_references->add_text("\n");
        }
        m_references->add_text("\n");
    };
    section("References", outgoing);
    section("Referenced by", incoming);
    if (!entry.m_unlocks.empty()) {
        append_heading(*m_references, "Unlock keys");
        for (const auto& unlock : entry.m_unlocks)
            m_references->add_text(text(unlock.m_id) + "\n");
    }
    m_tabs->set_tab_title(1, "References (" + godot::String::num_uint64(outgoing.size() + incoming.size()) + ")");
}

void ContentCatalogSceneController::append_value(godot::RichTextLabel& label, const debug::CatalogEntry& entry, const nlohmann::ordered_json& value, const std::string& path) {
    if (value.is_primitive()) {
        this->append_scalar(label, entry, value, path);
        return;
    }
    if (value.empty()) {
        label.push_color(get_theme_color("muted", "ContentCatalog"));
        label.add_text("None\n");
        label.pop();
        return;
    }
    if (value.is_array()) {
        constexpr std::size_t preview_limit = 128;
        const auto            count = std::min(value.size(), preview_limit);
        for (std::size_t i = 0; i < count; ++i) {
            const auto location = child_path(path, std::to_string(i));
            if (!value[i].is_string()) {
                label.push_color(get_theme_color("muted", "ContentCatalog"));
                label.add_text("[" + godot::String::num_uint64(i) + "]  ");
                label.pop();
            }
            this->append_value(label, entry, value[i], location);
            label.add_text("\n");
        }
        if (count < value.size())
            label.add_text("Showing 128 entries. Data contains the full definition.\n");
        return;
    }
    label.push_table(2);
    label.set_table_column_expand(0, true);
    label.set_table_column_expand(1, true);
    for (const auto& [key, child] : value.items()) {
        const auto location = child_path(path, key);
        label.push_cell();
        const auto reference = std::ranges::find_if(entry.m_references, [&](const auto& ref) {
            return ref.m_path == location && ref.m_on_key;
        });
        if (reference != entry.m_references.end()) {
            this->append_link(label, reference->m_target);
        } else {
            label.push_color(get_theme_color("muted", "ContentCatalog"));
            label.add_text(title(key));
            label.pop();
        }
        label.pop();
        label.push_cell();
        this->append_value(label, entry, child, location);
        label.pop();
    }
    label.pop();
}

void ContentCatalogSceneController::append_scalar(godot::RichTextLabel& label, const debug::CatalogEntry& entry, const nlohmann::ordered_json& value, const std::string& path) {
    const auto reference = std::ranges::find_if(
        entry.m_references,
        [&](const auto& ref) {
            return ref.m_path == path && !ref.m_on_key;
        }
    );
    if (reference != entry.m_references.end()) {
        this->append_link(label, reference->m_target);
    } else if (value.is_string()) {
        const auto& string = value.get_ref<const std::string&>();
        label.add_text(string.empty() ? godot::String("\"\"") : text(string));
    } else {
        label.add_text(text(value.dump()));
        if (path.ends_with("/scaling") || path.ends_with("/chance"))
            label.add_text(" bp");
        if (path.ends_with("/interval") || path.ends_with("/offline_cap"))
            label.add_text(" s");
    }
}

void ContentCatalogSceneController::append_link(godot::RichTextLabel& label, const debug::CatalogKey& key) {
    godot::Array metadata;
    metadata.push_back(static_cast<int64_t>(key.m_kind));
    metadata.push_back(text(key.m_id));
    label.push_color(get_theme_color("link", "ContentCatalog"));
    label.push_meta(metadata, godot::RichTextLabel::META_UNDERLINE_ON_HOVER, "Inspect " + text(key.m_id));
    label.add_text(text(key.m_id));
    label.pop();
    label.pop();
}

void ContentCatalogSceneController::append_heading(godot::RichTextLabel& label, const godot::String& value) {
    label.push_color(get_theme_color("muted", "ContentCatalog"));
    label.push_font_size(14);
    label.add_text(value.to_upper());
    label.pop();
    label.pop();
    label.add_text("\n");
}

void ContentCatalogSceneController::on_category_selected() {
    if (m_rebuilding)
        return;
    if (const auto* item = m_categories->get_selected())
        this->select_category(static_cast<debug::Catalog_kind>(static_cast<int64_t>(item->get_metadata(0))));
}

void ContentCatalogSceneController::on_entry_selected(int64_t index) {
    if (m_rebuilding || index < 0 || static_cast<std::size_t>(index) >= m_entry_indices.size())
        return;
    this->select_entry(m_snapshot.m_entries[m_entry_indices[static_cast<std::size_t>(index)]].m_key);
}

void ContentCatalogSceneController::on_reference_clicked(const godot::Variant& metadata) {
    if (metadata.get_type() != godot::Variant::ARRAY)
        return;
    const godot::Array values = metadata;
    if (values.size() != 2 || values[0].get_type() != godot::Variant::INT ||
        values[1].get_type() != godot::Variant::STRING)
        return;
    const auto kind = static_cast<int64_t>(values[0]);
    if (kind < 0 || static_cast<std::size_t>(kind) >= debug::catalog_categories().size())
        return;
    const godot::String id = values[1];
    select_entry({static_cast<debug::Catalog_kind>(kind), id.utf8().get_data()});
}

void ContentCatalogSceneController::on_back_pressed() {
    if (m_history.empty())
        return;
    const auto previous = std::move(m_history.back());
    m_history.pop_back();
    this->select_entry(previous.m_key, previous.m_tab, false);
}

void ContentCatalogSceneController::on_close_pressed() {
    this->hide();
}
