#if !defined(AFX_OBJ_CAL_H__7B32C921_E41F_11D1_B3F8_006052017E36__INCLUDED_)
#define AFX_OBJ_CAL_H__7B32C921_E41F_11D1_B3F8_006052017E36__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Obj_Cal.h : header file
//
//disable le warning 4244
//warning C4244: '=' : conversion from 'int' to 'short', possible loss of data
#pragma warning(disable : 4244)
/////////////////////////////////////////////////////////////////////////////
// CObj_Cal window
/*
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
Objet JFC	: Obj_Cal v7.7a
Tâche		: Affichage d'un calendrier horizontal jour/semaine/mois/annee paramétrable
Type 		: Amelioration
Fichiers	: Obj_Cal.cpp, Obj_Cal.h
Auteur		: VLF
Date		: 24/07/00
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
*/

#define OBJ_CAL_VER		7.7

////////////////////////////////////////////////////////////////////
struct SInitSelection
{
	short		LevelSelect;					//niveau de sélection (1=jour à 7=tous)
	COLORREF	ColorSel;						//RGB de la selection
	short		TypeSelect;						//indique le type de selection (gauche(0) ou droite(1))
	short		ModeSel;						//4 modes: SIMPLE, MULTIPLE, ETENDUE, ETENDUE_JFC
	short		ShowRect;						//zone d'affichage : jour(0) ou toute la hauteur du calendrier(1)
};
////////////////////////////////////////////////////////////////////
#include "Obj_Gril.h"


class CObj_Cal : public CStatic
{
//style
#define STYLE_CAL_DEFAUT			0			//sans grille pige
#define STYLE_CAL_PIGE				1			//avec la grille PIGE
#define STYLE_CAL_CREUSET			2			//avec la grille PIGE
//Options de style
#define OPTION_CAL_SPLITGRAY		16			//trait jour en gris
#define OPTION_CAL_BIGTABLIER		32			//tablier définit sur toute la hauteur
#define OPTION_CAL_PRINTER			64			//pour dessiner calendrier vers imprimante
////////////////////////////////////////////////////////////////////
//format standard
#define FORMAT_CAL_STANDARD			0			//libellé mois (01-12), semaine(1-52)), jour(1-31)
//format standard des mois
#define FORMAT_CAL_MOISABR			1			//Jan-Déc
#define FORMAT_CAL_MOIS				2			//Janvier-Décembre
#define FORMAT_CAL_STDANABRG		3			//01/98
#define FORMAT_CAL_MOISANNEE		4			//Janvier 1998
//format standard des semaines
#define FORMAT_CAL_JOURDEBUTSEM		1			//07, 14, ...
//format standard des jours
#define FORMAT_CAL_JOUR				2			//Lundi-Dimanche
#define FORMAT_CAL_JOURSEM			1			//L	|M |M  ...
												//03|04|05 ...
////////////////////////////////////////////////////////////////////
//flag de selection
#define CAL_SIMPLE					1			//selection d'une seul cellule; obligatoire
#define CAL_MULTIPLE				2			//plusieurs sélections de cellules
#define CAL_ETENDUE					3			//sélection d'une vague; non obligatoire
#define CAL_ETENDUE_JFC				4			//sélection de plusieurs vagues
////////////////////////////////////////////////////////////////////
//niveau de sélection
#define	CAL_LEVEL_JOUR				1			//0x00000001 niveau de sélection
#define CAL_LEVEL_SEMAINE			2			//0x00000002 Combinaison des trois
#define CAL_LEVEL_MOIS				4			//0x00000004
#define CAL_LEVEL_ANNEE				8			//0x00000008
////////////////////////////////////////////////////////////////////
#define CAL_MOIS_CALENDAIRE			256			//0x00000100 sélection quotidienne indifférement du click
#define CAL_MOIS_HEBDOMADAIRE		512			//0x00000200 sélection hebdomadaire indifférement du click
#define CAL_SEL_INDEP_HIDE			1024		//0x00000400 sélection indépendante en dessous de la sélection
//règles de sélection
#define CAL_INDEPENDANTE			0
//Etat		Click	Resultat
//-----------------------------
//Sans sel	Gauche	Selection
//Selection	Gauche	Sans selection
//Sans sel	Droit	Sur-selection
//Sur-sel	Droit	Sans selection
#define CAL_CROISEE					1
#define CAL_LEFTINTORIGHT			2
#define CAL_RIGHTINTOLEFT			3
#define CAL_PREGNANCE				4			
//Etat		Click	Resultat
//-----------------------------
//Sans sel	Gauche	Selection
//Selection	Gauche	Sans selection
//Sur-sel	Gauche	Selection
//Sans sel	Droit	Sur-selection
//Selection	Droit	Sur-selection
//Sur-Sel	Droit	Selection
#define CAL_OPPOSITION				5
//Etat		Click	Resultat
//-----------------------------
//Sans sel	Gauche	Selection
//Selection	Gauche	Sans selection
//Sur-sel	Gauche	Selection
//Sans sel	Droit	Sur-selection
//Selection	Droit	Sur-selection
//Sur-Sel	Droit	Sans selection


//Indique le nb de niveau possible
#define CAL_NBNIVEAU				4

// Construction
public:
	CObj_Cal();
	CObj_Gril * m_pGrille;
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObj_Cal)
	//}}AFX_VIRTUAL

