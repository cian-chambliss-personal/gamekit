/*
-------------------------------------------------------------------------------
	This file is part of the Ogre GameKit port.
	http://gamekit.googlecode.com/

	Copyright (c) 2009 Charlie C.
-------------------------------------------------------------------------------
 This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
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
#include "gkLogger.h"
#include "OgreLogManager.h"
#include "OgreLog.h"


#ifdef _MSC_VER
# define gkvsnprintf	_vsnprintf
#else
# define gkvsnprintf	vsnprintf
#endif

using namespace Ogre;



// ----------------------------------------------------------------------------
#define GK_BUFSIZE (0xFFFF)

static char GK_ConsoleBuffer[GK_BUFSIZE+1];
static Log* gLog= 0;

// ----------------------------------------------------------------------------
void gkPrintf(const char *fmt, ...)
{
	va_list lst;
	va_start(lst, fmt);
	int size= gkvsnprintf(GK_ConsoleBuffer, GK_BUFSIZE, fmt, lst);
	va_end(lst);

	if (size < 0)
	{
		GK_ConsoleBuffer[GK_BUFSIZE]= 0;
		size= GK_BUFSIZE;
	}

	if (size > 0)
	{
		/// out to log stream so user def flags work
		GK_ConsoleBuffer[size]= 0;
		if (gLog != 0)
			gLog->logMessage(GK_ConsoleBuffer);
		else printf("%s", GK_ConsoleBuffer);
	}
}

// ----------------------------------------------------------------------------
void gkLogger::enable(const String &name, bool verbose)
{
	if (!gLog)
	{
		LogManager *mgr= LogManager::getSingletonPtr();
		if (!mgr)
			mgr= new LogManager();
		gLog= mgr->createLog(name);

		if (!verbose) gLog->setLogDetail(Ogre::LL_LOW);
	}
}


// ----------------------------------------------------------------------------
void gkLogger::disable()
{
	if (gLog)
	{
		LogManager::getSingleton().destroyLog(gLog);
		gLog= 0;
		delete LogManager::getSingletonPtr();
	}
}


// ----------------------------------------------------------------------------
void gkLogger::write(const String &msg)
{
	if (gLog != 0)
		gLog->logMessage(msg);
}
