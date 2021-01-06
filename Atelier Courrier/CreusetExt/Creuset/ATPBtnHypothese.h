#if !defined(AFX_ATPBtnHypothese_H__8A71EDCC_44A5_4903_AC80_E5B7CA170778__INCLUDED_)
#define AFX_ATPBtnHypothese_H__8A71EDCC_44A5_4903_AC80_E5B7CA170778__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les d�finitions n�cessaires
#include "JFCButtonList.h"
#include "JATPDocCreuset.h"

/////////////////////////////////////////////////////////////////////////////
// CATPBtnHypothese

class CATPBtnHypothese : public JFCButtonList 
{
public:
	// le constructeur
	CATPBtnHypothese();

	// la fonction pour d�finir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre � jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPBtnHypothese();

protected:
	// la fonction pour r�cup�rer l'�tat des �l�ments
	virtual void OnGetItemParams(LONG i, LONG j, BtnParams & Params);
	virtual void OnButtonSel(LONG i, LONG j);
	virtual bool OnBeginDrawing(CDC & dc);
	virtual void OnLedSel(LONG i, LONG j);

protected:
	// la fonction pour appliquer les mises � jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPBtnHypothese)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPBtnHypothese)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPBtnHypothese_H__8A71EDCC_44A5_4903_AC80_E5B7CA170778__INCLUDED_)