// Implementation
//fonction public pour l'extérieur

public:
	virtual ~CObj_Cal();
	
//VOIR LE FICHIER JFCHELP.HTML POUR INFOS
/////////////////////////////////////////////////////////////////////////////////
	//initialisation globale du calendrier
	void		Initialise(short Style=STYLE_CAL_DEFAUT,
					COleDateTime JourDeb=COleDateTime::GetCurrentTime(),
					short NbCases=365,
					CScrollBar  * ptScroll=NULL,
					CFont *ptFont=NULL,
					short DefPos=0,
					CRect * pR_Print=NULL);

	//initialisation de la sélection gauche
	void		InitialiseSelection(short ModeSelect=CAL_SIMPLE,
							 short LevelSelect=CAL_LEVEL_JOUR|CAL_LEVEL_SEMAINE|CAL_LEVEL_MOIS,
							 COLORREF ColorSelect=RGB(0,255,0));

	//initialisation de la sélection
	void		InitialiseSelection(SInitSelection * LeftSelection,
							 SInitSelection * RightSelection=NULL,
							 short SelectRules=CAL_INDEPENDANTE);

	//initialisation de chaque entité du calendrier
	//Annee
	void		InitialiseAnnee(short Format=FORMAT_CAL_STANDARD,
						COLORREF ColorBg=RGB(255,255,255),
						COLORREF ColorTxt=RGB(0,0,0),
						COLORREF ColorSplit=RGB(210,210,210),
						short UTVertical=16,
						short UTHorizontal=0,
						short NumOrdre=0);
	//Mois
	void		InitialiseMois(bool fAlignSem=0,
						short Format=FORMAT_CAL_STANDARD,
						COLORREF ColorBg=RGB(255,255,255),
						COLORREF ColorTxt=RGB(0,0,0),
						COLORREF ColorSplit=RGB(210,210,210),
						short UTVertical=16,
						short UTHorizontal=0,
						short NumOrdre=1);
	//Semaine
	void		InitialiseSemaine(short JourDebSem=0,
							short Format=FORMAT_CAL_STANDARD,
							COLORREF ColorBg=RGB(255,255,255),
							COLORREF ColorTxt=RGB(0,0,0),
							COLORREF ColorSplit=RGB(210,210,210),
							short UTVertical=16,
							short UTHorizontal=0,
							short NumOrdre=2);
	//Jour
	void		InitialiseJour(short Format=FORMAT_CAL_STANDARD,
						COLORREF ColorBg=RGB(255,255,255),
						COLORREF ColorTxt=RGB(0,0,0),
						COLORREF ColorSplit=RGB(210,210,210),
						short UTVertical=16,
						short UTHorizontal=0,
						short NumOrdre=3);
	
	//installe la sélection
	CByteArray *GetSelections(void);
	CByteArray *GetSelectionsRight(void); // Ajouté par rémi le 8/10/99 à 00h24 pour topchrono
	void		SetSelections(CByteArray * Selections, CByteArray * SurSelections);
	void		ChangeLeftSelectionColor(COLORREF color, bool flagRedraw);
	void		ChangeRightSelectionColor(COLORREF color, bool flagRedraw);
	//Modifie le mode et les regles de selections
	//MouseKey = MK_LBUTTON, MK_MBUTTON, MK_RBUTTON
	void		ChangeModeSelection(long MouseKey, long ModeSelect, long SelectRules, bool fRAZSelect = false);

	//Sélection indépendante au click-click souris
	//fOption peut-être CAL_SEL_INDEP_HIDE ou 0 par défaut
	void		SetSelectionIndep(CByteArray * IndepSel, short UTVerticalDebut = 0, short UTVerticalFin = 6, bool fClose = true, COLORREF ColorSelIndep=RGB(255,0,0), long fOption = 0, long nIndepSel = 0);
	void		AddSelectionIndep(CByteArray * Selections, short UTVerticalDebut = 0, short UTVerticalFin = 6, bool fClose = true, COLORREF ColorSelIndep=RGB(255,0,0), long fOption = 0);
	CByteArray *GetSelectionIndep(long nIndepSel = 0);
	void		RAZSelectionIndep(long nIndepSel = -1);

	//sélection d'un niveau de sélection parmis ceux existant
	void		SetLevelSelect(short Select, bool fRAZSel);
	
	//Bouge la position du calendrier
	short		SetPosition(short Pos, bool flag=false);
	
	//Recupérer de l'extérieur la position des coupures	(util pour le style PIGE)
	char		*GetCoupureCalend(void);
	
	//Curseurs/Tablier
	short		SetCurseur(short NumCurs, short Pos, COLORREF Color);
	short		SetTablier(short Curseur1, short Curseur2, COLORREF colorTablier);
	short		SetPosCurseur(short ncurs, short NewPos);

	//changement de la periode
	void		ChangePeriode(COleDateTime JourDeb, short NbCases, bool fUpdate=false);
	
	//ascenseur Hor
	short		MAJPosScrollH(UINT nSBCode, UINT nPos, short flag);

	//printer
	void		PaintToPrinter(CDC * pDC, CFont * pFont=NULL);

	//Libere pour une nouvelle initialisation dynamique
	void LibereCalend(void);
