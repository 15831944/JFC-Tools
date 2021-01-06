// TarifManuel.cpp: implementation of the CTarifManuel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TarifManuel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTarifManuel::CTarifManuel(void)
{
}

CTarifManuel::CTarifManuel(JUnt32 idsupport, JUnt32 idformat, JFlt32 trf, JDate dc, JDate ds)
{
	m_ID_Support = idsupport;
	m_ID_Format = idformat;
	m_Tarif = trf;
	m_DateC = dc;
	m_DateS = ds;
}

CTarifManuel::~CTarifManuel(void)
{
}

JUnt32 CTarifManuel::GetIDSupport() const
{
	return 	m_ID_Support;
}

JUnt32 CTarifManuel::GetIDFormat() const
{
	return m_ID_Format;
}

JFlt32 CTarifManuel::GetTarif() const
{
	return m_Tarif;
}

JDate CTarifManuel::GetDateC() const
{
	return m_DateC;
}

JDate CTarifManuel::GetDateS() const
{
	return m_DateS;
}

//JVoid CTarifManuel::SetIDSupport(JUnt32 id)
//{
//	m_ID_Support = id;
//}
//JVoid CTarifManuel::SetIDFormat(JUnt32 id)
//{
//	m_ID_Format = id;
//}
//JVoid CTarifManuel::SetTarif(JUnt32 trf)
//{
//	m_Tarif = trf;
//}
//JVoid CTarifManuel::SetDateC(JDate dc)
//{
//	m_DateC = dc;
//}
//JVoid CTarifManuel::SetDateS(JDate ds)
//{
//	m_DateS = ds;
//}

JArchive& CTarifManuel::Send(JArchive & lhs, JInt32 Version) const
{
	switch(Version)
	{
	case 1:
		lhs.Send( m_ID_Support );
		lhs.Send( m_ID_Format );
		lhs.Send( m_Tarif );
		m_DateC.Send(lhs);
		m_DateS.Send(lhs);
		break;

	default:
		TRACE("CTarifManuel   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;
}

JArchive& CTarifManuel::Recv(JArchive & lhs, JInt32 Version)
{
	switch(Version)
	{
	case 1:
		lhs.Recv( m_ID_Support );
		lhs.Recv( m_ID_Format );
		lhs.Recv( m_Tarif );
		m_DateC.Recv(lhs);
		m_DateS.Recv(lhs);
		break;

	default:
		TRACE("CTarifManuel   Invalid Version");
		throw JInternalError::GetInstance(); //("File Version error");
	}
	return lhs;	
}