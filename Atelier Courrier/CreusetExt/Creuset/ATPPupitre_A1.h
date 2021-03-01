#if !defined(AFX_ATPPUPITRE_A1_H__D8E1486C_D3BC_4CA1_9724_C49CFC47C1B4__INCLUDED_)
#define AFX_ATPPUPITRE_A1_H__D8E1486C_D3BC_4CA1_9724_C49CFC47C1B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les d�finitions n�cessaires
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPPupitre_A1

class CATPPupitre_A1 : public JFCTitle
{
public:
	// le constructeur
	CATPPupitre_A1();

	// la fonction pour d�finir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre � jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPPupitre_A1();

protected:
	// la fonction pour dessiner l'�l�ment
	JVoid OnDrawItem(CDC & dc, RECT rect);

protected:
	// la fonction pour appliquer les mises � jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPPupitre_A1)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPPupitre_A1)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPPUPITRE_A1_H__D8E1486C_D3BC_4CA1_9724_C49CFC47C1B4__INCLUDED_)
