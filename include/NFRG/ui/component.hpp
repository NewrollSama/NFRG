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
#include "base.hpp"

namespace nfrg {

namespace anim {
class Animator;
class MotionAnimator;
}

namespace ui {

/**
 * @brief UI 组件的共同基类。
 */
class Component : public Base {
    friend class anim::MotionAnimator;

protected:
    /**
     * @brief 组件的纹理。
     */
    SDL_Texture* texture_ = nullptr;

    /**
     * @brief 组件的前景色。默认为白色不透明。
     */
    SDL_Color fg_color_{ .r = 255, .g = 255, .b = 255, .a = SDL_ALPHA_OPAQUE };

    /**
     * @brief 组件旋转角度。
     *
     * @warning 实验性功能，也许不可用。
     */
    double r_ = 0.0;

    /**
     * @brief 标记组件是否需要重绘。
     */
    bool dirty_ = true;

    /**
     * @brief 标记组件是否持有纹理，如果是，则会自动在组件析构时销毁纹理。
     */
    bool own_texture_ = true;

    Component() = default;

    void set_texture(SDL_Texture* texture, bool set_size = true);

public:
    /**
     * @brief UI 组件基类的建造者。
     * @tparam T 传入待建造的组件类，便于暂存实际类型的组件。
     */
    template <typename T>
    class BuilderTemplate;

    /**
     * @brief 设定该组件对象的位置。
     * @param 待设定的位置。
     */
    void set_pos(SDL_FPoint pos);

    /**
     * @brief 设定该组件对象的宽高。
     * @param 待设定的宽高。
     */
    void set_size(SDL_FPoint size);

    /**
     * @brief 设定该组件对象的位置和宽高。
     * @param 待设定的位置和宽高。
     */
    void set_bound(SDL_FRect bound);

    /**
     * @brief 设定该组件对象的不透明度。
     *
     * 若组件不使用 texture 进行渲染，可能无效。
     *
     * @param alpha 待设定的不透明度。
     */
    void set_alpha(Uint8 alpha);

    /**
     * @brief 检查组件是否需要重排、重绘。
     * @return 表示组件是否需要重绘的布尔值。
     */
    [[nodiscard]] auto is_dirty() const -> bool;

    /**
     * @brief 计算由容器嵌套带来的渲染偏移。
     */
    void measure(SDL_FPoint offset) override;

    /**
     * @brief 对于有纹理的组件的通用渲染方式。
     * @return 渲染是否成功。
     */
    [[nodiscard]] auto render() const -> bool override;

    virtual ~Component();
};

}
}
