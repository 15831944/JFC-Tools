// CCourbe.cpp: implementation of the CCourbe class.
//
//////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include "Courbe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCourbe::CCourbe()
{
	SetCount(20);
}

CCourbe::CCourbe(JInt32 id, const JChar* lbl)
{
	SetCount(20);
	m_Id = id;
	m_Label = lbl;
}

CCourbe::~CCourbe()
{
}

JInt32 CCourbe::GetId() const
{
	return m_Id;
}

const JLabel & CCourbe::GetLabel() const
{
	return m_Label;
}


void CCourbe::SetLabel(const JChar* lbl) 
{
	m_Label = lbl;
}

JArchive& CCourbe::Recv(JArchive & lhs, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		lhs.Recv( m_Id );
		m_Label.Recv( lhs );
		for(int i = 0; i<20; i++)
			lhs.Recv( Item(i) );
		break;
	default:
		TRACE("CCourbe");
		throw JInternalError::GetInstance(); //("File Version error");
	}

	return lhs;
}

JArchive& CCourbe::Send(JArchive & lhs, JInt32 Version) const
{
	switch(Version)
	{
	case 1:
		lhs.Send( m_Id );
		m_Label.Send( lhs );
		for(int i = 0; i<20; i++)
			lhs.Send( Item(i) );
		break;
	default:
		TRACE("CCourbe");
		throw JInternalError::GetInstance(); //("File Version error");
	}

	return lhs;
}
