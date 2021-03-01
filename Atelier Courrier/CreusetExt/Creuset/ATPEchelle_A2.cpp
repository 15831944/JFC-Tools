// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "resource.h"
#include "ATPEchelle_Metriques.h"
#include "ATPEchelle_A2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPEchelle_A2::CATPEchelle_A2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPEchelle_A2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPEchelle_A2::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPEchelle_A2::~CATPEchelle_A2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPEchelle_A2, JFCGrid)
	//{{AFX_MSG_MAP(CATPEchelle_A2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'insertions d'un Support
JInt32 CATPEchelle_A2::OnGetNbInsertions(JUnt32 IdSupport) const
{
	// on initialise le nombre d'insertions
	JInt32 NbInsertions = 0;

	// on boucle sur toutes les hypoth�ses
	for (m_pATPDocCreuset->m_TblHypotheses.MoveFirst(); m_pATPDocCreuset->m_TblHypotheses.IsValid(); m_pATPDocCreuset->m_TblHypotheses.MoveNext())
	{
		// on r�cup�re les insertions de l'hypoth�se courante
		CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();

		// on boucle sur toutes les insertions
		for (PLNInsertions.MoveFirst_SFD(); PLNInsertions.IsValid_SFD(); PLNInsertions.MoveNext_SFD())
		{
			// on r�cup�re l'insertion courante
			const CInsertion * pInsertion = PLNInsertions.GetItem_SFD();

			// on met � jour le compteur
			if (pInsertion->m_IdSupport == IdSupport)
				NbInsertions += 1;
		}
	}

	// on renvoie le nombre d'insertions
	return (NbInsertions);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'insertions d'un Support sur la p�riode d'action uniquement
// (sur toutes les hypoth�ses)
JInt32 CATPEchelle_A2::OnGetNbInsertionsPA(JUnt32 IdSupport) const
{
	// on initialise le nombre d'insertions
	JInt32 NbInsertions = 0;

	// on boucle sur toutes les hypoth�ses
	for (m_pATPDocCreuset->m_TblHypotheses.MoveFirst(); m_pATPDocCreuset->m_TblHypotheses.IsValid(); m_pATPDocCreuset->m_TblHypotheses.MoveNext())
	{
		// on r�cup�re les insertions de l'hypoth�se courante
		CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();

		if (PLNInsertions.GetCount())
		{
			// on cr�e la cl� de l'insertion
			CInsKey Key;
			Key.m_IdSupport 	=   IdSupport;
			Key.m_IdParent		=	0; 
			Key.m_DateD			=	m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
			Key.m_IdFormat		=	0;

			// on cherche insertion � partir du d�but p�riode �tude (si mode hypoth�ses)
			if (!m_pATPDocCreuset->m_AFFPlanEtendu)
			{
				if(PLNInsertions.FindPAInsertions(Key,
												m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
  												m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF()))
				{
					// 1 insertion trouv� pour ce support
					NbInsertions++;
				}
			}
		}
	}

	// on renvoie le nombre d'insertions
	return (NbInsertions);
}


////////////////////////////////////////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'insertions d'un Support sur toute l'�tude
JInt32 CATPEchelle_A2::OnGetNbInsertionsEtude(JUnt32 IdSupport) const
{
	// on initialise le nombre d'insertions
	JInt32 NbInsertions = 0;

	// Voir si support sur ce plateau avec des insertions en dehors p�riode action
	if (m_pATPDocCreuset->CopyMasterPlanToHypothese(0))
	{
		m_pATPDocCreuset->m_TblHypotheses.MoveTo(0);

		// on v�rifie la validit� de l'hypoth�se
		if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
		{
			// on r�cup�re le plan de l'hypoth�se courante
			CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();

			if (PLNInsertions.GetCount())
			{
				// on cr�e la cl� de l'insertion
				CInsKey Key;
				Key.m_IdSupport 	=   IdSupport;
				Key.m_IdParent		=	0; 
				Key.m_DateD			=	m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
				Key.m_IdFormat		=	0;

				// on cherche insertion � partir du d�but p�riode �tude (si mode hypoth�ses)
				if (!m_pATPDocCreuset->m_AFFPlanEtendu)
				{
					if(PLNInsertions.FindPAInsertions(Key,
													m_pATPDocCreuset->m_KEYEtude.GetDateD(),
  													m_pATPDocCreuset->m_KEYEtude.GetDateF()))
					{
						// 1 insertion trouv� pour ce support
						NbInsertions++;
					}
				}
			}
		}
	}

	return NbInsertions;
}

/////////////////////////////////////////////////////
// la fonction pour dessiner l'int�rieur des cellules

JVoid CATPEchelle_A2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{

	// on r�cup�re l'identifiant du Support
	const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);
	CKeySupport KeySup;
	KeySup.m_IdSupport = IdSupport;

	// on s�lectionne le pinceau
	CBrush* pOldBrush = dc.SelectObject(&BrushB);

	// on calcule la position
	LONG x1 = rect.left + 4; 
	LONG y1 = ((rect.bottom - rect.top - 8) / 2) + rect.top - 1;

	// Index dans plateau �tendu
	JInt32 InxSuppPlateauEtendu = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.FindItem(KeySup);
	
	// Index dans plateau hypoth�ses
	JInt32 InxSuppPlateauHyp = m_pATPDocCreuset->m_PlateauSupports.FindItem(KeySup);

	bool bExistHorsPeriode = false;
	bool bExistDansPeriode = false;
	for(m_pATPDocCreuset->m_TblHypotheses.MoveFirst(); m_pATPDocCreuset->m_TblHypotheses.IsValid(); m_pATPDocCreuset->m_TblHypotheses.MoveNext())
	{
		// on cr�e la cl� de l'insertion
		CInsKey Key;
		Key.m_IdSupport 	=   IdSupport;
		Key.m_IdParent		=	0; 
		Key.m_DateD			=	m_pATPDocCreuset->m_KEYEtude.GetDateD();
		Key.m_IdFormat		=	0;

		// on cherche insertion � partir du d�but p�riode �tude (si mode hypoth�ses)
		//if (!m_pATPDocCreuset->m_AFFPlanEtendu)
		{
			// si appartient au plateau �tendu et insertion avant ou apr�s p�riode action
			if(	m_pATPDocCreuset->m_TblHypotheses.GetItem().FindPAInsertions(Key,
														        m_pATPDocCreuset->m_KEYEtude.GetDateD(),
  														        m_pATPDocCreuset->m_KEYEtude.GetDateF()))
					bExistHorsPeriode = true;
		
			//Key.m_DateD			=	m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
			// si appartient au plateau �tendu et insertion avant ou apr�s p�riode action
			if(	m_pATPDocCreuset->m_TblHypotheses.GetItem().FindPAInsertions(Key,
																m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
  														        m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF()))
					bExistDansPeriode = true;
		}

		if (!m_pATPDocCreuset->m_AFFPlanEtendu && InxSuppPlateauHyp >= 0)
			bExistDansPeriode = true;

		if (m_pATPDocCreuset->m_AFFPlanEtendu && InxSuppPlateauEtendu >= 0)
			bExistDansPeriode = true;

	}

	if(bExistHorsPeriode && !bExistDansPeriode)
	{
		// on dessine encadrement logo (indique insertions en dehors p�riode action)
		dc.MoveTo(x1, y1);
		dc.LineTo(x1 + 8, y1);
		dc.LineTo(x1 + 8, y1 + 8);
		dc.LineTo(x1, y1 + 8); dc.LineTo(x1, y1);

		// on restaure le pinceau
		dc.SelectObject(pOldBrush);
	}
	if(bExistDansPeriode)
	{
		// on dessine logo plein
		dc.BeginPath();
		dc.MoveTo(x1, y1); dc.LineTo(x1 + 8, y1); dc.LineTo(x1 + 8, y1 + 8); dc.LineTo(x1, y1 + 8); dc.LineTo(x1, y1);
		dc.EndPath(); dc.StrokeAndFillPath();

		// on restaure le pinceau
		dc.SelectObject(pOldBrush);
	}


	// on calcule le rectangle
	RECT RectL; RectL.left = rect.left + 18; RectL.right = rect.right; RectL.top = rect.top; RectL.bottom = rect.bottom;

	CString Text;

	Text.Format(_T("Support %09d"), IdSupport);

	// on teste si on a trouv� le Support
	if (IdSupport)
	{
		// on r�cup�re le Support
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem * pElem= pReg->GetIElemByID(IdSupport);
		if(pElem)
		{
			// on �crit le libell� du Support
			Text = pElem->GetLabel().AsJCharPtr();
		
			//quotidien
			if(pElem->IsTitre() &&  pElem->AsITitre()->GetPeriodicite() == 1)
				m_pATPDocCreuset->m_bLDPQtd ? Text += " (LDP)" : Text += " (LNM)";
		}
		

	}

	// on �crit le libell� du Support
	dc.DrawText(Text, &RectL, DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPEchelle_A2::OnLButtonDown(LONG i, LONG j)
{

	// En mode Plan �tendu, non actif
	if (!m_pATPDocCreuset->m_AFFPlanEtendu)
	{
		// on r�cup�re l'identifiant du Support
		const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);

		CKeySupport KeySup;
		KeySup.m_IdSupport = IdSupport;

		// on recherche le Support dans le plateau
		JInt32 IdxSupport = m_pATPDocCreuset->m_PlateauSupports.FindItem(KeySup);

		// d�tecte si support dans plateau �tendu avec insertion
		
		// on teste si on a trouv� le Support
		if (IdxSupport >= 0)
		{
			// on r�cup�re le nombre d'insertions du Support
			JInt32 NbInsertions = this->OnGetNbInsertions(IdSupport);

			// on teste si le Support porte des insertions
			if (NbInsertions == 0)
			{
				// on teste les exceptions
				try
				{
					// on supprime le Support au plateau
					m_pATPDocCreuset->m_PlateauSupports.Remove(IdxSupport);
				}
				catch (JException* pException)
				{
					// on affiche le message
					::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
				}

				// on positionne l'indicateur de modification
				m_pATPDocCreuset->SetModifiedFlag(TRUE);

				// on met � jour les vues du document
				m_pATPDocCreuset->UpdateAllViews(UPDATE_ETUDE);
			}
			else
			{
				// Si plan master actif / rien � faire
				if (!m_pATPDocCreuset->m_IdxHypotheseCourant)
					return;

				// Y a t-il des insertions valides sur la p�riode d'action
				JInt32 NbInsertions = this->OnGetNbInsertionsPA(IdSupport);

				if (NbInsertions > 0)
				{
					// effacement apr�s confirmation utilisateur
					HWND hHwnd = NULL;
					CWnd *phParent = this->GetParent(); if (phParent != 0) hHwnd = phParent->GetSafeHwnd();
					CString ttl, qst;
					ttl.LoadString(IDR_CR_ATPFRAME);
					qst.LoadString(IDS_CR_SUPPR_SUPPORTECH);
					if ( MessageBox(qst, ttl, MB_ICONQUESTION|MB_YESNO) == IDYES)
					{
						// on supprime toutes les insertions de ce support sur toutes les hypotheses
						// et uniquement sur la p�riode action
						SuppInsertionSupport(IdxSupport);

						// on teste l'affichage du zoom
						if (m_pATPDocCreuset->m_AFFZoomPlan.IsVisible())
							// on met � jour l'affichage du zoom
							m_pATPDocCreuset->m_AFFZoomPlan.Reset();

						// on r�cup�re le nombre d'insertions restantes sur toute l'�tude
						JInt32 NbInsReste = this->OnGetNbInsertionsPA(IdSupport);

						if (NbInsReste == 0)
						{
							// on supprime le support du plateau (s'il ne reste plus d'insertion sur toute l'�tude)
							m_pATPDocCreuset->m_PlateauSupports.Remove(IdxSupport);
						}

						// on positionne l'indicateur de modification
						m_pATPDocCreuset->SetModifiedFlag(TRUE);

						// on met � jour les vues du document
						m_pATPDocCreuset->UpdateAllViews(UPDATE_ETUDE);
					}
				}
				else
				{
					// pas d'insertion dans p�riode action, on peut enlever support du plateau
					m_pATPDocCreuset->m_PlateauSupports.Remove(IdxSupport);

					// on positionne l'indicateur de modification
					m_pATPDocCreuset->SetModifiedFlag(TRUE);

					// on met � jour les vues du document
					m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);

				}
			}
		}
		else
		{
			// on teste les exceptions
			try
			{
				// on ajoute le Support au plateau �tendu et hypothese
				m_pATPDocCreuset->m_PlateauSupports.Add(KeySup);

				// Si non existant sur plateau �tendu on le rajoute au plateau �tendu
				if (m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.FindItem(KeySup) < 0)
					m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Add(KeySup);

			}
			catch (JException* pException)
			{
				// on affiche le message
				::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
			}

			// on positionne l'indicateur de modification
			m_pATPDocCreuset->SetModifiedFlag(TRUE);

			// on met � jour les vues du document
			m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
		}
	}		
}

