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
#include "NFRG/ui/base.hpp"
using namespace nfrg;
using namespace ui;

auto Base::get_pos() const -> SDL_FPoint {
    SDL_FPoint pos;
    switch (align_x_) {
    default:
    case AlignX::left:
        pos.x = bound_.x;
        break;
    case AlignX::center:
        pos.x = (mgr::Video::width - bound_.w) / 2;
        break;
    case AlignX::right:
        pos.x = mgr::Video::width - bound_.x;
        break;
    }
    switch (align_y_) {
    default:
    case AlignY::top:
        pos.y = bound_.y;
        break;
    case AlignY::center:
        pos.y = (mgr::Video::height - bound_.h) / 2;
        break;
    case AlignY::bottom:
        pos.y = mgr::Video::height - bound_.y;
        break;
    }
    return { .x = pos.x, .y = pos.y };
}

auto Base::get_size() const -> SDL_FPoint {
    return { .x = bound_.w, .y = bound_.h };
}

auto Base::get_bound() const -> SDL_FRect {
    SDL_FPoint pos = get_pos();
    return { .x = pos.x, .y = pos.y, .w = bound_.w, .h = bound_.h };
}

void Base::set_align_x(AlignX align) {
    align_x_ = align;
}

void Base::set_align_y(AlignY align) {
    align_y_ = align;
}

auto Base::get_align_x() const -> AlignX {
    return align_x_;
}

auto Base::get_align_y() const -> AlignY {
    return align_y_;
}
