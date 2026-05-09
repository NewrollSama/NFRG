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
#include "NFRG/ui/latency/detection_line.hpp"
using namespace nfrg;
using namespace ui;
using namespace latency;

#include "NFRG/mgr/video.hpp"

auto DetectionLine::render() const -> bool {
    SDL_SetRenderDrawColor(mgr::Video::renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_FRect rect = bound_;
    return SDL_RenderFillRect(mgr::Video::renderer, &rect);
}
