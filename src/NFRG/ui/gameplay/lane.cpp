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
#include "NFRG/ui/gameplay/lane.hpp"
using namespace nfrg;
using namespace ui;
using namespace gameplay;

#include <algorithm>

void Lane::calc(float y) {
    bottom_ = std::clamp(y, 0.F, static_cast<float>(mgr::Video::height));
    top_ = y - bound_.h;
}

auto Lane::render() const -> bool {
    // SDL_Log("%f %f", bottom, bound_.h);
    if (bottom_ == 0.F) {
        return true;
    }
    SDL_SetRenderDrawColor(mgr::Video::renderer, 255, 255, 255, 128);
    // SDL_RenderLine(mgr::Video::renderer, bound_.x, top, bound_.x, bottom);
    // SDL_RenderLine(mgr::Video::renderer, bound_.x + bound_.w / 2, top, bound_.x + bound_.w / 2, bottom);
    // SDL_RenderLine(mgr::Video::renderer, bound_.x + bound_.w, top, bound_.x + bound_.w, bottom);
    SDL_FRect rect{ .x = bound_.x, .y = top_, .w = bound_.w, .h = bound_.h };  // top_ ???
    // SDL_Log("%f %f %f %f", rect.x, rect.y, rect.w, rect.h);
    SDL_RenderFillRect(mgr::Video::renderer, &rect);
    if (top_ >= 0.F) {
        SDL_RenderLine(mgr::Video::renderer, bound_.x, top_, bound_.x + bound_.w, top_);
    }
    return true;
}
