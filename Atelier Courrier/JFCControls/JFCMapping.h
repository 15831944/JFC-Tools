#if !defined(AFX_JFCMAPPING_H__B7C6DAAD_B22C_490C_905A_20230BB80905__INCLUDED_)
#define AFX_JFCMAPPING_H__B7C6DAAD_B22C_490C_905A_20230BB80905__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JFCIMapping.h"
#include "JFCICurve.h"
#include "JFCView.h"

/////////////////////////////////////////////////////////////////////////////
// JFCMapping.h : header file
//

class AFX_EXT_CLASS JFCMapping : public JFCView
{
public:
	// le constructeur
	JFCMapping();

	// la fonction pour initialiser les éléments
	virtual JFCIMapping* SetItems(JFCIMapping* pIMapping, BOOL update = TRUE);

	// la fonction pour initialiser les courbes
	virtual JFCICurve* SetCurve(JFCICurve* pICurve, BOOL update = TRUE);

	// la fonction pour supprimer les éléments
	virtual void Reset(BOOL update = TRUE);

	// la fonction pour exporter le dessin vers un MetaFile
	virtual BOOL ExportItems(long VSize, const TCHAR* pFilename = NULL);

	// le destructeur
	virtual ~JFCMapping();

// Implementation
private:
	// les fonctions pour dessiner les éléments
	bool OnBeginDrawing(CDC & dc);
	void OnDrawItems(CDC & dc, RECT rect);
	void OnEndDrawing(CDC & dc);

// Implementation
private:
	// les fonctions pour calculer la position des éléments
	long OnGetXPosition(double value, long HSize);
	long OnGetYPosition(double value, long VSize);

// Implementation
private:
	// les fonctions pour modifier le déplacement des libellés
	float OnSetDxLibelle(long value);
	float OnSetDyLibelle(long value);

	// les fonctions pour calculer le déplacement des libellés
	long OnGetDxLibelle(float value, long HSize);
	long OnGetDyLibelle(float value, long VSize);

// Implementation
private:
	// les fonctions de recherche des losanges et des libellés
	long OnGetLosangeAtPoint	(long x, long y);
	long OnGetLibelleAtPoint	(long x, long y);
	long OnGetCourbeAtPoint     (long x, long y);
	long OnGetLibEquationAtPoint(long x, long y);

// Implementation
private:
	// les fonctions pour dessiner le losange, le libellé et la courbe de tracking
	void OnTrackLosange		(long mode, long indice, long x, long y);
	void OnTrackLibelle		(long mode, long indice, long x, long y);
	void OnTrackCourbe		(long mode, long indice);
	void OnTrackLibEquation	(long mode, long indice, long x, long y);

// Implementation
private:
	// la fonction pour sélectionner les losanges, les libellés et les courbes
	void OnEndTrackLosanges   (long x1, long y1, long x2, long y2, bool reset, bool invert);
	void OnEndTrackLibelles   (long indice, long dx, long dy);
	void OnEndTrackCourbes    (long indice, bool reset);
	void OnEndTrackLibEquation(long indice, long dx, long dy, long x, long y);

// Implementation
private:
	// la fonction pour dessiner les axes
	void DrawAxes  (CDC & dc, long HSize, long VSize, bool bCadre=false);
	void OnDrawAxes(CDC & dc, RECT rect);

	// les fonctions pour dessiner les losanges
	void DrawLosanges  (CDC & dc, long HSize, long VSize);
	void OnDrawLosanges(CDC & dc, RECT rect);

	// les fonctions pour dessiner les libellés
	void DrawLibelles  (CDC & dc, long HSize, long VSize);
	void OnDrawLibelles(CDC & dc, RECT rect);

	// les fonctions pour dessiner les traits des libelles
	void DrawTraitLib  (CDC & dc, long HSize, long VSize);
	void OnDrawTraitLib(CDC & dc, RECT rect);

	// les fonctions pour dessiner les courbes
	void DrawCurves  (CDC & dc, long HSize, long VSize);
	void OnDrawCurves(CDC & dc, RECT rect);

	// les fonctions pour dessiner les traits des equations
	void DrawTraitEquation  (CDC & dc, long HSize, long VSize);
	void OnDrawTraitEquation(CDC & dc, RECT rect);

	// les fonctions pour dessiner les libelles des equations
	void DrawLibEquation  (CDC & dc, long HSize, long VSize);
	void OnDrawLibEquation(CDC & dc, RECT rect);

	// la fonction pour dessiner un cadre autour du dessin
	void DrawCadre(CDC & dc, CRect& rect);

// Implementation
private:
	// les fonctions pour mettre à jour les éléments
	void OnUpdateLongueurs();
	void OnUpdateGrandeurs();

// Implementation
private:
	// les fonctions pour récupérer l'état de l'univers
	bool OnGetSelection();
	bool OnGetFiltrage();
	bool OnGetExistCourbe();
	bool OnGetCourbeSelec();
	bool OnGetCourbeDegr1();
	bool OnGetCourbeDegr2();
	bool OnGetInfosCourbe();

