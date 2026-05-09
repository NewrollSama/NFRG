/* Copyright (C) 2026 NewrollSama (https://github.com/NewrollSama)
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#pragma once
#include "../scene.hpp"
#include "NFRG/ui/list.hpp"
#include "NFRG/ui/text.hpp"

namespace nfrg::scene {

class SettingsScene : public Scene {
protected:
    enum class State : Uint8 {
        MainMenu,
        KeyBind,
        KeyBindWait,
    };
    State state_ = State::MainMenu;

    std::shared_ptr<ui::List> main_list_;
    std::shared_ptr<ui::List> key_list_;
    std::shared_ptr<ui::Text> prompt_text_;
    std::shared_ptr<ui::Text> hint_text_;
    Uint8 current_track_count_ = 4;
    Uint8 selected_key_track_ = 0;

    void refresh_main_menu();
    void rebuild_key_list();
    void enter_key_bind();
    void leave_key_bind();
    void update_hint();

    static constexpr std::array<Uint8, 4> TRACK_COUNTS = { 4, 5, 6, 7 };

public:
    SettingsScene();
    void handle_key_event(SDL_KeyboardEvent& event) override;
    void update() override {}
    void measure() override;
};

}
