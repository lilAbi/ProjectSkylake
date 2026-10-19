#ifndef PROJECTSKYLAKE_VIEWS_H
#define PROJECTSKYLAKE_VIEWS_H

#include "game/character/character.h"
#include "game/character/skills.h"
#include "game/character/stats.h"
#include "game/core/ids.h"
#include "game/economy/resources.h"
#include "game/items/item.h"
#include "game/roster/team.h"
#include "game/state/game_state.h"

#include <vector>

struct CharacterSummaryView {
    CharacterID  m_id;
    std::string  m_display_name;
    ClassID      m_class_id;
    std::int32_t m_level{};
};

struct CharacterDetailsView {
    CharacterSummaryView m_summary;
    StatBlock            m_stats;
    std::vector<TraitID> m_traits;
    SkillTree            m_tree;
    CharacterProgression m_progression;
    Equipment            m_equipment;
};

struct RosterView {
    Revision                          m_revision;
    std::vector<CharacterSummaryView> m_characters;
    std::vector<Team>                 m_teams;
};

struct InventoryView {
    Revision                  m_revision;
    std::vector<ItemInstance> m_items;
    std::int64_t              m_capacity{};
};

struct BannerView {
    BannerID     m_id;
    ResourceCost m_cost_per_summon;
    bool         m_affordable{};
    bool         m_unlocked{};
};

struct RecipeView {
    RecipeID     m_id;
    ResourceCost m_cost;
    bool         m_affordable{};
    bool         m_unlocked{};
};

struct CampView {
    Revision                      m_revision;
    std::vector<BuildingInstance> m_buildings;
    ResourceBundle                m_resources;
};

struct ProgressionView {
    Revision              m_revision;
    FloorNumber           m_highest_completed;
    FloorNumber           m_next_floor;
    std::vector<RegionID> m_unlocked_regions;
};

struct IdleView {
    Revision                    m_revision;
    std::vector<IdleAssignment> m_assignments;
    ResourceBundle              m_pending;
};

#endif //PROJECTSKYLAKE_VIEWS_H
