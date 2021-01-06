#pragma once

// on inclut les définitions nécessaires
#include "JATPDocCreuset.h"
#include "ATPFramePlan.h"
#include "ATPFrameHypo.h"
#include "ATPFrameQuickPlan.h"

class AFX_EXT_CLASS CCreuset
{
public:
	CCreuset(void);
	~CCreuset(void);

	void Open(CString path);
	void Close();

	// Fermeture ou non de la boite de dialogue choix supports
	JBool CanClose();

	void ShowPlan();
	void HidePlan();
	BOOL GetPlanState();

	void ShowHypo();
	void HideHypo();
	BOOL GetHypoState();

	void ShowQuickPlan();
	void HideQuickPlan();
	BOOL GetQuickPlanState();

	BOOL ForwardMessage(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);


private:
// Les 2 frames Plan et Hypo
	CATPFramePlan m_FramePlan;
	CATPFrameHypo m_FrameHypo;

	CATPFrameQuickPlan m_FrameQuickPlan;


	JATPDocCreuset m_ATPDocCreuset; // le document
	// chaine d'initialisation du GDI+
	ULONG_PTR           m_gdiplusToken;
};
