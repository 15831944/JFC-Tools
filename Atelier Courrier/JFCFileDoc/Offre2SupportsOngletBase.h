#pragma once

#include "resource.h"
#include <vector>
#include <list>
#include "afxwin.h"
#include "xlistbox.h"

class COffre2Supports;
class JATPOffre;
class JSRCTables;
class JSource;
class COffre2MainDialog;

//=============================================================================	
// Interface COffre2SupportsOngletBase
//=============================================================================	
class COffre2SupportsOngletBase : public CDialog
{
	DECLARE_DYNAMIC(COffre2SupportsOngletBase)

public:
	COffre2SupportsOngletBase(UINT nIDTemplate);   // constructeur standard
	virtual ~COffre2SupportsOngletBase();

	// assigne le pointeur vers l'onglet 'titres' dialogue de l'Offre 
	void SetData (COffre2Supports * pOngletTitres, COffre2MainDialog * pDialogOffre);

	// à redefinir dans les onglets (retourne true si l'état de la sélection est correct)
	virtual bool IsSelectionOk() const = 0;

	// remplit la partie de l'offre correspondant à l'onglet
	virtual void FillOffre(JATPOffre * pOffre) = 0;

	// appelé pour la mise à jour
	virtual void UpdateContent() = 0;

protected:
	virtual void OnOK();
	virtual void OnCancel();

	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()

	COffre2Supports *	m_pOngletTitres;
	COffre2MainDialog *	m_pDialogOffre;
};