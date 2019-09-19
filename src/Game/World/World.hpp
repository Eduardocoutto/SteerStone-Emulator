/*
* Liam Ashdown
* Copyright (C) 2019
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#include <PCH/Precompiled.hpp>
#include "Singleton/Singleton.hpp"
#include "Utility/UtiLockedQueue.hpp"
#include "GameFlags.hpp"

#define WORLD_SLEEP_TIMER 50

namespace SteerStone { namespace Game { namespace Entity {

    class Player;

}   ///< namespace Entity
}   ///< namespace Game
}   ///< namespace SteerStone

namespace SteerStone { namespace Game { namespace World {

    class Entity::Player;

    enum BoolConfigs
    {
        BOOL_CONFIG_MAX = 1
    };

    enum IntConfigs
    {
        INT_CONFIG_MAP_INTERVAL,
        INT_CONFIG_CHECK_FOR_PLAYER,
        INT_CONFIG_MAX,
    };

    enum FloatConfigs
    {
        FLOAT_CONFIG_MAX = 1
    };

    /// World
    class Base
    {
        SINGLETON_P_D(Base);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

        public:
            /// Load the World
            void Load();

            /// Update World
            /// @p_Diff : Execution Diff
            void Update(uint32 const p_Diff);
            /// Update Players
            /// @p_Diff : Execution Diff
            void UpdatePlayers(uint32 p_Diff);
            /// Stop World Updating
            bool StopWorld() const;

            /// Load Configs
            void LoadConfigs();

            /// Returns bool value
            bool GetBoolConfig(BoolConfigs p_Index);
            /// Returns integer value
            uint32 GetIntConfig(IntConfigs p_Index);
            /// Returns float value
            float GetFloatConfig(FloatConfigs p_Index);

            /// Add Player to queue
            /// @p_Player : Player being added
            void AddPlayer(Entity::Player* p_Player);
            /// Remove Player from world
            /// @p_Player : Player being removed
            void RemovePlayer(Entity::Player* p_Player);
            /// Return Player in world
            /// @p_Id : Id of player
            Entity::Player* FindPlayer(uint32 const p_Id);
            /// Process Player in queue
            /// @p_Player : being processed
            void ProcessPlayer(Entity::Player* p_Player);

        private:
            std::map<uint32, Entity::Player*> m_Players;                ///< Player in world
            Core::Utils::LockedQueue<Entity::Player*> m_PlayerQueue;    ///< Player in queue storage
            uint32 m_IntConfigs[INT_CONFIG_MAX];                        ///< Holds config settings for int
            bool m_BoolConfigs[BOOL_CONFIG_MAX];                        ///< Holds config settings for bool
            float m_FloatConfigs[FLOAT_CONFIG_MAX];                     ///< Holds config settings for float

            static volatile bool s_StopWorld;                           ///< Stop World Updating
    };

}   ///< namespace World
}   ///< namespace Game
}   ///< namespace Steerstone

#define sWorldManager SteerStone::Game::World::Base::GetSingleton()