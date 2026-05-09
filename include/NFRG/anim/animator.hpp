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

namespace nfrg {

namespace ui {
class Component;
}

namespace anim {

/**
 * @brief 动画器基类。
 */
class Animator {
    // friend class ui::Component;

protected:
    /**
     * @brief 动画器是否只使用一次。
     *
     * 若为 true，则在该动画器结束时自动销毁。
     */
    bool disposable_ = true;

    /**
     * @brief 动画器所操作的组件。
     */
    std::weak_ptr<ui::Component> component_;

    /**
     * @brief 动画持续时间。
     */
    float duration_ = 0.0f;

    /**
     * @brief 动画已过时间。
     */
    Uint64 elapsed_ns_ = 0;

    /**
     * @brief 动画重映射函数。
     *
     * 默认动画是线性动画，若要实现诸如缓动等自定义效果，需要覆盖此函数。
     *
     * 重映射函数 f(x) 指示了动画在特定时间进度 "x" 时的实际进度 "f(t)"。
     * 对于其中所提到的两个进度，我们约定：\n
     * - 时间进度：动画在时间上运行了多少，例如动画持续时间为 1 秒，已运行
     *   0.5 秒，该进度则为 0.5。\n
     * - 实际进度：动画的效果运行了多少，例如物体从 (0, 0) 移动到 (1, 1)，
     *   不论动画已经运行了多久，当其处在 (0.6, 0.6) 时，该进度则为 0.6。
     *
     * @param x 动画的时间进度，范围为 0 ~ 1。
     * @return 动画的实际进度，范围为 0 ~ 1。
     */
    double (*remap_func_)(double x) = [](double x) { return x; };

    Animator() = default;

    /**
     * @brief 计算动画的当前进度。
     * @return 动画的当前进度，范围为 0 ~ 1。
     */
    [[nodiscard]] auto calc_progress() const -> double;

public:
    /**
     * @brief 动画器基类的构造器。
     */
    template <typename T>
    class BuilderTemplate;

    /**
     * @brief 为动画器绑定组件。
     * @param component 待绑定的组件。
     */
    void bind_component(const std::weak_ptr<ui::Component>&& component);

    /**
     * @brief 启动动画器。
     * @param initial_ns 初始已过时间，用于补偿动画激活延迟，单位为纳秒。
     */
    void start(Uint64 initial_ns = 0);

    /**
     * @brief 派生类运行动画的具体操作。
     * @param delta_ns 距离上次处理的间隔时间，单位为纳秒。
     * @return 动画是否结束。在动画结束时，返回 true。
     */
    virtual auto process(Uint64 delta_ns) -> bool = 0;

    virtual ~Animator() = default;
};

}
}
