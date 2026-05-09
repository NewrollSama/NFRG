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
#include "NFRG/ui/select/beatmap_set.hpp"
#include "NFRG/util/beatmap_set.hpp"

namespace nfrg::scene {

class SelectScene : public Scene {
protected:
    /**
     * @brief 谱面集文件夹的路径。
     */
    std::string beatmapsets_dir_;
    /**
     * @brief 存储所有谱面集。
     */
    std::vector<std::unique_ptr<util::BeatmapSet>> beatmapsets_;
    /**
     * @brief 当前浏览的谱面集索引。
     */
    Uint64 current_beatmapset_index_ = 0;
    /**
     * @brief 当前谱面集背景图片的纹理。
     */
    SDL_Texture* bgi_texture_;
    /**
     * @brief 场景中展示
     */
    std::shared_ptr<ui::BeatmapSet> beatmapset_;
    /**
     * @brief 不为空时，以此哈希值定位谱面集。
     * 用于导入谱面集时定位导入的谱面集。
     */
    std::string reload_focus;

    /**
     * @brief 加载当前谱面集的资源和信息。
     */
    void load_current_beatmapset();

    /**
     * @brief 重新加载谱面集。
     */
    void reload();

public:
    SelectScene();

    /**
     * @brief 导入指定路径的谱面集文件。
     *
     * @param beatmapset_path 待导入的谱面集文件的路径。
     * @return std::string 导入文件的 SHA-256 哈希值，若导入失败则为空。
     */
    auto import_beatmapset(std::string&& beatmapset_path) -> std::string;

    void handle_key_event(SDL_KeyboardEvent& event) override;

    void update() override;

    ~SelectScene() override = default;
};

}
