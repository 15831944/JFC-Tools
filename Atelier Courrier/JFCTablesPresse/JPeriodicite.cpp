// JPeriodicite.cpp: implementation of the JPeriodicite class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JPeriodicite.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JPeriodicite::JPeriodicite(JUnt32 id, const JChar* lbl, const JChar* lblcourt) : IElem(id, lbl)
{
	m_LabelCourt = lblcourt;
}

JPeriodicite::~JPeriodicite()
{
}

const JChar* JPeriodicite::GetLabelCourt() const
{
	return m_LabelCourt.AsJCharPtr();
}
