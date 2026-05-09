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
#include "NFRG/anim/alpha.hpp"
using namespace nfrg;
using namespace anim;

auto Alpha::process(Uint64 delta_ns) -> bool {
    elapsed_ns_ += delta_ns;
    auto so = component_.lock();
    if (!so) return true;
    double progress = calc_progress();
    bool finish = disposable_ && progress >= 1.0;
    double remapped_progress = remap_func_(progress);
    so->set_alpha(std::min(finish ? dst_ : static_cast<Uint8>(src_ + (dst_ - src_) * remapped_progress),
        static_cast<Uint8>(255)));
    return finish;
}

auto Alpha::Builder::set_source(Uint8 alpha) -> Alpha::Builder& {
    animator_->src_ = alpha;
    return *this;
}

auto Alpha::Builder::set_destination(Uint8 alpha) -> Alpha::Builder& {
    animator_->dst_ = alpha;
    return *this;
}
