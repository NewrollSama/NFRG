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
#include "../list.hpp"

namespace nfrg::ui {

template <typename T>
auto List::BuilderTemplate<T>::set_items(std::vector<std::string> items) -> T::Builder& {
    this->component_->items_ = std::move(items);
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto List::BuilderTemplate<T>::set_max_visible(Uint8 n) -> T::Builder& {
    this->component_->max_visible_ = n;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto List::BuilderTemplate<T>::set_font_size(float ptsize) -> T::Builder& {
    this->component_->font_size_ = ptsize;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto List::BuilderTemplate<T>::set_font_color(SDL_Color color) -> T::Builder& {
    this->component_->font_color_ = color;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto List::BuilderTemplate<T>::set_bg_color(SDL_Color color) -> T::Builder& {
    this->component_->bg_color_ = color;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto List::BuilderTemplate<T>::set_hl_color(SDL_Color color) -> T::Builder& {
    this->component_->hl_color_ = color;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto List::BuilderTemplate<T>::build() -> std::shared_ptr<T> {
    this->component_->vertical_ = true;
    this->component_->gap_ = 16;
    this->component_->padding_ = { .x = 16, .y = 8 };

    const Uint8 num = std::min(this->component_->max_visible_, static_cast<Uint8>(this->component_->items_.size()));
    this->component_->slots_.resize(num);
    for (Uint8 i = 0; i < num; ++i) {
        this->component_->slots_[i] = ui::Text::Builder()
                                          .set_content(this->component_->items_[i])
                                          .set_font_size(this->component_->font_size_)
                                          .set_foreground_color(this->component_->font_color_)
                                          .build_and_add_to(this->component_.get())
                                          .get();
    }
    return Flex::BuilderTemplate<T>::build();
}

}
