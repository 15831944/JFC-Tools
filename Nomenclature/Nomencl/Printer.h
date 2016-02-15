// Printer.h: interface for the CPrinter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINTER_H__70889DA5_FC24_11D3_B5BB_00105AA2BB33__INCLUDED_)
#define AFX_PRINTER_H__70889DA5_FC24_11D3_B5BB_00105AA2BB33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Obj_Printer.h"

class CNomenclatureView;

class CPrinter  : public CObj_Printer
{
public:

	CPrinter();
	virtual ~CPrinter();
	
	void Initialise(CDC * pDC,SIniPrint * pInit);
	bool DrawTete(CDC * pDC, CRect * pR, short NrPage, short NrLig);
	bool DrawTete(CDC * pDC, CRect * pR, short NrPage, short NrLig,short PremierNrTab,short DernierNrTab);
	bool DrawPieds(CDC * pDC, CRect * pR, short NrPage, short NrLig);
	bool DrawTab(CDC * pDC, CRect * pR, short NrTab, short NrType, short NrLig);	
	bool DrawVTab(CDC * pDC, CRect * pR_haut, CRect * pR_bas,short NrTab);	

    void DessineLigne(CDC * pDC, CRect * pR,CFont * pFont,CString Txt);

	CNomenclatureView * m_pView;
	// CBitmap m_bmp;
};

#endif // !defined(AFX_PRINTER_H__70889DA5_FC24_11D3_B5BB_00105AA2BB33__INCLUDED_)