/////////////////////////////////////////////////////////////////////////////////

protected:
	//fonction de curseur
	short		GetPosCurseur(short ncurs);

	//surdéfinition de fonctions virtuelles
	virtual bool OnCurseurMoved(short NumCurs, short Pos);
	virtual bool OnSelectionChanged(CByteArray * Selections, CByteArray * SurSelections);
	virtual bool OnLevelSelect(short Level, UINT nFlags);

// Generated message map functions
//Variables d'initialisation
private:
	short		m_Style;						//style général de l'objet
	bool		m_fStyle;						//flag util pour le style PIGE
	COleDateTime m_jourdeb;						//date de début du calendrier
	short		m_nbcases;						//nb de cases total
	short		m_nbcasesvisu;					//nb de cases visualisable à l'écran
	short		m_posactive;					//origine de la 1er case (0-nbcases)
	CScrollBar	*m_ptscroll;					//pointeur sur un scrollbar Hor
	CFont		*m_ptFont;						//pointeur sur une font pour le libellés
	short		m_ModeSel;						//5 modes: NOSELECTION, SIMPLE, MULTIPLE, ETENDUE, ETENDUE_JFC
	short		m_LevelSelect;					//niveau de sélection (1=jour à 7=tous)
	bool		m_fAlignSem;					//alignement des semaines (soit avec jours, soit ?)
	short		m_FormatAnnee,
				m_FormatMois,					//valeur du format mois
				m_FormatSem,					//				   semaine
				m_FormatJour;					//				   jour
	short		m_dx;							//largeur du niveau le plus petit
	short		m_dxAnnee,
				m_dxMois,
				m_dxSem,
				m_dxJour;						//largeur des  niveaux
	short		m_dyAnnee,
				m_dyMois,
				m_dySem,
				m_dyJour;						//hauteur des niveaux
	short		m_yAnnee,
				m_yMois,
				m_ySem,
				m_yJour;						//position y de début de niveau(0 en haut)
	short		m_JourDebSem;						//
	short		m_numOrdreAnnee,
				m_numOrdreMois,					//0; ordre dans lequel apparait les niveaux
				m_numOrdreSem,					//1;
				m_numOrdreJour;					//2;
	short		m_SelectRules;					//Regle de sélection : INDEPENDANTE ou CROISE ou LEFTINTORIGHT ou RIGHTINTOLEFT
	COLORREF	m_colorSel,						//RGB de la selection click gauche
				m_colorSursel;					//RGB de la selection click droit
	COLORREF	m_colorBgAnnee,
				m_colorBgMois,					//RGB du fond pour chaque niveau
				m_colorBgSem,					//
				m_colorBgJour;					//
	COLORREF	m_ColorTxtAnnee,
				m_ColorTxtMois,					//RGB du texte mois
				m_ColorTxtSem,					//RGB du texte semaine
				m_ColorTxtJour;					//RGB du texte jour
	COLORREF	m_colorSplitAnnee,
				m_colorSplitJour,				//RGB des coupures jours
				m_colorSplitSem,				//RGB des coupures semaines
				m_colorSplitMois;				//RGB des coupures mois

	//Variable d'initialisation du tablier
	short		m_PosTablier1,
				m_PosTablier2;					//position des curseurs
	COLORREF	m_ColorTablier;					//couleur servant de mask pour le tablier

