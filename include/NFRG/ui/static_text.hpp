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
#include "builder/component.hpp"

namespace nfrg::ui {

class StaticText : public Component {
    friend class Base::BuilderTemplate<StaticText>;

protected:
    /**
     * @brief 文本内容。
     */
    std::string content_;

    StaticText() = default;

public:
    class Builder;
};

class StaticText::Builder : public Component::BuilderTemplate<StaticText> {
public:
    Builder() = default;

    /**
     * @brief 设定文本内容。
     * @param content 新的文本内容。
     */
    auto set_content(std::string& content, float ptsize) -> Builder&;
    /**
     * @brief 设定文本内容。
     * @param content 新的文本内容。
     */
    auto set_content(std::string&& content, float ptsize) -> Builder&;
};

}
