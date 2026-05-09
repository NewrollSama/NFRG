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
#include "NFRG/ui/component.hpp"
using namespace nfrg;
using namespace ui;

void Component::set_texture(SDL_Texture* texture, bool set_size) {
    if (own_texture_ && texture_ != nullptr) {
        SDL_DestroyTexture(texture_);
    }
    texture_ = texture;
    if (set_size) {
        SDL_GetTextureSize(texture, &bound_.w, &bound_.h);
    }
    dirty_ = false;
}

void Component::set_pos(SDL_FPoint pos) {
    bound_.x = pos.x;
    bound_.y = pos.y;
}

void Component::set_size(SDL_FPoint size) {
    bound_.w = size.x;
    bound_.h = size.y;
}

void Component::set_bound(SDL_FRect bound) {
    bound_ = bound;
}

void Component::set_alpha(Uint8 alpha) {
    SDL_SetTextureAlphaMod(texture_, alpha);
}

auto Component::is_dirty() const -> bool {
    return dirty_;
}

void Component::measure(SDL_FPoint offset) {
    render_offset_ = offset;
}

auto Component::render() const -> bool {
    if (texture_ == nullptr) {
        return true;
    }
    SDL_FRect render_bound = { .x = bound_.x + render_offset_.x, .y = bound_.y + render_offset_.y, .w = bound_.w, .h = bound_.h };
    bool r = false;
    if (r_ > 0.0001f) {
        r = SDL_RenderTextureRotated(SDL_GetRendererFromTexture(texture_), texture_, nullptr, &render_bound, r_, nullptr, SDL_FLIP_NONE);
    } else {
        r = SDL_RenderTexture(SDL_GetRendererFromTexture(texture_), texture_, nullptr, &render_bound);
    }
    return r;
}

Component::~Component() {
    if (own_texture_) {
        SDL_DestroyTexture(texture_);
    }
}
