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

#include "Server/Opcodes/Packets/Server/MapPackets.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "Utility/UtilMaths.hpp"
#include "SurroundingObject.hpp"

namespace SteerStone { namespace Game { namespace Entity {

    /// Constructor
    /// @p_Object : Object
    /// @p_PlayerObject : Player
    SurroundingObject::SurroundingObject(Object* p_Object, Object* p_PlayerObject)
        : m_Object(p_Object), m_PlayerObject(p_PlayerObject)
    {
        m_ScheduleForDespawn = false;
        m_IntervalDelayRemoval.SetInterval(sWorldManager->GetIntConfig(World::IntConfigs::INT_CONFIG_DELAY_REMOVAL));
    }
    /// Deconstructor
    SurroundingObject::~SurroundingObject()
    {
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Update
    /// @p_Diff : Execution Time
    bool SurroundingObject::Update(uint32 const p_Diff)
    {
        /// This can happen if object (player) logs out
        if (!m_Object)
        {
            /// Send despawn packet just to make sure
            Despawn();
            return false;
        }

        /// If unit is dead, remove from client
        if (m_Object->ToUnit()->GetDeathState() == Entity::DeathState::DEAD)
        {
            Despawn();
            return false;
        }
        
        /// Object is scheduled to be despawned from client
        if (m_ScheduleForDespawn)
        {
            m_IntervalDelayRemoval.Update(p_Diff);
            if (m_IntervalDelayRemoval.Passed())
            {
                Despawn();
                return false;
            }
        }

        if (!IsScheduledForDespawn())
        {
            /// Check whether the object is still in our radius view
            if (!Core::Utils::IsInCircleRadius(m_PlayerObject->GetSpline()->GetPositionX(), m_PlayerObject->GetSpline()->GetPositionY(),
                m_Object->GetSpline()->GetPositionX(), m_Object->GetSpline()->GetPositionY(), PLAYER_RADIUS_SCAN))
                ScheduleForDespawn();
        }

        return true;
    }

    /// Schedule to despawn object
    void SurroundingObject::ScheduleForDespawn()
    {
        if (m_ScheduleForDespawn)
            return;

        m_ScheduleForDespawn = true;
    }

    /// Remove object from being despawned
    void SurroundingObject::RemoveScheduleFromDespawn()
    {
        m_ScheduleForDespawn = false;
        m_IntervalDelayRemoval.ResetCurrent();
    }

    /// Send Packet
    /// @p_PacketBuffer : Packet being sent
    void SurroundingObject::SendPacket(Server::PacketBuffer const* p_PacketBuffer)
    {
        if (m_Object->GetType() == Entity::Type::OBJECT_TYPE_PLAYER)
        {
            if (m_Object->ToPlayer()->IsInSurrounding(m_PlayerObject))
                m_Object->ToPlayer()->SendPacket(p_PacketBuffer);
        }
    }

    /// Whether object is set to be despawned
    bool SurroundingObject::IsScheduledForDespawn() const
    {
        return m_ScheduleForDespawn;
    }

    /// Despawn Object
    void SurroundingObject::Despawn()
    {
        Server::Packets::DespawnShip l_Packet;
        l_Packet.Id = m_Object->GetObjectGUID().GetCounter();
        m_PlayerObject->ToPlayer()->SendPacket(l_Packet.Write());
    }

}   ///< namespace Entity
}   ///< namespace Game
}   ///< namespace Steerstone