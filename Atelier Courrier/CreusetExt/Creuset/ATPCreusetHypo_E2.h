#if !defined(AFX_ATPCreusetHypo_E2_H__0AF4083E_A4A9_4F7B_B144_8919950208B4__INCLUDED_)
#define AFX_ATPCreusetHypo_E2_H__0AF4083E_A4A9_4F7B_B144_8919950208B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les d�finitions n�cessaires
#include "JATPDocCreuset.h"
#include "JFCWave.h"
#include "Insertion.h"

class CATPCreusetHypo;

/////////////////////////////////////////////////////////////////////////////
// CATPCreusetHypo_E2

class CATPCreusetHypo_E2 : public JFCGrid
{
public:
	// le constructeur
	CATPCreusetHypo_E2();

	// la fonction pour d�finir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);
	virtual JVoid SetParent(CATPCreusetHypo* pATPCreusetHypo);

	// la fonction pour mettre � jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPCreusetHypo_E2();

protected:
	// la fonction pour r�cup�rer l'�tat des �l�ments
	LONG OnGetItemState(JUnt32 IndHyp, JUnt32 IndSupport);


protected:
	// la fonction pour dessiner l'int�rieur des cellules
	JVoid OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);

protected:
	// la fonction pour appliquer les mises � jour
	JVoid OnUpdate();

	// Ajout d'un support au plateau pour synchro Plan Calendaire <> Plan global
	JBool AjoutSupportPlateau(CKeySupport KeySup);

protected:
	//{{AFX_VIRTUAL(CATPCreusetHypo_E2)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPCreusetHypo_E2)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

protected:
	JATPDocCreuset* m_pATPDocCreuset; // le document
	CATPCreusetHypo* m_pATPCreusetHypo; // le parent

protected:
	JInt32 OnGetNbInsertions(JUnt32 IdSupport,JUnt32 IdHypothese,JBool &SuppPossible) const;

};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetHypo_E2_H__0AF4083E_A4A9_4F7B_B144_8919950208B4__INCLUDED_)
