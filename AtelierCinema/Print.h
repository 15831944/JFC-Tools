// Print.h: interface for the CPrint class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRINT_H__92942E81_66A8_11D3_989D_9A7D9F72CD3D__INCLUDED_)
#define AFX_PRINT_H__92942E81_66A8_11D3_989D_9A7D9F72CD3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "campagne.h"
#include "campagneagglo.h"
class CPrint : public CObj_Printer  
{
public:
	void SetInfo(CCampagneAgglo * pCampagne);
	CPrint();
	virtual ~CPrint();
private:
	CWordArray m_TableType;
	void InitCalendrier();
	void Initialise(CDC * pDC,SIniPrint * pInit);
	bool DrawTete(CDC * pDC, CRect * pR, short NrPage, short NrLig,short PremierNrTab,short DernierNrTab);
	bool DrawPieds(CDC * pDC, CRect * pR, short NrPage, short NrLig);
	bool DrawTab(CDC * pDC, CRect * pR, short NrTab, short NrType, short NrLig);
	bool DrawVTab(CDC * pDC, CRect * pR_haut, CRect * pR_bas,short NrTab);
	bool DrawCol(CDC * pDC, CRect * pR, short NrTab, short NrType, short NrLig,short NrCol,short NbCol);

	CCampagneAgglo * m_pCampagne;
//	CBitmap m_bmp;


	// Pour le calendrier
	CWordArray m_NroBActifs;
	CDWordArray m_BBudgets;
	CStringArray m_NomBinome;
	int m_NbBActif;
	int ** m_Passage;
	char **** m_Spots;
	char m_LegendeFormat[NBFORMATMAX][2][50];
	int m_NbLegende;
	int m_TSemaine[3][2]; // semaine début et semaine de fin
	int m_NbTab;
	bool m_fAlloc;
};

#endif // !defined(AFX_PRINT_H__92942E81_66A8_11D3_989D_9A7D9F72CD3D__INCLUDED_)
