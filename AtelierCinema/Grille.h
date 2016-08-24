#if !defined(AFX_GRILLE_H__72B9B3C0_0BA1_11D4_989E_004095420987__INCLUDED_)
#define AFX_GRILLE_H__72B9B3C0_0BA1_11D4_989E_004095420987__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Grille.h : header file
//



#include "campagne.h"

// paramètres pysique
#define LARGCOLONE_ACCOGRIL (16) // largeur de la colone dans le coeur de grille
#define HAUTLIGNE_ACCOGRIL (16) // hauteur de la ligne dans le coeur de grille
#define PASSAGEPIX_ACCOGRIL (15) // taille d'un carré (x et y)


/////////////////////////////////////////////////////////////////////////////
// CGrille window

class CGrille : public CStatic
{
// Construction
public:
	CGrille();

// Attributes
public:

private:
	void InitAscenceur();
	void SetModifiedFlag(bool flag=1);
	void LanceCalcul(int type);
	int m_Part; //0: Calendrier 1:Binôme 2:NbPassage 3:Coeur de grille
	CCampagne * m_pCampagne;
	bool m_fCapture;



// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrille)
	//}}AFX_VIRTUAL

// Implementation
public:
	void InitAccelerateur();
	void InitDessin();
	void Init(int part,CCampagne * campagne);
	virtual ~CGrille();

private:
	RECT m_BtnRect;
	void LiberationBlocPassACCOGRIL();
	void DessineBlocDeplacementACCOGRIL(HDC hDC);
	void DessineCelluleSpeedACCOGRIL(HDC hDCdest,HDC hDCspeed,int posx,int posy,int TC);
	void SetTextACCOGRIL(HDC hDC, LPCSTR  txt , RECT  R);
	void DessineCelluleACCOGRIL(HDC hDC,int posx,int posy,int TC);
	void DrawRectPlisse(HDC hDC,RECT R);
	int TypeDeCelluleACCOGRIL(int NrParc,int NrSemaine);
	int TraitSousCurseurACCOGRIL(POINT pt);
	void MajAffichageACCOGRIL(char fColoneBinome);
	void ConstruitBlocPassACCOGRIL(int SDebut,int PDebut,char fEfface);
	void MajAffichageCalendACCOGRIL();
	void MajInfoCalendACCOGRIL();
	void CopyBlocPassACCOGRIL(int SDebut,int PDebut);
	void ConstruitAccelerateurACCOGRIL();




	// Generated message map functions
protected:
	//{{AFX_MSG(CGrille)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRILLE_H__72B9B3C0_0BA1_11D4_989E_004095420987__INCLUDED_)
