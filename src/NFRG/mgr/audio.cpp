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
#include "NFRG/mgr/audio.hpp"
using namespace nfrg;
using namespace mgr;

#include <array>
#include <random>

SDL_AudioDeviceID Audio::audio_device = 0;
SDL_AudioStream* Audio::audio_stream_bg = nullptr;
SDL_AudioStream* Audio::audio_stream_se = nullptr;
std::vector<Sint16> Audio::audio_buf_1;
std::vector<Sint16> Audio::audio_buf_2;
bool Audio::skip_clear_bg = false;
Audio::BGPlayback Audio::bg_playback{
    .snd_bg = nullptr,
    .samples = 0,
    .timestamp = 0,
    .record = false,
};

enum class DrumType : Uint8 {
    KICK_DRUM,
    SNARE_DRUM,
    HI_HAT,
};

static void generate_noise(
    std::vector<Sint16>& buf, const SDL_AudioSpec spec, const Sint16 amp, DrumType type) {
    double duration;
    switch (type) {
    case DrumType::KICK_DRUM:
        duration = 0.5;
        break;
    case DrumType::SNARE_DRUM:
        duration = 0.3;
        break;
    case DrumType::HI_HAT:
        duration = 0.1;
        break;
    default:
        duration = 0.2;
    }

    auto totalSamples = static_cast<size_t>(duration * spec.freq);
    buf.resize(totalSamples * spec.channels);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    for (size_t i = 0; i < totalSamples; i++) {
        double time = static_cast<double>(i) / spec.freq;
        double envelope;
        double sample = 0.0;

        switch (type) {
        case DrumType::KICK_DRUM: {  // 底鼓
            envelope = exp(-time * 15.0);
            double freq = 100.0 * exp(-time * 8.0);
            sample = sin(2.0 * SDL_PI_D * freq * time) * 0.8;
            sample += dis(gen) * 0.2 * exp(-time * 25.0);
            break;
        }
        case DrumType::SNARE_DRUM: {  // 军鼓
            envelope = exp(-time * 25.0);
            double noise = dis(gen);
            double tone = sin(2.0 * SDL_PI_D * 180.0 * time) * exp(-time * 15.0);
            sample = noise * 0.7 + tone * 0.3;
            break;
        }
        case DrumType::HI_HAT: {  // 踩镲
            envelope = exp(-time * 50.0);
            sample = dis(gen);
            // 添加轻微的高频振荡
            sample += sin(2.0 * SDL_PI_D * 10000.0 * time) * 0.2;
            break;
        default:
            envelope = 0;
            break;
        }
        }

        // 应用包络
        sample *= envelope;

        // 转换为16位有符号整数
        auto sampleValue = static_cast<Sint16>(sample * amp);

        // 填充所有声道
        for (int ch = 0; ch < spec.channels; ch++) {
            buf[i * spec.channels + ch] = sampleValue;
        }
    }
}

static inline void stream_put_empty(SDL_AudioStream* stream, int amount) {
    std::array<float, 1024> zeros{ 0 };
    for (; static_cast<Uint64>(amount) > zeros.size(); amount -= static_cast<int>(zeros.size())) {
        SDL_PutAudioStreamData(stream, zeros.data(), static_cast<int>(zeros.size() * sizeof(float)));
    }
    SDL_PutAudioStreamData(stream, zeros.data(), static_cast<int>(amount * sizeof(float)));
}

