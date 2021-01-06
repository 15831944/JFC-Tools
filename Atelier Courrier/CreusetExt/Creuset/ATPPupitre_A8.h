#pragma once

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "JFCButtonList.h"

/////////////////////////////////////////////////////////////////////////////
// CATPPupitre_A8

class CATPPupitre_A8 : public JFCButtonList
{
public:
	// le constructeur
	CATPPupitre_A8();

	// la fonction pour définir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre à jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPPupitre_A8();

protected:
	// la fonction pour récupérer l'état des éléments
	virtual void OnGetItemParams(LONG i, LONG j, BtnParams & Params);
	virtual void OnButtonSel(LONG i, LONG j);
	virtual void OnLedSel(LONG i, LONG j);
	virtual bool OnBeginDrawing(CDC & dc);

	// la fonction pour appliquer les mises à jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPPupitre_A8)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPPupitre_A8)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
};
