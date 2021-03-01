// JFormat.cpp: implementation of the JFormat class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JFormat.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

JFormat::JFormat()
{
}

JFormat::JFormat(JUnt32 id, const JChar* lbl, const JChar* lblcourt) : IElem(id, lbl)
{
	m_LabelCourt = lblcourt;
}

JFormat::~JFormat()
{
}

const JLabel & JFormat::GetLabelCourt() const
{
	return m_LabelCourt;
}

JArchive& JFormat::Send(JArchive & lhs, JInt32 Version) const
{
	return lhs;
}