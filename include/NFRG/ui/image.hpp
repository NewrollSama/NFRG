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
#include "builder/component.hpp"

namespace nfrg::ui {

class Image : public Component {
    friend Component::BuilderTemplate<Image>;

public:
    Image() = default;

public:
    class Builder;

    auto load(std::string&& filename) -> bool;
    auto load(SDL_IOStream* io) -> bool;

    [[nodiscard]] auto render() const -> bool override;
};

class Image::Builder : public Component::BuilderTemplate<Image> {
public:
    Builder() = default;
    auto load(std::string&& filename) -> Builder&;
    auto load(SDL_IOStream* io) -> Builder&;
};

}
