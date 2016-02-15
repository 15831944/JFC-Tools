#if !defined(AFX_COBJ_HORR_H__64DD38CF_E3F4_11D1_843A_444553540000__INCLUDED_)
#define AFX_COBJ_HORR_H__64DD38CF_E3F4_11D1_843A_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// Obj_Horr window
/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Objet JFC	: Obj_Horr v1.1a
Tâche		: Découpage horaire
Fichiers	: Obj_Horr.cpp, Obj_Horr.h, Obj_Horr.rco, Obj_Horr.ico1, Obj_Horr.ico2, Obj_Horr.ico3, Obj_Horr.ico4
Auteur		: VLF
Creer le	: 14/05/98
Mise à jour : 10/07/00
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#define	OBJ_HORR_VER		1.1a

class CObj_Horr : public CStatic
{
// Styles de l'objet
#define HOURS_STANDARD		0					// style par defaut
#define MINUTES_STANDARD	1					// style minutes
#define HOURS_MINUTES		2					// style heure avec une division minute
#define TV100MINUTES		3					// style "100" minutes en télé

//Options
#define INVISIBLE_VSCROLL	256					//0x100
#define QH_ZOOM				512					//0x200
#define SCROLL_HEURE		1024				//0x400 scroll des heures
#define COUPURE_OBLIGATOIRE	2048				//0x800 bloque la première tranche


//Nombre de divisions pour chacun des types
#define DECOUPAGE_HOUR		97
#define DECOUPAGE_MIN		1441
#define DECOUPAGE_100MIN	2401
// Construction
public:
	CObj_Horr();

// Attributes
public:

// Operations
public:

	void			Initialise(int Styles,int HoraireDebut,char *Selections);
	short			DonneResultHour(char * Selections);
	short			DonneResultZoom();
	short			SetZoomQH(bool fVisible=false);
	void			SetPosition(int Position, bool fUpdate=true);
	void			SetSelections(char *Selections, bool fUpdate=true);
	virtual void	OnSelectionChanged(char *Selections);
	virtual void	OnZoom(int QH);
	void			LibereObjet(void);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObj_Horr)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual			~CObj_Horr();

	// Generated message map functions
protected:
	//{{AFX_MSG(CObj_Horr)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	// Mécanique
	short	m_HeureDebut;		//300->3h00
	short	m_QhDebut;			// 1,2,3 ou 4 qh
	short	m_MinDebut;			//1,2,3,4,5,6 ... 11,12ème d'heure
	char	*m_Selections;		//rouge(1) pour qh, bleu(2) pour l'inter-qh
	int		m_Style;
	int		m_NbLigne;
	int		m_PosAsc;
	int		m_PosZoom;
	bool	m_fCapture,			//Capture de la loupe
			m_fCaptureH,		//Capture de la barre des heures
			m_fInit;

	//barre Heure 
	short	m_Hdeb,
			m_Hsel,
			m_NbHeure,
			m_Hoffset;

	// GDI
	CBrush	m_BrushNoir,
			m_BrushGrisC;
	CPen	m_PenGrisF,
			m_PenBlanc,
			m_PenNoir,
			m_PenGrisC,
			m_Pen4B;
	HICON	m_Ico1,	//fleche rouge
			m_Ico2,	//fleche bleu
			m_Ico3,	//loupe zoom
			m_Ico4;	//fleche bleu retourne 
	CFont	m_Font,
			*m_pFont,
			*m_pFontB;
	CScrollBar * m_ScrollBar;
	CRect	m_RZoom,
			m_RHeure,			//Rectangle du cadre des miniheures
			m_RposHeure[24];	//position des heures

	// buffer
	char	m_tamponalloc[500];

private:
	void	DessineGraduations(CDC * DC);
	void	DessineLibelles(CDC * DC);
	void	DessineZoom(CDC * DC);
	void	DessineRaccourciH(CDC * DC);
	void	ClickInminiHeures(CPoint pt);
};

/////////////////////////////////////////////////////////////////////////////
#endif
