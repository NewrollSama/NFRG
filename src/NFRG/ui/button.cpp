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
#include "NFRG/ui/button.hpp"
using namespace nfrg;
using namespace ui;

void Button::measure(SDL_FPoint offset) {
    bound_.w = text_->get_size().x + 2 * padding_.x;
    bound_.h = text_->get_size().y + 2 * padding_.y;

    Group::measure(offset);
}

auto Button::render() const -> bool {
    SDL_SetRenderDrawBlendMode(mgr::Video::renderer, SDL_BLENDMODE_BLEND);
    mgr::Video::set_render_draw_color(bg_color_);
    SDL_FRect bound{ .x = bound_.x + render_offset_.x, .y = bound_.y + render_offset_.y, .w = bound_.w, .h = bound_.h };
    SDL_RenderFillRect(mgr::Video::renderer, &bound);
    return Group::render();
}

auto Button::Builder::set_text(std::shared_ptr<ui::Text>&& text_obj) -> Button::Builder& {
    component_->text_ = text_obj.get();
    component_->components_.emplace_back(std::move(text_obj));
    return *this;
}

auto Button::Builder::set_background_color(SDL_Color bg_color) -> Button::Builder& {
    component_->bg_color_ = bg_color;
    return *this;
}
