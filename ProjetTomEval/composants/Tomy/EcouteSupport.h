// EcouteSupport.h: interface for the CEcouteSupport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ECOUTESUPPORT_H__685BE187_F268_45DE_8412_497BF07B942D__INCLUDED_)
#define AFX_ECOUTESUPPORT_H__685BE187_F268_45DE_8412_497BF07B942D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CEcouteSupport  
{
public:
	CEcouteSupport();

	void SetNbEcoutes(long NbEcoutes);

	virtual ~CEcouteSupport();

public:
	long            m_NbEcoutes;
	long*           m_pNoIndividus;
	unsigned short* m_pValEcoutes;
};

#endif // !defined(AFX_ECOUTESUPPORT_H__685BE187_F268_45DE_8412_497BF07B942D__INCLUDED_)
