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
#include "NFRG/scene/impl/gameplay.hpp"
using namespace nfrg;
using namespace scene;

#include "NFRG/mgr/audio.hpp"
#include "NFRG/mgr/config.hpp"
#include "NFRG/mgr/scene.hpp"
#include "NFRG/scene/impl/result.hpp"
#include "NFRG/ui/builder/list.hpp"
#include "NFRG/ui/flex.hpp"
#include "NFRG/ui/image.hpp"
#include "NFRG/ui/static_text.hpp"
#include "NFRG/ui/text.hpp"
#include "NFRG/util/beatmap_set.hpp"

#include <algorithm>
#include <format>

void GameplayScene::update_text() {
    const Uint64 score = counters_.perfect * 100 + counters_.great * 60 + std::max(combo_, max_combo_) * 10;
    const Uint32 hit = counters_.perfect * 100 + counters_.great * 60;
    const Uint32 total = counters_.perfect + counters_.great + counters_.miss;
    const float accuracy = static_cast<float>(hit) / total;
    text_score_->set_content(std::to_string(score));
    text_combo_->set_content(std::to_string(combo_));
    text_accuracy_->set_content(std::format("{:.2f}%", accuracy));
}

void GameplayScene::toggle_pause() {
    paused_ = !paused_;
    if (paused_) {
        mgr::Audio::pause_bg();
    } else {
        mgr::Audio::resume_bg();
    }
}

void GameplayScene::highlight_track(Uint8 track, Sint8 result) {
    if (track >= track_highlights_.size()) return;
    auto& h = track_highlights_[track];
    h.alpha = 1.0f;
    switch (result) {
    case -1:
        h.color = { .r = 255, .g = 255, .b = 255, .a = 255 };
        break;
    case 0:
        h.color = { .r = 255, .g = 60, .b = 60, .a = 255 };
        break;
    case 1:
        h.color = { .r = 40, .g = 192, .b = 255, .a = 255 };
        break;
    case 2:
        h.color = { .r = 255, .g = 215, .b = 0, .a = 255 };
        break;
    }
}

void GameplayScene::show_judgment(Sint8 result) {
    const char* content;
    SDL_Color color;
    switch (result) {
    case 0:
        content = "Miss";
        color = { .r = 255, .g = 60, .b = 60, .a = 255 };
        break;
    case 1:
        content = "Great";
        color = { .r = 40, .g = 192, .b = 255, .a = 255 };
        break;
    case 2:
        content = "Perfect";
        color = { .r = 255, .g = 215, .b = 0, .a = 255 };
        break;
    default:
        return;
    }

    auto text = ui::Text::Builder()
                    .set_content(content)
                    .set_font_size(72)
                    .set_foreground_color(color)
                    .build();
    const float x = (mgr::Video::width - text->get_size().x) / 2.0f;
    const float y = (mgr::Video::height - text->get_size().y) / 2.0f;
    text->set_pos({ .x = x, .y = y });

    components_.push_back(text);
    judgment_popups_.push_back({ .text = std::move(text),
        .color = color,
        .start_ns = SDL_GetTicksNS(),
        .base_y = y });
}

