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
#include "base.hpp"

namespace nfrg::ui {

/**
 * @brief 容纳多个组件的容器。
 */
class Group : public Base {
    template <typename T>
    friend class Base::BuilderTemplate;

protected:
    std::vector<std::shared_ptr<Base>> components_;
    SDL_FPoint padding_{ .x = 0, .y = 0 };

public:
    auto operator=(Group&& other) noexcept -> Group& = default;

    template <typename T>
    class BuilderTemplate;

    class Builder;

    void set_padding(SDL_FPoint padding);

    void add_component(std::shared_ptr<Base>&& component);

    void measure(SDL_FPoint offset) override;
    [[nodiscard]] auto render() const -> bool override;
};

}
