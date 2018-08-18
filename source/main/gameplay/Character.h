/*
    This source file is part of Rigs of Rods
    Copyright 2005-2012 Pierre-Michel Ricordel
    Copyright 2007-2012 Thomas Fischer
    Copyright 2017-2018 Petr Ohlidal

    For more information, see http://www.rigsofrods.org/

    Rigs of Rods is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3, as
    published by the Free Software Foundation.

    Rigs of Rods is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Rigs of Rods. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "ForwardDeclarations.h"

#include <OgreVector3.h>
#include <OgreMath.h> // Ogre::Radian
#include <OgreUTFString.h>

#include <string>

class Character
{
public:

    Character(int source = -1, unsigned int streamid = 0, int color_number = 0, bool is_remote = true);
    ~Character();
       
    int            getSourceID() const                  { return m_source_id; }
    bool           isRemote() const                     { return m_is_remote; }
    int            GetColorNum() const                  { return m_color_number; }
    Ogre::UTFString const& GetNetUsername()             { return m_net_username; }
    Ogre::Quaternion const& GetOrientation() const      { return m_character_orientation; }
    Ogre::Radian   getRotation() const                  { return m_character_rotation; }
    bool           IsCoupledWithActor() const           { return m_have_coupling_seat; }
    Actor*         GetActorCoupling()                   { return m_actor_coupling; }
    void           setColour(int color)                 { this->m_color_number = color; }
    Ogre::Vector3  getPosition();
    bool           getVisible(); 
    void           setPosition(Ogre::Vector3 position);
    void           setRotation(Ogre::Radian rotation);
    void           setVisible(bool visible);
    void           move(Ogre::Vector3 offset);
    void           unwindMovement(float distance);
    void           update(float dt);
//OLD    void           updateCharacterNetworkColour();
    void           updateCharacterRotation();
    void           updateMapIcon();
    void           updateLabels();
    void           receiveStreamData(unsigned int& type, int& source, unsigned int& streamid, char* buffer);
    void           SetActorCoupling(bool enabled, Actor* actor = nullptr);
    RoR::GfxCharacter*  SetupGfx();

private:

    void           AddPersonToSurveyMap();
  //OLD  void           ResizePersonNetLabel();
    void           ReportError(const char* detail);
    void           SendStreamData();
    void           SendStreamSetup();
    void           SetAnimState(std::string mode, float time = 0);

    Actor*           m_actor_coupling; //!< The vehicle or machine which the character occupies
    SurveyMapEntity* m_survey_map_entity;
    Ogre::Radian     m_character_rotation;
    float            m_character_h_speed;
    float            m_character_v_speed;
    Ogre::Vector3    m_character_position;
    bool             m_character_visible;
    int              m_color_number;
    int              m_stream_id;
    int              m_source_id;
    bool             m_can_jump;
    bool             m_is_remote;
    bool             m_hide_own_net_label;
    bool             m_have_coupling_seat;
    std::string      m_last_anim;
    std::string      m_instance_name;
    Ogre::UTFString  m_net_username;
    Ogre::Quaternion m_character_orientation;
    RoR::GfxCharacter*        m_gfx_character;
    Ogre::AnimationStateSet*  m_anim_state;
    std::deque<Ogre::Vector3> m_prev_positions; 
};

namespace RoR {

struct GfxCharacter
{
    struct SimBuffer //!< Buffered simulation state for async gfx scene update
    {
        Ogre::Vector3      simbuf_character_pos;
        Ogre::Radian       simbuf_character_rot; //!< When on foot
        Ogre::Quaternion   simbuf_character_orientation; //!< When in vehicle
        Ogre::UTFString    simbuf_net_username;
        int                simbuf_color_number;
        Actor*             simbuf_actor_coupling;
        bool               simbuf_coupling_has_seat;
    };
    
    ~GfxCharacter();
    
    void            BufferSimulationData();
    void            UpdateCharacterInScene();
    
    Ogre::SceneNode*          xc_scenenode;
    Ogre::MovableText*        xc_movable_text; // TODO: Remake using GUI; the network labels shouldn't be part of scene. ~only_a_ptr, 05/2018
    SimBuffer                 xc_simbuf;
    SimBuffer                 xc_simbuf_prev;
    Character*                xc_character;
    std::string               xc_instance_name; // TODO: Store MaterialPtr-s directly ~only_a_ptr, 05/2018
};
    
} // namespace RoR

