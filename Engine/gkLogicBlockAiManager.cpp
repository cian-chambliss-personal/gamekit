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

#include "gkLogicBlockAiManager.h"

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
