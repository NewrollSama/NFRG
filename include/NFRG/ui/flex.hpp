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
#include "NFRG/ui/group.hpp"

namespace nfrg::ui {

class Flex : public Group {

protected:
    bool vertical_ = false;
    float gap_ = 0;

public:
    template <typename T>
    class BuilderTemplate;

    class Builder;

    void set_vertical(bool vertical);
    void set_gap(float gap);

    [[nodiscard]] auto get_pos() const -> SDL_FPoint override;
    void measure(SDL_FPoint offset) override;
};

}
