#include "../ecs/manager.hpp"
#include "../../game/system/list.hpp"
#include "../../game/component/list.hpp"
#include "camera.hpp"
#include "scene.hpp"

/*debug*/ #include <iostream>

namespace stay
{
    Scene::Scene()
        : mPhysicsWorld({0.F, -10.F})
        , mPixelsPerMeter(100.F)
    {
        Node::setGlobalRegistry(&mManager.getRegistry());
        initialize();
    }
    Scene::~Scene()
    {
        /*debug*/ std::cout << "~scene" << std::endl;
        Node::root().clearChildren();
        Node::setGlobalRegistry(nullptr);
    }
    void Scene::update(float dt)
    {
        mManager.update(dt);
    }
    void Scene::start()
    {
        mManager.start();
    }
    void Scene::input(const sf::Event& event)
    {
        mManager.input(event);
    }
    void Scene::render(RTarget* target)
    {
        mCamera.setOn(target);
        mManager.render(target);
    }
    void Scene::initialize()
    {
        // mManager.registerSystem<sys::RawRenderSystem>();
        mManager.registerSystem<sys::OrderedRenderSystem>();
        mManager.registerSystem<sys::PhysicsDebugSystem>()->initialize(&mPhysicsWorld);
        mManager.registerSystem<sys::PhysicsSystem>()->initialize(&mPhysicsWorld);
        
        auto* node = Node::create();
        auto& body = node->addComponents<phys::RigidBody>(Vector2(0, 0), 45, phys::BodyType::DYNAMIC);
        // body.setAngularVelocity(100);
        auto& col = node->addComponents<phys::Collider>(phys::Collider::Box{Vector2(0, 0), Vector2(1.F, 2.F)});
        node->addComponents<comp::Render>(sf::Color::Black, sf::Vector2f{0.5F, 1.F});

        node = Node::create();
        auto& body1 = node->addComponents<phys::RigidBody>(Vector2(0, -6), 0, phys::BodyType::STATIC);
        auto& col1 = node->addComponents<phys::Collider>(phys::Collider::Box{Vector2(0, 0), Vector2(4.F, 0.5F)});
        node->addComponents<comp::Render>(sf::Color::Green, sf::Vector2f{0.5F, 1.F});

        /*debug*/
        // col1.setTrigger(true);
        col1.OnCollisionEnter.addEventListener([](phys::Collider&, b2Contact&) { std::cout << "sth entered static collider" << std::endl; });
        col1.OnTriggerEnter.addEventListener([](phys::Collider&, b2Contact&) { std::cout << "sth entered static trigger" << std::endl; });
        /**/

        // for (int i = 1; i <= 4; ++i)
        // {
        //     const auto& trans = node->getLocalTransform().getMatrix();
        //     node = createChild(node);
        //     node->addComponents<comp::Render>(sf::Color::Red, sf::Vector2f{0.4F, 1.2F});
        //     node->getLocalTransform().move({1.3F, 1.3F, 0});
        //     node->getLocalTransform().rotate(10);
        // }
    }
} // namespace stay
