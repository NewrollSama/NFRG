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
#include "../builder/component.hpp"

namespace nfrg::ui::gameplay {

class Lane : public Component {
    friend class Base::BuilderTemplate<Lane>;

protected:
    /**
     * @brief 长条顶端的 y 轴位置。
     *
     */
    float top_ = 0.F;
    /**
     * @brief 长条底端的 y 轴位置。
     *
     */
    float bottom_ = 0.F;

    Lane() = default;

public:
    class Builder;

    /**
     * @brief 根据传入的音符 y 轴位置，计算长条渲染所需的数据。
     *
     * @param y 音符 y 轴位置。
     */
    void calc(float y);

    [[nodiscard]] auto render() const -> bool override;
};

class Lane::Builder : public Component::BuilderTemplate<Lane> {
public:
    Builder() = default;
};

}
