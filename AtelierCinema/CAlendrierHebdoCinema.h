// CAlendrierHebdoCinema.h: interface for the CCAlendrierHebdoCinema class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CALENDRIERHEBDOCINEMA_H__2AC72F53_3F56_400B_AAE8_E9FA62CB77B7__INCLUDED_)
#define AFX_CALENDRIERHEBDOCINEMA_H__2AC72F53_3F56_400B_AAE8_E9FA62CB77B7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCAlendrierHebdoCinema  
{
public:
	int GetNumSemaine(COleDateTime date);
	int GetIndexSemaine(COleDateTime date);
	int GetMois(COleDateTime date);
	int GetAnnee(COleDateTime date);
	CCAlendrierHebdoCinema();
	virtual ~CCAlendrierHebdoCinema();

};

#endif // !defined(AFX_CALENDRIERHEBDOCINEMA_H__2AC72F53_3F56_400B_AAE8_E9FA62CB77B7__INCLUDED_)
