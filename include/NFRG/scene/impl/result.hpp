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
#include "NFRG/util/beatmap_set.hpp"

namespace nfrg::scene {

class ResultScene : public Scene {
protected:
    Uint64 score_;
    Uint32 max_combo_;
    float accuracy_;
    Uint32 perfect_;
    Uint32 great_;
    Uint32 miss_;

    util::BeatmapSet* beatmapset_ = nullptr;
    size_t beatmap_index_ = 0;
    SDL_Texture* bgi_texture_ = nullptr;

public:
    ResultScene(Uint64 score, Uint32 max_combo, float accuracy,
                Uint32 perfect, Uint32 great, Uint32 miss,
                util::BeatmapSet* beatmapset, SDL_Texture* bgi_texture, size_t beatmap_index);

    void handle_key_event(SDL_KeyboardEvent& event) override;
    void update() override {}
};

}
