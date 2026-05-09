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
#include "../flex.hpp"
#include "group.hpp"

namespace nfrg::ui {

template <typename T>
class Flex::BuilderTemplate : public Group::BuilderTemplate<T> {
public:
    auto set_vertical(bool vertical) -> Builder&;
    auto set_gap(float gap) -> Builder&;
};

class Flex::Builder : public Flex::BuilderTemplate<Flex> {};

template <typename T>
auto Flex::BuilderTemplate<T>::set_vertical(bool vertical) -> Flex::Builder& {
    this->component_->vertical_ = vertical;
    return *static_cast<T::Builder*>(this);
}

template <typename T>
auto Flex::BuilderTemplate<T>::set_gap(float gap) -> Flex::Builder& {
    this->component_->gap_ = gap;
    return *static_cast<T::Builder*>(this);
}

}