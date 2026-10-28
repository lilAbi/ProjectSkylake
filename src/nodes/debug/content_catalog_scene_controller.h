#pragma once

#include "game/content/catalog.h"
#include "game/debug/catalog_inspection.h"
#include "util/console.hpp"

#include <godot_cpp/classes/button.hpp>
#include <godot_cpp/classes/item_list.hpp>
#include <godot_cpp/classes/label.hpp>
#include <godot_cpp/classes/rich_text_label.hpp>
#include <godot_cpp/classes/tab_container.hpp>
#include <godot_cpp/classes/text_edit.hpp>
#include <godot_cpp/classes/tree.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/input_event.hpp>
#include <godot_cpp/core/class_db.hpp>

class ContentCatalogSceneController : public godot::Control {
    GDCLASS(ContentCatalogSceneController, godot::Control)
    struct Selection {
        debug::CatalogKey m_key;
        int               m_tab = 0;
    };

public:
    ContentCatalogSceneController() = default;
    ~ContentCatalogSceneController() override = default;

protected:
    static void _bind_methods();

private:
    inline void get_godot_objects();
    inline void bind_signals();

public:
    void _ready() override;
    void _input(const godot::Ref<godot::InputEvent>& event) override;
    // C++-only injection. The scene owns its projection, not GameRuntime.
    void configure(const ContentCatalog& catalog);
    void configure_error(const std::string& message);

private:
    void rebuild();
    void select_category(debug::Catalog_kind kind);
    void select_entry(const debug::CatalogKey& key, int tab = 0, bool remember = true);
    void populate_entries(debug::Catalog_kind kind);
    void render_entry(const debug::CatalogEntry& entry);
    void render_fields(const debug::CatalogEntry& entry);
    void render_references(const debug::CatalogEntry& entry);
    void append_value(godot::RichTextLabel& label, const debug::CatalogEntry& entry, const nlohmann::ordered_json& value, const std::string& path);
    void append_scalar(godot::RichTextLabel& label, const debug::CatalogEntry& entry, const nlohmann::ordered_json& value, const std::string& path);
    void append_link(godot::RichTextLabel& label, const debug::CatalogKey& key);
    void append_heading(godot::RichTextLabel& label, const godot::String& value);
    void on_category_selected();
    void on_entry_selected(int64_t index);
    void on_reference_clicked(const godot::Variant& metadata);
    void on_back_pressed();
    void on_close_pressed();

private:
    Logger*                          m_logger = Logger::get();
    debug::CatalogSnapshot           m_snapshot;
    std::string                      m_error;
    std::optional<debug::CatalogKey> m_selection;
    std::vector<Selection>           m_history;
    std::vector<std::size_t>         m_entry_indices;
    std::vector<godot::TreeItem*>    m_category_items;
    bool                             m_configured = false;
    bool                             m_ready = false;
    bool                             m_rebuilding = false;

    godot::Tree*          m_categories = nullptr;
    godot::ItemList*      m_entries = nullptr;
    godot::Label*         m_summary = nullptr;
    godot::Label*         m_category_title = nullptr;
    godot::Label*         m_entry_count = nullptr;
    godot::Label*         m_cpp_type = nullptr;
    godot::Label*         m_detail_type = nullptr;
    godot::Label*         m_detail_name = nullptr;
    godot::Label*         m_detail_id = nullptr;
    godot::Label*         m_footer = nullptr;
    godot::Button*        m_back = nullptr;
    godot::Button*        m_close = nullptr;
    godot::TabContainer*  m_tabs = nullptr;
    godot::RichTextLabel* m_fields = nullptr;
    godot::RichTextLabel* m_references = nullptr;
    godot::TextEdit*      m_data = nullptr;
};