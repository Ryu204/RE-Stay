#pragma once

#include <filesystem>

#include "../../ecs/type.hpp"
#include "../../type/vector.hpp"
#include "quicktypefwd.hpp"

/*
    This class is used in case there is no suitable file found for `SceneLoader`
*/

namespace stay
{
    class Node;
    class RawSceneLoader
    {
        public:
            using Path = typename std::filesystem::path;
            // @return Root node if succeeded, null if failed
            Uptr<Node> load(Path&& filename, const std::string& switchReason);
        private:
            void init(const ldtk::Level& level, const ldtk::LayerInstance& layer);
            void loadTiles(Node* currentRoot, const ldtk::Level& level, const ldtk::LayerInstance& layer);
            void loadColliders(Node* currentRoot, const ldtk::Level& level, const ldtk::LayerInstance& layer);
            void loadPlayer(Node* currentRoot, const ldtk::Level& level, const ldtk::LayerInstance& layer);
            Vector2 fileToWorld(const Vector2& pos) const;

            static std::string& error();
            std::unordered_map<ecs::Entity, ecs::Entity> mParentOf;
            
            float mPxPerMeter;
            Vector2 mLayerSize;
            Vector2 mLayerOffset;
            float mTileSize;
    };
} // namespace stay
