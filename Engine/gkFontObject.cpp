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
#include "gkMesh.h"
#include "gkTextManager.h"
#include "gkTextFile.h"
#include "gkEntity.h"
#include "utSingleton.h"

gkFontObject::gkFontObject(gkInstancedManager* creator, const gkResourceName& name, const gkResourceHandle& handle)
	: gkGameObject(creator, name, handle, GK_FONT_OBJECT) , right(Ogre::Real(1), Ogre::Real(0), Ogre::Real(0)) 
	, up(Ogre::Real(0), Ogre::Real(1), Ogre::Real(0)) , lineSpacing(Ogre::Real(0), Ogre::Real(-1), Ogre::Real(0))
	, textColor(1.f,1.f,1.f,1.f)
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
	//gkTextFile* tf = gkTextManager::getSingletonPtr() ? (gkTextFile*)gkTextManager::getSingleton().getByName(filename) : 0;
	//if (tf) 
	//{
        // const gkString& buf = tf->getText();
	//}
}



void gkFontObject::setFaceName(const gkString& _faceName)
{
    faceName = _faceName;
}

void gkFontObject::setSize(float charWidth, float charHeight)
{
	right = charWidth * gkVector3(Ogre::Real(1), Ogre::Real(0), Ogre::Real(0));
	up = charHeight * gkVector3(Ogre::Real(0), Ogre::Real(1), Ogre::Real(0));
}

void gkFontObject::setTextColor(gkColor _textColor) {
	textColor = _textColor;
}

class gkFontSpriteChUv
{
public:
	int left;
	int top;
	int right;
	int bottom;
	gkFontSpriteChUv(int _left, int _top, int _right, int _bottom) : left(_left), top(_top), right(_right), bottom(_bottom)
	{
	}
};
/*
 Format...
 texture:defaultfont.png
 size:596,290
 32:10,10,19,64
 33:27,10,40,64
 ...
 */
