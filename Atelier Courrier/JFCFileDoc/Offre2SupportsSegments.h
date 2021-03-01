#pragma once

// control headers
#include "resource.h"
#include "JFCXListBox.h"

// base class header
#include "Offre2SupportsOngletBase.h"

//=============================================================================	
// Boîte de dialogue Offre2SupportsSegments
//=============================================================================	
class Offre2SupportsSegments : public COffre2SupportsOngletBase
{
	DECLARE_DYNAMIC(Offre2SupportsSegments)

public:

	Offre2SupportsSegments();   // constructeur standard
	virtual ~Offre2SupportsSegments();

	// retourne true si l'état de la sélection est correct
	virtual bool IsSelectionOk() const;

	// remplit la partie de l'offre correspondant à l'onglet
	virtual void FillOffre(JATPOffre * pOffre);

	// Données de boîte de dialogue
	enum { IDD = IDD_FD_DLG_OFFRE2_TITRES_SEL_SEGMENTS };

	void FillSegments();

	// appelé pour la mise à jour
	virtual void UpdateContent();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnLbnSelchangeLbSelSegments();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	CJFCXListBox m_SegmentsList;
};