	// Calcul courbes de tendance degré 1 & 2
	void CalculCrbTendance1();
	void CalculCrbTendance2();

	// Affichage ou pas de tous les libellés points
	void SetShowAllLib(bool etat);

// Implementation
private:
	// les identificateurs des menus
	static const LONG IdShowAllLibelle;
	static const LONG IdHideAllLibelle;
	static const LONG IdShowSelLibelle;
	static const LONG IdHideSelLibelle;
	static const LONG IdShowSelItem;
	static const LONG IdHideSelItem;
	static const LONG IdShowAllItem;
	static const LONG IdAllSelect;
	static const LONG IdInvSelect;
	static const LONG IdNoSelect;
	static const LONG IdCreateCourbeD1;
	static const LONG IdCreateCourbeD2;
	static const LONG IdHideAllCourbe;
	static const LONG IdHideCourbeSel;
	static const LONG IdShowLibCourbe;
	static const LONG IdHideLibCourbe;

// Implementation
private:
	long m_Track;   // le flag de tracking
	long m_TrkIdx;  // l'indice de l'élément
	long m_TrkPar1; // le premier   paramètre
	long m_TrkPar2; // la deuxième  paramètre
	long m_TrkPar3; // le troisième paramètre
	long m_TrkPar4; // le quatrième paramètre

// Implementation
private:
	long m_LbHg;    // la hauteur des libellés
	long m_LbEqHg;  // la hauteur des libellés des courbes

// Implementation
private:
	CArray<double, double &> m_BarycentreX; // la liste des abscisses des barycentre de courbes
	CArray<double, double &> m_BarycentreY; // la liste des ordonnees des barycentre de courbes
	CArray<double, double &> m_AccrochageX; // la liste des abscisses des points d'accroche des libelle a leur courbe
	CArray<double, double &> m_AccrochageY; // la liste des ordonnees des points d'accroche des libelle a leur courbe

// Implementation
private:
	CArray<long, long &>	m_LbLg;			// la longueur des libellés
	CArray<long, long &>	m_LbEqLg;	    // la longueur des libellés des courbes

// Implementation
private:
	JFCIMapping* m_pIMapping; // les éléments
	JFCICurve*   m_pICurve;   // les courbes

// Implementation
private:
	double m_XMin; // l'abscisse minimum
	double m_YMin; // l'ordonnée minimum
	double m_YMax; // l'ordonnée maximum
	double m_XMax; // l'abscisse maximum
	double m_XMoy; // l'abscisse moyenne
	double m_YMoy; // l'ordonnée moyenne

// Implementation
private:
	double m_XGrdMin; // l'abscisse minimum des graduations
	double m_YGrdMin; // l'ordonnée minimum des graduations
	double m_YGrdMax; // l'ordonnée maximum des graduations
	double m_XGrdMax; // l'abscisse maximum des graduations

// Implementation
private:
	CFont m_FontP; // la petite fonte
	CFont m_FontM; // la fonte moyenne
	CFont m_FontG; // la grande fonte

// Implementation
private:
	CBrush m_BrushG;     // le pinceau vert
	CBrush m_BrushY;     // le pinceau jaune
	CBrush m_BrushB;     // le pinceau bleu
	CBrush m_BrushBlue;  // le pinceau bleu
	CBrush m_BrushGreen; // le pinceau vert
	CBrush m_BrushSky;   // le pinceau bleu ciel
	CBrush m_BrushUmbre; // le pinceau violet
	CBrush m_BrushOrang; // le pinceau orange

// Implementation
private:
	CPen m_PenG;     // le stylo gris
	CPen m_PenB;     // le stylo noir
	CPen m_PenR;     // le stylo rouge
	CPen m_PenBlue;	 // le stylo bleu
	CPen m_PenGreen; // le stylo vert
	CPen m_PenSky;	 // le stylo bleu ciel
	CPen m_PenUmbre; // le stylo violet
	CPen m_PenOrang; // le stylo orange

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(JFCMapping)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Generated message map functions
protected:
	//{{AFX_MSG(JFCMapping)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg void OnShowAllLibelle();
	afx_msg void OnHideAllLibelle();
	afx_msg void OnShowSelLibelle();
	afx_msg void OnHideSelLibelle();
	afx_msg void OnShowSelItem();
	afx_msg void OnHideSelItem();
	afx_msg void OnShowAllItem();
	afx_msg void OnAllSelect();
	afx_msg void OnInvSelect();
	afx_msg void OnNoSelect();
	afx_msg void OnCreateCourbeD1();
	afx_msg void OnCreateCourbeD2();
	afx_msg void OnHideAllCourbe();
	afx_msg void OnHideCourbeSel();
	afx_msg void OnShowLibCourbe();
	afx_msg void OnHideLibCourbe();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JFCMAPPING_H__B7C6DAAD_B22C_490C_905A_20230BB80905__INCLUDED_)