#define MAXIMUM_PARSE_VALUES 6
class gkFontSpriteMeshFace
{
protected:
	typedef enum { parseState_none, parseState_size, parseState_texture, parseState_uv } parseState_t;
	typedef utHashTable<utIntHashKey, gkFontSpriteChUv * >   SpriteFontChUVMap;
	SpriteFontChUVMap uvs;
	int width;
	int height;
	gkString textName;
public:
	gkFontSpriteMeshFace(const gkString& def) :	width(0) , height(0)
	{
		const char *str = def.c_str();
		int i;
		int start = 0;
		int chValue = -1;
		parseState_t state = parseState_none;
		int nValue = 0;
		int values[MAXIMUM_PARSE_VALUES+1];

		for (i = 0; str[i]; ++i)
		{
			if (str[i] == ':')
			{
				int len = (i - start);
				if (isdigit(str[start]))
				    {
					chValue = atoi(str + start);
					state = parseState_uv;
					nValue = 0;
					start = i + 1;
				    }
				else if ( len >= 4  && memicmp(str + start, "size", 4) == 0) 
					{
					state = parseState_size;
					nValue = 0;
					start = i + 1;
					}
			    else if ( len >= 7 && memicmp(str + start, "texture", 7) == 0)
					{
					state = parseState_texture;
					start = i + 1;
					}
			}
			else if (str[i] == ',')
			{
				if (isdigit(str[start]))
					{
					int value = atoi(str + start);
					if (nValue < MAXIMUM_PARSE_VALUES)
						{
						values[nValue++] = value;
						}
					}
				start = i + 1;
			}
			else if (str[i] == '\n' || !str[i + 1] )
			{
				if (isdigit(str[start]))
				{
					int value = atoi(str + start);
					if (nValue < MAXIMUM_PARSE_VALUES)
					{
						values[nValue++] = value;
					}
				}
				switch (state)
				{
				case parseState_size:
					if (nValue >= 2) 
					{
						width = values[0];
						height = values[1];
					}
					break;
				case parseState_texture:
					{
						gkString name(str + start, i - start);
						textName = name;
					}
					break;
				case parseState_uv:
					if (nValue >= 4)
					{
						uvs.insert(utIntHashKey(chValue),new gkFontSpriteChUv(values[0], values[1], values[2], values[3]));
					}
					break;
				}
				state = parseState_none;
				start = i + 1;
			}
		}
	}
	~gkFontSpriteMeshFace()
	{
		utHashTableIterator<SpriteFontChUVMap> iter(uvs);
		while (iter.hasMoreElements())
		{
			gkFontSpriteChUv* uvm = iter.getNext().second;
			delete uvm;
		}
		uvs.clear();
	}
	gkSubMesh* generateTextSubMesh(const gkString &text, gkVector3 &start, gkVector3 &right, gkVector3 &up, gkVector3 &lineSpacing,gkColor &textColor )
	{
		gkSubMesh*subMesh = new gkSubMesh();
		int offset = 0;
		gkVector3 normal = right;
		int triflag = gkTriangle::TRI_COLLIDER;
		gkVector3 lineStart = start;
		normal = normal.crossProduct(up);
		for (char ch : text)
		{
			if (ch == '\n') 
			{
				lineStart += lineSpacing;
				start = lineStart;
				continue;
			}
			UTsize pos = uvs.find(utIntHashKey((int)ch));
			gkFontSpriteChUv* uv = 0;
			if (pos != UT_NPOS)
			{
			   uv = uvs.at(pos);
			   float uvleft = (float)uv->left / (float)width;
			   float uvtop  = (float)uv->top  / (float)height;
			   float uvright = (float)uv->right / (float)width;
			   float uvbottom = (float)uv->bottom / (float)height;
			   float charWidth = ((uvright - uvleft) * width) / ((uvbottom - uvtop) * height);
			   uvtop = 1.f - uvtop;
			   uvbottom = 1.f - uvbottom;
			   gkVertex v0;
			   gkVertex v1;
			   gkVertex v2;
			   gkVertex v3;
			   v0.uv[0].x = uvleft;
			   v0.uv[0].y = uvtop;
			   v0.co.x = start.x+up.x;
			   v0.co.y = start.y+up.y;
			   v0.co.z = start.z+up.z;
			   v1.no = normal;
			   v1.uv[0].x = uvleft;
			   v1.uv[0].y = uvbottom;
			   v1.co.x = start.x;
			   v1.co.y = start.y;
			   v1.co.z = start.z;
			   v1.no = normal;
			   start += right * charWidth;
			   v2.uv[0].x = uvright;
			   v2.uv[0].y = uvbottom;
			   v2.co.x = start.x;
			   v2.co.y = start.y;
			   v2.co.z = start.z;
			   v2.no = normal;
			   v3.uv[0].x = uvright;
			   v3.uv[0].y = uvtop;
			   v3.co.x = start.x + up.x;
			   v3.co.y = start.y + up.y;
			   v3.co.z = start.z + up.z;
			   v3.no = normal;
			   subMesh->addTriangle(v0,offset+0,v1,offset+1,v2,offset+2, triflag);
			   subMesh->addTriangle(v0, offset + 0, v2, offset + 2, v3, offset + 3, triflag);
			}
		}
		subMesh->setTotalLayers(1);
		gkMaterialProperties&  mat = subMesh->getMaterial();
		gkString fontColorName;
		fontColorName = "r"+std::to_string((int)(textColor.r*255.f))+"g"+ std::to_string((int)(textColor.g*255.f)) + "b" + std::to_string((int)(textColor.b*255.f)) + "a" + std::to_string((int)(textColor.a*255.f));
		mat.m_name = "fontmaterial_" + textName + fontColorName;
		//mat.m_mode = gkMaterialProperties::MA_LIGHTINGENABLED | gkMaterialProperties::MA_DEPTHWRITE | gkMaterialProperties::MA_TWOSIDE | gkMaterialProperties::MA_ALPHABLEND | gkMaterialProperties::MA_HASFACETEX;
		//mat.m_mode = gkMaterialProperties::MA_ALPHABLEND | gkMaterialProperties::MA_DEPTHWRITE | gkMaterialProperties::MA_LIGHTINGENABLED;
		mat.m_mode = gkMaterialProperties::MA_ALPHABLEND | gkMaterialProperties::MA_DEPTHWRITE | gkMaterialProperties::MA_LIGHTINGENABLED; // test with no transparency		
		mat.m_diffuse = textColor;
		mat.m_specular.r = 1;
		mat.m_specular.g = 1;
		mat.m_specular.b = 1;
		mat.m_specular.a = 1;
		mat.m_specular = textColor;
		mat.m_emissive = 2.f;
		mat.m_ambient = 1.f;
		mat.m_spec = 0.5f;
		mat.m_alpha = 1.f;
		mat.m_totaltex = 1;
		gkTextureProperties &tp = mat.m_textures[0];
		tp.m_name = textName;
		tp.m_image = textName;
		tp.m_mix = 1.00000000;
		tp.m_color = mat.m_diffuse;
		tp.m_normalFactor = 1.00000000;
		tp.m_scale.x = 1.0;
		tp.m_scale.y = 1.0;
		tp.m_scale.z = 1.0;
		return subMesh;
	}
};

