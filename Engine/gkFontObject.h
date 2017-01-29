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
#ifndef _gkFontObject_h_
#define _gkFontObject_h_

#include "gkGameObject.h"
#include "gkSerialize.h"

class gkFontObjectSettings
{
public:
	gkString faceName;
	gkVector3 right;
	gkVector3 up;
	gkVector3 lineSpacing;
	gkColor textColor;
	int alignX;
	gkFontObjectSettings();
};

class gkFontObject : public gkGameObject
{
protected:
    gkString text;
	gkFontObjectSettings settings;
public:
	enum AlignX
	{
		Left,
		Right,
		Middle,
		Justify,
		Flush
	};	
	gkFontObject(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle);
	virtual ~gkFontObject() {}
	void setXAlign(int xAlign);
	void setText(const gkString& _text);
    void setFaceName(const gkString& _faceName);
	void setSize( float charWidth , float charHeight );
	void setTextColor(gkColor _textColor);
	void regenerateMesh();
	static void finalize(void);
private:
	virtual void createInstanceImpl(void);
	virtual void destroyInstanceImpl(void);	
};
#endif//_gkFontObject_h_
