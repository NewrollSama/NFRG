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
#include "../scene.hpp"

namespace nfrg {

namespace ui {
class Text;
}

namespace scene {

class LatencyScene : public Scene {
private:
    /**
     * @brief 一次延迟检查的流程。
     */
    class CheckProcedure;
    std::unique_ptr<CheckProcedure> cp_;

    /**
     * @brief 显示延迟的文本组件。
     */
    std::weak_ptr<ui::Text> text_latency_;

    /**
     * @brief 反复执行延迟检查的定时器。
     */
    SDL_TimerID timer_ = 0;

public:
    LatencyScene();
    void handle_key_event(SDL_KeyboardEvent& event) override;
    void update() override;
    ~LatencyScene() override;
};

}
}
