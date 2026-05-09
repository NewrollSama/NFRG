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
#include "../builder/component.hpp"

#include "NFRG/ui/gameplay/note.hpp"

#include <deque>
#include <cstddef>

namespace nfrg {

namespace scene {
class GameplayScene;
class Scene;
}

namespace ui::gameplay {

class Board : public Component {
    friend class Base::BuilderTemplate<Board>;

protected:
    /**
     * @brief 轨道数量。
     */
    Uint8 num_tracks_;
    /**
     * @brief 单列轨道的宽度。
     */
    int track_width_ = 0;
    /**
     * @brief 轨道最左侧的位置。
     */
    int track_left_x_ = 0;
    /**
     * @brief 判定线的 y 轴位置。
     */
    float detection_line_y_ = 0;

    /**
     * @brief 歌曲的每分钟节拍数。
     */
    Uint16 bpm_ = 120;

    /**
     * @brief 分别存储每个轨道的音符。
     */
    std::vector<std::deque<std::shared_ptr<Note>>> notes_;
    /**
     * @brief 记录每个轨道是否正在处理长条。
     */
    std::vector<bool> lanes_;
    /**
     * @brief 记录每个轨道最多启动到第几个音符的动画器。
     */
    std::vector<size_t> animator_next_index_;
    /**
     * @brief 记录每个轨道下一次判定的索引。
     */
    std::vector<size_t> judge_next_;
    /**
     * @brief 记录本帧内超时 Miss 的轨道索引。
     */
    std::vector<Uint8> overtime_missed_tracks_;

    /**
     * @brief 将六十四分之一拍数转换为对应的时间。
     *
     * @param beat 六十四分之一拍数。
     * @return 以纳秒为单位的时间。
     */
    [[nodiscard]] auto beat_to_ns(Uint16 beat) const -> Uint64;
    /**
     * @brief 计算第几条轨道的 x 轴位置。
     *
     * @param track 需要计算的轨道。
     * @return 轨道的 x 轴位置。
     */
    [[nodiscard]] auto track_to_x(Uint8 track) const -> float;
    /**
     * @brief 将长条的六十四分之一拍数转换为对应的高度。
     *
     * @param beat 长条的六十四分之一拍数。
     * @return 长条的高度。
     */
    [[nodiscard]] auto beat_to_lane_height(Uint16 beat) const -> float;

    Board() = default;

public:
    class Builder;

    /**
     * @brief 对特定轨道进行判定。
     *
     * @param track 要判定的轨道。
     * @param event 对应的 SDL 键盘事件。
     * @return Sint8 判定结果，0 为 Miss，1 为 Great，2 为 Perfect；
     * 255 表示不满足判定条件，因此没有进行判定。
     */
    auto track_event(Uint8 track, SDL_KeyboardEvent& event) -> Sint8;

    /**
     * @brief 读取特定谱面文件，将其加载到游戏中。
     *
     * @param beatmap_path 谱面文件的路径。
     * @param gameplay 包含并调用了该对象的游玩场景类，用于将动画器加入到场景中。
     */
    // void load_beatmap(std::string&& beatmap_path, scene::GameplayScene& gameplay);

    /**
     * @brief 检查并判定超时的音符。
     *
     * @param ticks_ns 当前的时间。
     * @return 超时的音符数。
     */
    auto detect_overtime(Uint64 ticks_ns) -> Uint8;

    [[nodiscard]] auto all_judged() const -> bool;

    [[nodiscard]] auto get_track_left_x() const -> int { return track_left_x_; }
    [[nodiscard]] auto get_track_width() const -> int { return track_width_; }
    [[nodiscard]] auto get_detection_line_y() const -> float { return detection_line_y_; }
    [[nodiscard]] auto get_overtime_missed_tracks() const -> const std::vector<Uint8>& { return overtime_missed_tracks_; }

    /**
     * @brief 按音频进度激活待音符的动画器。
     *
     * @param progress 当前的音频进度，单位为纳秒。
     * @param scene 要添加动画器的场景。
     */
    void activate_pending_notes(Uint64 progress, scene::Scene& scene);

    [[nodiscard]] auto render() const -> bool override;
};

class Board::Builder : public Component::BuilderTemplate<Board> {
public:
    Builder() = default;

    /**
     * @brief 设定轨道数量，并据此进行初始化。
     *
     * @param num_tracks 要设定的轨道数量。
     * @return 建造者的引用，以便链式调用。
     */
    auto set_num_tracks(Uint8 num_tracks) -> Builder&;

    auto set_bpm(Uint16 bpm) -> Builder&;

    auto load_beatmap(const std::vector<Uint16>& beatmap_binary, Sint64 delay) -> Builder&;
};

}

}