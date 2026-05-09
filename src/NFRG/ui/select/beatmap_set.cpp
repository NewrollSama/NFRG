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
#include "NFRG/ui/select/beatmap_set.hpp"
using namespace nfrg;
using namespace ui;

#include "NFRG/ui/builder/flex.hpp"
#include "NFRG/ui/button.hpp"
#include "NFRG/ui/image.hpp"
#include "NFRG/ui/static_text.hpp"

void BeatmapSet::change_beatmaps_current_index(Sint16 offset) {
    if (offset == -1) {
        selector_->up();
    } else if (offset == 1) {
        selector_->down();
    }
}

auto BeatmapSet::get_selected_beatmap() -> Uint16 {
    return selector_->get_current();
}

auto BeatmapSet::Builder::set_bgi_texture(SDL_Texture* texture) -> Builder& {
    ui::Image::Builder()
        .set_texture(texture, false)
        .set_size(mgr::Video::get_window_size())
        .add_to(component_.get());
    return *this;
}

auto BeatmapSet::Builder::set_title(std::string_view title) -> Builder& {
    title_obj_ = ui::StaticText::Builder()
                     .set_content(title.data(), 48)
                     .build();
    return *this;
}

auto BeatmapSet::Builder::set_artist(std::string_view artist) -> Builder& {
    artist_obj_ = ui::StaticText::Builder()
                      .set_content(artist.data(), 36)
                      .build();
    return *this;
}

auto BeatmapSet::Builder::set_beatmaps(std::vector<std::pair<Uint8, std::string_view>> beatmaps) -> Builder& {
    selector_obj_ = ui::BeatmapSelector::Builder()
                        .set_beatmaps(beatmaps)
                        .set_bg_color({ 255, 255, 255, 128 })
                        .build();
    component_->selector_ = selector_obj_.get();
    return *this;
}

auto BeatmapSet::Builder::build() -> std::shared_ptr<BeatmapSet> {
    ui::Flex::Builder()
        .set_pos({ .x = 32, .y = 64 })
        .set_vertical(true)
        .add_component(std::move(title_obj_))
        .add_component(std::move(artist_obj_))
        .add_to(component_.get());
    ui::Flex::Builder()
        .set_align_x(AlignX::right)
        .set_align_y(AlignY::bottom)
        .set_pos({ .x = 32, .y = 32 })
        .set_padding({ .x = 16, .y = 16 })
        .add_component(std::move(selector_obj_))
        .add_component(ui::Button::Builder()
                .set_text(ui::Text::Builder()
                        .set_content("开始")
                        .set_foreground_color({ .r = 0, .g = 0, .b = 0, .a = 255 })
                        .build())
                .set_background_color({ .r = 255, .g = 255, .b = 255, .a = 192 })
                .set_padding({ .x = 16, .y = 8 })
                .build())
        .add_to(component_.get());
    return BuilderTemplate<BeatmapSet>::build();
}
