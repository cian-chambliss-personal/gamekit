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
#include "gkFontObject.h"
#include "OgreLight.h"
#include "OgreSceneManager.h"
#include "OgreSceneNode.h"
#include "gkScene.h"


gkFontObject::gkFontObject(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	:    gkGameObject(creator, name, handle, GK_FONT_OBJECT)
{
}


void gkFontObject::createInstanceImpl(void)
{
	gkGameObject::createInstanceImpl();

	//Ogre::SceneManager* manager = m_scene->getManager();
	//m_light = manager->createLight(m_name.getName());
	//m_node->attachObject(m_light);
}



void gkFontObject::destroyInstanceImpl(void)
{
	//if (!m_scene->isBeingDestroyed())
	//{
	//	Ogre::SceneManager* manager = m_scene->getManager();
    //
    //		m_node->detachObject(m_light);
    //		manager->destroyLight(m_light);
    //	}
	//m_light = 0;

	gkGameObject::destroyInstanceImpl();
}

void gkFontObject::setText(const gkString& _text)
{
	text = _text;
}



