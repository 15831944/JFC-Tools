// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPEchelle_Metriques.h"
#include "ATPEchelle_Grandeurs.h"
#include "ATPEchelleQP_D1.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPEchelleQP_D1::CATPEchelleQP_D1()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPEchelleQP_D1::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	//m_grandeurs.SetDocument(pATPDocCreuset); 

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPEchelleQP_D1::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPEchelleQP_D1::~CATPEchelleQP_D1()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPEchelleQP_D1, JFCGrid)
	//{{AFX_MSG_MAP(CATPEchelleQP_D1)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPEchelleQP_D1::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// on initialise le tri
	JInt32 Sort = 0;

	// on calcule le rectangle
	RECT Rect; Rect.left = rect.left - 3; Rect.right = rect.right + 3; Rect.top = rect.top - 2; Rect.bottom = rect.bottom + 1;

	// on dessine le fond de la cellule
	dc.FillRect(&Rect, &BrushB);
	LONG OldCol = dc.SetTextColor(CATPColors::GetColorWhite());

	CString str;
//////////////

	m_pATPDocCreuset->m_listFormatsQP.MoveFirst();
	for(JInt32 inc = 0; inc < i ; inc++)
		m_pATPDocCreuset->m_listFormatsQP.MoveNext();

	// Récup Table des formats
	const JSRCPresse  & srcpresse = JSRCPresse::GetInstance();
	
	srcpresse.m_TBLFormats.MoveTo(m_pATPDocCreuset->m_listFormatsQP.GetItem());
	// Récup item correspondant à l'indice
	if(srcpresse.m_TBLFormats.IsValid())
	{
		IElem *pElem = srcpresse.m_TBLFormats.GetItem();
		if(pElem)
		//	str = pElem->GetLabel().AsJCharPtr();  
		//RECT calcRect = rect;
		//dc.DrawText(str, &calcRect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE |DT_CALCRECT);
		//if(calcRect.right > rect.right)
			str = srcpresse.m_TBLFormats.GetLabelCourtByID(m_pATPDocCreuset->m_listFormatsQP.GetItem());  

	}
	dc.DrawText(str, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

	CPen pen;
	CPen*  pOldPen  = dc.GetCurrentPen();
	pen.CreatePen(PS_SOLID, 1, CATPColors::GetColorBlack());
	dc.SelectObject(&pen);
	// on teste le tri de la colonne
	if (Sort > 0)
	{
		// on dessine la flèche vers le haut
		dc.MoveTo(Rect.right - 6, Rect.bottom - 6);
		dc.LineTo(Rect.right - 9, Rect.bottom - 3);
		dc.LineTo(Rect.right - 3, Rect.bottom - 3);
		dc.LineTo(Rect.right - 6, Rect.bottom - 6);
	}
	else if (Sort < 0)
	{
		// on dessine la flèche vers le bas
		dc.MoveTo(Rect.right - 6, Rect.bottom - 3);
		dc.LineTo(Rect.right - 9, Rect.bottom - 6);
		dc.LineTo(Rect.right - 3, Rect.bottom - 6);
		dc.LineTo(Rect.right - 6, Rect.bottom - 3);
	}

	// on dessine Un carré
	CRect RectLed;
	RectLed = CRect(Rect.left+3,Rect.top+6,Rect.left+12,Rect.bottom-7);

	//// dessin led (forme carré)
	
	dc.SelectObject(&m_PenA);
	CBrush*  pOldBrush  = dc.SelectObject(&m_BrushD);
	
	dc.BeginPath();
	dc.MoveTo(RectLed.TopLeft());		dc.LineTo(RectLed.right,RectLed.top);
	dc.LineTo(RectLed.BottomRight());	dc.LineTo(RectLed.left,RectLed.bottom);
	dc.LineTo(RectLed.TopLeft());
	dc.EndPath(); dc.FillPath();

	// le cadre carré
	dc.MoveTo(RectLed.TopLeft());		dc.LineTo(RectLed.right,RectLed.top);
	dc.LineTo(RectLed.BottomRight());	dc.LineTo(RectLed.left,RectLed.bottom);
	dc.LineTo(RectLed.TopLeft());

	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
	dc.SetTextColor(OldCol);
}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris
void CATPEchelleQP_D1::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	// on teste si on a des éléments
	if ((m_HorzCount > 0) && (m_VertCount > 0))
	{
		LONG i,j;

		// on récupère l'élément sous le curseur
		if (this->GetIdxAtPos(point.x, point.y, i, j))
		{
			
			CRect rectCase;
			GetPosAtIdx(i,j,rectCase.left,rectCase.top,rectCase.right,rectCase.bottom);

			CRect RectLed;
			RectLed = CRect(rectCase.left+3,rectCase.top+6,rectCase.left+12,rectCase.bottom-7);

			//On teste si on est dans la led
			if(RectLed.PtInRect(point))
			{
				// on effectue le traitement
				this->OnLButtonDown(i, j);
			}
		}
	}
	
	// on appelle le gestionnaire de base
	this->JFCViewport::OnLButtonDown(nFlags, point);
}

