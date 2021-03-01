#pragma once

// control headers
#include "resource.h"
#include "XListBox.h"

// base class header
#include "Offre2SupportsOngletBase.h"

//=============================================================================	
// Boîte de dialogue Offre2SupportsTypesPoids
//=============================================================================	
class Offre2SupportsTypesPoids : public COffre2SupportsOngletBase
{
	DECLARE_DYNAMIC(Offre2SupportsTypesPoids)

public:

	Offre2SupportsTypesPoids();   // constructeur standard
	virtual ~Offre2SupportsTypesPoids();

	// retourne true si l'état de la sélection est correct
	virtual bool IsSelectionOk() const;

	// remplit la partie de l'offre correspondant à l'onglet
	virtual void FillOffre(JATPOffre * pOffre);

	// Données de boîte de dialogue
	enum { IDD = IDD_FD_DLG_OFFRE2_TITRES_SEL_TYPESPOIDS };

	void FillTypesPoids();

	// appelé pour la mise à jour
	virtual void UpdateContent();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	CXListBox m_TypesPoidsList;
	afx_msg void OnLbnSelchangeLbSelTypespoids();
	virtual BOOL OnInitDialog();
};