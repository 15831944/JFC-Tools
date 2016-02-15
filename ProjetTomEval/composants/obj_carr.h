#if !defined(AFX_OBJ_CARR_H__D23902A2_DE95_11D1_B3F8_006052017E36__INCLUDED_)
#define AFX_OBJ_CARR_H__D23902A2_DE95_11D1_B3F8_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CObj_Carr window
/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Objet JFC	: Obj_Carr v1.1a
Tâche		: Découpage journalier
Fichiers	: Obj_Carr.cpp, Obj_Carr.h, Obj_Carr.rco, Obj_Carr.icon1
Auteur		: VLF
Creer le	: 14/05/98 
Mise à jour : 10/07/00
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#define	OBJ_CARR_VER		1.1a

// Paramètres de compilation
#define TAILLETABLCCARR (1) // (nombre premier) volume du tableau de L.C.

// coordonnées rigides du carré
#define CARRX_COTEICONE (32) // cote des ivone du style std

// Styles de l'objet
#define CARRS_STANDARD  0 // style par defaut
#define CARRS_DEMINEUR  1 // style démineur

class CObj_Carr : public CStatic
{
// Construction
public:
	CObj_Carr();
// Attributes
public:

// Operations
public:
	short	Initialise(short style, CByteArray * Selections);
	short	DonneResult(CByteArray * Selections);
	void	LibereJour(void);	//Libération de l'objet pour réinitialisation

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObj_Carr)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CObj_Carr();

	// Generated message map functions
protected:
	//{{AFX_MSG(CObj_Carr)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	//}}AFX_MSG
	HICON m_hIcon1;
	CBrush m_CarrB_GrisC, m_brushwhite, m_brushyellow;
	RECT m_R;

	CByteArray * m_Selections;
	CPtrList * m_Matrice;	// par ligne colone
	short m_Style;
	short m_LargCol; // en pix
	short m_HautLig; // en pix	

	DECLARE_MESSAGE_MAP()
	void DessineCarr(CPaintDC * dcDest);
	void Carr_DessineIcone(HDC hDC, RECT R, HICON hIco, short TailleX, short TailleY);
	void MajMatrice(void);
	BYTE GetByteSelection(short ligne, short col);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJ_CARR_H__D23902A2_DE95_11D1_B3F8_006052017E36__INCLUDED_)
