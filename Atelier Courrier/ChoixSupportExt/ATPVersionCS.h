//
// Fichier: ATPVersionCS.h
// Date:    09/04/2004
// Gestion Information Version Choix Supports
//
#pragma once

// Boîte de dialogue CATPVersionCS

class CATPVersionCS : public CDialog
{
	DECLARE_DYNAMIC(CATPVersionCS)

public:
	CATPVersionCS(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CATPVersionCS();

// Données de boîte de dialogue
	enum { IDD = IDD_CS_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
};
