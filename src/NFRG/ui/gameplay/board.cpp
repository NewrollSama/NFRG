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
#include "NFRG/ui/gameplay/board.hpp"
using namespace nfrg;
using namespace ui;
using namespace gameplay;

#include "NFRG/anim/note_motion.hpp"
#include "NFRG/mgr/audio.hpp"
#include "NFRG/mgr/config.hpp"

constexpr Sint64 MISS_THRESHOLD = 120'000'000;
constexpr Sint64 GREAT_THRESHOLD = 50'000'000;

auto Board::beat_to_ns(Uint16 beat) const -> Uint64 {
    return static_cast<Uint64>(beat & static_cast<Uint16>(0x7FFF)) * 60'000'000'000 / 64 / bpm_;
}

auto Board::track_to_x(Uint8 track) const -> float {
    return track_left_x_ + track * track_width_ + 10.F;
}

auto Board::beat_to_lane_height(Uint16 beat) const -> float {
    return 60.F / bpm_ / 64 * static_cast<Uint64>(beat & static_cast<Uint16>(0x7FFF)) * detection_line_y_ / mgr::Config::spd * 1000 + 20;
}

auto Board::track_event(Uint8 track, SDL_KeyboardEvent& event) -> Sint8 {
    const bool is_release_event = lanes_[track];

    if (!is_release_event && !event.down)
        return -1;

    while (judge_next_[track] < notes_[track].size() && notes_[track][judge_next_[track]] == nullptr) {
        ++judge_next_[track];
    }
    if (judge_next_[track] >= notes_[track].size())
        return -1;

    const auto& note = notes_[track][judge_next_[track]];
    Sint64 note_target_time = (is_release_event ? note->get_release_time() : note->get_hit_time());
    Sint64 diff = event.timestamp - mgr::Audio::get_bg_start_time() - note_target_time;
    // SDL_Log("%llu %llu", event.timestamp, note_target_time);
    Uint8 res = 0;
    if (diff < -MISS_THRESHOLD) {
        if (!is_release_event) return -1;  // 过早按下不判定
        res = 0;                           // 过早松开判定为 Miss
    } else if (std::abs(diff) > GREAT_THRESHOLD) {
        res = 1;  // 判定为 Great
    } else {
        res = 2;  // 判定为 Perfect
    }

    if (is_release_event) {
        lanes_[track] = false;
    }

    if (!note->has_lane() || is_release_event) {
        notes_[track][judge_next_[track]] = nullptr;
    } else {
        lanes_[track] = true;
    }

    return res;
}

static inline auto beat_first_bit(Uint16 beat) -> bool {
    return beat & 0x8000;
}

auto Board::detect_overtime(Uint64 ticks_ns) -> Uint8 {
    Uint8 cnt = 0;
    overtime_missed_tracks_.clear();
    for (Uint8 i = 0; i < num_tracks_; ++i) {
        while (judge_next_[i] < notes_[i].size() && notes_[i][judge_next_[i]] == nullptr) {
            ++judge_next_[i];
        }
        if (judge_next_[i] >= notes_[i].size())
            continue;
        const auto& note = notes_[i][judge_next_[i]];
        Sint64 note_hit_time = lanes_[i] ? note->get_release_time() : note->get_hit_time();
        if (static_cast<Sint64>(ticks_ns) - note_hit_time > MISS_THRESHOLD) {
            ++cnt;
            overtime_missed_tracks_.push_back(i);
            notes_[i][judge_next_[i]] = nullptr;
            if (lanes_[i]) {
                lanes_[i] = false;
            }
        }
    }
    return cnt;
}

auto Board::all_judged() const -> bool {
    for (Uint8 i = 0; i < num_tracks_; ++i) {
        if (judge_next_[i] < notes_[i].size()) {
            return false;
        }
    }
    return true;
}

auto Board::render() const -> bool {
    if (!Component::render()) return false;  // 渲染轨道背景

    // 渲染音符
    for (const auto& track_notes : notes_) {
        for (const auto& note : track_notes) {
            if (note == nullptr) continue;
            if (!note->render()) return false;
        }
    }
    return true;
}

