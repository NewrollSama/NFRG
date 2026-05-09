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
#include "animator.hpp"

#include "NFRG/scene/scene.hpp"

namespace nfrg::anim {

template <typename T>
class Animator::BuilderTemplate {
protected:
    std::unique_ptr<T> animator_;

public:
    BuilderTemplate();
    auto set_disposable(bool disposable) -> T::Builder&;
    auto set_duration(float duration) -> T::Builder&;
    auto set_remap_func(double (*remap_func)(double x)) -> T::Builder&;
    auto bind_component(const std::weak_ptr<ui::Component>&& component) -> T::Builder&;

    auto build() -> std::unique_ptr<T>;
    auto build_and_add_to(scene::Scene* scene) -> Animator*;
};

template <typename T>
Animator::BuilderTemplate<T>::BuilderTemplate() {
    animator_ = std::unique_ptr<T>(new T());
}

template <typename T>
auto Animator::BuilderTemplate<T>::set_disposable(bool disposable) -> T::Builder& {
    animator_->disposable_ = disposable;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto Animator::BuilderTemplate<T>::set_duration(float duration) -> T::Builder& {
    animator_->duration_ = duration;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto Animator::BuilderTemplate<T>::set_remap_func(double (*remap_func)(double x)) -> T::Builder& {
    animator_->remap_func_ = remap_func;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto Animator::BuilderTemplate<T>::bind_component(const std::weak_ptr<ui::Component>&& component) -> T::Builder& {
    animator_->component_ = component;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto Animator::BuilderTemplate<T>::build() -> std::unique_ptr<T> {
    return std::move(animator_);
}

template <typename T>
auto Animator::BuilderTemplate<T>::build_and_add_to(scene::Scene* scene) -> Animator* {
    scene->animators_.push_front(std::move(animator_));
    return scene->animators_.front().get();
}

}