auto Audio::open_audio() -> bool {
    audio_device = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

    SDL_AudioSpec audio_spec;
    SDL_GetAudioDeviceFormat(audio_device, &audio_spec, nullptr);

    SDL_AudioSpec bg_spec{ .format = SDL_AUDIO_F32, .channels = 2, .freq = 48000 };
    SDL_AudioSpec se_spec{ .format = SDL_AUDIO_S16, .channels = 1, .freq = 48000 };
    audio_stream_bg = SDL_CreateAudioStream(&bg_spec, &audio_spec);
    audio_stream_se = SDL_CreateAudioStream(&se_spec, &audio_spec);

    std::array streams{ audio_stream_bg, audio_stream_se };
    SDL_BindAudioStreams(audio_device, streams.data(), static_cast<int>(streams.size()));
    generate_noise(audio_buf_1, se_spec, 15000, DrumType::KICK_DRUM);
    generate_noise(audio_buf_2, se_spec, 8000, DrumType::SNARE_DRUM);

    auto bg_callback = [](void* userdata, SDL_AudioStream* stream, int additional_amount, int /*total_amount*/) -> void {
        auto& playback = *static_cast<BGPlayback*>(userdata);
        if (playback.snd_bg == nullptr || !playback.playing) {
            return;
        }

        bool playback_finished = false;

        const int items = additional_amount / sizeof(float);
        if (bg_playback.samples < 0) {  // 开头延迟一段时间
            stream_put_empty(stream, items);
        } else {  // 读取音频文件
            std::vector<float> buf(items);
            const int read_amount = static_cast<int>(playback.snd_bg->read(buf.data(), items) * sizeof(float));
            // SDL_Log("- %d %d %d", additional_amount, items / bg_playback.snd_bg->channels(), SDL_GetAudioStreamQueued(stream));
            SDL_PutAudioStreamData(stream, buf.data(), read_amount);
            playback_finished = read_amount != additional_amount;
        }

        if (playback_finished) {  // 播放完毕
            SDL_FlushAudioStream(stream);
            playback.snd_bg.reset();
            playback.samples = 0;
            playback.timestamp = 0;
            playback.record = false;
        } else if (playback.record) {  // 仍需播放，启用记录时记录相关信息
            Uint64 samples = items / playback.snd_bg->channels();
            playback.samples += samples;
            playback.timestamp = SDL_GetTicksNS() + samples / playback.snd_bg->samplerate() * 1'000'000'000;
            // SDL_Log("%llu %llu", playback.samples / playback.snd_bg->samplerate(), playback.timestamp);
        }
    };
    SDL_SetAudioStreamGetCallback(audio_stream_bg, bg_callback, static_cast<void*>(&bg_playback));

    return audio_device != 0 && audio_stream_bg != nullptr && audio_stream_se != nullptr;
}

void Audio::close_audio() {
    clear_bg();
    SDL_DestroyAudioStream(audio_stream_bg);
    SDL_DestroyAudioStream(audio_stream_se);
    SDL_CloseAudioDevice(audio_device);
}

void Audio::play_bg(const char* file) {
    bg_playback.snd_bg = std::make_unique<SndfileHandle>(file);
    if (bg_playback.snd_bg->error()) {
        SDL_Log("Failed to open audio file: %s.", bg_playback.snd_bg->strError());
        bg_playback.snd_bg.reset();
        return;
    }
    SDL_AudioSpec spec{ .format = SDL_AUDIO_F32, .channels = bg_playback.snd_bg->channels(), .freq = bg_playback.snd_bg->samplerate() };
    SDL_SetAudioStreamFormat(audio_stream_bg, &spec, nullptr);
    bg_playback.playing = true;
}

void Audio::play_bg(std::unique_ptr<SndfileHandle>&& sndfile, bool play, bool enable_record) {
    bg_playback.record = enable_record;
    bg_playback.snd_bg = std::move(sndfile);
    SDL_AudioSpec spec{ .format = SDL_AUDIO_F32, .channels = bg_playback.snd_bg->channels(), .freq = bg_playback.snd_bg->samplerate() };
    SDL_SetAudioStreamFormat(audio_stream_bg, &spec, nullptr);
    bg_playback.playing = play;
}

void Audio::bg_set_silence(float duration) {
    bg_playback.samples = -static_cast<Sint64>(duration * bg_playback.snd_bg->samplerate());
}

auto Audio::get_bg_start_time() -> Uint64 {
    if (bg_playback.timestamp == 0) {
        return 0;
    }
    return bg_playback.timestamp - bg_playback.samples * 1'000'000'000 / bg_playback.snd_bg->samplerate();
}

auto Audio::get_bg_playback_progress() -> Uint64 {
    Sint64 progress = bg_playback.samples * 1'000'000'000 / bg_playback.snd_bg->samplerate();
    if (bg_playback.timestamp == 0) {
        return progress;
    }
    return bg_playback.playing ? progress + SDL_GetTicksNS() - bg_playback.timestamp : progress;
}

void Audio::pause_bg() {
    bg_playback.playing = false;
}

void Audio::resume_bg() {
    bg_playback.playing = true;
    bg_playback.timestamp = SDL_GetTicksNS();
}

void Audio::clear_bg() {
    if (skip_clear_bg) {
        skip_clear_bg = false;
        return;
    }
    bg_playback.snd_bg.reset();
    bg_playback.samples = 0;
    bg_playback.timestamp = 0;
    bg_playback.record = false;
    SDL_ClearAudioStream(audio_stream_bg);
}

void Audio::prevent_next_clear_bg() {
    skip_clear_bg = true;
}

void Audio::clear_se() {
    SDL_ClearAudioStream(audio_stream_se);
}

void Audio::fill_stream_se(int kind) {
    switch (kind) {
    case 1:
        SDL_PutAudioStreamData(audio_stream_se, audio_buf_1.data(), static_cast<int>(audio_buf_1.size() * sizeof(Sint16)));
        break;
    case 2:
        SDL_PutAudioStreamData(audio_stream_se, audio_buf_2.data(), static_cast<int>(audio_buf_2.size() * sizeof(Sint16)));
        break;
    }
}