class gkFontSpriteMeshGenerator : public utSingleton<gkFontSpriteMeshGenerator>
{
protected:
    typedef utHashTable<gkHashedString, gkFontSpriteMeshFace*>   FontSpriteMeshFaceMap;
    FontSpriteMeshFaceMap spriteFonts;
public:
	gkFontSpriteMeshGenerator() 
	{
	}
	virtual ~gkFontSpriteMeshGenerator()
	{
		utHashTableIterator<FontSpriteMeshFaceMap> iter(spriteFonts);
		while (iter.hasMoreElements())
		{
			gkFontSpriteMeshFace* smf = iter.getNext().second;
			delete smf;
		}
		spriteFonts.clear();
	}
	gkSubMesh* generateTextSubMesh(const gkString &faceName, const gkString &text, gkVector3 &start, gkVector3 &right, gkVector3 &up, gkVector3 &lineSpacing, gkColor &textColor)
	{
		gkSubMesh* subMesh = 0;
		gkHashedString key(faceName);
		UTsize pos = spriteFonts.find(key);
		gkFontSpriteMeshFace* mf = 0;
		if (pos != UT_NPOS)
		{
			mf = spriteFonts.at(pos);
		}	
		if (!mf) 
			{
				gkTextFile* tf = 0;
				if (!faceName.empty())
				{
					tf = gkTextManager::getSingletonPtr() ? (gkTextFile*)gkTextManager::getSingleton().getByName(faceName + ".spritefont") : 0;
				}
				if (!tf)
				{
					tf = gkTextManager::getSingletonPtr() ? (gkTextFile*)gkTextManager::getSingleton().getByName("default.spritefont") : 0;
				}
				if (tf)
				{
					const gkString& def = tf->getText();
					mf = new gkFontSpriteMeshFace(def);
					spriteFonts.insert(key, mf);
				}
			}
		if (mf)
		    {
			subMesh = mf->generateTextSubMesh(text, start, right, up, lineSpacing,textColor );
		    }
		return subMesh;
	}
	UT_DECLARE_SINGLETON(gkFontSpriteMeshGenerator);
};

UT_IMPLEMENT_SINGLETON(gkFontSpriteMeshGenerator);


void gkFontObject::regenerateMesh()
{
	// TBD - lookup 'font' definition
	gkSubMesh* subMesh = 0;
	gkFontSpriteMeshGenerator *gks = gkFontSpriteMeshGenerator::getSingletonPtr();
	if (!gks)
	{
		gks = new gkFontSpriteMeshGenerator();
	}
	gkVector3 start(0, 0, 0);	
	subMesh = gks->generateTextSubMesh(faceName, text,start, right, up,lineSpacing , textColor );
	if (subMesh) 
	{
		gkHashedString meshName("mesh_"+m_name.getName()+ "_gen");
		gkHashedString entName(m_name.getName() + "_gen");
		gkEntity* ent = m_scene->createEntity(entName);
		gkMesh* meshPtr = m_scene->createMesh(meshName);
		if( meshPtr && ent ) 
		{
			meshPtr->addSubMesh(subMesh);
			ent->getEntityProperties().m_mesh = meshPtr;
			ent->getProperties().m_parent = this->m_name.getName();
			//addConstraint			
			ent->getProperties().m_transform = getProperties().m_transform;
			ent->setActiveLayer((m_scene->getLayer() & getLayer()) != 0);
			ent->setLayer(getLayer());
		}
		else 
		{
			delete subMesh;
		}		
	}
}


void gkFontObject::finalize(void)
{
	gkFontSpriteMeshGenerator *gks = gkFontSpriteMeshGenerator::getSingletonPtr();
	if (gks)
	{
		delete gks;
	}

}