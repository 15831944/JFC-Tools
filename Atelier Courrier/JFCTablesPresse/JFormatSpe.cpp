// JFormatSpe.cpp: implementation of the JFormatSpe class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JFormatSpe.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


JFormatSpe::JFormatSpe()
{
}

JFormatSpe::JFormatSpe(JUnt32 id, const JChar* lbl, const JChar* lblcourt) : JFormat(id, lbl, lblcourt)
{
}

JFormatSpe::~JFormatSpe()
{
}

JVoid JFormatSpe::SetLabel(const JChar* lblLong)
{
	m_Label = lblLong;
}

JVoid JFormatSpe::SetLabelCourt(const JChar* lblCourt)
{
	m_LabelCourt = lblCourt;
}


JArchive& JFormatSpe::Send(JArchive & lhs, JInt32 Version) const
{
	switch(Version)
	{
	case 1:
		lhs.Send(m_ID);
		m_LabelCourt.Send(lhs);
		m_Label.Send(lhs);
		break;

	default:
		throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;
}

JArchive& JFormatSpe::Recv(JArchive & lhs, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		lhs.Recv(m_ID);
		//label court
		m_LabelCourt.Recv(lhs);
		//label long
		m_Label.Recv(lhs);
		break;

	default:
		throw JInternalError::GetInstance(); //("File Version error");
		break;
	}
	return lhs;	
}
