// JTitre.cpp: implementation of the JTitre class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JTitre.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JTitre::JTitre(JUnt32 id, const JChar* lbl, JUnt32 period, JUnt32 IdAudience, JBool Agregat) : IElem(id, lbl), ITitre(period,IdAudience,Agregat)
{
}

JTitre::~JTitre()
{
}