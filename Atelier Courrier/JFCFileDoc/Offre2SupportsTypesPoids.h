#pragma once

// control headers
#include "resource.h"
#include "XListBox.h"

// base class header
#include "Offre2SupportsOngletBase.h"

//=============================================================================	
// Bo�te de dialogue Offre2SupportsTypesPoids
//=============================================================================	
class Offre2SupportsTypesPoids : public COffre2SupportsOngletBase
{
	DECLARE_DYNAMIC(Offre2SupportsTypesPoids)

public:

	Offre2SupportsTypesPoids();   // constructeur standard
	virtual ~Offre2SupportsTypesPoids();

	// retourne true si l'�tat de la s�lection est correct
	virtual bool IsSelectionOk() const;

	// remplit la partie de l'offre correspondant � l'onglet
	virtual void FillOffre(JATPOffre * pOffre);

	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_FD_DLG_OFFRE2_TITRES_SEL_TYPESPOIDS };

	void FillTypesPoids();

	// appel� pour la mise � jour
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