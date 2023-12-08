#pragma once

#include <efsw/efsw.hpp>

#include "type.hpp"
#include "listener.hpp"

namespace stay
{

    namespace asset
    {
        class Asset;
        // @brief Watches a folder and callbacks at changes
        class FolderWatcher
        {
            public:
                FolderWatcher(Path folder, float minCallbackIntervalSeconds = 1.F);
                void add(Asset& asset);
                void update(float dt);
            private:
                std::filesystem::path mFolderPath;
                efsw::FileWatcher mWatcher;
                detail::Listener mListener;
        };
    } // namespace asset
} // namespace stay