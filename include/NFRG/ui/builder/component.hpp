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
#include "../component.hpp"
#include "base.hpp"

namespace nfrg::ui {

template <typename T>
class Component::BuilderTemplate : public Base::BuilderTemplate<T> {
public:
    /**
     * @brief 设定待建造组件对象的前景色。
     * @param fg_color 待设置的前景色。
     * @return 建造者的引用，以便链式调用。
     */
    auto set_foreground_color(SDL_Color fg_color) -> T::Builder&;

    /**
     * @brief 为组件设置纹理。
     *
     * @param texture 待设置的材质。
     * @param own_texture 设置组件是否接管材质的所有权。
     * @return 建造者的引用，以便链式调用。
     */
    auto set_texture(SDL_Texture* texture, bool own_texture = false) -> T::Builder&;
};

template <typename T>
auto Component::BuilderTemplate<T>::set_foreground_color(SDL_Color fg_color) -> T::Builder& {
    this->component_->fg_color_ = fg_color;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto Component::BuilderTemplate<T>::set_texture(SDL_Texture* texture, bool own_texture) -> T::Builder& {
    if (texture == nullptr) {
        SDL_Log("Warning: Setting texture of nullptr");
    }
    this->component_->set_texture(texture);
    this->component_->own_texture_ = own_texture;
    return static_cast<T::Builder&>(*this);
}

}
