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
#include "../scene.hpp"

#include <unordered_map>

#include "NFRG/ui/gameplay/board.hpp"
#include "NFRG/ui/list.hpp"
#include "NFRG/ui/text.hpp"
#include "NFRG/util/beatmap.hpp"

namespace nfrg {

namespace util {
class BeatmapSet;
}

namespace scene {

class GameplayScene : public Scene {
protected:
    struct {
        Uint32 perfect = 0;  // 完美判定次数
        Uint32 great = 0;    // 良好命中次数
        Uint32 miss = 0;     // 失误次数
    } counters_;
    Uint32 combo_ = 0;      // 连击次数
    Uint32 max_combo_ = 0;  // 最大连击次数

    ui::gameplay::Board* board_;  // 显示轨道背景和音符的组件
    ui::Text* text_score_;        // 显示分数的文本组件
    ui::Text* text_combo_;        // 显示连击数的文本组件
    ui::Text* text_accuracy_;     // 显示准确度的文本组件

    std::unordered_map<SDL_Keycode, Uint8> key_to_track_;

    bool paused_ = false;
    Uint64 last_progress_ns_ = 0;
    Uint64 end_wait_start_ = 0;
    Uint64 last_wall_ns_ = 0;

    std::shared_ptr<ui::Group> pause_overlay_;
    ui::List* pause_list_;

    util::BeatmapSet* beatmapset_ = nullptr;
    size_t beatmap_index_ = 0;
    SDL_Texture* bgi_texture_ = nullptr;

    struct TrackHighlight {
        float alpha = 0.0f;
        SDL_Color color = { .r = 255, .g = 255, .b = 255, .a = 255 };
    };
    std::vector<TrackHighlight> track_highlights_;
    struct JudgmentPopup {
        std::shared_ptr<ui::Text> text;
        SDL_Color color;
        Uint64 start_ns;
        static constexpr Uint64 DURATION_NS = 600'000'000;
        static constexpr float MOVE_UP_PX = 40.0f;
        float base_y = 0.0f;
    };
    std::vector<JudgmentPopup> judgment_popups_;

    void update_text();
    void toggle_pause();
    void highlight_track(Uint8 track, Sint8 result);
    void show_judgment(Sint8 result);

public:
    GameplayScene(const util::Beatmap& beatmap, util::BeatmapSet* beatmapset,
        SDL_Texture* bgi_texture, size_t beatmap_index = 0);

    void process_animators(Uint64 delta_ns) override;
    void handle_key_event(SDL_KeyboardEvent& event) override;
    void update() override;
    void measure() override;
    auto render() const -> bool override;
};
}

}
