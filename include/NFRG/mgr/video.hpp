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

namespace nfrg::mgr {

class Video {
public:
    static SDL_Window* window;
    static SDL_Renderer* renderer;
    static int width;   // 窗口宽度。
    static int height;  // 窗口高度。

    /**
     * @brief 获取窗口尺寸。
     * @return 窗口尺寸。
     */
    static auto get_window_size() -> SDL_FPoint;
    /**
     * @brief 设置渲染器绘制颜色。
     * @param color 待设置的颜色。
     */
    static void set_render_draw_color(SDL_Color color);
    /**
     * @brief 应用设定的显示模式。
     */
    static void apply_display_mode();
};

}
