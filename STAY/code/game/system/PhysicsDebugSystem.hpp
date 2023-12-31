#pragma once

#include "../../common/ecs/manager.hpp"
#include "../../common/physics/debugDraw.hpp"
#include "../../common/physics/world.hpp"

/*
    Render colliders and other physics components using `b2World::DebugDraw()`
*/

namespace stay
{
    namespace sys
    {
        struct PhysicsDebugSystem : public ecs::RenderSystem, public ecs::System
        {
                PhysicsDebugSystem(ecs::Manager* manager)
                    : ecs::RenderSystem(1)
                    , ecs::System(manager)
                { }
                virtual ~PhysicsDebugSystem()
                {
                    // Clear the drawer
                    if (phys::World::avail())
                    {
                        phys::World::get().SetDebugDraw(nullptr);
                    }
                }

                void initialize()
                {
                    mDrawer = std::make_unique<phys::DebugDraw>();
                    uint32 flags = 0;
                    flags += b2Draw::e_shapeBit;
                    // flags += b2Draw::e_jointBit;
                    // flags += b2Draw::e_aabbBit;
                    flags += b2Draw::e_centerOfMassBit;
                    mDrawer->SetFlags(flags);
                    phys::World::get().SetDebugDraw(mDrawer.get());
                }

                void render(RTarget* target, Node* /*root*/) override
                {
                    mDrawer->setRenderTarget(target);
                    phys::World::get().DebugDraw();
                }

            private:
                Uptr<phys::DebugDraw> mDrawer;
        };
    } // namespace sys
} // namespace stay