JVoid CATPEchelle_A2::SuppInsertionSupport(JUnt32 IdxSupport)
{

	// Suppression sur la p�riode d'action ???? A VOIR
	JDate DateD = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
	JDate DateF = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();

	// on initialise le code
	JInt32 Ret = 0;

	if (IdxSupport >= 0)
	{
		// R�cup�re le support
		CKeySupport KeySup;
		KeySup = m_pATPDocCreuset->m_PlateauSupports.Item(IdxSupport);

		for (m_pATPDocCreuset->m_TblHypotheses.MoveFirst();
			 m_pATPDocCreuset->m_TblHypotheses.IsValid();
			 m_pATPDocCreuset->m_TblHypotheses.MoveNext()) 
		{

			if (!KeySup.m_IdParent)
			{
				// Cl� insertion
				CInsKeySFD KeySFD;

				// on r�cup�re le plan de l'hypoth�se courante
				CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();
				CPlanInsertions Tmp;

				// Nb insertions
				if (PLNInsertions.GetCount())
				{

					// balayage des insertions de cette hypoth�se
					for (PLNInsertions.MoveFirst_SFD(); PLNInsertions.IsValid_SFD();PLNInsertions.MoveNext_SFD())
					{
						// on r�cup�re l'insertion
						CInsertion * pInsertion = PLNInsertions.GetItem_SFD();

						// on teste si l'insertion correspond � notre demande
						if (pInsertion->m_IdSupport	== KeySup.m_IdSupport &&
							pInsertion->m_IdParent	== KeySup.m_IdParent)
						{
							/*
							// Suppression de toutes les insertions � cet endroit 
							while (pInsertion->m_IdSupport == KeySup.m_IdSupport && 
								pInsertion->m_IdParent  == KeySup.m_IdParent  &&
  								pInsertion->HasInsertions()) 
							{
								// on enl�ve insertion une par une  (� optimiser / A FAIRE) 
								PLNInsertions.DeleteInsertion_SFD(DateD,DateF);
							}
							*/
							// rien � faire
						}
						else
						{
							// Insertion non supprim�e , on ajoute au map insertion tmp
							Tmp.AddInsertion(*pInsertion,DateD,DateF);
						}
					}

					// Puis on remet en place la mise � jour plan insertions
					PLNInsertions.Swap(Tmp); 
				}
			}
		}
	}
}