//Variables pour le fonctionnement du calendrier
private:
	struct STabOrd
	{
		short m_y;
		short m_dy;
	};
	STabOrd m_TabOrd[4];

	//structure concernant les infos des curseurs
	struct SCurseur
	{
		short		NumCurseur;					//random
		COLORREF	Color;						//RGB
		short		Pos;						//0-(m_nbcases-1)
		CRect		R;							//zone curseur
		bool		Select;						//flag 1=select 0=non select
	};
	SCurseur	*m_SCurseur;
	short		m_nbcurseur;
	CPtrArray	*m_ArrayCurseur;				//[m_nbcurseur]

	//structure contenant les infos de tous les jours
	struct SDataJour
	{
		short JourMois;							//1, 31
		short JourSem;							//1=Dimanche, 2=Lundi, ...
		short NoSem;							//1, 53
		short JourDebSem;						//1, 31
		short Mois;								//1, 12
		short Annee;
	};
	SDataJour	*m_SCalend;						//[nbcase]
	
	//structure de selections
	struct SSelection
	{
		short		ColSelect,					//num de la case sélectionné par la souris
					debSelect;					//num de la case sélectionné au début de la sélection
		short		debL,
					debR;						//num des cases placé aux extrémités d'une sélection; Gauche et droite
		CByteArray	Selections,					//[nbcases][0 ou 1]
					SelectionsTmp,
					SelectionsRAZ;
		CRect		RSelect;					//zone de clickage actuel
		short		LevelSelect;				//niveau de sélection (1=jour à 7=tous)
		COLORREF	ColorSel;					//RGB de la selection click gauche
		short		TypeSelect;					//indique le type de selection (gauche(0) ou droite(1))
		short		ModeSel;					//4 modes: SIMPLE, MULTIPLE, ETENDUE, ETENDUE_JFC
		short		ShowRect;					//zone d'affichage : jour(0) ou toute la hauteur du calendrier(1)
		long		fOption;					//Option de la selection (CAL_SEL_MENSUELLE par exemple)
		SSelection(){}
		SSelection(const SSelection &source)
		{
			ColSelect	= source.ColSelect;
			debSelect	= source.debSelect;
			debL		= source.debL;
			debR		= source.debR;
			Selections.Copy(source.Selections);
			SelectionsTmp.Copy(source.SelectionsTmp);
			SelectionsRAZ.Copy(source.SelectionsRAZ);
			RSelect		= source.RSelect;
			LevelSelect	= source.LevelSelect;
			ColorSel	= source.ColorSel;
			TypeSelect	= source.TypeSelect;
			ModeSel		= source.ModeSel;
			ShowRect	= source.ShowRect;
			fOption		= source.fOption;
		}
		SSelection & operator=(SSelection &source)
		{
			ColSelect		= source.ColSelect;
			debSelect	= source.debSelect;
			debL		= source.debL;
			debR		= source.debR;
			Selections.Copy(source.Selections);
			SelectionsTmp.Copy(source.SelectionsTmp);
			SelectionsRAZ.Copy(source.SelectionsRAZ);
			RSelect		= source.RSelect;
			LevelSelect	= source.LevelSelect;
			ColorSel	= source.ColorSel;
			TypeSelect	= source.TypeSelect;
			ModeSel		= source.ModeSel;
			ShowRect	= source.ShowRect;
			fOption		= source.fOption;
			return *this;
		}
	};
	SSelection	*m_LeftSelect,
				*m_RightSelect,
				*m_CurrSelect;
