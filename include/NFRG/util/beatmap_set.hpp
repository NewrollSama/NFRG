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
#include "beatmap.hpp"

#include <fstream>
#include <sndfile.hh>

namespace nfrg::util {

class BeatmapSet {
protected:
    std::ifstream ifs_;
    Uint16 bpm_;
    std::vector<Beatmap> beatmaps_;
    Uint32 bgm_offset_;
    Uint32 bgm_size_;
    std::string bgm_title_;
    std::string bgm_artist_;
    Uint32 bgm_preview_offset_;
    Uint32 bgi_offset_;
    Uint32 bgi_size_;

    SF_VIRTUAL_IO bgm_;
    SDL_IOStreamInterface bgi_iface;

public:
    BeatmapSet(std::string& beatmapset_path);

    [[nodiscard]] auto get_bpm() const -> Uint16 {
        return bpm_;
    }

    [[nodiscard]] auto get_bgm_title() const -> std::string_view {
        return bgm_title_;
    }

    [[nodiscard]] auto get_bgm_artist() const -> std::string_view {
        return bgm_artist_;
    }

    [[nodiscard]] auto get_preview_offset() const -> Uint32 {
        return bgm_preview_offset_;
    }

    [[nodiscard]] auto get_beatmaps() const -> const std::vector<Beatmap>& {
        return beatmaps_;
    }

    [[nodiscard]] auto get_ifs() -> std::ifstream& {
        return ifs_;
    }

    auto get_bgm() -> std::unique_ptr<SndfileHandle> {
        ifs_.seekg(bgm_offset_, std::ios::beg);
        return std::make_unique<SndfileHandle>(bgm_, this, SFM_READ);
    }

    auto get_bgi_vio() -> SDL_IOStream* {
        ifs_.seekg(bgi_offset_, std::ios::beg);
        return SDL_OpenIO(&bgi_iface, static_cast<void*>(this));
    }

    [[nodiscard]] auto get_beatmaps_info() const -> std::vector<std::pair<Uint8, std::string_view>> {
        std::vector<std::pair<Uint8, std::string_view>> beatmaps_info;
        beatmaps_info.reserve(beatmaps_.size());
        for (const auto& beatmap : beatmaps_) {
            // SDL_Log("  - [%dK] %s", beatmap.get_trucks(), beatmap.get_caption().data());
            beatmaps_info.emplace_back(beatmap.get_trucks(), beatmap.get_caption());
        }
        return beatmaps_info;
    }
};

}