JVoid CATPEchelle_A2::OnRButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPEchelle_A2::OnUpdate()
{
	// on initialise le nombre d'�l�ments
	LONG cx = 0;
	LONG cy = 0;

	// on v�rifie la validit� du document
	if (m_pATPDocCreuset != 0)
	{
		// on r�cup�re le nombre d'�l�ments
		cx = 1;
		cy = m_pATPDocCreuset->m_EchelleSupports.GetCount();
	}

	// on met � jour le composant
	this->SetHorzCount(cx, FALSE);
	this->SetVertCount(cy, TRUE);
}

/////////////////////////////////////
// CATPEchelle_A2 message handlers

void CATPEchelle_A2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPEchelle_Metriques::CX_Ax(), TRUE, FALSE);
	this->SetVertItemSize(CATPEchelle_Metriques::CY_x2(), TRUE, FALSE);
}

BOOL CATPEchelle_A2::PreTranslateMessage(MSG* pMsg)
{
	// TODO : ajoutez ici votre code sp�cialis� et/ou l'appel de la classe de base
	InitToolTip();
	
	m_tool.RelayEvent(pMsg);

	return JFCGrid::PreTranslateMessage(pMsg);
}

JVoid CATPEchelle_A2::OnMouseMove(LONG i, LONG j)
{
	//on s'�pargne le calcul si la souris ne change pas de cellule
	if (j == m_prev_j && i == m_prev_i )
		return;

	m_tool.Activate(FALSE);
	if (i == -1)
		return;

	CString strTipText;
	const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);
	CRegroupements * pReg = CRegroupements::GetInstance();
	IElem * pElem= pReg->GetIElemByID(IdSupport);
	if(pElem)
	{
		// on �crit le libell� du Support
		strTipText = pElem->GetLabel().AsJCharPtr();
	
		//quotidien
		if(pElem->IsTitre() &&  pElem->AsITitre()->GetPeriodicite() == 1)
			m_pATPDocCreuset->m_bLDPQtd ? strTipText += " (LDP)" : strTipText += " (LNM)";
	}

	//strTipText.Format("toto i=%d j=%d",i,j);

	if (m_tool.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		CString sText;
		m_tool.AddTool(this, (LPCTSTR)strTipText, rectBtn, 1);
	} 

	// Set text for tooltip
	m_tool.UpdateTipText((LPCTSTR)strTipText, this, 1);
	m_tool.Activate(TRUE);
	m_prev_j = j;
	m_prev_i = i;
}



