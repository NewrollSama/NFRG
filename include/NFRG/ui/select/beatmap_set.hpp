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
#include "beatmap_selector.hpp"

#include "../static_text.hpp"

namespace nfrg::ui {

class BeatmapSet : public Group {
protected:
    ui::BeatmapSelector* selector_ = nullptr;

public:
    auto operator=(BeatmapSet&& other) noexcept -> BeatmapSet& = default;

    class Builder;

    void change_beatmaps_current_index(Sint16 offset);
    auto get_selected_beatmap() -> Uint16;

    virtual ~BeatmapSet() = default;
};

class BeatmapSet::Builder : public BuilderTemplate<BeatmapSet> {
protected:
    std::shared_ptr<StaticText> title_obj_;
    std::shared_ptr<StaticText> artist_obj_;
    std::shared_ptr<BeatmapSelector> selector_obj_;

public:
    auto set_bgi_texture(SDL_Texture* bgi_texture) -> Builder&;
    auto set_title(std::string_view title) -> Builder&;
    auto set_artist(std::string_view artist) -> Builder&;
    auto set_beatmaps(std::vector<std::pair<Uint8, std::string_view>> beatmaps) -> Builder&;

    auto build() -> std::shared_ptr<BeatmapSet> override;
};

}