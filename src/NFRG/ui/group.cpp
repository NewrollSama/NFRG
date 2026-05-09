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
#include "NFRG/ui/group.hpp"
using namespace nfrg;
using namespace ui;

#include "NFRG/ui/builder/group.hpp"

void Group::set_padding(SDL_FPoint padding) {
    padding_ = padding;
}

void Group::add_component(std::shared_ptr<Base>&& component) {
    components_.push_back(component);
}

void Group::measure(SDL_FPoint offset) {
    render_offset_ = offset;
    offset.x += bound_.x + padding_.x;
    offset.y += bound_.y + padding_.y;
    for (const auto& component : components_) {
        component->measure(offset);
    }
}

auto Group::render() const -> bool {
    for (const auto& component : components_) {
        if (!component->render()) {
            return false;
        }
    }
    return true;
}
