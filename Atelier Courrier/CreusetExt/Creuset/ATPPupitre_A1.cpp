// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPPupitre_A1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPPupitre_A1::CATPPupitre_A1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPPupitre_A1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPPupitre_A1::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPPupitre_A1::~CATPPupitre_A1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPPupitre_A1, JFCTitle)
	//{{AFX_MSG_MAP(CATPPupitre_A1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

//////////////////////////////////////
// la fonction pour dessiner l'élément

JVoid CATPPupitre_A1::OnDrawItem(CDC & dc, RECT rect)
{
	
	// on écrit le libellé de la source d'audience
	if (m_pATPDocCreuset && m_pATPDocCreuset->IsValid() && m_pATPDocCreuset->m_IdSrcAudienceSel)
	{
		// Récup Table
		const JSRCTables  & srcTables	= JSRCTables::GetInstance();

		// Récup la source audience courante
		JUnt32 SrcID			= m_pATPDocCreuset->m_IdSrcAudienceSel;  

		// Récup le nouveau source
		const JSource * pt		= srcTables.m_TBLSources.GetItem(SrcID);

		// Recuperer dans table source
		
		CRect rc(rect);
		rc.DeflateRect(5,0);

		// Font spécifique (bold et taille moyenne 12)
		LOGFONT lf;
		dc.GetCurrentFont()->GetLogFont(&lf);
		lf.lfWeight = FW_BOLD;
		lf.lfHeight = -16;
		CFont fontBold;
		fontBold.CreateFontIndirect(&lf);

		// Sauve ancienne font
		CFont* pOldFont = dc.SelectObject(&fontBold);

		// Libellé de la source
		dc.DrawText(pt->m_Libelle.AsJCharPtr(), &rc, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE | DT_WORD_ELLIPSIS );
	}
	else
	{
		// Font spécifique (bold et taille moyenne 12)
		LOGFONT lf;
		dc.GetCurrentFont()->GetLogFont(&lf);
		lf.lfWeight = FW_BOLD;
		CFont fontBold;
		fontBold.CreateFontIndirect(&lf);

		// Sauve ancienne font
		CFont* pOldFont = dc.SelectObject(&fontBold);

		dc.DrawText(_T("Terrain"), &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
	}
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPPupitre_A1::OnUpdate()
{
	// on redessine la fenêtre
	this->InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////
// CATPPupitre_A1 message handlers

void CATPPupitre_A1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCTitle::PreSubclassWindow();
}
