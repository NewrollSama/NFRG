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
#include <sndfile.hh>

namespace nfrg::mgr {

class Audio {
private:
    static SDL_AudioDeviceID audio_device;      // 播放音频使用的音频设备。
    static SDL_AudioStream* audio_stream_bg;    // 背景音乐使用的音频流。
    static SDL_AudioStream* audio_stream_se;    // 音效使用的音频流。
    static std::vector<Sint16> audio_buf_1;     // 底鼓音效的数据。
    static std::vector<Sint16> audio_buf_2;     // 军鼓音效的数据。
    static bool skip_clear_bg;                  // 跳过下一次清除背景音乐流。
    static struct BGPlayback {                  // 背景音乐播放相关数据。
        std::unique_ptr<SndfileHandle> snd_bg;  // 负责音频解码的库接口。
        Sint64 samples;                         // 已处理的采样数。
        Uint64 timestamp;                       // 播放至以上采样时的绝对时间。
        bool record;                            // 是否要记录以上信息以实现计算播放进度。
        bool playing = false;                   // 是否正在播放。
    } bg_playback;

public:
    /**
     * @brief 应用初始化时进行的音频初始化。
     * @return 初始化是否无异常。
     */
    static auto open_audio() -> bool;
    /**
     * @brief 应用退出时的处理。
     */
    static void close_audio();

    /**
     * @brief 将指定路径的文件作为背景音乐进行播放。
     * @param file 要播放的音频文件。
     */
    static void play_bg(const char* file);
    /**
     * @brief 使用自定义的 SndfileHandle 作为背景音乐进行播放。
     * @param sndfile 自定义的 SndfileHandle。
     * @param play 立即开始播放音乐。
     * @param enable_record 需要记录播放进度。
     */
    static void play_bg(std::unique_ptr<SndfileHandle>&& sndfile, bool play = true, bool enable_record = false);

    /**
     * @brief 通过当前的绝对时间和背景音乐的播放进度，计算背景音乐开始时的绝对时间。
     * @return 背景音乐开始时的绝对时间，单位为纳秒。
     */
    [[nodiscard]] static auto get_bg_start_time() -> Uint64;
    /**
     * @brief 通过记录的数据，计算背景音乐的播放进度。
     * @return Uint64 音乐的播放进度，单位为纳秒。
     */
    [[nodiscard]] static auto get_bg_playback_progress() -> Uint64;

    /**
     * @brief 暂停背景音乐的播放。
     */
    static void pause_bg();
    /**
     * @brief 继续背景音乐的播放。
     */
    static void resume_bg();
    /**
     * @brief 停止背景音乐的播放。
     */
    static void clear_bg();

    /**
     * @brief 修改 bg_playback.samples，使其在开始播放前出现一段时间的无声。
     *
     * @param duration 无声持续的时间，单位为秒。
     */
    static void bg_set_silence(float duration);

    /**
     * @brief 使下一次调用的 clear_bg 无效。
     */
    static void prevent_next_clear_bg();

    /**
     * @brief 停止音效的播放。
     */
    static void clear_se();

    /**
     * @brief 向音效音频流中添加音效。
     * @param kind 音效种类。
     */
    static void fill_stream_se(int kind);
};

}
