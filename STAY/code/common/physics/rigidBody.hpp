#pragma once

#include <box2d/box2d.h>

#include "../ecs/component.hpp"
#include "../type/vector.hpp"

namespace stay
{
    namespace phys
    {
        enum class BodyType
        {
            STATIC = b2_staticBody,         // Move under explicit `setPosition(...)`, only collides DYNAMIC
            KINEMATIC = b2_kinematicBody,   // Move under`setVelocity(...)`, only collides DYNAMIC
            DYNAMIC = b2_dynamicBody        // Move by internal solver and (sometimes) explicit user controls
        };
        class Collider;
        // @brief An abstract component, only functions if a collider is presented. Manipulate velocity and force.
        class RigidBody : public ecs::Component
        {
            public:
                // @param `angle` angle from OY+ to object's local OY+ in degree, positive if the angle is anti-clockwise
                RigidBody(const Vector2& position = Vector2(10.F, -10.F), float angle = 0.F, BodyType type = BodyType::STATIC);
                ~RigidBody() override;
                BodyType type() const;
                void setType(const BodyType& type);
                void setPosition(const Vector2& position);
                Vector2 getPosition() const;
                // @param `angle` the angle between OY+ and object's local OY+ in degree, positive if anti-clockwise
                void setAngle(float angle);
                float getAngle() const;
                void applyForce(const Vector2& force);
                void setVelocity(const Vector2& velocity);
                Vector2 getVelocity() const;
                // @ param `velocity` the angular velocity between OY+ and object's local OY+ in degree, positive if rotating clockwise
                void setAngularVelocity(float velocity);
                float getAngularVelocity() const;
                void setFixedRotation(bool fixed);
                void setGravityScale(float scale);
                Vector2 gravity() const;
                float gravityScale() const;
                bool bullet() const;
                void setBullet(bool isBullet);
                float linearDamping() const;
                void setLinearDamping(float damp);
                float horizontalDamping() const;
                void setHorizontalDamping(float damp);
                b2Fixture* attachFixture(const b2FixtureDef& properties);
                b2Body* body();

                Json::Value toJSONObject() const override;
                bool fetch(const Json::Value& value) override;
            private:
                
                b2World* mWorld;
                b2Body* mBody;
                float mHorizontalDamping;
        };

        struct RigidBody01 : public RigidBody 
        {
            using RigidBody::RigidBody;
        };
        struct RigidBody02 : public RigidBody 
        {
            using RigidBody::RigidBody;
        };
    } // namespace phys
} // namespace stay