GameplayScene::GameplayScene(const util::Beatmap& beatmap, util::BeatmapSet* beatmapset,
    SDL_Texture* bgi_texture, size_t beatmap_index)
    : beatmapset_(beatmapset), beatmap_index_(beatmap_index), bgi_texture_(bgi_texture) {
    const auto num_tracks = beatmap.get_trucks();
    ui::Image::Builder()
        .set_texture(bgi_texture)
        .set_size(mgr::Video::get_window_size())
        .add_to(this);
    board_ = ui::gameplay::Board::Builder()
                 .set_num_tracks(num_tracks)
                 .set_bpm(beatmapset->get_bpm())
                 .load_beatmap(beatmap.parse(beatmapset->get_ifs()),
                     beatmap.get_beatmap_delay())
                 .build_and_add_to(this)
                 .get();
    text_score_ = ui::Text::Builder()
                      .set_content("0")
                      .set_pos({ .x = 16, .y = 16 })
                      .build_and_add_to(this)
                      .get();
    text_combo_ = ui::Text::Builder()
                      .set_content("0")
                      .set_pos({ .x = 16, .y = 80 })
                      .build_and_add_to(this)
                      .get();
    text_accuracy_ = ui::Text::Builder()
                         .set_content("100.00%")
                         .set_pos({ .x = 16, .y = 144 })
                         .build_and_add_to(this)
                         .get();

    auto& bindings = mgr::Config::get_key_bindings(num_tracks);
    for (Uint8 i = 0; i < static_cast<Uint8>(bindings.size()) && i < num_tracks; ++i) {
        key_to_track_[bindings[i]] = i;
    }

    track_highlights_.resize(num_tracks);

    auto title_text = ui::StaticText::Builder()
                          .set_content("已暂停", 48)
                          .set_foreground_color({ .r = 255, .g = 255, .b = 255, .a = 255 })
                          .set_align_x(ui::AlignX::center)
                          .build();

    auto list = ui::List::Builder()
                    .set_items({ "继续游玩", "重新开始", "结束游玩" })
                    .set_font_size(36)
                    .set_font_color({ .r = 255, .g = 255, .b = 255, .a = 255 })
                    .set_bg_color({ .r = 0, .g = 0, .b = 0, .a = 0 })
                    .set_hl_color({ .r = 255, .g = 255, .b = 255, .a = 64 })
                    // .set_align_x(ui::AlignX::center)
                    // .set_align_y(ui::AlignY::center)
                    .build();
    pause_list_ = list.get();

    auto overlay = ui::Flex::Builder()
                       .set_vertical(true)
                       .set_gap(32)
                       .set_bound({ .x = 0, .y = 0, .w = static_cast<float>(mgr::Video::width), .h = static_cast<float>(mgr::Video::height) })
                       .add_component(std::move(title_text))
                       .add_component(std::move(list))
                       .set_align_x(ui::AlignX::center)
                       .set_align_y(ui::AlignY::center)
                       .build();
    pause_overlay_ = std::move(overlay);

    mgr::Audio::resume_bg();
}

void GameplayScene::process_animators(Uint64 /*delta_ns*/) {
    if (paused_) {
        return;
    }
    Uint64 ticks_ns = mgr::Audio::get_bg_playback_progress();
    if (last_progress_ns_ == 0) {
        last_progress_ns_ = ticks_ns;
        return;
    }
    Uint64 delta_ns_ = ticks_ns - last_progress_ns_;
    last_progress_ns_ = ticks_ns;

    Scene::process_animators(delta_ns_);
}

void GameplayScene::handle_key_event(SDL_KeyboardEvent& event) {
    if (paused_) {
        if (!event.down) return;
        switch (event.key) {
        case SDLK_ESCAPE:
            toggle_pause();
            break;
        case SDLK_UP:
            pause_list_->up();
            break;
        case SDLK_DOWN:
            pause_list_->down();
            break;
        case SDLK_RETURN: {
            switch (pause_list_->get_current()) {
            case 0:
                toggle_pause();
                break;
            case 1: {
                auto* bs = beatmapset_;
                auto idx = beatmap_index_;
                auto* tex = bgi_texture_;
                const auto& bm = bs->get_beatmaps()[idx];
                mgr::Scene::ActiveScene::quit();
                mgr::Audio::play_bg(bs->get_bgm(), false, true);
                mgr::Scene::create<GameplayScene>(bm, bs, tex, idx);
                return;
            }
            case 2:
                mgr::Scene::ActiveScene::quit();
                return;
            }
            break;
        }
        }
        return;
    }

    if (event.key == SDLK_ESCAPE) {
        if (event.down) toggle_pause();
        return;
    }

    if (event.key >= SDLK_A && event.key <= SDLK_Z) {
        if (event.repeat) return;
        auto it = key_to_track_.find(event.key);
        if (it != key_to_track_.end()) {
            Sint8 res = board_->track_event(it->second, event);
            if (event.down) highlight_track(it->second, res);
            if (res == -1) return;
            show_judgment(res);
            switch (res) {
            case 0: {
                if (combo_ > max_combo_)
                    max_combo_ = combo_;
                combo_ = 0;
                break;
            }
            case 1: {
                ++combo_;
                ++counters_.great;
                break;
            }
            case 2: {
                ++combo_;
                ++counters_.perfect;
                break;
            }
            }
            update_text();
        }
        return;
    }

    Scene::handle_key_event(event);
}

