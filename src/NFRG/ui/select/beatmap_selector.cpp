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
#include "NFRG/ui/select/beatmap_selector.hpp"
using namespace nfrg;
using namespace ui;

#include <format>

auto BeatmapSelector::Builder::set_beatmaps(std::vector<std::pair<Uint8, std::string_view>>& beatmaps) -> BeatmapSelector::Builder& {
    component_->beatmaps_info_.reserve(beatmaps.size());
    for (const auto [tracks_num, beatmap] : beatmaps) {
        component_->beatmaps_info_.emplace_back(tracks_num, beatmap);
    }
    return *this;
}

auto BeatmapSelector::Builder::build() -> std::shared_ptr<BeatmapSelector> {
    component_->items_.reserve(component_->beatmaps_info_.size());
    for (const auto& [tracks_num, title] : component_->beatmaps_info_) {
        component_->items_.emplace_back(std::format("[{}K] {}", tracks_num, title));
    }
    return BuilderTemplate<BeatmapSelector>::build();
}