//				*m_IndepSelect;
	CArray <SSelection, SSelection&> m_vectIndepSel;

	//tableaux contenant les formats d'affichage des libelles
	short	m_nbmois;
	CString	**m_LibMois;						//[nbmois][format]
	short	m_nbjour;
	CString **m_LibJour;						//[nbjour][format]

	char	*m_CoupureCalend;					//[nbcase+2]
												//0=jour
												//1=sem&jour
												//2=mois&jour
												//3=sem&mois&jour
												//4=Annee&jour
												//5=Annee&Sem&jour
												//6=Annee&Mois&jour
												//7=Annee&Mois&Sem&Jour
	short	m_nbCol;							//nb de colonnes visu; pour l'ascenseur Hor

	//flags
	bool	m_fInit,							//indique si initialisation OK
			m_fCapture,							//indique si click gauche=selection de cellule
			m_fSurCapture,						//indique si click droit=selection de cellule PIGE
			m_fLBCapture,						//souris click gauche select
			m_fRBCapture,						//souris click droite select
			m_fSelIndep,						//Indique si sélection indépendante présente
			m_fAscHor,							//indique si asc bouge
			m_fSelect,							//indique si selection ou déselection de cellule (Style PIGE)
			m_fCursor,							//indique si click droit=selection du curseur
			m_fTablier,							//indique si tablier présent
			m_fTablierCreuset;					//indique si selection Pige (selection définie le tablier)
	short	m_fClickLevel;						//indique si selection de cellules en zone jour/semaine/mois	
	UINT	m_nFlags;							//flags souris

	long	m_fModeSelect;						//indique si selection ou déselection de cellule
												//0=Deselection, 1=Selection, 2=Selection Gauche/Deselection droite, 3=Selection droite/Deselection gauche

	//zones rectangle
	CRect	m_RClient,							//rectangle de l'objet static
			m_RAnnee,
			m_RMois,							//rectangle mois
			m_RSem,								//rectangle semaine
			m_RJour,							//rectangle jour
			m_RLevelSel;						//zone du niveau selectionné
	
	COLORREF m_colorBg;							//RGB tempo du fond
	CFont	*m_pTimesNewRoman,					//Font chargé par défaut si aucune
			*m_pMSSansSerif;					//2eme font; non utilisé
	CPen	m_PenAnnee,
			m_PenJour,							//pen des coupures jours
			m_PenSem,							//pen des coupures semaines
			m_PenMois,							//pen des coupures mois
			*m_PenCurr;							//pen courant

	HCURSOR m_hCursorArrow, m_hCursorSizewe;

	CRect	m_PrntRect;
	CDC 	*m_PrntDC;

	// Generated message map functions
	//{{AFX_MSG(CObj_Cal)
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()


private:
	//Aloue et initialise les différentes variables
	void AlloueCalend(void);

	//fonction de validation des zones
	bool	ValideCalend(void);
	
	//fonction qui initialise les strutures
	short	ConstruitCalendrier(void);
	short	CalculCoupure(void);

	//fonction de dessin
	short	DessineAnnee(CDC * dc, short OffsetY);
	short	DessineCadreAnnee(CDC * dc);
	short	DessineMois(CDC * dc, short OffsetY);
	short	DessineCadreMois(CDC * dc);
	short	DessineSem(CDC * dc, short OffsetY);
	short	DessineCadreSem(CDC * dc);
	short	DessineJour(CDC * dc, short OffsetY);
	short	DessineCadreJour(CDC * dc);
	short	DessineCoupures(CDC * dc);
	short	DessineCoupureJour(CDC * dc, short pos);
	short	DessineCoupureSem(CDC * dc, short pos);
	short	DessineCoupureMois(CDC * dc, short pos);
	short	DessineCoupureAnnee(CDC * dc, short pos);
	void	SetCoupuresJour(BYTE sel, CDC * dc, short pos);

	short	DessineLibelles(CDC * dc);
	short	DessineLibelleJour(CDC * dc, short pos);
	short	DessineLibelleSem(CDC * dc, short pos);
	short	DessineLibelleMois(CDC * dc, short pos);
	short	DessineLibelleAnnee(CDC * dc, short pos);

	void	DessineSelectionIndep(CDC * dc, SSelection *IndepSelect = NULL);
	void	DessineSelection(CDC * dc);
	void	DessineSelectionPIGE(CDC * dc);
//	void	DessineSurSelection(CDC * dc);		//style pige
	void	DessineCurseur(CDC * dc);
	void	DessineTablier(CDC * dc);

	void	DessineCalendrier(CDC * pDC,CRect * R, CFont * pFont, bool fPrinter=false);

	short	DelCurseur(short NumCursor);
	bool	IsMouseOnCursor(CPoint pt);

	//fonction de selection
	void	MAJSelections(void);
	void	ChangeSelection(SSelection * Selection);
	short	GetClickInfo(CPoint point);
	bool	SetClickSelections(short f);
	bool	IsSelectRules(short click, short pos);

	//detecte le mode de selection selon les modes
	void	DetectModeSelect(long borne1 = 0, long borne2 = 0, long curs2 = 0); 
	//Selectionne les cellules entourant le click souris suivant le mode de selection
	void	SetCellsSelection(long Cell);
	//Recupère l'état actuelle de la selection; retourne 0, 1, 2 ou 3
	long	GetEtatSelection(long borne1, long borne2, long curs2);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJ_CAL_H__7B32C921_E41F_11D1_B3F8_006052017E36__INCLUDED_)
