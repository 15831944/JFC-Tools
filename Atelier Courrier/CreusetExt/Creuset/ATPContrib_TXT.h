#if !defined(AFX_ATPContrib_TXT_H__1D223155_A483_401C_8BA5_F0757A756821__INCLUDED_)
#define AFX_ATPContrib_TXT_H__1D223155_A483_401C_8BA5_F0757A756821__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPContrib_TXT

class CATPContrib_TXT : public JFCGrid
{
public:
	// le constructeur
	CATPContrib_TXT();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPContrib_TXT();

protected:
	// la fonction pour dessiner l'intérieur des cellules
	JVoid OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD);

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPContrib_TXT)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPContrib_TXT)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPContrib_TXT_H__1D223155_A483_401C_8BA5_F0757A756821__INCLUDED_)
