#if !defined(AFX_ATPBtnPlanMaitre_H__8A71EDCC_44A5_4903_AC80_E5B7CA170778__INCLUDED_)
#define AFX_ATPBtnPlanMaitre_H__8A71EDCC_44A5_4903_AC80_E5B7CA170778__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "JFCButtonList.h"
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPBtnPlanMaitre

class CATPBtnPlanMaitre : public JFCButtonList 
{
public:
	// le constructeur
	CATPBtnPlanMaitre();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPBtnPlanMaitre();

protected:
	// la fonction pour récupérer l'état des éléments
	virtual void OnGetItemParams(LONG i, LONG j, BtnParams & Params);
	virtual void OnButtonSel(LONG i, LONG j);
	virtual bool OnBeginDrawing(CDC & dc);
	virtual void OnLedSel(LONG i, LONG j);

protected:
	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPBtnPlanMaitre)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPBtnPlanMaitre)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPBtnPlanMaitre_H__8A71EDCC_44A5_4903_AC80_E5B7CA170778__INCLUDED_)
