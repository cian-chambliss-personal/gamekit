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
//#include "gkGameObjectManager.cpp"
#include "gkSteeringObject.h"
#include "gkSteeringCapture.h"
#include "gkSteeringPathFollowing.h"
#include "gkSteeringWander.h"

/*
 gkLogicBlockAiContextImpl - shared data

 gkGameObject*     m_trackingObject;
 gkSteeringObject* m_steeringObject;
 gkSteeringCapture* m_steeringCapture;
 gkSteeringPathFollowing* m_steeringFollowing;
 gkSteeringWander* m_steeringWander;

 */
const gkVector3 FORWARD(0, 1, 0);
const gkVector3 UP(0, 0, 1);
const gkVector3 SIDE(1, 0, 0);


class gkLogicBlockAiContextImpl : public gkLogicBlockAiContext {
private:
	void Rest();
	void Seek();
	void Flee();
	void PathFollowing();
	void Wander();
public:
	gkLogicBlockAiDefinition m_def;
	// Nav Mesh context data
	PNAVMESHDATA             m_navMeshData;
	PDT_NAV_MESH             m_navMesh;
	// Steering Object context data
	gkGameObject*     m_trackingObject;
	gkSteeringObject* m_steeringObject;
	gkSteeringCapture* m_steeringCapture;
	gkSteeringPathFollowing* m_steeringFollowing;
	gkSteeringWander* m_steeringWander;
	gkLogicBlockAiContextImpl();
	~gkLogicBlockAiContextImpl();
	void CreateNavMesh(gkGameObject *navMeshObj);
	void updateAIDefinition(gkLogicBlockAiDefinition &def);
	virtual void dispose(void);
	virtual void update(gkScalar tick);
};


void gkLogicBlockAiContextImpl::Rest() {
	m_steeringObject = 0;
}

void gkLogicBlockAiContextImpl::Seek() {
	if (!m_steeringCapture) {
		m_steeringCapture = new gkSteeringCapture(
			m_trackingObject,
			m_def.m_velocity,
			FORWARD,
			UP,
			SIDE,
			m_def.m_targetObj,
			0.5f,
			5
		);
	}
	m_steeringCapture->setMaxForce(m_def.m_acceleration);
	m_steeringCapture->reset();
	m_steeringCapture->setGoalPosition(m_def.m_targetObj->getPosition());
	m_steeringObject = m_steeringCapture;
}

void gkLogicBlockAiContextImpl::Flee() {
	Wander(); // TBD -- figure out 'flee'
}

void gkLogicBlockAiContextImpl::PathFollowing() {
	if (!m_steeringFollowing) {
		m_steeringFollowing = new gkSteeringPathFollowing(
			m_trackingObject,
			m_def.m_velocity,
			FORWARD,
			UP,
			SIDE,
			gkVector3(2, 4, 2),
			256,
			0.003f
		);
		m_steeringFollowing->setNavMesh(m_navMesh);
	}
	m_steeringFollowing->setGoalRadius(m_def.m_dist);
	m_steeringFollowing->setMaxForce(m_def.m_acceleration);
	m_steeringFollowing->reset();
	m_steeringObject = m_steeringFollowing;
}

void gkLogicBlockAiContextImpl::Wander() {
	if (!m_steeringWander) {
		m_steeringWander = new gkSteeringWander(
			m_trackingObject,
			m_def.m_velocity,
			FORWARD,
			UP,
			SIDE,
			0.3f,
			5
		);		
	}	
	m_steeringWander->setGoalRadius(m_def.m_dist);
	m_steeringWander->setMaxForce(m_def.m_acceleration);
	m_steeringWander->reset();
	m_steeringObject = m_steeringWander;
}

gkLogicBlockAiContextImpl::gkLogicBlockAiContextImpl() : m_navMeshData(0), m_navMesh(0), m_trackingObject(0), m_steeringObject(0)
, m_steeringCapture(0), m_steeringFollowing(0), m_steeringWander(0) {
}
gkLogicBlockAiContextImpl::~gkLogicBlockAiContextImpl() {
	if (m_steeringCapture)
		delete m_steeringCapture;
	if (m_steeringFollowing)
		delete m_steeringFollowing;
	if (m_steeringWander)
		delete m_steeringWander;
}

void gkLogicBlockAiContextImpl::CreateNavMesh(gkGameObject *navMeshObj) {
	m_navMeshData = PNAVMESHDATA(new gkNavMeshData(navMeshObj->getOwner()));
	m_navMeshData->singleNavMeshCreate(navMeshObj);
	gkRecast::Config config;
	m_navMesh = gkRecast::createNavMesh(PMESHDATA(m_navMeshData->cloneData()), config);
}

