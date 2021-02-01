#if !defined(AFX_ATPCreusetHypo_E3_H__F55C2542_8361_4923_825F_2D90ACFCC1D5__INCLUDED_)
#define AFX_ATPCreusetHypo_E3_H__F55C2542_8361_4923_825F_2D90ACFCC1D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les d�finitions n�cessaires
#include "JATPDocCreuset.h"
#include "JFCWave.h"

/////////////////////////////////////////////////////////////////////////////
// CATPCreusetHypo_E3

class CATPCreusetHypo_E3 : public JFCGrid
{
public:
	// le constructeur
	CATPCreusetHypo_E3();

	// la fonction pour d�finir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre � jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPCreusetHypo_E3();

protected:
	// la fonction pour appliquer les mises � jour
	JVoid OnUpdate();

	// la fonction pour dessiner l'int�rieur des cellules
	JVoid OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);


protected:
	//{{AFX_VIRTUAL(CATPCreusetHypo_E3)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPCreusetHypo_E3)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetHypo_E3_H__F55C2542_8361_4923_825F_2D90ACFCC1D5__INCLUDED_)