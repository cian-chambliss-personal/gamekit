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
#ifndef _gkSteeringActuator_h_
#define _gkSteeringActuator_h_

#include "gkLogicActuator.h"

class gkSteeringActuator : public gkLogicActuator
{
public:
	enum Mode
	{
		SA_SEEK,
		SA_FLEE,
		SA_PATH_FOLLOWING,
		SA_WANDER
	};
private:
	int         m_mode; // Seek, Flee, Path-Following, Wander
	float       m_dist; // The maximum distance for the game object approach the Target Object.
	float       m_velocity; // The velocity used to seek the Target Object.
	float       m_acceleration; //The maximum acceleration to use when seeking the Target Object.
	float       m_turnspeed; // The maximum turning speed to use when seeking the Target Object.
	int         m_updateTime;
	bool        m_selfTerminating;
	bool        m_enableVisualization;
	bool        m_automaticFacing;
	bool        m_normalUp;
	bool        m_lockZVelocity;
	gkString    m_target;
	gkString    m_navMesh;
public:
	gkSteeringActuator(gkGameObject* object, gkLogicLink* link, const gkString& name);
	virtual ~gkSteeringActuator();
	gkLogicBrick* clone(gkLogicLink* link, gkGameObject* dest);

	GK_INLINE void  setMode(int v) { m_mode = v; }
	GK_INLINE void  setTargetObject(const gkString& v) { m_target = v; }
	GK_INLINE void  setNavMeshObject(const gkString& v) { m_navMesh = v; }
	GK_INLINE void  setDistance(const float v) { m_dist = v; }
	GK_INLINE void  setVelocity(const float v) { m_velocity = v; }
	GK_INLINE void  setAcceleration(float v) { m_acceleration = v; }
	GK_INLINE void  setTurnSpeed(float v) { m_turnspeed = v; }
	GK_INLINE void  setUpdateTime(int v) { m_updateTime = v; }
	GK_INLINE void  setSelfTerminating(bool v) { m_selfTerminating = v; }
	GK_INLINE void  setEnableVisualization(bool v) { m_enableVisualization = v; }
	GK_INLINE void  setAutomaticFacing(bool v) { m_automaticFacing = v; }
	GK_INLINE void  setNormalUp(bool v) { m_normalUp = v; }
	GK_INLINE void  setLockZVelocity(bool v) { m_lockZVelocity = v; }

	GK_INLINE int   getMode(void)                         const { return m_mode; }
	GK_INLINE const gkString& getTargetObject(void)       const { return m_target; }
	GK_INLINE const gkString& getNavMeshObject(void)      const { return m_navMesh; }
	GK_INLINE float getDistance()                         const { return m_dist; }
	GK_INLINE float getVelocity(void)                     const { return m_velocity; }
	GK_INLINE float getAcceleration(void)                 const { return m_acceleration; }
	GK_INLINE float getTurnSpeed(void)                    const { return m_turnspeed; }
	GK_INLINE float getUpdateTime(void)                   const { return m_updateTime; }
	GK_INLINE bool  getSelfTerminating(void)              const { return m_selfTerminating; }
	GK_INLINE bool  getEnableVisualization(void)          const { return m_enableVisualization; }
	GK_INLINE bool  getAutomaticFacing(void)              const { return m_automaticFacing; }
	GK_INLINE bool  getNormalUp(void)					  const { return m_normalUp; }
	GK_INLINE bool  getLockZVelocity(void)                const { return m_lockZVelocity; }

	void execute(void);
};

#endif//_gkSteeringActuator_h_
