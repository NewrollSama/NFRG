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
#include "../group.hpp"
#include "base.hpp"

#include "NFRG/scene/scene.hpp"

namespace nfrg::ui {

template <typename T>
class Group::BuilderTemplate : public Base::BuilderTemplate<T> {
public:
    auto set_pos(SDL_FPoint pos) -> T::Builder&;
    auto set_bound(SDL_FRect bound) -> T::Builder&;

    auto set_padding(SDL_FPoint padding) -> T::Builder&;

    auto add_component(std::shared_ptr<Base>&& component) -> T::Builder&;
};

class Group::Builder : public Group::BuilderTemplate<Group> {};

template <typename T>
auto Group::BuilderTemplate<T>::set_pos(SDL_FPoint pos) -> T::Builder& {
    this->component_->bound_.x = pos.x;
    this->component_->bound_.y = pos.y;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto Group::BuilderTemplate<T>::set_bound(SDL_FRect bound) -> T::Builder& {
    this->component_->bound_ = bound;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto Group::BuilderTemplate<T>::set_padding(SDL_FPoint padding) -> T::Builder& {
    this->component_->padding_ = padding;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto Group::BuilderTemplate<T>::add_component(std::shared_ptr<Base>&& component) -> T::Builder& {
    this->component_->add_component(std::move(component));
    return static_cast<T::Builder&>(*this);
}

}