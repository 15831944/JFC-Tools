#pragma once
#include "dlganalyseur.h"

class CDlgAdvision : public CDlgAnalyseur
{
	DECLARE_DYNAMIC(CDlgAdvision)

public:

	// Constructeur
	CDlgAdvision(CWnd* pParent = NULL);

	// Destructeur
	virtual ~CDlgAdvision(void);

protected:

	// Positionnement des grandeurs à utiliser
	virtual void SetupGrandeurs();
	// virtual void SetupGrandeursBase();

	// Update Résultats
	virtual void UpdateResultStyles();

    virtual void SetupCrossTable();

	// Suppression des options menu bouton grandeur
	virtual void SuppOptionButGlobal(CMenu *pMenu);

	// Setup spécifique des controles
	virtual void SetResultControls();

	// Traitement spécifique bouton H%
	virtual void ClickedAnButHoriz();

	// Gestion Bouton Global
	virtual void LClickedAnButGlobal();
	virtual void RClickedAnButGlobal();
	virtual void ClickedAnButGlobal();

public:

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
};
