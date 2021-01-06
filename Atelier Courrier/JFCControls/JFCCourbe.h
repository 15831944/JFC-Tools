#if !defined(AFX_JFCCourbe_H__DC738038_D675_45B6_BC6B_E4F930A8B98E__INCLUDED_)
#define AFX_JFCCourbe_H__DC738038_D675_45B6_BC6B_E4F930A8B98E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JFCView.h"
#include "JFCScroll.h"
#include "JFCGraduation.h"

// on inclut les définitions nécessaires
#include "JFCICourbe.h"

/////////////////////////////////////////////////////////////////////////////
// JFCCourbe

class AFX_EXT_CLASS JFCCourbe : public JFCView
{
public:
	// le constructeur
	JFCCourbe();

	// la fonction pour initialiser les éléments
	virtual JFCICourbe* SetItems(JFCICourbe* pICourbe, BOOL Update = TRUE);

	// la fonction pour définir l'ascenseur externe
	virtual void SetHorzScroll(JFCScroll* pScroll, BOOL Master, BOOL update = TRUE);

	// la fonction pour définir les pas de graduation en X
	virtual void SetPasGraduationEnX(LONG PasGraduationX);

	// la fonction pour définir les pas de graduation en Y
	virtual void SetPasGraduationEnY(LONG PasGraduationY);

	// les fonctions pour récupérer l'ascenseur externe
	virtual JFCScroll* GetHorzScroll();

	///////////////////////////////////////////////////////////
	// la fonction pour exporter le graphe dde la courbe
	virtual JBool ExportGraph(LPCTSTR pFilename);

	// le destructeur
	virtual ~JFCCourbe();

private:
	// la fonction pour mettre à jour les graduations
	void OnUpdateGraduations();

private:
	// la fonction pour configurer les ascenseurs actifs
	void OnSetScrollBar(BOOL Update);

	// la fonction pour récupérer la position de l'ascenseur actif
	long OnGetHorzPosition();

private:
	// la fonction pour calculer la position des éléments
	long OnGetYPosition(long VSize, double YGrdMin, double YGrdMax, double Value);

private:
	// les fonctions pour dessiner les axes et les moustaches
	void OnDrawAxes(CDC & dc, RECT rect);
	void OnDrawMoustaches(CDC & dc, RECT rect);

	// Dessin des courbes
	void OnDrawCourbes(CDC & dc, RECT rect);

private:
	// les fonctions pour dessiner les éléments
	bool OnBeginDrawing(CDC & dc);
	void OnDrawItems(CDC & dc, RECT rect);
	void OnEndDrawing(CDC & dc);

private: 

	// les fonctions pour dessiner les éléments
	bool OnBeginDrawing(CMetaFileDC &MetaDC);
	void OnDrawItems(CMetaFileDC &MetaDC, RECT rect);
	void OnEndDrawing(CMetaFileDC &MetaDC);

	/////////////////// Même fonction mais pour le Métafile /////////////////////
	void OnDrawAxes(CMetaFileDC &MetaDC, RECT rect);
	void OnDrawCourbes(CMetaFileDC &MetaDC, RECT rect);

protected:
	//{{AFX_VIRTUAL(JFCCourbe)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(JFCCourbe)
	afx_msg void OnSize(UINT nType, INT cx, INT cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JFCICourbe* m_pICourbe;			// les éléments

	JFCScroll* m_pHorzScroll;		// le pointeur sur l'ascenseur

	BOOL m_HorzMaster;				// l'indicateur de l'ascenseur

	JFCGraduation m_Graduations;	// les graduations

private:

	CFont m_FontP;					// la petite fonte
	CFont m_FontO;					// la fonte oblique

	CBrush m_Brushes;				// le pinceau

	CPen m_PenColor;				// le stylo couleur courbe		
	CPen m_PenB;					// le stylo noir
	CPen m_PenG;					// le stylo gris

private :
	// Découpage chaine par milliers
	void FormatString(CString &Txt, JInt32 PasDecoup);
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JFCCourbe_H__DC738038_D675_45B6_BC6B_E4F930A8B98E__INCLUDED_)
