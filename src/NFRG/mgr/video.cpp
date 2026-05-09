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
#include "NFRG/mgr/video.hpp"
using namespace nfrg;
using namespace mgr;

#include "NFRG/mgr/config.hpp"

SDL_Window* Video::window = nullptr;
SDL_Renderer* Video::renderer = nullptr;
int Video::width = 1920;
int Video::height = 1080;

auto Video::get_window_size() -> SDL_FPoint {
    return { .x = static_cast<float>(width), .y = static_cast<float>(height) };
}

void Video::set_render_draw_color(SDL_Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void Video::apply_display_mode() {
    switch (Config::display_mode) {
    case 0:
        SDL_SetWindowFullscreen(window, false);
        SDL_SetWindowBordered(window, true);
        SDL_SetWindowResizable(window, true);
        SDL_SetWindowSize(window, width, height);
        SDL_SyncWindow(window);
        break;
    case 1: {
        SDL_SetWindowFullscreen(window, false);
        SDL_SetWindowBordered(window, false);
        SDL_SetWindowResizable(window, false);
        const SDL_DisplayID display = SDL_GetDisplayForWindow(window);
        SDL_Rect bounds;
        if (SDL_GetDisplayBounds(display, &bounds)) {
            SDL_SetWindowSize(window, bounds.w, bounds.h);
            SDL_SetWindowPosition(window, 0, 0);
            width = bounds.w;
            height = bounds.h;
        }
        SDL_SyncWindow(window);
        break;
    }
    case 2:
        SDL_SetWindowFullscreen(window, true);
        SDL_SyncWindow(window);
        break;
    }
}
