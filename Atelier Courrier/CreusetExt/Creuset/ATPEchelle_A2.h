#if !defined(AFX_ATPECHELLE_A2_H__C0620813_9F2D_428B_9E16_0183A621CE3D__INCLUDED_)
#define AFX_ATPECHELLE_A2_H__C0620813_9F2D_428B_9E16_0183A621CE3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPEchelle_A2

class CATPEchelle_A2 : public JFCGrid
{
public:
	// le constructeur
	CATPEchelle_A2();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPEchelle_A2();

protected:
	// la fonction récupération nombre d'insertions Support
	JInt32 OnGetNbInsertions(JUnt32 IdSupport) const;

	// la fonction récupération nombre d'insertions Support sur la période d'action uniquement
	JInt32 OnGetNbInsertionsPA(JUnt32 IdSupport) const;

	// la fonction récupération nombre d'insertions Support sur toute l'étude
	JInt32 OnGetNbInsertionsEtude(JUnt32 IdSupport) const;

protected:
	// la fonction pour dessiner l'intérieur des cellules
	JVoid OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);

	// les fonctions pour capter les clicks de la souris
	JVoid OnLButtonDown(LONG i, LONG j);
	JVoid OnRButtonDown(LONG i, LONG j);
	JVoid OnMouseMove(LONG i, LONG j);

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

	// procédure pour virer les insertions d'un support
	JVoid SuppInsertionSupport(JUnt32 IdxSupport);

protected:
	//{{AFX_VIRTUAL(CATPEchelle_A2)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	//{{AFX_MSG(CATPEchelle_A2)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
	LONG m_prev_i; 
	LONG m_prev_j;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPECHELLE_A2_H__C0620813_9F2D_428B_9E16_0183A621CE3D__INCLUDED_)
