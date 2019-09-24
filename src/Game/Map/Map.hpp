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
#include "PoolManager.hpp"
#include "GameFlags.hpp"
#include <mutex>

namespace SteerStone { namespace Game { namespace Entity {

    class Object;

}   ///< namespace Entity
}   ///< namespace Game
}   ///< namespace SteerStone

namespace SteerStone { namespace Game { namespace Map {

    class Entity::Object;

    /// Map
    class Base
    {
        DISALLOW_COPY_AND_ASSIGN(Base);

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////
    public:
        /// Constructor
        /// @p_Id : Map Id
        Base(uint32 const p_Id);
        /// Deconstructor
        ~Base();

        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////
    public:

        /// Getter Functions
        uint32 GetId()          const { return m_Id;          }
        MapType GetMapType()    const { return m_MapType;     }
        uint32 GetGridRadiusX() const { return m_GridRadiusX; }
        uint32 GetGridRadiusY() const { return m_GridRadiusY; }
        uint32 GetMapSizeX()   const  { return m_MapSizeX;    }
        uint32 GetMapSizeY()   const  { return m_MapSizeY;    }

        /// Load Map
        void Load();

        /// Load Portals to map
        void LoadPortals();
        /// Load Stations to map
        void LoadStations();
        /// Load Mobs to map
        void LoadMobs();

        /// Send Objects which always appear on map regardless of what grid user is in
        /// @p_Object : Packet being sent to
        void SendConstantObjects(Entity::Object* p_Object);

        /// Calculate Grid By Object Position
        /// @p_Object : Object
        std::tuple<uint32, uint32> CalculateGridByPosition(Entity::Object* p_Object);
        /// Calculate Grid By Object Position
        /// @p_Object : Object
        /// @p_PositionX : X Axis
        /// @p_PositionY : Y Axis
        std::tuple<uint32, uint32> CalculateGridByNewPosition(Entity::Object* p_Object, float const p_PositionX, float const p_PositionY);
        
        /// This function is only called when object is added to map
        /// Add Object to map
        /// @p_Object : Object being added to map
        void Add(Entity::Object* p_Object);
        /// Remove Object from map
        /// @p_Object : Object being removed from map
        void Remove(Entity::Object* p_Object);
        /// Delay Removal
        /// @p_Player : Player
        /// @p_Object : Object
        void AddToDelayRemoval(Entity::Object* p_Player, Entity::Object* p_Object);

        /// Unload Maps
        void UnloadAll();

        /// Move Object
        /// @p_Object : Object being moved
        void Move(Entity::Object* p_Object);

        /// Add Player to Jump Queue
        /// @p_ObjectPlayer : Player being added
        /// @p_ObjectPortal : Portal
        void AddToJumpQueue(Entity::Object* p_ObjectPlayer, Entity::Object* p_ObjectPortal);

        /// Process Jump Queue
        /// @p_Diff : Execution Time
        void ProcessJumpQueue(uint32 const p_Diff);

        /// Return Grid
        /// @p_Object : Object we are getting grid from
        Grid* GetGrid(Entity::Object const* p_Object);

        /// Find Object in map
        /// @p_Counter : Counter of object
        Entity::Object* FindObject(uint32 const p_Counter);

        /// Send Packet to everyone in map
        /// @p_PacketBuffer : Packet being sent to
        void SendPacketEveryone(Server::PacketBuffer const* p_PacketBuffer);

        /// Update Removal
        /// @p_Diff : Execution Time
        void UpdateRemoval(uint32 const p_Diff);

        /// Update Maps
        /// @p_Diff : Execution Time
        bool Update(uint32 const p_Diff);


        //////////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////////

    private:
        uint32 m_Id;                                          ///< Map Id
        MapType m_MapType;                                    ///< Map Type
        uint32 m_GridRadiusX;                                 ///< Grid Radius X
        uint32 m_GridRadiusY;                                 ///< Grid Radius Y
        uint32 m_MapSizeX;                                    ///< Map Size X
        uint32 m_MapSizeY;                                    ///< Map Size Y
        Grid* m_Grids[GRID_CELLS][GRID_CELLS];                ///< Grids
        PoolManager m_PoolManager;                            ///< Pool Manager
        std::vector<Entity::Object*> m_ConstantObjects;       ///< Constant Objects
        std::unordered_map<Entity::Object*, std::set<Entity::Object*>> m_DelayRemoval; ///< Delay Removal
        Core::Diagnostic::IntervalTimer m_IntervalDelayRemoval; ///< Interval Timer
        std::unordered_map<Entity::Object*, Entity::Object*> m_PlayersToJump;///< Process players to jump
        Core::Diagnostic::IntervalTimer m_IntervalJumpPlayer; ///< Interval Timer
        std::mutex m_Mutex;                                   ///< Mutex
        std::mutex m_JumpPlayerMutex;                         ///< For Jump
    };

}   ///< namespace Map
}   ///< namespace Game
}   ///< namespace Steerstone