#pragma once

// STL Headers
#include <vector>
#include <list>

// control headers
#include "resource.h"
#include "JFCXListbox.h"

// base class header
#include "Offre2SupportsOngletBase.h"

//=============================================================================	
// forward declarations
class JSRCTables;
class JSource;

//=============================================================================	
// Bo�te de dialogue Offre2SupportsSource
//=============================================================================	
class Offre2SupportsSource : public COffre2SupportsOngletBase
{
	DECLARE_DYNAMIC(Offre2SupportsSource)

public:

	Offre2SupportsSource();
	virtual ~Offre2SupportsSource();

	// retourne true si l'�tat de la s�lection est correct
	virtual bool IsSelectionOk() const;

	// remplit la partie de l'offre correspondant � l'onglet
	virtual void FillOffre(JATPOffre * pOffre);

	// appel� pour la mise � jour
	virtual void UpdateContent();

	void FillSources();

	// Donn�es de bo�te de dialogue
	enum { IDD = IDD_FD_DLG_OFFRE2_TITRES_SEL_SOURCE };

protected:

	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnCbnSelchangeCbSelSource();
	afx_msg void OnLbnSelchangeLbSelTerrains();

	// data
	const JSRCTables *				m_psrcTables;
	std::vector<const JSource*>		m_Sources;

	// controls
	CComboBox						m_SourcesCombo;
	CJFCXListBox					m_TerrainsList;

	// ui management
	bool							m_FirstUpdate;

	bool AffichageMsgCasParticuliersTerrains(const int IdxSelTerrains);
	void CheckAllSupports(std::list<JUnt32> & lAddedTerrains);
	bool CheckOneSupport (JUnt32 IdSupport, const std::list<JUnt32> & lAddedTerrains, CString &txt);

	void FillTerrains();
};
