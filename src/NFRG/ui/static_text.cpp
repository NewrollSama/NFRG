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
#include "NFRG/ui/static_text.hpp"
using namespace nfrg;
using namespace ui;

#include "NFRG/mgr/font.hpp"

auto StaticText::Builder::set_content(std::string& content, float ptsize) -> StaticText::Builder& {
    this->component_->content_ = content;
    this->component_->set_texture(mgr::Font::create_texture(this->component_->content_, ptsize));
    return *this;
}

auto StaticText::Builder::set_content(std::string&& content, float ptsize) -> StaticText::Builder& {
    this->component_->content_ = std::move(content);
    this->component_->set_texture(mgr::Font::create_texture(this->component_->content_, ptsize));
    return *this;
}
