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
#include <unordered_map>

namespace nfrg::mgr {

class Font {
private:
    static std::unordered_map<float, TTF_Font*> fonts;  // 不同字体尺寸下的 TTF_Font。
    static TTF_TextEngine* text_engine;                 // SDL_ttf 提供的文本引擎，创建 TTF_Text 时使用。

public:
    /**
     * @brief 应用初始化时进行的字体初始化。
     */
    static void open_font();

    /**
     * @brief 以给定的文本创建纹理。
     * @param content 文本内容。
     * @param ptsize 字体尺寸。
     * @param color 字体颜色。
     * @return 创建的纹理。
     */
    static auto create_texture(std::string& content, float ptsize, SDL_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 }) -> SDL_Texture*;
    /**
     * @brief 以给定的文本创建 TTF_Text。
     * @param content 文本内容。
     * @param ptsize 字体尺寸。
     * @return 创建的 TTF_Text。
     */
    static auto create_text(std::string_view content, float ptsize = 48) -> TTF_Text*;

    /**
     * @brief 获取特定字体尺寸的 TTF_Font。
     *
     * @param ptsize 字体尺寸。
     * @return 该字体尺寸的 TTF_Font。
     */
    static auto get_font(float ptsize) -> TTF_Font*;

    /**
     * @brief 应用退出时的处理。
     */
    static void close_font();
};

}
