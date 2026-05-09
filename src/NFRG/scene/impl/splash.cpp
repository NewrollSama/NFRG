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
#include "NFRG/scene/impl/splash.hpp"
using namespace nfrg;
using namespace scene;

#include "NFRG/mgr/scene.hpp"
#include "NFRG/scene/impl/latency.hpp"
#include "NFRG/scene/impl/select.hpp"
#include "NFRG/scene/impl/settings.hpp"
#include "NFRG/ui/splash/icon.hpp"

SplashScene::SplashScene() {
    icon_ = ui::splash::Icon::Builder()
                .set_bound({ .x = 512, .y = (1080 - 512) / 2, .w = 512, .h = 512 })
                .set_align_y(ui::AlignY::center)
                .build_and_add_to(this);

    menu_list_ = ui::List::Builder()
                     .set_items({ "选曲", "设置", "退出" })
                     .set_font_size(36)
                     .set_max_visible(3)
                     .set_pos({ .x = 1152, .y = 0 })
                     .set_align_y(ui::AlignY::center)
                     .set_bg_color({ .r = 0, .g = 0, .b = 0, .a = 255 })
                     .set_hl_color({ .r = 255, .g = 255, .b = 255, .a = 64 })
                     .set_font_color({ .r = 255, .g = 255, .b = 255, .a = 255 })
                     .build();
}

void SplashScene::handle_key_event(SDL_KeyboardEvent& event) {
    if (!event.down) return;
    if (menu_list_ && menu_shown_) {
        switch (event.key) {
        case SDLK_UP:
            menu_list_->up();
            return;
        case SDLK_DOWN:
            menu_list_->down();
            return;
        case SDLK_RETURN: {
            switch (menu_list_->get_current()) {
            case 0:
                mgr::Scene::create<scene::SelectScene>();
                return;
            case 1:
                mgr::Scene::create<scene::SettingsScene>();
                return;
            case 2: {
                SDL_Event quit_event{ SDL_EVENT_QUIT };
                SDL_PushEvent(&quit_event);
                return;
            }
            }
        }
        }
    }
    switch (event.key) {
    case SDLK_SLASH: {
        mgr::Scene::create<scene::LatencyScene>();
        break;
    }
    }
}

void SplashScene::update() {
    auto now = SDL_GetTicksNS();
    if (last_tick_ns_ == 0) {
        last_tick_ns_ = now;
        return;
    }
    Uint64 delta = now - last_tick_ns_;
    last_tick_ns_ = now;
    if (icon_) icon_->update(delta);
    if (!menu_shown_ && icon_->all_settled()) {
        components_.push_back(menu_list_);
        menu_shown_ = true;
    }
}