void GameplayScene::update() {
    if (paused_) return;
    Uint64 progress_ns = mgr::Audio::get_bg_playback_progress();

    Uint64 now_ns = SDL_GetTicksNS();
    if (last_wall_ns_ == 0) last_wall_ns_ = now_ns;
    float wall_dt = static_cast<float>(now_ns - last_wall_ns_) / 1'000'000'000.0f;
    last_wall_ns_ = now_ns;

    static constexpr float HIGHLIGHT_FADE_S = 0.15f;
    for (auto& h : track_highlights_) {
        if (h.alpha > 0.0f) {
            h.alpha = std::max(0.0f, h.alpha - wall_dt / HIGHLIGHT_FADE_S);
        }
    }

    for (auto it = judgment_popups_.begin(); it != judgment_popups_.end();) {
        Uint64 elapsed = now_ns - it->start_ns;
        if (elapsed >= JudgmentPopup::DURATION_NS) {
            auto comp_it = std::ranges::find(components_, it->text);
            if (comp_it != components_.end()) {
                components_.erase(comp_it);
            }
            it = judgment_popups_.erase(it);
        } else {
            float progress = static_cast<float>(elapsed) / JudgmentPopup::DURATION_NS;
            auto alpha = static_cast<Uint8>(255.0f * (1.0f - progress));
            SDL_Color c = it->color;
            c.a = alpha;
            it->text->set_color(c);
            auto pos = it->text->get_pos();
            pos.y = it->base_y - JudgmentPopup::MOVE_UP_PX * progress;
            it->text->set_pos(pos);
            ++it;
        }
    }

    board_->activate_pending_notes(progress_ns, *this);

    const Uint8 missed = board_->detect_overtime(progress_ns);
    if (missed > 0) {
        for (Uint8 track : board_->get_overtime_missed_tracks()) {
            highlight_track(track, 0);
            show_judgment(0);
        }
        if (combo_ > max_combo_)
            max_combo_ = combo_;
        combo_ = 0;
        counters_.miss += missed;
        update_text();
    }
    if (board_->all_judged()) {
        if (end_wait_start_ == 0) {
            end_wait_start_ = SDL_GetTicksNS();
        } else if (SDL_GetTicksNS() - end_wait_start_ >= 1'000'000'000) {
            const Uint64 score = counters_.perfect * 100 + counters_.great * 60 + std::max(combo_, max_combo_) * 10;
            const Uint32 total = counters_.perfect + counters_.great + counters_.miss;
            const float accuracy = total > 0 ? static_cast<float>(counters_.perfect * 100 + counters_.great * 60) / total : 0;
            const Uint32 perfect = counters_.perfect;
            const Uint32 great = counters_.great;
            const Uint32 miss = counters_.miss;
            const Uint32 max_combo = max_combo_;
            auto* bs = beatmapset_;
            auto idx = beatmap_index_;
            auto* tex = bgi_texture_;
            mgr::Audio::prevent_next_clear_bg();
            mgr::Scene::ActiveScene::quit();
            mgr::Scene::create<ResultScene>(score, max_combo, accuracy, perfect, great, miss, bs, tex, idx);
            return;
        }
    }
}

void GameplayScene::measure() {
    Scene::measure();
    if (paused_) pause_overlay_->measure({ .x = 0.F, .y = 0.F });
}

auto GameplayScene::render() const -> bool {
    if (!Scene::render()) return false;

    for (Uint8 t = 0; t < static_cast<Uint8>(track_highlights_.size()); ++t) {
        const auto& h = track_highlights_[t];
        if (h.alpha <= 0.0f) continue;
        static constexpr int NUM_STRIPS = 16;
        static constexpr float HIGHLIGHT_HEIGHT = 500.0f;
        float x = static_cast<float>(board_->get_track_left_x() + t * board_->get_track_width());
        float w = static_cast<float>(board_->get_track_width());
        float bottom_y = static_cast<float>(mgr::Video::height);
        float strip_h = HIGHLIGHT_HEIGHT / NUM_STRIPS;
        SDL_SetRenderDrawBlendMode(mgr::Video::renderer, SDL_BLENDMODE_BLEND);
        for (int s = 0; s < NUM_STRIPS; ++s) {
            float strip_alpha_f = h.alpha * (1.0f - static_cast<float>(s) / NUM_STRIPS);
            Uint8 strip_alpha = static_cast<Uint8>(strip_alpha_f * 100.0f);
            if (strip_alpha == 0) continue;
            SDL_FRect rect = { .x = x, .y = bottom_y - (s + 1) * strip_h, .w = w, .h = strip_h };
            SDL_SetRenderDrawColor(mgr::Video::renderer, h.color.r, h.color.g, h.color.b, strip_alpha);
            SDL_RenderFillRect(mgr::Video::renderer, &rect);
        }
    }

    if (paused_) {
        SDL_SetRenderDrawBlendMode(mgr::Video::renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(mgr::Video::renderer, 0, 0, 0, 128);
        SDL_FRect dim{ .x = 0, .y = 0, .w = static_cast<float>(mgr::Video::width), .h = static_cast<float>(mgr::Video::height) };
        SDL_RenderFillRect(mgr::Video::renderer, &dim);
        return pause_overlay_->render();
    }
    return true;
}
