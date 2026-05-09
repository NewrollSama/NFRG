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
#include "NFRG/ui/gameplay/note.hpp"
using namespace nfrg;
using namespace ui;
using namespace gameplay;

#include "NFRG/ui/gameplay/lane.hpp"

auto Note::has_lane() const -> bool {
    return lane_ != nullptr;
}

auto Note::get_hit_time() const -> Uint64 {
    return hit_time_;
}

auto Note::get_release_time() const -> Uint64 {
    return release_time_;
}

auto Note::render() const -> bool {
    if (lane_ != nullptr) {
        lane_->calc(bound_.y);
        if (!lane_->render()) return false;
    }
    if (texture_ != nullptr) {
        return Component::render();
    }
    SDL_SetRenderDrawBlendMode(mgr::Video::renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(mgr::Video::renderer, fg_color_.r, fg_color_.g, fg_color_.b, fg_color_.a);
    return SDL_RenderFillRect(mgr::Video::renderer, &bound_);
}

auto Note::Builder::set_hit_time(Uint64 hit_time) -> Note::Builder& {
    component_->hit_time_ = hit_time;
    return *this;
}

auto Note::Builder::set_release_time(Uint64 release_time) -> Note::Builder& {
    component_->release_time_ = release_time;
    return *this;
}

auto Note::Builder::set_width(float width) -> Note::Builder& {
    component_->bound_.w = width;
    return *this;
}

auto Note::Builder::set_lane(float height) -> Note::Builder& {
    component_->lane_ = Lane::Builder()
                            .set_bound({
                                .x = component_->bound_.x,
                                .y = 0.F,
                                .w = component_->bound_.w,
                                .h = height,
                            })
                            .build();
    return *this;
}
