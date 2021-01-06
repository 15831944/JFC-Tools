// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPEchelle_Metriques.h"
#include "ATPEchelle_Grandeurs.h"
#include "ATPEchelleQP_D2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPEchelleQP_D2::CATPEchelleQP_D2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPEchelleQP_D2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;
	//m_grandeurs.SetDocument(pATPDocCreuset); 
	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPEchelleQP_D2::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPEchelleQP_D2::~CATPEchelleQP_D2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPEchelleQP_D2, JFCGrid)
	//{{AFX_MSG_MAP(CATPEchelleQP_D2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPEchelleQP_D2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on récupère l'identifiant du Support
	const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);
	
	// on recherche l'échelle
	m_pATPDocCreuset->m_ITREchelle.MoveTo(IdSupport);

	m_pATPDocCreuset->m_listFormatsQP.MoveFirst();
	for(JInt32 inc = 0; inc < i ; inc++)
		m_pATPDocCreuset->m_listFormatsQP.MoveNext();

	// Récup Table des formats
	
	int idFormat = m_pATPDocCreuset->m_listFormatsQP.GetItem();
	const JSRCPresse  & srcpresse = JSRCPresse::GetInstance();
	srcpresse.m_TBLFormats.MoveTo(idFormat);
	// Récup item correspondant à l'indice
	CString str;
	//if(srcpresse.m_TBLFormats.IsValid())
	//{
	//	IElem *pElem = srcpresse.m_TBLFormats.GetItem();
	//	if(pElem)
	//		str = pElem->GetLabel().AsJCharPtr();  
	//}
	double Tarif = 0.0;

	CRegroupements * pReg = CRegroupements::GetInstance();
	CTarifs * pTarifs = CTarifs::GetInstance();

	// Récup id support
	IElem *pElem = pReg->GetIElemByID(IdSupport);

	if(pElem && pTarifs)
	{

		JDate  dateClick(m_pATPDocCreuset->m_KEYEtude.GetDateD());
		JDate  dateLimiteMin(m_pATPDocCreuset->m_KEYEtude.GetDateD());
		JDate  dateLimiteMax(m_pATPDocCreuset->m_KEYEtude.GetDateF());
		JDate  dateExact;
		
		JUnt32 Duree;
		JUnt32 IdTitre;
		if(pElem->IsTitreApparie())
			IdTitre = pElem->AsTitreApparie()->GetIDTitre();
		else
			IdTitre = IdSupport;

		JUnt32 utarif = 0;

		// Recherche nouveau tarif par date, en testant les tarifs manuels s'ils existent
		pTarifs->GetNextTarifByDateNew( IdTitre, idFormat, m_pATPDocCreuset->m_MapTarifManuel, dateClick, dateLimiteMin, dateLimiteMax, dateExact, Duree, utarif);
		Tarif = utarif;
	}


	if(Tarif != 0.0)
	{
		str.Format("%.0f",Tarif); 
		CString Fmt = _T("### ### ### ### ### € ");
		CFormatStr::FormatNum(Fmt, str);
	}
	else
	{
		CBrush bruch;
		bruch.CreateSolidBrush( CATPColors::GetColorLight(CATPColors::GREY_MOUSE) );
		RECT Rect; Rect.left = rect.left - 3; Rect.right = rect.right + 3; Rect.top = rect.top - 2; Rect.bottom = rect.bottom + 1;
		dc.FillRect(&Rect, &bruch);
	}

	JInt32 substFormat;
	m_pATPDocCreuset->SubstitutFormat(IdSupport, substFormat);
	if(substFormat == idFormat)
	{
		CBrush bruch;
		bruch.CreateSolidBrush( CATPColors::GetColorSelect(CATPColors::COLORCREUSET) );
		//RECT Rect; Rect.left = rect.left - 3; Rect.right = rect.right + 3; Rect.top = rect.top - 2; Rect.bottom = rect.bottom + 1;
		dc.FillRect(&rect, &bruch);
		dc.SetTextColor(RGB(255,255,255));
	}

	dc.DrawText(str, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);


}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPEchelleQP_D2::OnLButtonDown(LONG i, LONG j)
{
	if(m_pATPDocCreuset)
	{
		// on récupère l'identifiant du Support
		const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);
		
		// on recherche l'échelle
		m_pATPDocCreuset->m_ITREchelle.MoveTo(IdSupport);

		m_pATPDocCreuset->m_listFormatsQP.MoveFirst();
		for(JInt32 inc = 0; inc < i ; inc++)
			m_pATPDocCreuset->m_listFormatsQP.MoveNext();

		// Récup Table des formats
		
		int idFormat = m_pATPDocCreuset->m_listFormatsQP.GetItem();
		CRegroupements * pReg = CRegroupements::GetInstance();
		CTarifs * pTarifs = CTarifs::GetInstance();

		// Récup id support
		IElem *pElem = pReg->GetIElemByID(IdSupport);

		if(pElem && pTarifs)
		{

			JDate  dateClick(m_pATPDocCreuset->m_KEYEtude.GetDateD());
			JDate  dateLimiteMin(m_pATPDocCreuset->m_KEYEtude.GetDateD());
			JDate  dateLimiteMax(m_pATPDocCreuset->m_KEYEtude.GetDateF());
			JDate  dateExact;
			
			JUnt32 Duree;
			JUnt32 IdTitre;
			if(pElem->IsTitreApparie())
				IdTitre = pElem->AsTitreApparie()->GetIDTitre();
			else
				IdTitre = IdSupport;

			JUnt32 utarif = 0;
			
			// Recherche nouveau tarif par date, en testant les tarifs manuels s'ils existent
			RETTRF ret = pTarifs->GetNextTarifByDateNew( IdTitre, idFormat, m_pATPDocCreuset->m_MapTarifManuel, dateClick, dateLimiteMin, dateLimiteMax, dateExact, Duree, utarif);

			if(ret!= NOTRF && ret != TRFOUT && utarif != 0)
			{
				m_pATPDocCreuset->SetSubstitutFormat(IdSupport,idFormat);
				m_pATPDocCreuset->UpdateAllViews(UPDATE_TARIFS_QP);
			}
		}
			//this->OnUpdate();
	}
}

JVoid CATPEchelleQP_D2::OnRButtonDown(LONG i, LONG j)
{
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPEchelleQP_D2::OnUpdate()
{
	// on initialise le nombre d'éléments
	LONG cx = 0;
	LONG cy = 0;

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{

		// on récupère le nombre d'éléments
		//cx = m_grandeurs.GetNbGrandeurs();
		cx = m_pATPDocCreuset->m_listFormatsQP.GetCount();
		cy = m_pATPDocCreuset->m_EchelleSupports.GetCount();
	}

	// on met à jour le composant
	this->SetHorzCount(cx, FALSE);
	this->SetVertCount(cy, TRUE);
}

/////////////////////////////////////
// CATPEchelleQP_D2 message handlers

void CATPEchelleQP_D2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPEchelle_Metriques::CX_Dx(), TRUE, FALSE);
	this->SetVertItemSize(CATPEchelle_Metriques::CY_x2(), TRUE, FALSE);
}
