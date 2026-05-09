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
#include "../base.hpp"

#include "NFRG/scene/scene.hpp"

namespace nfrg::ui {

template <typename T>
class Base::BuilderTemplate {
protected:
    /**
     * @brief 暂存在建造者内的组件。
     */
    std::shared_ptr<T> component_;

public:
    /**
     * @brief 建造者的构造函数。
     */
    template <typename... Args>
    BuilderTemplate(Args&&... args);

    /**
     * @brief 设定待建造界面元素对象的位置。
     * @param pos 待设置的位置。
     * @return 建造者的引用，以便链式调用。
     */
    auto set_pos(SDL_FPoint pos) -> T::Builder&;

    /**
     * @brief 设定待建造界面元素对象的宽高。
     * @param pos 待设置的宽高，其中 x 为宽，y 为高。
     * @return 建造者的引用，以便链式调用。
     */
    auto set_size(SDL_FPoint size) -> T::Builder&;

    /**
     * @brief 设定待建造界面元素对象的位置和宽高。
     * @param pos 待设置的位置和宽高。
     * @return 建造者的引用，以便链式调用。
     */
    auto set_bound(SDL_FRect bound) -> T::Builder&;

    auto set_align_x(AlignX align) -> T::Builder&;
    auto set_align_y(AlignY align) -> T::Builder&;

    /**
     * @brief 完成建造流程，建造对象。
     * @return 建造者所建造的组件对象的共享指针。
     */
    virtual auto build() -> std::shared_ptr<T>;

    // /**
    //  * @brief 完成建造流程，建造对象。
    //  * @return 建造者所建造的组件对象的独占指针。
    //  */
    // virtual auto build_unique() -> std::unique_ptr<T>;

    /**
     * @brief 完成建造流程，将建造的对象添加到提供的场景中。
     * @param scene 建造的组件对象将添加到的场景。
     */
    void add_to(scene::Scene* scene);
    /**
     * @brief 完成建造流程，将建造的对象添加到提供的组件容器中。
     * @param scene 建造的组件对象将添加到的组件容器。
     */
    void add_to(ui::Group* group);

    /**
     * @brief 完成建造流程，将建造的对象添加到提供的容器中，
     * 并返回共享指针，以便继续操作。
     * @param scene 建造的组件对象将添加到的场景。
     * @return 建造者所建造的组件对象的共享指针。
     */
    [[nodiscard]] virtual auto build_and_add_to(scene::Scene* scene) -> std::shared_ptr<T>;
    /**
     * @brief 完成建造流程，将建造的对象添加到提供的组件容器中，
     * 并返回共享指针，以便继续操作。
     * @param scene 建造的组件对象将添加到的组件容器。
     * @return 建造者所建造的组件对象的共享指针。
     */
    [[nodiscard]] auto build_and_add_to(ui::Group* group) -> std::shared_ptr<T>;

    /**
     * @brief 完成建造流程，将建造的对象添加到提供的场景中，
     * 并返回共享指针对象的引用，以便继续操作。
     * @param scene 建造的组件对象将添加到的场景。
     * @return 建造者所建造的组件对象的共享指针。
     */
    [[nodiscard]] auto add_to_and_get_ref(scene::Scene* scene) -> std::shared_ptr<Base>&;
    /**
     * @brief 完成建造流程，将建造的对象添加到提供的组件容器中，
     * 并返回共享指针对象的引用，以便继续操作。
     * @param scene 建造的组件对象将添加到的组件容器。
     * @return 建造者所建造的组件对象的共享指针。
     */
    [[nodiscard]] auto add_to_and_get_ref(ui::Group* group) -> std::shared_ptr<Base>&;
};

template <typename T>
template <typename... Args>
Base::BuilderTemplate<T>::BuilderTemplate(Args&&... args) {
    component_ = std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

template <typename T>
auto Base::BuilderTemplate<T>::set_pos(SDL_FPoint pos) -> T::Builder& {
    component_->bound_.x = pos.x;
    component_->bound_.y = pos.y;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto Base::BuilderTemplate<T>::set_size(SDL_FPoint size) -> T::Builder& {
    component_->bound_.w = size.x;
    component_->bound_.h = size.y;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto Base::BuilderTemplate<T>::set_bound(SDL_FRect bound) -> T::Builder& {
    component_->bound_ = bound;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto Base::BuilderTemplate<T>::set_align_x(AlignX align) -> T::Builder& {
    component_->align_x_ = align;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto Base::BuilderTemplate<T>::set_align_y(AlignY align) -> T::Builder& {
    component_->align_y_ = align;
    return static_cast<T::Builder&>(*this);
}

template <typename T>
auto Base::BuilderTemplate<T>::build() -> std::shared_ptr<T> {
    return std::move(component_);
}

// template <typename T>
// auto Base::BuilderTemplate<T>::build_unique() -> std::unique_ptr<T> {
//     return std::move(component_);
// }

template <typename T>
void Base::BuilderTemplate<T>::add_to(scene::Scene* scene) {
    scene->components_.emplace_back(build());
}

template <typename T>
void Base::BuilderTemplate<T>::add_to(ui::Group* group) {
    group->components_.emplace_back(build());
}

template <typename T>
auto Base::BuilderTemplate<T>::build_and_add_to(scene::Scene* scene) -> std::shared_ptr<T> {
    auto component = build();
    scene->components_.emplace_back(component);
    return std::move(component);
}

template <typename T>
auto Base::BuilderTemplate<T>::build_and_add_to(ui::Group* group) -> std::shared_ptr<T> {
    auto component = build();
    group->components_.emplace_back(component);
    return std::move(component);
}

template<typename T>
auto Base::BuilderTemplate<T>::add_to_and_get_ref(scene::Scene* scene) -> std::shared_ptr<Base>& {
    return scene->components_.emplace_back(build());
}

template<typename T>
auto Base::BuilderTemplate<T>::add_to_and_get_ref(ui::Group* group) -> std::shared_ptr<Base>& {
    return group->components_.emplace_back(build());
}

}
