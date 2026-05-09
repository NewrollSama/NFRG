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
#include "NFRG/scene/impl/select.hpp"
using namespace nfrg;
using namespace scene;

#include "NFRG/mgr/audio.hpp"
#include "NFRG/mgr/scene.hpp"
#include "NFRG/platform/platform.hpp"
#include "NFRG/scene/impl/gameplay.hpp"
#include "NFRG/ui/image.hpp"
#include "NFRG/ui/select/beatmap_set.hpp"

#include <format>

auto load_beatmapsets(std::vector<std::unique_ptr<util::BeatmapSet>>& beatmapsets,
    std::string& beatmapsets_dir, std::string_view focus = "") -> Sint64 {
    // 确认谱面集目录存在
    SDL_PathInfo path_info;
    if (!SDL_GetPathInfo(beatmapsets_dir.c_str(), &path_info)) {
        SDL_CreateDirectory(beatmapsets_dir.c_str());
        return -1;
    } else if (path_info.type != SDL_PATHTYPE_DIRECTORY) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Path unavailable");
        return -1;
    }

    std::string focused_file = std::string(focus) + ".bms";
    Sint64 idx = 0;
    auto pair = std::make_tuple(&beatmapsets, focused_file, &idx);

    // 遍历谱面集目录下的文件
    SDL_EnumerateDirectory(
        beatmapsets_dir.c_str(),
        [](void* userdata, const char* dirname, const char* fname) -> SDL_EnumerationResult {
            if (SDL_strcmp(SDL_strndup(fname + SDL_strlen(fname) - 4, 5), ".bms") != 0) {
                return SDL_ENUM_CONTINUE;  // 跳过后缀不符的文件
            }
            auto [beatmapsets, focused, idx_ptr] = *static_cast<decltype(pair)*>(userdata);
            if (focused.compare(fname) == 0) {
                *idx_ptr = beatmapsets->size();
            }
            std::string beatmapset_path;
            beatmapset_path.reserve(SDL_strlen(dirname) + SDL_strlen(fname));
            beatmapset_path.append(dirname).append(fname);
            beatmapsets->push_back(std::make_unique<util::BeatmapSet>(beatmapset_path));
            return SDL_ENUM_CONTINUE;
        },
        &pair);
    return idx;
}

void SelectScene::load_current_beatmapset() {
    auto& beatmapset = beatmapsets_[current_beatmapset_index_];
    if (bgi_texture_ != nullptr) {
        SDL_DestroyTexture(bgi_texture_);
    }
    bgi_texture_ = IMG_LoadTexture_IO(mgr::Video::renderer, beatmapset->get_bgi_vio(), true);
    if (bgi_texture_ == nullptr) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "From SelectScene::load_current_beatmapset():");
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
    }
    auto beatmapset_ptr = ui::BeatmapSet::Builder()
                              .set_size(mgr::Video::get_window_size())
                              .set_bgi_texture(bgi_texture_)
                              .set_title(beatmapset->get_bgm_title())
                              .set_artist(beatmapset->get_bgm_artist())
                              .set_beatmaps(beatmapset->get_beatmaps_info())
                              .build();
    if (beatmapset_ == nullptr) {
        beatmapset_ = beatmapset_ptr;
        components_.push_back(beatmapset_);
    } else {
        *beatmapset_ = std::move(*beatmapset_ptr);
    }
    auto bgm = beatmapset->get_bgm();
    bgm->seek(bgm->samplerate() * beatmapset->get_preview_offset() / 1000, SF_SEEK_SET);
    mgr::Audio::play_bg(std::move(bgm));
}

void SelectScene::reload() {
    mgr::Audio::clear_bg();
    beatmapsets_.clear();
    current_beatmapset_index_ = load_beatmapsets(beatmapsets_, beatmapsets_dir_, reload_focus);
    load_current_beatmapset();
}

