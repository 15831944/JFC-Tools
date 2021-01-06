#if !defined(AFX_ATPPERIODE_H__17F708BD_848E_4568_9B1C_87BA25504BF1__INCLUDED_)
#define AFX_ATPPERIODE_H__17F708BD_848E_4568_9B1C_87BA25504BF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "Resource.h"

//JFCControls
#include <Label.h>
#include <BtnST.h>

/////////////////////////////////////////////////////////////////////////////
// CATPPeriode

class CATPPeriode : public CDialog
{
public:
	// le constructeur
	CATPPeriode(CWnd* pParent = NULL);

	// les fonctions pour manipuler la date de début
	JVoid SetDateD(JDate DateD);
	JDate GetDateD() const;

	// les fonctions pour manipuler la date de fin
	JVoid SetDateF(JDate DateF);
	JDate GetDateF() const;

	void SetDateDReadOnly(bool bRead = true);

protected:
	//{{AFX_DATA(CATPPeriode)
	enum { IDD = IDD_CR_ATPPERIODE };
	CDateTimeCtrl   m_CtrlDateD;
	CDateTimeCtrl   m_CtrlDateF;
	CSpinButtonCtrl m_SpinS;
	CSpinButtonCtrl m_SpinJ;
	//}}AFX_DATA

protected:
	// la fonction pour calculer la durée maximum
	JInt32 OnGetMaxDuree(JDate Date) const;

protected:
	// la fonction pour mettre à jour la boîte
	VOID OnUpdateSpin();
	VOID OnUpdateDateFin();

protected:
	//{{AFX_VIRTUAL(CATPPeriode)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPPeriode)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChangeDateD(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeDateF(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeSpinS(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeSpinJ(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JInt32 m_Duree; // la durée en jours

private:
	JDate m_DateND; // la nouvelle date de début
	JDate m_DateNF; // la nouvelle date de fin

private:
	JDate m_DateD; // la date de début
	JDate m_DateF; // la date de fin

///Elements de la boite
	CLabel m_Backgnd;
	CLabel m_Ttl;
	CLabel m_Txt1;
	CLabel m_Txt2;
	CLabel m_Txt3;
	CLabel m_Txt4;
	CLabel m_Txt5;
	CButtonST m_btnOk;
	CButtonST m_btnCancel;

	bool m_bDateDReadOnly;

///init de la boite
	void SetupControls();
	void SetColors();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPPERIODE_H__17F708BD_848E_4568_9B1C_87BA25504BF1__INCLUDED_)
