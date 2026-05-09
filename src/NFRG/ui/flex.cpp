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
#include "NFRG/ui/flex.hpp"
using namespace nfrg;
using namespace ui;

void Flex::set_vertical(bool vertical) {
    vertical_ = vertical;
}

void Flex::set_gap(float gap) {
    gap_ = gap;
}

auto Flex::get_pos() const -> SDL_FPoint {
    if (align_x_ == AlignX::right) {
        return {
            .x = mgr::Video::width - bound_.w - 2 * padding_.x,
            .y = mgr::Video::height - bound_.h - 2 * padding_.y,
        };
    }
    return Base::get_pos();
}

void Flex::measure(SDL_FPoint offset) {
    render_offset_ = offset;

    {  // 需要计算自身宽高
        float main = 0;
        float sub = 0;
        for (const auto& component : components_) {
            if (!vertical_) {
                main += component->get_size().x + gap_;
                sub = std::max(sub, component->get_size().y);
            } else {
                main += component->get_size().y + gap_;
                sub = std::max(sub, component->get_size().x);
            }
        }
        if (!vertical_) {
            bound_.w = 2 * padding_.x + main - gap_;
            bound_.h = 2 * padding_.y + sub;
        } else {
            bound_.w = 2 * padding_.x + sub;
            bound_.h = 2 * padding_.y + main - gap_;
        }
    }

    offset.x += get_pos().x + padding_.x;
    offset.y += get_pos().y + padding_.y;

    if (!vertical_) {
        float cross_space = bound_.h - 2 * padding_.y;
        for (const auto& component : components_) {
            float child_y = offset.y;
            switch (component->get_align_y()) {
            case AlignY::center:
                child_y += (cross_space - component->get_size().y) / 2.F;
                break;
            case AlignY::bottom:
                child_y += cross_space - component->get_size().y;
                break;
            default:
                break;
            }
            component->measure({offset.x, child_y});
            offset.x += component->get_size().x + gap_;
        }
    } else {
        float cross_space = bound_.w - 2 * padding_.x;
        for (const auto& component : components_) {
            float child_x = offset.x;
            switch (component->get_align_x()) {
            case AlignX::center:
                child_x += (cross_space - component->get_size().x) / 2.F;
                break;
            case AlignX::right:
                child_x += cross_space - component->get_size().x;
                break;
            default:
                break;
            }
            component->measure({child_x, offset.y});
            offset.y += component->get_size().y + gap_;
        }
    }
}
