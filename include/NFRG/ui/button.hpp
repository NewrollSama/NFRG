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
#include "builder/group.hpp"

#include "text.hpp"

namespace nfrg::ui {

class Button : public Group {
    friend class Base::BuilderTemplate<Button>;

protected:
    ui::Text* text_;
    SDL_Color bg_color_ = { .r = 0, .g = 0, .b = 0, .a = 0 };

    Button() = default;

public:
    class Builder;

    void measure(SDL_FPoint offset) override;
    [[nodiscard]] auto render() const -> bool override;
};

class Button::Builder : public Group::BuilderTemplate<Button> {
public:
    Builder() = default;

    auto set_text(std::shared_ptr<ui::Text>&& text_obj) -> Builder&;
    // auto set_text(std::string&& content) -> Builder&;

    auto set_background_color(SDL_Color bg_color) -> Builder&;
};

}