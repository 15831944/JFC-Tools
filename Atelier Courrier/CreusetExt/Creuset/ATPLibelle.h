#if !defined(AFX_ATPLIBELLE_H__4089078A_E67C_4F5B_97EC_64BAA460B132__INCLUDED_)
#define AFX_ATPLIBELLE_H__4089078A_E67C_4F5B_97EC_64BAA460B132__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// on inclut les définitions nécessaires
#include "Resource.h"

//JFCControls
#include <Label.h>
#include <BtnST.h>

/////////////////////////////////////////////////////////////////////////////
// CATPLibelle

class CATPLibelle : public CDialog
{
public:
	// le constructeur
	CATPLibelle(JLabel TitreFen,CWnd* pParent = NULL);
	CATPLibelle(UINT strId, CWnd* pParent = NULL);

	// les fonctions pour manipuler le libellé
	JVoid  SetLibelle(const JLabel & Libelle);
	JLabel GetLibelle() const;

protected:
	//{{AFX_DATA(CATPLibelle)
	enum { IDD = IDD_CR_ATPLIBELLE };
	CEdit m_EditL;
	//}}AFX_DATA

protected:
	// la fonction pour mettre à jour le libellé
	BOOL OnUpdateLibelle();

protected:
	// la fonction pour mettre à jour la boîte
	VOID OnUpdate();

protected:
	//{{AFX_VIRTUAL(CATPLibelle)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CATPLibelle)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeLibelle();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	JLabel m_TitreFen;
	JLabel m_Libelle;

///Elements de la boite
	CLabel m_Backgnd;
	CLabel m_Ttl;
	CButtonST m_btnOk;
	CButtonST m_btnCancel;

///init de la boite
	void SetupControls();
	void SetColors();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ATPLIBELLE_H__4089078A_E67C_4F5B_97EC_64BAA460B132__INCLUDED_)
