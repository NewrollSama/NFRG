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

class Alpha : public Animator {
    friend class Animator::BuilderTemplate<Alpha>;

protected:
    Uint8 src_;
    Uint8 dst_;

    Alpha() = default;

public:
    class Builder;

    auto process(Uint64 delta_ns) -> bool override;
};

class Alpha::Builder : public Animator::BuilderTemplate<Alpha> {
public:
    Builder() = default;
    auto set_source(Uint8 alpha) -> Builder&;
    auto set_destination(Uint8 alpha) -> Builder&;
};

}