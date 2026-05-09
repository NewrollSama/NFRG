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
#include "NFRG/scene/impl/latency.hpp"
using namespace nfrg;
using namespace scene;

#include "NFRG/anim/motion.hpp"
#include "NFRG/mgr/audio.hpp"
#include "NFRG/ui/gameplay/note.hpp"
#include "NFRG/ui/latency/detection_line.hpp"
#include "NFRG/ui/text.hpp"

class LatencyScene::CheckProcedure {
private:
    /**
     * @brief 流程的开始时间。
     */
    Uint64 start_time_ = 0;

    /**
     * @brief 音符所绑定的动画器，每次流程开始时将其启动。
     */
    anim::Animator* note_animator_;

public:
    CheckProcedure(anim::Animator* note_animator);
    /**
     * @brief 运行一次流程。
     */
    void run();

    /**
     * @brief 将传入的时间与期望的按键时间做比对。
     *
     * @param ticks_ns 键盘按键按下时的时间。
     * @return int 以毫秒为单位的时间差，代表按键的延迟。
     */
    [[nodiscard]] auto detect(Uint64 ticks_ns) const -> int;
};

LatencyScene::LatencyScene() {
    const auto& text_latency = ui::Text::Builder()
                                   .set_pos({ .x = 16, .y = 16 })
                                   .set_content("当方块接触线时，按下空格")
                                   .build_and_add_to(this);
    text_latency_ = std::static_pointer_cast<ui::Text>(text_latency);
    ui::latency::DetectionLine::Builder()
        .set_bound({ .x = 0.0f, .y = 960.0f, .w = 1920.0f, .h = 4.0f })
        .add_to(this);
    auto note = ui::gameplay::Note::Builder()
                    .set_bound({ .x = 860.0f, .y = -40.0f, .w = 200.0f, .h = 40.0f })
                    .build_and_add_to(this);
    anim::Animator* note_animator = anim::MotionAnimator::Builder()
                                        .set_duration(1000.0f)
                                        .set_source({ .x = 860.0f, .y = -40.0f })
                                        .set_destination({ .x = 840.0f, .y = 940.0f })
                                        .set_disposable(false)
                                        .bind_component(note)
                                        .build_and_add_to(this);
    cp_ = std::make_unique<CheckProcedure>(note_animator);
    auto timer_callback = [](void* userdata, SDL_TimerID /*timerID*/, Uint64 /*interval*/) -> Uint64 {
        auto& cp = *static_cast<std::unique_ptr<CheckProcedure>*>(userdata);
        cp->run();
        return 2'000'000'000;
    };
    timer_ = SDL_AddTimerNS(0, timer_callback, static_cast<void*>(&cp_));
}

void LatencyScene::handle_key_event(SDL_KeyboardEvent& event) {
    switch (event.key) {
    case SDLK_SPACE: {
        if (!event.down) {
            return;
        }
        Sint64 diff = cp_->detect(event.timestamp);
        if (!text_latency_.expired()) {
            text_latency_.lock()->set_content(std::to_string(diff) + "ms");
        }
        break;
    }
    default: {
        Scene::handle_key_event(event);
        break;
    }
    }
}

void LatencyScene::update() {}

LatencyScene::~LatencyScene() {
    SDL_RemoveTimer(timer_);
}

LatencyScene::CheckProcedure::CheckProcedure(anim::Animator* note_animator)
    : note_animator_(note_animator) {
}

void LatencyScene::CheckProcedure::run() {
    start_time_ = SDL_GetTicksNS();
    note_animator_->start();
    // Fill audio buffer
    for (int i = 0; i < 2; ++i) {
        mgr::Audio::fill_stream_se(1);
    }
    mgr::Audio::fill_stream_se(2);
}

auto LatencyScene::CheckProcedure::detect(Uint64 ticks_ns) const -> int {
    Sint64 diff = (ticks_ns - start_time_ - 1'000'000'000);
    return static_cast<int>(diff / 1'000'000);
}
