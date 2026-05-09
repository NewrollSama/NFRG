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
#include "NFRG/scene/impl/settings.hpp"
using namespace nfrg;
using namespace scene;
using namespace mgr;

#include "NFRG/mgr/config.hpp"
#include "NFRG/mgr/scene.hpp"
#include "NFRG/mgr/video.hpp"
#include "NFRG/scene/impl/latency.hpp"
#include "NFRG/ui/builder/list.hpp"

#include <algorithm>
#include <format>

SettingsScene::SettingsScene() {
    main_list_ = ui::List::Builder()
                     .set_items({
                         "显示模式: 窗口化",
                         "按键设置",
                         "延迟调整: 0 ms",
                         "音符流速: 10.0",
                         "返回",
                     })
                     .set_font_size(32)
                     .set_font_color({ .r = 255, .g = 255, .b = 255, .a = 255 })
                     .set_bg_color({ .r = 0, .g = 0, .b = 0, .a = 180 })
                     .set_hl_color({ .r = 255, .g = 255, .b = 255, .a = 64 })
                     .set_max_visible(5)
                     .set_align_x(ui::AlignX::center)
                     .set_pos({ .x = 0, .y = 260 })
                     .build_and_add_to(this);

    hint_text_ = ui::Text::Builder()
                     .set_font_size(22)
                     .set_foreground_color({ .r = 200, .g = 200, .b = 200, .a = 255 })
                     .set_align_x(ui::AlignX::left)
                     .set_align_y(ui::AlignY::bottom)
                     .set_pos({ .x = 16, .y = 16 })
                     .build();
    components_.push_back(hint_text_);

    refresh_main_menu();
    update_hint();
}

void SettingsScene::refresh_main_menu() {
    constexpr std::array mode_names{ "窗口化", "无边框", "全屏" };
    main_list_->set_item_text(0, std::format("显示模式: {}", mode_names[Config::display_mode]));
    main_list_->set_item_text(2, std::format("延迟调整: {} ms", Config::latency_ms));
    main_list_->set_item_text(3, std::format("音符流速: {:.1f}", Config::spd_display));
    main_list_->refresh_items();
}

void SettingsScene::rebuild_key_list() {
    if (key_list_) {
        auto it = std::ranges::find(components_, key_list_);
        if (it != components_.end()) {
            components_.erase(it);
        }
    }

    auto& bindings = Config::get_key_bindings(current_track_count_);
    std::vector<std::string> items;
    items.push_back(std::format("轨道: {}K", current_track_count_));
    for (Uint8 i = 0; i < static_cast<Uint8>(bindings.size()); ++i) {
        items.push_back(std::format("按键 {}: {}", i + 1, SDL_GetKeyName(bindings[i])));
    }

    key_list_ = ui::List::Builder()
                    .set_items(items)
                    .set_font_size(28)
                    .set_font_color({ .r = 255, .g = 255, .b = 255, .a = 255 })
                    .set_bg_color({ .r = 0, .g = 0, .b = 0, .a = 180 })
                    .set_hl_color({ .r = 255, .g = 255, .b = 255, .a = 64 })
                    .set_max_visible(static_cast<Uint8>(items.size()))
                    .set_align_x(ui::AlignX::center)
                    .set_pos({ .x = 0, .y = 200 })
                    .build();
    components_.push_back(key_list_);
}

void SettingsScene::enter_key_bind() {
    state_ = State::KeyBind;
    current_track_count_ = 4;
    selected_key_track_ = 0;
    rebuild_key_list();
    update_hint();
}

void SettingsScene::leave_key_bind() {
    state_ = State::MainMenu;
    if (key_list_) {
        auto it = std::ranges::find(components_, key_list_);
        if (it != components_.end()) {
            components_.erase(it);
        }
        key_list_.reset();
    }
    update_hint();
}

void SettingsScene::update_hint() {
    if (!hint_text_) return;

    switch (state_) {
    case State::MainMenu:
        if (!main_list_) return;
        switch (main_list_->get_current()) {
        case 0:
            hint_text_->set_content("左/右方向键 切换显示模式");
            break;
        case 1:
            hint_text_->set_content("Enter 进入按键设置");
            break;
        case 2:
            hint_text_->set_content("左/右方向键 调整延迟  Enter 延迟校准");
            break;
        case 3:
            hint_text_->set_content("左/右方向键 调整流速");
            break;
        case 4:
            hint_text_->set_content("Enter 返回上级菜单");
            break;
        }
        break;
    case State::KeyBind:
        if (!key_list_) return;
        if (key_list_->get_current() == 0) {
            hint_text_->set_content("左/右方向键 切换轨道数  Esc 返回");
        } else {
            hint_text_->set_content("Enter 修改按键  Esc 返回");
        }
        break;
    case State::KeyBindWait:
        hint_text_->set_content("按下新按键...  Esc 取消");
        break;
    }
}

