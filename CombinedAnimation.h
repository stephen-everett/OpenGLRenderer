#pragma once
#include "Object3D.h"
#include "Animation.h"

class CombinedAnimation : public Animation {
private:
    std::vector<std::shared_ptr<Animation>> m_animations;

    void applyAnimation(float_t dt) override {
        for (auto& anim : m_animations) {
            anim->tick(dt);
        }
    }

public:
    CombinedAnimation(std::shared_ptr<Object3D> obj, std::vector<std::shared_ptr<Animation>>& animations) :
        Animation(obj, animations.front()->duration()), m_animations(std::move(animations)) {}

    // Remove 'override' specifier
    void start() {
        for (auto& anim : m_animations) {
            anim->start();
        }
    }

    // Remove 'override' specifier
    void stop() {
        for (auto& anim : m_animations) {
            anim->stop();
        }
    }
};
