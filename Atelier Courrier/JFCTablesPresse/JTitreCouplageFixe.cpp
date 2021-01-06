// JTitreCouplageFixe.cpp: implementation of the JTitreCouplageFixe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JTitreCouplageFixe.h"
#include "JSRCPresse.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JTitreCouplageFixe::JTitreCouplageFixe(JUnt32 id, const JChar* lbl, JUnt32 Periodicite, JUnt32 IDAudience, JUnt32 nbInsert) : IElem(id, lbl), JTitreCouplage(Periodicite, IDAudience, nbInsert)
{
}

JTitreCouplageFixe::~JTitreCouplageFixe()
{
}

