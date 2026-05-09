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
#include <stack>

namespace nfrg {

namespace scene {
class Scene;
}

namespace mgr {

/**
 * @brief 场景管理器类。
 */
class Scene {
private:
    /**
     * @brief 存储场景。
     */
    static std::stack<std::unique_ptr<scene::Scene>> scenes;

public:
    /**
     * @brief 对当前场景进行操作。
     */
    class ActiveScene {
    public:
        /**
         * @brief 获取当前场景的指针。
         * @return 当前场景的指针。
         */
        static auto get() -> scene::Scene*;

        /**
         * @brief 处理当前场景的动画器。
         */
        static void process_animators(Uint64 delta_ns);


        /**
         * @brief 调用当前场景的 update 函数。
         */
        static void update();

        /**
         * @brief 通知场景对组件进行计算。
         */
        static void measure();

        /**
         * @brief 渲染当前场景。
         * @return 渲染是否成功。
         */
        static auto render() -> bool;

        /**
         * @brief 处理键盘按键事件。
         * @param event 来自 SDL 的键盘事件。
         */
        static void handle_key_event(SDL_KeyboardEvent& event);

        /**
         * @brief 退出并销毁当前场景。
         */
        static void quit();
    };

    /**
     * @brief 创建一个场景。
     * @tparam T 场景类。
     * @tparam ...Args 构造场景使用的参数类型。
     * @param ...args 构造场景使用的参数。
     */
    template <typename T, typename... Args>
    static void create(Args&&... args) {
        static_assert(std::is_base_of_v<scene::Scene, T>, "T not derived.");
        scenes.emplace(std::make_unique<T>(std::forward<Args>(args)...));
    }

    /**
     * @brief 退出时的清理操作。
     */
    static void cleanup();
};

}
}
