#pragma once

// control headers
#include "resource.h"
#include "JFCXListBox.h"

// base class header
#include "Offre2SupportsOngletBase.h"

//=============================================================================	
// Bo�te de dialogue Offre2SupportsSegments
//=============================================================================	
class Offre2SupportsSegments : public COffre2SupportsOngletBase
{
	DECLARE_DYNAMIC(Offre2SupportsSegments)

public:

	Offre2SupportsSegments();   // constructeur standard
	virtual ~Offre2SupportsSegments();

	// retourne true si l'�tat de la s�lection est correct
	virtual bool IsSelectionOk() const;

	// remplit la partie de l'offre correspondant � l'onglet
	virtual void FillOffre(JATPOffre * pOffre);

	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_FD_DLG_OFFRE2_TITRES_SEL_SEGMENTS };

	void FillSegments();

	// appel� pour la mise � jour
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