JVoid CATPEchelleQP_D1::OnLButtonDown(LONG i, LONG j)
{
	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on vérifie la validité de l'étude
		if (m_pATPDocCreuset->IsValid())
		{
			
			m_pATPDocCreuset->m_listFormatsQP.MoveFirst();
			for(JInt32 inc = 0; inc < i ; inc++)
				m_pATPDocCreuset->m_listFormatsQP.MoveNext();
			int idFormat = m_pATPDocCreuset->m_listFormatsQP.GetItem();

			CRegroupements * pReg = CRegroupements::GetInstance();
			CTarifs * pTarifs = CTarifs::GetInstance();
			if(!pTarifs && !pReg)
				return;

			bool hasSubst = false;
			bool canSubst = false;
			//on boucle sur le supports
			for(int j = 0; j<m_pATPDocCreuset->m_EchelleSupports.GetCount();j++)
			{
				const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);
				IElem *pElem = pReg->GetIElemByID(IdSupport);
				JInt32 idF;
				if(pElem && m_pATPDocCreuset->SubstitutFormat(IdSupport, idF) && idF != idFormat)
				{
					//il y a déja des format substitués
					hasSubst = true;
					
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
						canSubst = true;//on peut modifier cette substitution
				}
			}

			bool b_ecrase = false;
			//si on a une substitution et si on peut l'ecraser on demande l'autorisation
			if(hasSubst && canSubst && AfxMessageBox("Voulez-vous remplacer les formats déjà attribués ?", MB_YESNO|MB_ICONQUESTION) == IDYES)
				b_ecrase = true;// l'utilisateur permet d'ecraser la substitution

			//on boucle sur le supports
			for(int j = 0; j<m_pATPDocCreuset->m_EchelleSupports.GetCount();j++)
			{
				const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);

				// Récup id support
				IElem *pElem = pReg->GetIElemByID(IdSupport);

				JInt32 trf;
				bool hasSubst = m_pATPDocCreuset->SubstitutFormat(IdSupport, trf);
				//on test si on a le droit de substituer
				if(pElem && !(hasSubst==1 && b_ecrase==0))
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
						m_pATPDocCreuset->SetSubstitutFormat(IdSupport,idFormat);
				}
			}


			// on met à jour les vues du document
			m_pATPDocCreuset->UpdateAllViews(UPDATE_TARIFS_QP);
		}
	}
}

JVoid CATPEchelleQP_D1::OnRButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPEchelleQP_D1::OnUpdate()
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
		cy = 1;
	}

	// on met à jour le composant
	this->SetHorzCount(cx, FALSE);
	this->SetVertCount(cy, TRUE);
}

/////////////////////////////////////
// CATPEchelleQP_D1 message handlers

void CATPEchelleQP_D1::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPEchelle_Metriques::CX_Dx(), TRUE, FALSE);
	this->SetVertItemSize(CATPEchelle_Metriques::CY_x1(), TRUE, FALSE);
}
