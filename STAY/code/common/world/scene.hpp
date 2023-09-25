#pragma once

#include <box2d/box2d.h>

#include "../ecs/manager.hpp"
#include "camera.hpp"

namespace stay
{
    class Scene
    {
        public:
            Scene();
            ~Scene();
            void update(float dt);
            void start();
            void input(const sf::Event& event);
            void render(RTarget* target);
        private:
            void initialize();

            Camera mCamera;
            ecs::Manager mManager;

            b2World mPhysicsWorld;
            const float mPixelsPerMeter;
    };
} // namespace stay
