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
#pragma once

#include "animator_builder.hpp"

namespace nfrg::anim {

/**
 * @brief 位移动画器。
 */
class MotionAnimator : public Animator {
    template <typename T>
    friend class Animator::BuilderTemplate;

protected:
    /**
     * @brief 起点位置。
     */
    SDL_FPoint src_{ .x = 0.0f, .y = 0.0f };

    /**
     * @brief 终点位置。
     */
    SDL_FPoint dst_{ .x = 0.0f, .y = 0.0f };

    MotionAnimator() = default;

public:
    template <typename T>
    class BuilderTemplate;

    class Builder;

    auto process(Uint64 delta_ns) -> bool override;

    ~MotionAnimator() override = default;
};

template <typename T>
class MotionAnimator::BuilderTemplate : public Animator::BuilderTemplate<T> {
public:
    BuilderTemplate() = default;
    auto set_source(SDL_FPoint src) -> T::Builder&;
    auto set_destination(SDL_FPoint dst) -> T::Builder&;
};

template <typename T>
auto MotionAnimator::BuilderTemplate<T>::set_source(SDL_FPoint src) -> T::Builder& {
    this->animator_->src_ = src;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto MotionAnimator::BuilderTemplate<T>::set_destination(SDL_FPoint dst) -> T::Builder& {
    this->animator_->dst_ = dst;
    return static_cast<T::Builder&>(*this);
}

class MotionAnimator::Builder : public BuilderTemplate<MotionAnimator> {};

}
