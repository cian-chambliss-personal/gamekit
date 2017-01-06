/*
-------------------------------------------------------------------------------
    This file is part of a fork of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2017 Cian Chambliss
    Contributor(s): Cian Chambliss
-------------------------------------------------------------------------------
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#include "gkSteeringActuator.h"
#include "gkEngine.h"
#include "gkGameObject.h"
#include "gkScene.h"


gkSteeringActuator::gkSteeringActuator(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:   gkLogicActuator(object, link, name) , m_mode(gkSteeringActuator::Mode::SA_SEEK) , m_dist(0) , m_velocity(0)
	 , m_acceleration(0) , m_turnspeed(0) , m_updateTime(0) , m_selfTerminating(false)
	 , m_enableVisualization(false) , m_automaticFacing(false) , m_normalUp(false) , m_lockZVelocity(false)

{
}


gkSteeringActuator::~gkSteeringActuator()
{
}


gkLogicBrick* gkSteeringActuator::clone(gkLogicLink* link, gkGameObject* dest)
{
	gkSteeringActuator* act = new gkSteeringActuator(*this);
	act->cloneImpl(link, dest);
	return act;
}


void gkSteeringActuator::execute(void)
{
}
