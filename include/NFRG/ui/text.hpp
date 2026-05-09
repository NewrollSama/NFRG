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

#include "NFRG/ui/builder/component.hpp"

namespace nfrg::ui {

class Text : public Component {
    friend class Base::BuilderTemplate<Text>;

protected:
    /**
     * @brief 实际的 TTF_Text。
     */
    TTF_Text* text_ = nullptr;

    Text() = default;

    void calc_size();

public:
    class Builder;

    /**
     * @brief 设定文本内容。
     * @param content 新的文本内容。
     */
    void set_content(std::string_view content);

    /**
     * @brief 设定文本颜色。
     * @param color 新的文本颜色。
     */
    void set_color(SDL_Color color);

    [[nodiscard]] auto render() const -> bool override;

    virtual ~Text();
};

class Text::Builder : public Component::BuilderTemplate<Text> {
protected:
    std::string content_;
    float ptsize_ = 48.F;

public:
    Builder();

    /**
     * @brief 设定文本内容。
     * @param content 新的文本内容。
     */
    auto set_content(std::string_view content) -> Builder&;

    /**
     * @brief 设定字体尺寸。
     * @param ptsize 要设置的字体尺寸
     */
    auto set_font_size(float ptsize) -> Builder&;

    auto build() -> std::shared_ptr<Text> override;
};

}
