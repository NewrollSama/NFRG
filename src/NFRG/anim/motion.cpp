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
#include "NFRG/anim/motion.hpp"
using namespace nfrg;
using namespace anim;

#include "NFRG/ui/component.hpp"

auto MotionAnimator::process(Uint64 delta_ns) -> bool {
    elapsed_ns_ += delta_ns;
    double progress = calc_progress();
    bool finish = disposable_ && progress >= 1.0;
    double remapped_progress = remap_func_(progress);
    if (auto so = component_.lock()) {
        so->bound_.x = finish ? dst_.x : static_cast<float>(src_.x + (dst_.x - src_.x) * remapped_progress);
        so->bound_.y = finish ? dst_.y : static_cast<float>(src_.y + (dst_.y - src_.y) * remapped_progress);
    } else {  // 组件已经不存在，请求销毁动画器
        return true;
    }
    return finish;
}