auto Board::Builder::set_num_tracks(Uint8 num_tracks) -> Board::Builder& {
    component_->num_tracks_ = num_tracks;
    component_->notes_.resize(num_tracks);
    component_->lanes_.resize(num_tracks);
    component_->animator_next_index_.resize(num_tracks);
    component_->judge_next_.resize(num_tracks);

    component_->set_texture(SDL_CreateTexture(mgr::Video::renderer,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        mgr::Video::width, mgr::Video::height));

    const int track_width = component_->track_width_ = mgr::Video::width / std::max(num_tracks, static_cast<Uint8>(12));
    const int tracks_width = track_width * num_tracks;
    const int left = component_->track_left_x_ = (mgr::Video::width - tracks_width) / 2;

    SDL_Color backup_color;
    SDL_GetRenderDrawColor(mgr::Video::renderer, &backup_color.r, &backup_color.g, &backup_color.b, &backup_color.a);

    SDL_SetRenderTarget(mgr::Video::renderer, component_->texture_);
    SDL_SetRenderDrawColor(mgr::Video::renderer, 0, 0, 0, 128);
    SDL_RenderClear(mgr::Video::renderer);
    SDL_SetRenderDrawColor(mgr::Video::renderer, 255, 255, 255, 255);
    int right = left + tracks_width;
    float detection_line_y = component_->detection_line_y_ = std::min(mgr::Video::height * 0.9f, static_cast<float>(mgr::Video::height) - 120.0f);
    for (int i = left; i < right; i += track_width) {
        SDL_RenderLine(mgr::Video::renderer, static_cast<float>(i), 0.0f, static_cast<float>(i), static_cast<float>(mgr::Video::height));
        SDL_RenderLine(mgr::Video::renderer, static_cast<float>(i + 10), detection_line_y, static_cast<float>(i + track_width - 10), detection_line_y);
    }
    SDL_RenderLine(mgr::Video::renderer, static_cast<float>(right), 0.0f, static_cast<float>(right), static_cast<float>(mgr::Video::height));
    SDL_SetRenderTarget(mgr::Video::renderer, nullptr);
    SDL_SetRenderDrawColor(mgr::Video::renderer, backup_color.r, backup_color.g, backup_color.b, backup_color.a);
    return *this;
}

auto Board::Builder::set_bpm(Uint16 bpm) -> Board::Builder& {
    component_->bpm_ = bpm;
    return *this;
}

auto Board::Builder::load_beatmap(const std::vector<Uint16>& beatmap_binary, Sint64 delay_ms) -> Board::Builder& {
    delay_ms += mgr::Config::latency_ms;
    if (delay_ms < mgr::Config::spd) {
        mgr::Audio::bg_set_silence((mgr::Config::spd - delay_ms) / 1'000.f);
    }

    struct Track {
        Uint64 hit_time = 0;
        bool has_lane = false;
        bool no_more = false;
    };

    std::vector<Track> current_tracks;
    current_tracks.resize(component_->num_tracks_);

    auto beatmap_it = beatmap_binary.begin();
    // SDL_Log("tracks size: %zu, beatmap size: %zu", current_tracks.size(), beatmap_binary.size());
    for (int i = 0; i < component_->num_tracks_; ++i) {  // 第一轮处理，只读取第一个音符出现的时间
        if (beat_first_bit(beatmap_it[i])) {
            current_tracks[i].no_more = true;
        }
        current_tracks[i].hit_time = delay_ms * 1'000'000 + component_->beat_to_ns(beatmap_it[i]);
    }
    for (beatmap_it += component_->num_tracks_; beatmap_it != beatmap_binary.end(); beatmap_it += component_->num_tracks_) {
        for (Uint8 i = 0; i < component_->num_tracks_; ++i) {
            if (current_tracks[i].no_more) continue;
            const auto& beat = beatmap_it[i];
            auto& track_hit_time = current_tracks[i].hit_time;
            auto& note_has_lane = current_tracks[i].has_lane;

            if (note_has_lane) {              // 上一次是长条的开头
                if (!beat_first_bit(beat)) {  // 顺便检查一下文件内容有没有问题
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Corrupted beatmap.");
                }
                note_has_lane = false;
            } else {  // 创建新的音符
                const auto track_width = static_cast<float>(component_->track_width_);
                const float x = component_->track_to_x(i);
                Uint64 note_hit_time = track_hit_time;
                auto note_builder = ui::gameplay::Note::Builder();
                note_builder
                    .set_hit_time(note_hit_time)
                    .set_release_time(note_hit_time + component_->beat_to_ns(beat))
                    .set_size({ .x = track_width - 20, .y = 40.F })
                    .set_pos({ .x = x, .y = -40.F });
                if (beat_first_bit(beat)) {
                    note_builder.set_lane(component_->beat_to_lane_height(beat));
                    note_has_lane = true;
                }
                auto note = note_builder.build();
                component_->notes_[i].push_back(std::move(note));
            }

            const Uint64 next_hit_time = component_->beat_to_ns(beat);
            if (next_hit_time != 0) {
                track_hit_time += next_hit_time;
            } else {
                current_tracks[i].no_more = true;
            }
        }
    }
    return *this;
}

void Board::activate_pending_notes(Uint64 progress_ns, scene::Scene& scene) {
    for (Uint8 t = 0; t < num_tracks_; ++t) {
        size_t idx = animator_next_index_[t];
        for (; idx < notes_[t].size(); ++idx) {
            if (notes_[t][idx] == nullptr) continue;
            Uint64 anim_start = notes_[t][idx]->get_hit_time() - static_cast<Uint64>(mgr::Config::spd * 1'000'000);
            if (progress_ns < anim_start) break;
            float x = track_to_x(t);
            auto animator = anim::NoteMotion::Builder()
                                .set_disposable(false)
                                .set_source({ .x = x, .y = -40 })
                                .set_destination({ .x = x, .y = detection_line_y_ })
                                .set_duration(mgr::Config::spd)
                                .bind_component(notes_[t][idx])
                                .build();
            animator->start(progress_ns - anim_start);
            scene.add_animator(std::move(animator));
        }
        animator_next_index_[t] = idx;
    }
}
