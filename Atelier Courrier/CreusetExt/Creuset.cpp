#include "StdAfx.h"
#include ".\creuset.h"
#include "ATPMessages.h"

CCreuset::CCreuset(void)
: m_FramePlan(&m_ATPDocCreuset), m_FrameHypo(&m_ATPDocCreuset), m_FrameQuickPlan(&m_ATPDocCreuset)
{
	// CATPColors::SetAppColor(CATPColors::COLORCREUSET, AfxGetApp()->GetProfileInt("Options", "ColorCreuset", CATPColors::TERRE_SIENNE) );

	CATPColors::SetAppColor(CATPColors::COLORCREUSET, AfxGetApp()->GetProfileInt("Options", "ColorCreuset", CATPColors::NEWBLUE) );

	// Initialize GDI+.
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	// TODO // Auto delete des 2 frames (Plan + Hypo)
	m_FramePlan.m_bAutoDelete = FALSE;
	m_FrameHypo.m_bAutoDelete = FALSE;
	m_FrameQuickPlan.m_bAutoDelete = FALSE;

	// on charge les frames à partir des ressources
	m_FramePlan.LoadFrame(IDR_CR_ATPCREUSET, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE | WS_CLIPCHILDREN, NULL);
	m_FrameHypo.LoadFrame(IDR_CR_ATPCREUSET, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE | WS_CLIPCHILDREN, NULL);
	m_FrameQuickPlan.LoadFrame(IDR_CR_ATPCREUSET, WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE | WS_CLIPCHILDREN, NULL);

	// on initialise les frame du document
	m_ATPDocCreuset.AddFrame(&m_FramePlan);
	m_ATPDocCreuset.AddFrame(&m_FrameHypo);
	m_ATPDocCreuset.AddFrame(&m_FrameQuickPlan);
}

CCreuset::~CCreuset(void)
{
	m_FramePlan.DestroyWindow();
	m_FrameHypo.DestroyWindow();
	m_FrameQuickPlan.DestroyWindow();

	//On eteind le GDI+
	GdiplusShutdown(m_gdiplusToken);
	//On sauve la couleur utilisée
	AfxGetApp()->WriteProfileInt("Options","ColorCreuset",CATPColors::GetAppColor(CATPColors::COLORCREUSET));
}



void CCreuset::ShowPlan()
{
	m_ATPDocCreuset.m_AFFCreuset.SetPlanActif(1);
	m_ATPDocCreuset.m_AFFCreuset.SetAffPlan(1);
	m_FramePlan.BringWindowToTop();
	m_ATPDocCreuset.UpdateAllViews(UPDATE_AFF_HYPO);
	m_ATPDocCreuset.UpdateAllViews(UPDATE_AFF_PLAN);
}
void CCreuset::HidePlan()
{
	m_ATPDocCreuset.m_AFFCreuset.SetPlanActif(0);
	m_ATPDocCreuset.m_AFFCreuset.SetAffPlan(0);
	m_ATPDocCreuset.UpdateAllViews(UPDATE_AFF_HYPO);
	m_ATPDocCreuset.UpdateAllViews(UPDATE_AFF_PLAN);
}
BOOL CCreuset::GetPlanState()
{
	return m_ATPDocCreuset.m_AFFCreuset.GetPlanActif();
}

/////////

void CCreuset::ShowHypo()
{
//	m_ATPDocCreuset.m_AFFCreuset.SetPlanActif(0);
//	m_ATPDocCreuset.m_AFFCreuset.SetAffPlan(0);
	m_ATPDocCreuset.m_AFFCreuset.SetHypoActif(1);
	m_ATPDocCreuset.m_AFFCreuset.SetAffHypo(1);
	m_FrameHypo.BringWindowToTop();
	m_ATPDocCreuset.UpdateAllViews(UPDATE_AFF_HYPO);
	m_ATPDocCreuset.UpdateAllViews(UPDATE_AFF_PLAN);
}
void CCreuset::HideHypo()
{
	m_ATPDocCreuset.m_AFFCreuset.SetHypoActif(0);
	m_ATPDocCreuset.m_AFFCreuset.SetAffHypo(0);
	m_ATPDocCreuset.UpdateAllViews(UPDATE_AFF_HYPO);
	m_ATPDocCreuset.UpdateAllViews(UPDATE_AFF_PLAN);
}
BOOL CCreuset::GetHypoState()
{
	return m_ATPDocCreuset.m_AFFCreuset.GetHypoActif();
}

void CCreuset::Open(CString path)
{
	ShowPlan();
	m_FramePlan.OnLoadEtude(m_ATPDocCreuset, path);
}

/////////////////////////////////////////////////////////////////////////////////
// Fermeture d'un dossier analyse offre
void CCreuset::Close()
{
	m_FramePlan.CanClose();
	m_FrameHypo.CanClose();
}

/////////////////////////////////////////////////////////////////////////////////
// Fermeture d'un dossier analyse offre
JBool CCreuset::CanClose()
{
	return (m_FramePlan.CanClose() && m_FrameHypo.CanClose());
}



//Permet de transmettre un message provenant d'un module
BOOL CCreuset::ForwardMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case ATPMSG_TARIF_UPDATED:
		m_ATPDocCreuset.UpdateAllViews(UPDATE_TARIFS);
	}
	return TRUE;
}

void CCreuset::ShowQuickPlan()
{
	m_ATPDocCreuset.m_AFFCreuset.SetAffQuickPlan(true);
	m_ATPDocCreuset.UpdateAllViews(UPDATE_AFF_HYPO);
	m_ATPDocCreuset.UpdateAllViews(UPDATE_AFF_PLAN);
}
void CCreuset::HideQuickPlan()
{
	m_ATPDocCreuset.m_AFFCreuset.SetAffQuickPlan(false);
	m_ATPDocCreuset.UpdateAllViews(UPDATE_AFF_HYPO);
	m_ATPDocCreuset.UpdateAllViews(UPDATE_AFF_PLAN);
}
BOOL CCreuset::GetQuickPlanState()
{
	return m_ATPDocCreuset.m_AFFCreuset.GetAffQuickPlan();
}

