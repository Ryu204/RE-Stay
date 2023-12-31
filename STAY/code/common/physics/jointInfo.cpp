#include "jointInfo.hpp"
#include "../utility/variantHelper.hpp"
#include "../utility/convert.hpp"
#include "rigidBody.hpp"

#include <box2d/b2_math.h>
#include <box2d/b2_revolute_joint.h>
#include <box2d/box2d.h>
#include <json/value.h>
#include <memory>

namespace stay
{
    namespace phys
    {
        Prismatic::Prismatic(Vector2 anchorA, Vector2 anchorB, Vector2 axis)
            : anchorA(std::move(anchorA))
            , anchorB(std::move(anchorB))
            , axis(std::move(axis))
        {}

        Revolute::Revolute(Vector2 anchor)
            : anchor(std::move(anchor))
        {}
        
        Json::Value JointInfo::toJSONObject() const
        {
            Json::Value res;
            std::visit(utils::VariantVisitor{
                [&res](const Prismatic& pris) {
                    res["type"] = "prismatic";
                    pris.toJSONObject().swap(res["data"]);
                },
                [&res](const Revolute& rev) {
                    res["type"] = "revolute";
                    rev.toJSONObject().swap(res["data"]);
                }
            }, *this);
            return res;
        }
            
        bool JointInfo::fetch(const Json::Value& value)
        {
            auto string = value["type"].isString() ? value["type"].asString() : std::string();
            if (string == "prismatic")
                operator=(Prismatic{});
            else
                return false;
            return std::visit(utils::VariantVisitor{
                [&](Prismatic& obj) {
                    return obj.fetch(value["data"]);
                },
                [&](Revolute& rev) {
                    return rev.fetch(value["data"]);
                }
            }, *this);
        }
        
        Uptr<b2JointDef> JointInfo::createDef(RigidBody& a, RigidBody& b) const
        {
            return std::visit(utils::VariantVisitor{
                [&](const Prismatic& pris) -> Uptr<b2JointDef>{
                    auto r1 = std::make_unique<b2PrismaticJointDef>();
                    r1->bodyA = a.body();
                    r1->bodyB = b.body();
                    r1->localAnchorA = a.body()->GetLocalPoint(utils::convertVec2<b2Vec2>(pris.anchorA));
                    r1->localAnchorB = b.body()->GetLocalPoint(utils::convertVec2<b2Vec2>(pris.anchorB));
                    r1->referenceAngle = r1->bodyB->GetAngle() - r1->bodyA->GetAngle();
                    r1->localAxisA = r1->bodyA->GetLocalVector(utils::convertVec2<b2Vec2>(pris.axis));
                    return std::move(r1);
                },
                [&](const Revolute& rev) -> Uptr<b2JointDef> {
                    auto r1 = std::make_unique<b2RevoluteJointDef>();
                    r1->Initialize(a.body(), b.body(), utils::convertVec2<b2Vec2>(rev.anchor));
                    return std::move(r1);
                }
            }, *this);
        }
    } // namespace phys
} // namespace stay