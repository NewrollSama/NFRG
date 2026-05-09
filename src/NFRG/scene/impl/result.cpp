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
#include "NFRG/scene/impl/result.hpp"
using namespace nfrg;
using namespace scene;

#include "NFRG/mgr/audio.hpp"
#include "NFRG/mgr/scene.hpp"
#include "NFRG/scene/impl/gameplay.hpp"
#include "NFRG/ui/flex.hpp"
#include "NFRG/ui/image.hpp"
#include "NFRG/ui/text.hpp"

#include <format>

ResultScene::ResultScene(Uint64 score, Uint32 max_combo, float accuracy,
    Uint32 perfect, Uint32 great, Uint32 miss,
    util::BeatmapSet* beatmapset, SDL_Texture* bgi_texture, size_t beatmap_index)
    : score_(score), max_combo_(max_combo), accuracy_(accuracy),
      perfect_(perfect), great_(great), miss_(miss),
      beatmapset_(beatmapset), beatmap_index_(beatmap_index), bgi_texture_(bgi_texture) {

    if (bgi_texture_) {
        ui::Image::Builder()
            .set_texture(bgi_texture_, false)
            .set_size(mgr::Video::get_window_size())
            .add_to(this);
    }

    SDL_Surface* dim = SDL_CreateSurface(1, 1, SDL_PIXELFORMAT_RGBA8888);
    SDL_FillSurfaceRect(dim, nullptr, SDL_MapSurfaceRGBA(dim, 0, 0, 0, 160));
    SDL_Texture* dim_tex = SDL_CreateTextureFromSurface(mgr::Video::renderer, dim);
    SDL_DestroySurface(dim);
    ui::Image::Builder()
        .set_texture(dim_tex, true)
        .set_size(mgr::Video::get_window_size())
        .add_to(this);

    ui::Text::Builder()
        .set_content("结算")
        .set_font_size(64)
        .set_foreground_color({ .r = 255, .g = 255, .b = 255, .a = 255 })
        .set_align_x(ui::AlignX::center)
        .set_pos({ .x = 64, .y = 32 })
        .add_to(this);

    auto score_text = ui::Text::Builder()
                          .set_content(std::format("得分: {}", score_))
                          .set_font_size(36)
                          .set_foreground_color({ .r = 255, .g = 255, .b = 255, .a = 255 })
                          .build();
    auto combo_text = ui::Text::Builder()
                          .set_content(std::format("最大连击: {}", max_combo_))
                          .set_font_size(36)
                          .set_foreground_color({ .r = 255, .g = 255, .b = 255, .a = 255 })
                          .build();
    auto acc_text = ui::Text::Builder()
                        .set_content(std::format("准确度: {:.2f}%", accuracy_))
                        .set_font_size(36)
                        .set_foreground_color({ .r = 255, .g = 255, .b = 255, .a = 255 })
                        .build();
    auto perfect_text = ui::Text::Builder()
                            .set_content(std::format("Perfect: {}", perfect_))
                            .set_font_size(36)
                            .set_foreground_color({ .r = 255, .g = 255, .b = 255, .a = 255 })
                            .build();
    auto great_text = ui::Text::Builder()
                          .set_content(std::format("Great: {}", great_))
                          .set_font_size(36)
                          .set_foreground_color({ .r = 255, .g = 255, .b = 255, .a = 255 })
                          .build();
    auto miss_text = ui::Text::Builder()
                         .set_content(std::format("Miss: {}", miss_))
                         .set_font_size(36)
                         .set_foreground_color({ .r = 255, .g = 255, .b = 255, .a = 255 })
                         .build();

    ui::Flex::Builder()
        .set_vertical(true)
        .set_gap(16)
        .set_align_x(ui::AlignX::center)
        .set_pos({ .x = 0, .y = 220 })
        .add_component(std::move(score_text))
        .add_component(std::move(combo_text))
        .add_component(std::move(acc_text))
        .add_component(std::move(perfect_text))
        .add_component(std::move(great_text))
        .add_component(std::move(miss_text))
        .add_to(this);

    ui::Text::Builder()
        .set_content("按 R 重新游玩  |  Esc 返回选曲")
        .set_font_size(24)
        .set_foreground_color({ .r = 200, .g = 200, .b = 200, .a = 255 })
        .set_align_x(ui::AlignX::center)
        .set_pos({ .x = 64, .y = static_cast<float>(mgr::Video::height) - 64 })
        .add_to(this);
}

void ResultScene::handle_key_event(SDL_KeyboardEvent& event) {
    if (!event.down) return;
    if (event.key == SDLK_R) {
        const auto& bm = beatmapset_->get_beatmaps()[beatmap_index_];
        auto* bs = beatmapset_;
        auto idx = beatmap_index_;
        auto* tex = bgi_texture_;
        mgr::Scene::ActiveScene::quit();
        mgr::Audio::clear_bg();
        mgr::Audio::play_bg(bs->get_bgm(), false, true);
        mgr::Scene::create<GameplayScene>(bm, bs, tex, idx);
        return;
    }
    Scene::handle_key_event(event);
}
