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
#include "NFRG/anim/animator.hpp"
using namespace nfrg;
using namespace anim;

void Animator::bind_component(const std::weak_ptr<ui::Component>&& component) {
    component_ = component;
}

auto Animator::calc_progress() const -> double {
    return static_cast<double>(elapsed_ns_) / 1'000'000.0 / duration_;
}

void Animator::start(Uint64 initial_ns) {
    elapsed_ns_ = initial_ns;
}