void SettingsScene::handle_key_event(SDL_KeyboardEvent& event) {
    if (!event.down) return;

    switch (state_) {
    case State::MainMenu: {
        switch (event.key) {
        case SDLK_UP:
            main_list_->up();
            update_hint();
            break;
        case SDLK_DOWN:
            main_list_->down();
            update_hint();
            break;
        case SDLK_LEFT:
        case SDLK_RIGHT: {
            const bool right = event.key == SDLK_RIGHT;
            switch (main_list_->get_current()) {
            case 0:
                if (right) {
                    Config::display_mode = (Config::display_mode + 1) % 3;
                } else {
                    Config::display_mode = (Config::display_mode + 2) % 3;
                }
                mgr::Video::apply_display_mode();
                refresh_main_menu();
                break;
            case 2:
                Config::latency_ms += right ? 1 : -1;
                Config::latency_ms = std::clamp(Config::latency_ms, -500, 500);
                refresh_main_menu();
                break;
            case 3: {
                float new_val = Config::spd_display + (right ? 0.1F : -0.1F);
                Config::spd_display = std::clamp(new_val, 1.0F, 40.0F);
                Config::spd = 10000.0F / Config::spd_display;
                refresh_main_menu();
                break;
            }
            }
            break;
        }
        case SDLK_RETURN:
            switch (main_list_->get_current()) {
            case 1:
                enter_key_bind();
                break;
            case 2:
                mgr::Scene::create<LatencyScene>();
                break;
            case 4:
                mgr::Scene::ActiveScene::quit();
                break;
            }
            break;
        case SDLK_ESCAPE:
            mgr::Scene::ActiveScene::quit();
            break;
        }
        break;
    }
    case State::KeyBind: {
        switch (event.key) {
        case SDLK_UP:
            key_list_->up();
            update_hint();
            break;
        case SDLK_DOWN:
            key_list_->down();
            update_hint();
            break;
        case SDLK_LEFT:
        case SDLK_RIGHT: {
            if (key_list_->get_current() == 0) {
                auto idx = std::ranges::find(TRACK_COUNTS, current_track_count_) - TRACK_COUNTS.begin();
                if (event.key == SDLK_RIGHT) {
                    idx = (idx + 1) % TRACK_COUNTS.size();
                } else {
                    idx = (idx + TRACK_COUNTS.size() - 1) % TRACK_COUNTS.size();
                }
                current_track_count_ = TRACK_COUNTS[idx];
                rebuild_key_list();
                update_hint();
            }
            break;
        }
        case SDLK_RETURN: {
            const auto cur = key_list_->get_current();
            if (cur >= 1 && cur - 1 < current_track_count_) {
                selected_key_track_ = static_cast<Uint8>(cur - 1);
                state_ = State::KeyBindWait;
                if (!prompt_text_) {
                    prompt_text_ = ui::Text::Builder()
                                       .set_content("按下新按键...")
                                       .set_font_size(28)
                                       .set_foreground_color({ .r = 255, .g = 255, .b = 0, .a = 255 })
                                       .set_align_x(ui::AlignX::center)
                                       .set_pos({ .x = 0, .y = static_cast<float>(mgr::Video::height) - 100 })
                                       .build();
                }
                 components_.push_back(prompt_text_);
                 update_hint();
             }
             break;
        }
        case SDLK_ESCAPE:
            leave_key_bind();
            break;
        }
        break;
    }
    case State::KeyBindWait: {
        switch (event.key) {
        case SDLK_ESCAPE:
             state_ = State::KeyBind;
             if (prompt_text_) {
                 auto it = std::find(components_.begin(), components_.end(), prompt_text_);
                 if (it != components_.end()) {
                     components_.erase(it);
                 }
             }
             update_hint();
             break;
        default:
            if (event.key >= SDLK_A && event.key <= SDLK_Z) {
                auto& bindings = Config::get_key_bindings(current_track_count_);
                if (selected_key_track_ < bindings.size()) {
                    bindings[selected_key_track_] = event.key;
                }
                 rebuild_key_list();
                 state_ = State::KeyBind;
                 if (prompt_text_) {
                     auto it = std::ranges::find(components_, prompt_text_);
                     if (it != components_.end()) {
                         components_.erase(it);
                     }
                 }
                 update_hint();
             }
             break;
        }
        break;
    }
    }
}

void SettingsScene::measure() {
    Scene::measure();
}
