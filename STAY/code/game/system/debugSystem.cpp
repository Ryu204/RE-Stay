#include "debugSystem.hpp"
#include "../../common/physics/rigidBody.hpp"
#include "../component/player.hpp"
#include "../component/playerDebug.hpp"
#include "../../common/utility/convert.hpp"
#include "../../common/world/camera.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <cassert>

namespace stay
{
    DebugSystem::DebugSystem(ecs::Manager* manager)
        : ecs::System{manager}
        , ecs::UpdateSystem{0}
        , mCamera{nullptr}
        , mWindow{nullptr}
    {}

    void DebugSystem::initialize(Camera* camera, RWin* window)
    {
        mCamera = camera;
        mWindow = window;
    }

    void DebugSystem::update(float /*dt*/)
    {
        if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Q))
            return;
        for (const auto& [entity, player, unused] : mManager->getRegistryRef().view<Player, PlayerDebug>().each())
        {
            const auto mouse = mousePosition();
            player.hookBody->setPosition(mouse);
            player.movementBody->setPosition(mouse);
        }
    }

    Vector2 DebugSystem::mousePosition() const
    {
        assert(mCamera != nullptr && mWindow != nullptr && "uninitialized debug system");
        auto res = utils::convertVec2<Vector2>(mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow), mCamera->getView()));
        res.y *= -1.F;
        return res;
    }
} // namespace stay