SelectScene::SelectScene() {
    const char* pref_path = SDL_GetPrefPath("NewrollSama", "NFRG");
    beatmapsets_dir_.reserve(SDL_strlen(pref_path) + sizeof("beatmapsets") - 1);
    beatmapsets_dir_.append(pref_path).append("beatmapsets");

    current_beatmapset_index_ = load_beatmapsets(beatmapsets_, beatmapsets_dir_);

    if (beatmapsets_.size() > 0) {
        load_current_beatmapset();
    }

    const auto window_w = static_cast<float>(mgr::Video::width);
    SDL_Surface* bar_surf = SDL_CreateSurface(1, 1, SDL_PIXELFORMAT_RGBA8888);
    SDL_FillSurfaceRect(bar_surf, nullptr, SDL_MapSurfaceRGBA(bar_surf, 0, 0, 0, 160));
    SDL_Texture* bar_tex = SDL_CreateTextureFromSurface(mgr::Video::renderer, bar_surf);
    SDL_DestroySurface(bar_surf);

    ui::Image::Builder()
        .set_texture(bar_tex, true)
        .set_pos({ .x = 0, .y = 0 })
        .set_size({ .x = window_w, .y = 48 })
        .add_to(this);

    auto hint_text = ui::Text::Builder()
                         .set_content("按 Ctrl+I 导入谱面")
                         .set_font_size(24)
                         .set_foreground_color({ .r = 255, .g = 255, .b = 255, .a = 255 })
                         .build();
    hint_text->set_pos({ .x = window_w - hint_text->get_size().x - 16, .y = (48 - hint_text->get_size().y) / 2 });
    components_.push_back(hint_text);
}

auto SelectScene::import_beatmapset(std::string&& beatmapset_path) -> std::string {
    platform::SHA256Calculator calc;
    std::ifstream ifs(beatmapset_path, std::ios::binary | std::ios::in);
    std::array<char, 8192> buffer;
    while (ifs.read(buffer.data(), buffer.size()) || ifs.gcount() > 0) {
        calc.update(buffer.data(), buffer.size());
    }
    std::string hash{ calc.finalize() };
    bool res = SDL_CopyFile(beatmapset_path.c_str(),
        std::format("{}/{}.bms", beatmapsets_dir_, hash).c_str());
    if (res) {
        return hash;
    } else {
        return "";
    }
}

void SelectScene::handle_key_event(SDL_KeyboardEvent& event) {
    if (!event.down) return;
    switch (event.key) {
    case SDLK_I: {
        if (event.mod & SDL_KMOD_CTRL) {
            constexpr std::array filters{ SDL_DialogFileFilter{ .name = "NFRG 谱面集", .pattern = "bms;nfrg" } };
            SDL_ShowOpenFileDialog([](void* userdata, const char* const* filelist, int /*filter*/) -> void {
                auto& scene = *static_cast<SelectScene*>(userdata);
                if (filelist == nullptr) {
                    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
                    return;
                } else if (*filelist == nullptr) {
                    return;
                }
                std::string hash;
                for (; *filelist != nullptr; ++filelist) {
                    SDL_Log("File name: %s", *filelist);
                    hash = scene.import_beatmapset(*filelist);
                    if (hash.empty()) {
                        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "%s", SDL_GetError());
                    }
                }
                SDL_Log("Imported");
                scene.reload_focus = std::move(hash);
            },
                this, mgr::Video::window, filters.data(), static_cast<int>(filters.size()), nullptr, false);
        }
        break;
    }
    case SDLK_UP: {
        beatmapset_->change_beatmaps_current_index(-1);
        break;
    }
    case SDLK_DOWN: {
        beatmapset_->change_beatmaps_current_index(1);
        break;
    }
    case SDLK_LEFT: {
        if (current_beatmapset_index_ == 0) {
            current_beatmapset_index_ = beatmapsets_.size() - 1;
        } else {
            --current_beatmapset_index_;
        }
        load_current_beatmapset();
        break;
    }
    case SDLK_RIGHT: {
        if (current_beatmapset_index_ == beatmapsets_.size() - 1) {
            current_beatmapset_index_ = 0;
        } else {
            ++current_beatmapset_index_;
        }
        load_current_beatmapset();
        break;
    }
    case SDLK_RETURN: {
        mgr::Audio::clear_bg();
        mgr::Audio::play_bg(beatmapsets_[current_beatmapset_index_]->get_bgm(), false, true);
        const auto& beatmapset = beatmapsets_[current_beatmapset_index_];
        const util::Beatmap& beatmap = beatmapset->get_beatmaps()[beatmapset_->get_selected_beatmap()];
        mgr::Scene::create<scene::GameplayScene>(beatmap, beatmapset.get(), bgi_texture_, beatmapset_->get_selected_beatmap());
        break;
    }
    default: {
        Scene::handle_key_event(event);
        break;
    }
    }
}

void SelectScene::update() {
    if (!reload_focus.empty()) {
        reload();
        reload_focus.clear();
    }
}
