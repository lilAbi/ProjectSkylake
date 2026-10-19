#ifndef PROJECTSKYLAKE_DTO_H
#define PROJECTSKYLAKE_DTO_H

#include <string>
#include <cstdint>
#include <map>
#include <optional>
#include <vector>

struct PlayerDTO {
    std::string m_profile_id;
    std::string m_display_name;
    std::string m_master_seed_hex;
};

struct GenerationDTO {
    std::string   m_seed_hex;
    std::string   m_content_version;
    std::uint32_t m_generator_version{1};
};

struct StatModifierDTO {
    std::string  m_stat;
    std::string  m_operation;
    std::string  m_source_kind;
    std::string  m_source_key;
    std::int64_t m_value{};
    std::int32_t m_priority{};
};

struct SkillNodeDTO {
    std::int64_t              m_id{};
    std::string               m_ability;
    std::vector<std::int64_t> m_prerequisites;
    std::int32_t              m_point_cost{};
};

struct CharacterDTO {
    std::int64_t                        m_id{};
    std::string                         m_display_name;
    std::string                         m_class_id;
    std::map<std::string, std::int64_t> m_rolled_stats;
    std::vector<std::string>            m_traits;
    std::vector<SkillNodeDTO>           m_skill_nodes;
    std::vector<std::int64_t>           m_unlocked_skills;
    std::int32_t                        m_available_skill_points{};
    std::int32_t                        m_level{1};
    std::int64_t                        m_experience{};
    std::map<std::string, std::int64_t> m_equipment;
    GenerationDTO                       m_generation;
};

struct ItemDTO {
    std::int64_t                 m_id{};
    std::string                  m_definition;
    std::int32_t                 m_level{1};
    std::vector<StatModifierDTO> m_affixes;
    GenerationDTO                m_generation;
};

struct TeamDTO {
    std::int64_t                             m_id{};
    std::string                              m_name;
    std::vector<std::optional<std::int64_t>> m_slots;
};

struct BuildingDTO {
    std::int64_t m_id{};
    std::string  m_definition;
    std::int32_t m_level{1};
};

struct IdleAssignmentDTO {
    std::int64_t m_id{};
    std::int64_t m_team{};
    std::int64_t m_checkpoint_unix_seconds{};
    std::string  m_activity;
};

struct IdentityCountersDTO {
    std::int64_t m_next_character{1};
    std::int64_t m_next_item{1};
    std::int64_t m_next_team{1};
    std::int64_t m_next_building{1};
    std::int64_t m_next_battle{1};
    std::int64_t m_next_receipt{1};
    std::int64_t m_next_idle_assignment{1};
};

struct RewardReceiptDTO {
    std::int64_t                         m_id{};
    std::int64_t                         m_battle{};
    std::map<std::string, std::int64_t>  m_resources;
    std::map<std::int64_t, std::int64_t> m_experience;
    std::vector<std::int64_t>            m_items;
};

struct RosterDTO {
    std::vector<CharacterDTO> m_characters;
    std::vector<TeamDTO>      m_teams;
};

struct InventoryDTO {
    std::vector<ItemDTO> m_items;
    std::int64_t         m_capacity{};
};

struct WalletDTO {
    std::map<std::string, std::int64_t> m_balances;
};

struct CampDTO {
    std::vector<BuildingDTO> m_buildings;
};

struct ProgressionDTO {
    std::int64_t             m_highest_completed{};
    std::string              m_region;
    std::vector<std::string> m_unlocks;
};

struct IdleProgressionDTO {
    std::vector<IdleAssignmentDTO>      m_assignments;
    std::map<std::string, std::int64_t> m_pending;
};

struct SaveGameDTO {
    std::uint32_t                 m_schema_version{1};
    std::string                   m_content_version;
    std::int64_t                  m_saved_at_unix_seconds{};
    PlayerDTO                     m_player;
    RosterDTO                     m_roster;
    InventoryDTO                  m_inventory;
    WalletDTO                     m_wallet;
    CampDTO                       m_camp;
    ProgressionDTO                m_progression;
    IdleProgressionDTO            m_idle;
    IdentityCountersDTO           m_identities;
    std::vector<RewardReceiptDTO> m_reward_receipts;
};

#endif //PROJECTSKYLAKE_DTO_H