void gkLogicBlockAiContextImpl::updateAIDefinition(gkLogicBlockAiDefinition &def) {
	if ( m_def.m_mode != def.m_mode ) {
		m_def = def;
		switch (m_def.m_mode) {
		case gkLogicBlockAiDefinition::Mode::SA_REST:
			Rest();
			break;
		case gkLogicBlockAiDefinition::Mode::SA_SEEK:
			Seek();
			break;
		case gkLogicBlockAiDefinition::Mode::SA_FLEE:
			Flee();
			break;
		case gkLogicBlockAiDefinition::Mode::SA_PATH_FOLLOWING:
			PathFollowing();
			break;
		case gkLogicBlockAiDefinition::Mode::SA_WANDER:
			Wander();
			break;
		}
	}
	if (m_steeringObject) {
		switch (m_def.m_mode) {
		case gkLogicBlockAiDefinition::Mode::SA_SEEK:
		case gkLogicBlockAiDefinition::Mode::SA_FLEE:
			m_steeringObject->setGoalPosition(m_def.m_targetObj->getPosition());
			break;
		}
	}
}

void gkLogicBlockAiContextImpl::dispose(void)
{
	delete this;
}

void gkLogicBlockAiContextImpl::update(gkScalar tick)
{
	if (m_steeringObject) {
		m_steeringObject->update(tick);
	}
}

/*
gkSteeringActuator
 */

gkSteeringActuator::gkSteeringActuator(gkGameObject* object, gkLogicLink* link, const gkString& name)
	:   gkLogicActuator(object, link, name) 
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
	int mode = m_def.m_mode;
	if (!m_def.m_navMeshObj) {
		m_def.m_navMeshObj = m_scene->getObject(m_navMesh);
		if (!m_def.m_navMeshObj)
			return;
		// Create the associated nav mesh object the first time in
		if (!m_def.m_navMeshObj->_getLogicBlockAiContext()) {
			gkLogicBlockAiContextImpl *meshAiContext = new gkLogicBlockAiContextImpl();
			m_def.m_navMeshObj->_setLogicBlockAiContext(meshAiContext);
			meshAiContext->CreateNavMesh(m_def.m_navMeshObj);
		}		
	}
	if (!m_def.m_targetObj && (mode == gkLogicBlockAiDefinition::Mode::SA_SEEK || mode == gkLogicBlockAiDefinition::Mode::SA_FLEE) ) {
		m_def.m_targetObj = m_scene->getObject(m_target);
		if (!m_def.m_targetObj)
			mode = gkLogicBlockAiDefinition::Mode::SA_REST;
	}
	// Handle navigation....
	gkLogicBlockAiContextImpl *aiContext = dynamic_cast<gkLogicBlockAiContextImpl *>(m_object->_getLogicBlockAiContext());
	if (!aiContext) {
		gkLogicBlockAiContextImpl *navPath = dynamic_cast<gkLogicBlockAiContextImpl *>(m_def.m_navMeshObj->_getLogicBlockAiContext());		
		aiContext = new gkLogicBlockAiContextImpl();
		aiContext->m_trackingObject = m_object;
		aiContext->m_navMesh = navPath->m_navMesh;
		m_object->_setLogicBlockAiContext(aiContext);
		gkLogicBlockAiManager * lbmanager = m_scene->getLogicBlockAiManager();
		lbmanager->trackLogicBlockAi(aiContext);
	}
	aiContext->updateAIDefinition(m_def);
}

gkLogicBlockAiManager::gkLogicBlockAiManager() {

}

gkLogicBlockAiManager::~gkLogicBlockAiManager() {

}
void gkLogicBlockAiManager::update(gkScalar tick) {
	for (int i = 0; i < m_logicBlockAi.size(); ++i) {
		m_logicBlockAi[i]->update(tick);
	}
}

void gkLogicBlockAiManager::trackLogicBlockAi(gkLogicBlockAiContext *logicBlockAi) {
	m_logicBlockAi.push_back(logicBlockAi);
}

void gkLogicBlockAiManager::untrackLogicBlockAi(gkLogicBlockAiContext *logicBlockAi) {
	for (int i = m_logicBlockAi.size()-1; i >= 0 ; --i ) {
		if (m_logicBlockAi[i] == logicBlockAi) {
			m_logicBlockAi.erase(m_logicBlockAi.begin() + i);
		}
	}
}

gkLogicBlockAiManager*  gkScene::getLogicBlockAiManager() {
	if (!m_logicBlockAiManager) {
		m_logicBlockAiManager = new gkLogicBlockAiManager();
	}
	return m_logicBlockAiManager;
}
