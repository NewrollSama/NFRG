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
#include <vector>

namespace nfrg::mgr {

class Config {
public:
    static float spd;                        // 音符从出现到接触判定线的时间，单位为毫秒。
    static float spd_display;                // 显示的速度值，与 spd 成反比。
    static Uint8 display_mode;               // 显示模式。
    static Sint32 latency_ms;                // 延迟调整中的值。
    static std::vector<SDL_Keycode> key_4k;  // 4K 下的按键映射。
    static std::vector<SDL_Keycode> key_5k;  // 5K 下的按键映射。
    static std::vector<SDL_Keycode> key_6k;  // 6K 下的按键映射。
    static std::vector<SDL_Keycode> key_7k;  // 7K 下的按键映射。

    static auto get_key_bindings(Uint8 tracks) -> std::vector<SDL_Keycode>&;  // 获取特定轨道数的按键映射。
};

}
