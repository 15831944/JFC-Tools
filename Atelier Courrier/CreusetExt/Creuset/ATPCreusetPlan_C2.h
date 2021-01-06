#if !defined(AFX_ATPCreusetPlan_C2_H__0AF4083E_A4A9_4F7B_B144_8919950208B4__INCLUDED_)
#define AFX_ATPCreusetPlan_C2_H__0AF4083E_A4A9_4F7B_B144_8919950208B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les d�finitions n�cessaires
#include "JATPDocCreuset.h"
#include "JFCWaveSelect.h"
#include "JFCWave.h"
#include "Insertion.h"


class CATPCreusetPlan;

/////////////////////////////////////////////////////////////////////////////
// CATPCreusetPlan_C2

class CATPCreusetPlan_C2 : public JFCWaveSelect
{
public:
	// le constructeur
	CATPCreusetPlan_C2();

	// la fonction pour d�finir le document
	virtual JVoid SetDocument(JATPDocCreuset* pATPDocCreuset);

	// la fonction pour mettre � jour le composant
	virtual JVoid Update();

	// le destructeur
	virtual ~CATPCreusetPlan_C2();

	JVoid SetParent(CATPCreusetPlan * pATPCreusetPlan);

protected:
	// la fonction pour r�cup�rer l'�tat des �l�ments
	LONG  OnGetItemState(LONG indice, JDate date);

	JBool SetInfoInsertion(CInsertion & Insertion, JDate & DateF);

	// les fonctions pour capter les clicks de la souris
	JVoid OnLButtonDown(LONG indice, JDate date);
	JVoid OnLButtonUp(LONG indice, JDate date);
	// JVoid OnRButtonDown(LONG indice, JDate date);

	// la fonction pour capter les d�placements de la souris
	JVoid OnMouseMove(LONG indice, JDate date);

protected:
	// la fonction pour appliquer les mises � jour
	JVoid OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPCreusetPlan_C2)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPCreusetPlan_C2)
	afx_msg void OnCrPopupgrilleItem0();
	afx_msg void OnCrPopupgrilleItem1();
    afx_msg void OnCrPopupgrilleItem2();
	//}}AFX_MSG

	afx_msg virtual void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	JATPDocCreuset* m_pATPDocCreuset; // le document
	CATPCreusetPlan * m_pATPCreusetPlan;

	LONG m_indCplEdited;
	JDate m_dateCplMin;
	JDate m_dateCplMax;
	CInsertion * m_insertCpl;

	void SetInsertStatus(JATPDATInsertion::InsertStatus stt);

	// Ajout de l'insertion sur support indic� et � la date en cours, retourne date fin insertion
	JBool AjouterInsertion(LONG indice, JDate date, JDate &DateFinIns, short StateCtrl = 0, short StateShift = 0);

	// Ajout insertion en s�rie (N fois) sur support indic� et � la date en cours
	JBool AjouterNInsertion(LONG indice, JDate DateDeb, JDate &DateFin, JUnt32 NbInsertions);

};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPCreusetPlan_C2_H__0AF4083E_A4A9_4F7B_B144_8919950208B4__INCLUDED_)
