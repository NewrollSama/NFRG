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
#include "NFRG/ui/splash/icon.hpp"

namespace nfrg::scene {

class SplashScene : public Scene {
protected:
    Uint64 last_tick_ns_ = 0;
    std::shared_ptr<ui::splash::Icon> icon_;
    std::shared_ptr<ui::List> menu_list_;
    bool menu_shown_ = false;

public:
    SplashScene();
    void handle_key_event(SDL_KeyboardEvent& event) override;
    void update() override;
};

}
