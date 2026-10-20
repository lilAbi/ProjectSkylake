#pragma once

#include <string>

namespace core::inline labels {

    //defined signals
    namespace signal {

        namespace TitleSceneController {
            constexpr inline auto new_game_requested{"new_game_requested"};
            constexpr inline auto load_game_requested{"load_game_requested"};
        }

        namespace SaveSceneController {
            constexpr inline auto game_title_requested{"game_title_requested"};
            constexpr inline auto start_game_requested{"start_game_requested"};
        }

        //godot "Button" node signals
        namespace BaseButton {
            constexpr inline auto pressed{"pressed"};
            constexpr inline auto mouse_entered{"mouse_entered"};
            constexpr inline auto mouse_exited{"mouse_exited"};
            constexpr inline auto button_down{"button_down"};
            constexpr inline auto button_up{"button_up"};
        }

        //godot "Control" node signals
        namespace Control {
            constexpr inline auto gui_input{"gui_input"};
            constexpr inline auto mouse_entered{"mouse_entered"};
            constexpr inline auto mouse_exited{"mouse_exited"};
        }

        namespace Timer {
            constexpr inline auto timeout{"timeout"};
        }

        namespace Transition {
            constexpr inline auto load_character_creator{"load_character_creator"};
            constexpr inline auto load_game_serialization{"load_game_serialization"};
            constexpr inline auto load_battle_menu{"load_battle_menu"};
            constexpr inline auto load_tactics_battle{"load_tactics_battle"};
            constexpr inline auto load_home{"load_home"};
            constexpr inline auto start_tactics_battle{"start_tactics_battle"};
            constexpr inline auto open_player_roster{"open_player_roster"};
        }

        namespace Tween {
            constexpr inline auto finished{"finished"};
        }

        namespace RichTextLabel {
            constexpr inline auto meta_clicked{"meta_clicked"};
        }

    }

    //labels of nodes in a scene
    namespace node {

        namespace CampUI {
            constexpr inline auto BarracksButton{"%BarracksButton"};
            constexpr inline auto WarTableButton{"%WarTableButton"};
            constexpr inline auto ForgeButton{"%ForgeButton"};
            constexpr inline auto CampfireButton{"%CampfireButton"};
            constexpr inline auto AdventureButton{"%AdventureButton"};
            constexpr inline auto ShrineButton{"%ShrineButton"};
        }

        namespace Main {
            constexpr inline auto ScreenRoot{"%ScreenRoot"};
            constexpr inline auto ActivityRoot{"%ActivityRoot"};
            constexpr inline auto SceneCoordinator{"%SceneCoordinator"};
            constexpr inline auto SaveController{"%SaveController"};
            constexpr inline auto AudioController{"%AudioController"};
            constexpr inline auto InputController{"%InputController"};
        }

        namespace Title {
            constexpr inline auto NewGameButton{"%NewGameButton"};
            constexpr inline auto LoadButton{"%LoadButton"};
            constexpr inline auto SettingsButton{"%SettingsButton"};
            constexpr inline auto QuitButton{"%QuitButton"};
        }

        namespace Save {
            constexpr inline auto Slot1Button{"%Slot1Button"};
            constexpr inline auto Slot2Button{"%Slot2Button"};
            constexpr inline auto Slot3Button{"%Slot3Button"};
            constexpr inline auto BackButton{"%BackButton"};
        }

    } //END OF "NODE" NAMESPACE

    namespace animations {
    }

    //paths to files
    namespace path {
        //.tscn related files
        namespace scene {

            namespace ui {

                namespace camp {
                    constexpr inline auto camp_ui{"res://scenes/ui/camp/camp_ui.tscn"};
                }

                namespace title {
                    constexpr inline auto title{"res://scenes/ui/title/title.tscn"};
                    constexpr inline auto save{"res://scenes/ui/title/save.tscn"};
                }

            }

        }

        //.tres related files
        namespace resource {

        }

        //asset related files
        namespace asset { //possibly can remove

        }

        namespace folder { //possibly can remove

        }

    }

    enum class LayerID : uint32_t {
        Player = 0x00000001,
        NPCs = 0x00000002,
        Projectiles = 0x00000004,
        Walls = 0x00000008,
        DamageZones = 0x00000010,
        DeathZones = 0x00000020,
        PhysicsObjects = 0x00000040,
        Layer08 = 0x00000080,
        Layer09 = 0x00000100,
        Layer10 = 0x00000200,
        Layer11 = 0x00000400,
        Layer12 = 0x00000800,
        Layer13 = 0x00001000,
        Layer14 = 0x00002000,
        Layer15 = 0x00004000,
        Layer16 = 0x00008000,
    };

}