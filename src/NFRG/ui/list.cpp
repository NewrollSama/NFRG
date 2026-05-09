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
#include "NFRG/ui/list.hpp"
using namespace nfrg;
using namespace ui;

auto List::get_visible_count() const -> Uint8 {
    return static_cast<Uint8>(std::min(static_cast<size_t>(max_visible_), items_.size()));
}

void List::full_update_text() {
    for (Uint16 i = 0; i < get_visible_count(); ++i) {
        slots_[i]->set_content(items_[scroll_offset_ + i]);
    }
}

void List::scroll_up() {
    --scroll_offset_;
}

void List::scroll_to_top() {
    scroll_offset_ = 0;
    current_index_ = 0;
}

void List::scroll_down() {
    ++scroll_offset_;
}

void List::scroll_to_bottom() {
    scroll_offset_ = static_cast<Uint16>(items_.size()) - get_visible_count();
    current_index_ = get_visible_count() - 1;
}

void List::set_item_text(size_t index, std::string_view text) {
    if (index < items_.size()) {
        items_[index] = text;
    }
}

void List::refresh_items() {
    full_update_text();
}

void List::up() {
    if (current_index_ - 1 < 0) {
        if (scroll_offset_ > 0) {
            scroll_up();
        } else {
            scroll_to_bottom();
        }
        full_update_text();
    } else {
        --current_index_;
    }
}

void List::down() {
    if (current_index_ + 1 >= get_visible_count()) {
        if (scroll_offset_ < items_.size() - get_visible_count()) {
            scroll_down();
        } else {
            scroll_to_top();
        }
        full_update_text();
    } else {
        ++current_index_;
    }
}

auto List::get_current() const -> Uint16 {
    return scroll_offset_ + current_index_;
}

auto List::render() const -> bool {
    SDL_SetRenderDrawBlendMode(mgr::Video::renderer, SDL_BLENDMODE_BLEND);
    const SDL_FPoint pos = get_pos();
    const float x = pos.x + render_offset_.x;
    const float y = pos.y + render_offset_.y;
    SDL_FRect bound{ .x = x, .y = y, .w = bound_.w, .h = bound_.h };
    mgr::Video::set_render_draw_color(bg_color_);
    SDL_RenderFillRect(mgr::Video::renderer, &bound);
    auto visible = get_visible_count();
    float slot_h = bound.h / static_cast<float>(visible);
    bound.h = slot_h;
    bound.y += current_index_ * slot_h;
    mgr::Video::set_render_draw_color(hl_color_);
    SDL_RenderFillRect(mgr::Video::renderer, &bound);
    return Flex::render();
}
