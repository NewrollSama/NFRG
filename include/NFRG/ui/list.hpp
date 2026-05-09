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
#include "NFRG/ui/builder/flex.hpp"
#include "NFRG/ui/text.hpp"

namespace nfrg::ui {

class List : public Flex {
    template <typename T>
    friend class Base::BuilderTemplate;

protected:
    Uint16 scroll_offset_ = 0;
    Uint16 current_index_ = 0;
    Uint8 max_visible_ = 3;
    SDL_Color bg_color_{ .r = 255, .g = 255, .b = 255, .a = 128 };
    SDL_Color hl_color_{ .r = 255, .g = 255, .b = 255, .a = 255 };
    float font_size_ = 32;
    SDL_Color font_color_{ .r = 0, .g = 0, .b = 0, .a = 255 };
    std::vector<std::string> items_;
    std::vector<ui::Text*> slots_;

    List() = default;

    [[nodiscard]] auto get_visible_count() const -> Uint8;
    void full_update_text();
    void scroll_up();
    void scroll_to_top();
    void scroll_down();
    void scroll_to_bottom();

public:
    template <typename T>
    class BuilderTemplate;

    class Builder;

    void up();
    void down();
    void set_item_text(size_t index, std::string_view text);
    void refresh_items();
    [[nodiscard]] auto get_current() const -> Uint16;
    [[nodiscard]] auto render() const -> bool override;
};

template <typename T>
class List::BuilderTemplate : public Flex::BuilderTemplate<T> {
public:
    BuilderTemplate() = default;
    auto set_items(std::vector<std::string> items) -> T::Builder&;
    auto set_max_visible(Uint8 n) -> T::Builder&;
    auto set_font_size(float ptsize) -> T::Builder&;
    auto set_font_color(SDL_Color color) -> T::Builder&;
    auto set_bg_color(SDL_Color color) -> T::Builder&;
    auto set_hl_color(SDL_Color color) -> T::Builder&;
    auto build() -> std::shared_ptr<T> override;
};

class List::Builder : public List::BuilderTemplate<List> {};

}
