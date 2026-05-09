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
#include "NFRG/ui/builder/list.hpp"

namespace nfrg::ui {

class BeatmapSelector : public List {
    friend class Base::BuilderTemplate<BeatmapSelector>;

protected:
    std::vector<std::pair<Uint8, std::string>> beatmaps_info_;

    BeatmapSelector() = default;

public:
    class Builder;
};

class BeatmapSelector::Builder : public List::BuilderTemplate<BeatmapSelector> {
public:
    Builder() = default;

    auto set_beatmaps(std::vector<std::pair<Uint8, std::string_view>>& beatmaps) -> Builder&;
    auto build() -> std::shared_ptr<BeatmapSelector> override;
};

}
