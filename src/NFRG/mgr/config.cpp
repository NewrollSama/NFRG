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
#include "NFRG/mgr/config.hpp"
using namespace nfrg;
using namespace mgr;

float Config::spd = 1000.F;
float Config::spd_display = 10.0F;
Uint8 Config::display_mode = 0;
Sint32 Config::latency_ms = 0;
std::vector<SDL_Keycode> Config::key_4k = { SDLK_D, SDLK_F, SDLK_J, SDLK_K };
std::vector<SDL_Keycode> Config::key_5k = { SDLK_D, SDLK_F, SDLK_SPACE, SDLK_J, SDLK_K };
std::vector<SDL_Keycode> Config::key_6k = { SDLK_S, SDLK_D, SDLK_F, SDLK_J, SDLK_K, SDLK_L };
std::vector<SDL_Keycode> Config::key_7k = { SDLK_S, SDLK_D, SDLK_F, SDLK_SPACE, SDLK_J, SDLK_K, SDLK_L };

auto Config::get_key_bindings(Uint8 tracks) -> std::vector<SDL_Keycode>& {
    switch (tracks) {
    case 5:
        return key_5k;
    case 6:
        return key_6k;
    case 7:
        return key_7k;
    default:
        return key_4k;
    }
}
