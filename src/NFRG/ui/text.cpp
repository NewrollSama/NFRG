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
#include "NFRG/ui/text.hpp"
using namespace nfrg;
using namespace ui;

#include "NFRG/mgr/font.hpp"

void Text::calc_size() {
    int w, h;
    TTF_GetTextSize(text_, &w, &h);
    bound_.w = static_cast<float>(w), bound_.h = static_cast<float>(h);
}

void Text::set_content(std::string_view content) {
    TTF_SetTextString(text_, content.data(), content.size());
    calc_size();
}

void Text::set_color(SDL_Color color) {
    fg_color_ = color;
    TTF_SetTextColor(text_, color.r, color.g, color.b, color.a);
}

auto Text::render() const -> bool {
    bool res = TTF_DrawRendererText(text_, bound_.x + render_offset_.x, bound_.y + render_offset_.y);
    return res;
}

Text::~Text() {
    TTF_DestroyText(text_);
}

Text::Builder::Builder() {
    component_->text_ = mgr::Font::create_text("", 48);
}

auto Text::Builder::set_content(std::string_view content) -> Text::Builder& {
    TTF_SetTextString(component_->text_, content.data(), content.size());
    component_->calc_size();
    return *this;
}

auto Text::Builder::set_font_size(float ptsize) -> Text::Builder& {
    TTF_SetTextFont(component_->text_, mgr::Font::get_font(ptsize));
    component_->calc_size();
    return *this;
}

auto Text::Builder::build() -> std::shared_ptr<Text> {
    TTF_SetTextColor(component_->text_,
        component_->fg_color_.r, component_->fg_color_.b,
        component_->fg_color_.b, component_->fg_color_.a);
    return BuilderTemplate<Text>::build();
}
