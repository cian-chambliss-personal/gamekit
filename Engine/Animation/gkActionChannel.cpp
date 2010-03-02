/*
-------------------------------------------------------------------------------
    This file is part of OgreKit.
    http://gamekit.googlecode.com/

    Copyright (c) 2006-2010 Charlie C.

    Contributor(s): none yet.
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
#include "gkAction.h"
#include "gkActionChannel.h"
#include "OgreBone.h"


using namespace Ogre;



gkActionChannel::gkActionChannel(gkAction *parent, gkBone* bone) :
        m_bone(bone), m_action(parent)
{
    GK_ASSERT(bone);
}


gkActionChannel::~gkActionChannel()
{
    gkBezierSpline **splines = m_splines.ptr();
    int len = getNumSplines(), i = 0;
    while (i < len)
        delete splines[i++];

}


void gkActionChannel::addSpline(gkBezierSpline* spline)
{
    if (m_splines.empty())
        m_splines.reserve(16);
    m_splines.push_back(spline);
}


const gkBezierSpline** gkActionChannel::getSplines(void)
{
    return (const gkBezierSpline**)m_splines.ptr();
}


int gkActionChannel::getNumSplines(void)
{
    return (int)m_splines.size();
}


void gkActionChannel::evaluate(float time, float delta, float weight)
{
    const gkBezierSpline **splines = getSplines();
    int len = getNumSplines(), i = 0, nvrt;

    // clear previous channel
    gkTransformState channel;
    channel.setIdentity();

    while (i < len)
    {
        const gkBezierSpline *spline = splines[i++];

        nvrt = spline->getNumVerts();
        const gkBezierVertex* verts = spline->getVerts();

        float eval = 0.f;
        if (nvrt > 0)
            eval = spline->interpolate(delta, time);

        switch (spline->getCode())
        {
        case SC_LOC_X: { channel.loc.x = eval; break; }
        case SC_LOC_Y: { channel.loc.y = eval; break; }
        case SC_LOC_Z: { channel.loc.z = eval; break; }
        case SC_SCL_X: { channel.scl.x = eval; break; }
        case SC_SCL_Y: { channel.scl.y = eval; break; }
        case SC_SCL_Z: { channel.scl.z = eval; break; }
        case SC_ROT_X: { channel.rot.x = eval; break; }
        case SC_ROT_Y: { channel.rot.y = eval; break; }
        case SC_ROT_Z: { channel.rot.z = eval; break; }
        case SC_ROT_W: { channel.rot.w = eval; break; }
        }
    }


    // prevent divide by zero
    if (gkFuzzy(channel.rot.Norm()))
        channel.rot = Ogre::Quaternion::IDENTITY;
    else
        channel.rot.normalise();

    GK_ASSERT(!channel.loc.isNaN());
    GK_ASSERT(!channel.rot.isNaN());
    GK_ASSERT(!channel.scl.isNaN());

    const gkTransformState &bind = m_bone->m_bind;
    gkTransformState &pose = m_bone->m_pose;

    // save previous pose
    gkTransformState blendmat = m_bone->m_pose;

    // combine relitave to binding position
    pose.loc = bind.loc + bind.rot * channel.loc;
    pose.rot = bind.rot * channel.rot;
    pose.scl = bind.scl * channel.scl;

    if (weight < 1.0)
    {
        // blend poses
        pose.loc = gkMathUtils::interp(blendmat.loc, pose.loc, weight);
        pose.rot = gkMathUtils::interp(blendmat.rot, pose.rot, weight);
        pose.rot.normalise();
        pose.scl = gkMathUtils::interp(blendmat.scl, pose.scl, weight);
    }


    Ogre::Bone *bone = m_bone->m_bone;
    bone->setPosition(pose.loc);
    bone->setOrientation(pose.rot);
    bone->setScale(pose.scl);
}
