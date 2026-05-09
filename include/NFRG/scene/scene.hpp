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
#include <forward_list>

#include "NFRG/anim/animator.hpp"
#include "NFRG/ui/component.hpp"
#include "NFRG/ui/group.hpp"

namespace nfrg::scene {

class Scene {
    template <typename T>
    friend class anim::Animator::BuilderTemplate;

    template <typename T>
    friend class ui::Base::BuilderTemplate;

protected:
    /**
     * @brief 存储场景中需要渲染的组件。
     */
    std::vector<std::shared_ptr<ui::Base>> components_;

    /**
     * @brief 存储场景中需要播放的动画。
     */
    std::forward_list<std::unique_ptr<anim::Animator>> animators_;

public:
    void add_animator(std::unique_ptr<anim::Animator>&& animator);

    /**
     * @brief 在每帧渲染时处理动画。
     */
    virtual void process_animators(Uint64 delta_ns);

    /**
     * @brief 在键盘事件产生时进行处理。
     * @param event 一次键盘事件。
     */
    virtual void handle_key_event(SDL_KeyboardEvent& event);

    /**
     * @brief 每帧刷新时调用，可以自定义每帧需要执行的逻辑。
     */
    virtual void update() = 0;

    /**
     * @brief 测量所有组件的渲染偏移。
     */
    virtual void measure();

    /**
     * @brief 渲染场景中的组件。
     *
     * @return 渲染是否成功。
     */
    [[nodiscard]] virtual auto render() const -> bool;

    virtual ~Scene();
};

}
