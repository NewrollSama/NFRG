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

namespace nfrg::ui {

enum class AlignX : Uint8 {
    left,
    center,
    right
};

enum class AlignY : Uint8 {
    top,
    center,
    bottom
};

/**
 * @brief 界面元素的基类。
 */
class Base {
protected:
    /**
     * @brief 界面元素的位置与宽高。
     */
    SDL_FRect bound_{ .x = 0.F, .y = 0.F, .w = 0.F, .h = 0.F };
    /**
     * @brief 实际渲染时应用的偏移。
     */
    SDL_FPoint render_offset_{ .x = 0.F, .y = 0.F };
    /**
     * @brief 界面元素位置中横轴对齐的方向。若为 center，设定的 x 将被忽略。
     */
    AlignX align_x_ = AlignX::left;
    /**
     * @brief 界面元素位置中纵轴对齐的方向。若为 center，设定的 y 将被忽略。
     */
    AlignY align_y_ = AlignY::top;

public:
    auto operator=(Base&& other) noexcept -> Base& = default;

    template <typename T>
    class BuilderTemplate;

    /**
     * @brief 获取界面元素的位置。
     * @return 该界面元素的位置。
     */
    [[nodiscard]] virtual auto get_pos() const -> SDL_FPoint;

    /**
     * @brief 获取界面元素的宽高。
     * @return 该界面元素的宽高，其中 x 为宽，y 为高。
     */
    [[nodiscard]] virtual auto get_size() const -> SDL_FPoint;

    /**
     * @brief 获取界面元素的位置和宽高。
     * @return 该界面元素的位置和宽高，其中 x 为宽，y 为高。
     */
    [[nodiscard]] virtual auto get_bound() const -> SDL_FRect;

    /**
     * @brief 设定界面元素的横轴对齐方向。
     * @param align 待设定的横轴对齐方向。
     */
    void set_align_x(AlignX align);
    /**
     * @brief 设定界面元素的纵轴对齐方向。
     * @param align 待设定的纵轴对齐方向。
     */
    void set_align_y(AlignY align);

    [[nodiscard]] auto get_align_x() const -> AlignX;
    [[nodiscard]] auto get_align_y() const -> AlignY;

    virtual void measure(SDL_FPoint offset) = 0;
    [[nodiscard]] virtual auto render() const -> bool = 0;

    virtual ~Base() = default;
};

}