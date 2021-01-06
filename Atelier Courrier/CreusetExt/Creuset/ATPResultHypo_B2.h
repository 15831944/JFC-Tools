#if !defined(AFX_ATPResultHypo_B2_H__9F23EF14_555B_4C5B_8782_FA70CACBDFD9__INCLUDED_)
#define AFX_ATPResultHypo_B2_H__9F23EF14_555B_4C5B_8782_FA70CACBDFD9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "ATPResult_GrandeursHypo.h"
#include "JFCGridMultiCell.h"
/////////////////////////////////////////////////////////////////////////////
// CATPResultHypo_B2 = Resultat support pour chaque hypothese

class CATPResultHypo_B2 : public JFCGridMultiCell
{
public:
	// le constructeur
	CATPResultHypo_B2();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPResultHypo_B2();

protected:
	// la fonction pour dessiner l'intérieur des cellules
	JVoid OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG ii, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);

	// les fonctions pour capter les clicks de la souris
	JVoid OnLButtonDown(LONG i, LONG j);
	JVoid OnRButtonDown(LONG i, LONG j);

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPResultHypo_B2)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPResultHypo_B2)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document

	CATPResult_GrandeursHypo m_ResultGrandeurs;

	int ConvertICellToFormat(int iCell);
	JMap<int,JInt32> m_mapICellFormat;
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPResultHypo_B2_H__9F23EF14_555B_4C5B_8782_FA70CACBDFD9__INCLUDED_)
