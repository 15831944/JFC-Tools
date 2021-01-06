// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATPGraph_Metriques.h"
#include "ATPGraphCouvJour.h"
#include "PrintAnyCWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPGraphCouvJour::CATPGraphCouvJour()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPGraphCouvJour::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on d�finit le document des composants
	m_B1.SetDocument(pATPDocCreuset);
	m_A2.SetDocument(pATPDocCreuset);

	SetAuto();
	// on met � jour le composant
	this->Update();
}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPGraphCouvJour::Update()
{
	// on met � jour les composants
	m_A1.Update();
	m_B1.Update();
	m_A2.Update();

	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPGraphCouvJour::~CATPGraphCouvJour()
{
	// on ne fait rien
}

///////////////////////////////////////////////
// la configuration pour configurer l'ascenseur

JFControl & CATPGraphCouvJour::SetHorzScrollBar(JFCScroll & Scroll)
{
	// on configure l'ascenseur
	m_B1.SetHorzScroll(&Scroll, FALSE, FALSE);
	m_A2.SetHorzScroll(&Scroll, FALSE, FALSE);

	// on renvoie le contr�le
	return (m_B1);
}

///////////////////////////////////////////////////////
// la fonction pour r�cup�rer le controle des abscisses

JFControl & CATPGraphCouvJour::GetAbcsisseControl()
{
	// on renvoie le contr�le
	return (m_A2);
}

///////////////////////////////////////////////////////////////////
// les fonctions pour r�cup�rer les dimensions minimales du panneau

LONG CATPGraphCouvJour::GetCXMin()
{
	// on calcule la largeur des composants
	LONG CX1 = CATPGraph_Metriques::CX_Ax() + CATPGraph_Metriques::CX_Bx();

	// on calcule la largeur des bordures et des interstices
	LONG CX2 = 0;

	// on renvoie la largeur
	return (CX1 + CX2);
}

LONG CATPGraphCouvJour::GetCYMin()
{
	// on calcule la hauteur des composants
	LONG CY1 = CATPGraph_Metriques::CY_x1();

	// on calcule la largeur des bordures et des interstices
	LONG CY2 = 0;

	// on renvoie la hauteur
	return (CY1 + CY2);
}

BEGIN_MESSAGE_MAP(CATPGraphCouvJour, JFCFlatPane)
	//{{AFX_MSG_MAP(CATPGraphCouvJour)
	ON_WM_CREATE()
//	ON_BN_CLICKED(12, OnBtnPrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CATPGraphCouvJour::OnBtnPrint()
{
	//m_btnPrint.ShowWindow(SW_HIDE);
	//CString str;
	//str = "Apport Couverture par jour " + GetLabel();
	//CPrintAnyCWnd::Print(this, str);
	//m_btnPrint.ShowWindow(SW_SHOW);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPGraphCouvJour::OnUpdate()
{
	// on met � jour le composant
	if (m_pATPDocCreuset != 0)
	{
		if(m_pATPDocCreuset->m_AFFCreuset.GetEchelleResult())
			this->ShowWindow(SW_HIDE);
		else 
			this->ShowWindow((m_pATPDocCreuset->m_AFFResultPlan.GetGraphCouvJour()) ? SW_SHOW : SW_HIDE);
		//m_btnPrint.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET) );
		//m_btnPrint.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET) );
		//m_btnPrint.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET) );
		//m_btnPrint.SetFontName(CATPColors::GetFontStaticName());
		//m_btnPrint.SetFontSize(CATPColors::GetFontStaticSize());
		//m_btnPrint.SetFontBold(0);
	}
}

/////////////////////////////////
// CATPGraphCouvJour message handlers

int CATPGraphCouvJour::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	// on appelle le gestionnaire de base
	if (this->JFCFlatPane::OnCreate(lpCreateStruct) == -1) return (-1);

	// on calcule la position des composants
	LONG x1_A1 = 0;					LONG x2_A1 = x1_A1 + CATPGraph_Metriques::CX_Ax();
	LONG y1_A1 = 0;					LONG y2_A1 = y1_A1 + 20;//y1_A1 + CATPGraph_Metriques::CY_x1();

	LONG x1_B1 = x2_A1;				LONG x2_B1 = x1_B1 + CATPGraph_Metriques::CX_Bx();
	LONG y1_B1 = y1_A1;				LONG y2_B1 = y1_A1 + 20;
	
	LONG x1_A2 = x2_A1;				LONG x2_A2 = x1_A2 + CATPGraph_Metriques::CX_Bx();
	LONG y1_A2 = y2_B1-1;				LONG y2_A2 = CATPGraph_Metriques::CY_x1();

	// on cr�e les composants A1 � T1
	if (m_A1.Create(NULL, WS_VISIBLE, CRect(x1_A1, y1_A1, x2_A1, y2_A1), this, 0) == FALSE) return (-1);
	if (m_B1.Create(NULL, WS_VISIBLE, CRect(x1_B1, y1_B1, x2_B1, y2_B1), this, 0) == FALSE) return (-1);
	if (m_A2.Create(NULL, WS_VISIBLE, CRect(x1_A2, y1_A2, x2_A2, y2_A2), this, 0) == FALSE) return (-1);
	if (m_btnPrint.Create(NULL, WS_VISIBLE, CRect(x1_A1, y1_A2, x2_A1, y2_A2), this, 0) == FALSE) return (-1);

	// on configure les d�placements
	this->SetMode(&m_A1, MGER_NONE, MGER_SIZE);
	this->SetMode(&m_B1, MGER_SIZE, MGER_SIZE);
	this->SetMode(&m_A2, MGER_SIZE, MGER_MOVE);
	this->SetMode(&m_btnPrint, MGER_NONE, MGER_MOVE);

	// on quitte	
	return (0);
}

void CATPGraphCouvJour::IncrMax()
{
	if (m_A1.GetMax() < 5.0)
	{
		m_A1.SetMax(m_A1.GetMax()+1.0);
		m_B1.SetMax(m_B1.GetMax()+1.0);
	}
	else
	{
		m_A1.SetMax(m_A1.GetMax()+5.0);
		m_B1.SetMax(m_B1.GetMax()+5.0);
	}
}

void CATPGraphCouvJour::DecrMax()
{
	if ( m_A1.GetMax() > 5.0)
	{
		m_A1.SetMax(m_A1.GetMax()-5.0);
		m_B1.SetMax(m_B1.GetMax()-5.0);
	}
	else
		if ( m_A1.GetMax() > 1.0)
		{
			m_A1.SetMax(m_A1.GetMax()-1.0);
			m_B1.SetMax(m_B1.GetMax()-1.0);
		}
}

void CATPGraphCouvJour::SetAuto()
{
	// on initialise le maximum
	JFlt64 Max = 0.0;
	// on v�rifie la validit� du document
	if (m_pATPDocCreuset != 0)
	{
		// on r�cup�re la p�riode d'action
		JDate DateD = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
		JDate DateF = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();

		// on v�rifie la validit� de la p�riode d'action
		if (DateD.IsValid() && DateF.IsValid())
		{

			// on recherche l'interrogation
			m_pATPDocCreuset->m_ITRHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);

			// on teste si on a trouv� l'hypoth�se
			if (m_pATPDocCreuset->m_ITRHypotheses.IsValid())
			{
				// on r�cup�re l'hypoth�se
				const JITPDATHypoFormat & DATHypoFormat = m_pATPDocCreuset->m_ITRHypotheses.GetItem();
				const JITPDATHypothese & DATHypothese = DATHypoFormat.GetDATHypoFormatActif();

				// on r�cup�re le nombre de jours
				JInt32 NbJours = DATHypothese.m_NbContactsJour.GetCount();

				// on r�cup�re l'effectif
				JFlt64 Effectif = DATHypoFormat.m_Effectif;

				// on teste l'effectif
				if (Effectif > 0.0)
				{
					// on boucle sur tous les jours
					for (JInt32 IdxJour = 0; IdxJour < NbJours; IdxJour += 1)
					{
						// on r�cup�re le nombre de contacts du jour courant
						JFlt64 Couv1 = 0.0;
						JFlt64 Couv = 100.0 * DATHypothese.m_DistributionJour.Item(IdxJour).Item(1).AsJFlt64() / Effectif;
						if(IdxJour>0)
							Couv1 = 100.0 * DATHypothese.m_DistributionJour.Item(IdxJour-1).Item(1).AsJFlt64() / Effectif;
						

						// on calcule le GRP du jour
						Couv = Couv - Couv1;

						// on met � jour le maximum
						if (Couv > Max) Max = Couv;
					}
				}
				
				int entier  = int((Max +10.0)/ 5);

				Max = entier*5.0;
			}
		}
	}

	m_A1.SetMax(Max);
	m_B1.SetMax(Max);

}


CString CATPGraphCouvJour::GetLabel()
{
	CString Text = "";
	// on v�rifie la validit� du document
	if (m_pATPDocCreuset->IsValid())
	{
		// on r�cup�re libell� filtre d'audience
		CString TxtFiltre = "";
		JSRCPresse* m_srcPresse;
		m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();
		if(m_pATPDocCreuset->GetIdFiltreAudienceSel() != 1)
		{
			TxtFiltre = m_srcPresse->m_TBLFiltresAudience.GetLabelByID(m_pATPDocCreuset->m_IdFiltreAudienceSel);
			Text += TxtFiltre;
			Text += " - ";
		}

		// on r�cup�re libell� de la source
		CTerrainManager* tm = CTerrainManager::GetInstance();
		CKeyTerrain key;
		//key.Set(m_pATPDocCreuset->m_IdSrcAudienceSel);
		//tm->GetTerrainNoRef(key)->m_sTerrainName;
		if (m_pATPDocCreuset && m_pATPDocCreuset->m_IdSrcAudienceSel)
		{
			// R�cup Table
			const JSRCTables  & srcTables	= JSRCTables::GetInstance();

			// R�cup la source audience courante
			JUnt32 SrcID			= m_pATPDocCreuset->m_IdSrcAudienceSel;  

			// R�cup le nouveau source
			const JSource * pt		= srcTables.m_TBLSources.GetItem(SrcID);

			// Recuperer dans table source
			Text += pt->m_Libelle.AsJCharPtr();
		}

		if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetCount() > 0)
		{
			if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetCount() > 1)
			{
				// Multi-terrains
				Text += " ";
				Text += "Multi-terrains";
			}
			else
			{
				// Mono-terrain
				m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();

				// Ident terrain s�lectionn�
				JUnt32 IdTerrainSel = m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey();

				// Test si terrain valide
				JBool IdTerrainValid = false;
				for (m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
					m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.IsValid();
					m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.MoveNext())
				{
					if (IdTerrainSel == m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.GetItem())
					{
						IdTerrainValid = true;
						break;
					}
				}

				if (IdTerrainValid)
				{
					// R�cup Table
					const JSRCTables  & srcTables	= JSRCTables::GetInstance();

					// R�cup la source audience courante
					JUnt32 SrcID			= m_pATPDocCreuset->m_IdSrcAudienceSel;  

					// R�cup le nouveau terrain
					const JTerrain * pt			= srcTables.m_TBLTerrains.GetItem(SrcID,IdTerrainSel);

					// Recuperer dans table terrain
					Text += " ";
					Text += pt->m_Libelle.AsJCharPtr();
				}
			}

			// Cas d'une cible normale s�lectionn�e
			if (!m_pATPDocCreuset->m_CiblesPondsActives)
			{
				// on recup�re libell� de la cible courante
				m_pATPDocCreuset->m_MAPpItems.MoveTo(m_pATPDocCreuset->m_IdItemSel);
				if(m_pATPDocCreuset->m_MAPpItems.IsValid() && Text != "")
				{
					// R�cup id cible
					JUnt32 Id = m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID();
					if(Id)
					{
						// R�cup Table des cibles
						// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

						// R�cup cible correspondant � l'indice
						// IItem * pItem = pItemConst->GetItemConstruit(Id);
						IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(Id);

						// Si cible trouv�, on retourne son libell�
						if(pItem)
						{
							// Recuperer libell� cible en cours
							Text += " - ";
							Text += pItem->GetLabel().AsJCharPtr();

							// R�cup�re effectif cible courante
							JFlt64 EffectifCible = 0.0;
							
							// on cr�e la cl� des effectifs
							CKeyMoteur keyMoteur;
							// on initialise la cl�
							// A FAIRE JEREMY
							m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
							if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.IsValid())
								keyMoteur.Set(m_pATPDocCreuset->m_IdSrcAudienceSel, m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey(), m_pATPDocCreuset->m_IdItemSel);
							// on initialise l'effectif
			
							EffectifCible = m_pATPDocCreuset->m_MoteurPresse.GetEffectif(keyMoteur);

							// si effectif <> 0 ajoute info effectif
							if (EffectifCible > 0)
							{
								CString TxtEffectif;
								Text += " (";

								TxtEffectif.Format("%.0f",EffectifCible); 
								CString Fmt = _T("### ### ### ### ###");
								CFormatStr::FormatNum(Fmt, TxtEffectif);
								Text += TxtEffectif;

								Text += " ";
								//on ajoute le libell� du type de poids s�lectionn�
								const JSRCTables  & srcTables	= JSRCTables::GetInstance();
								Text += srcTables.m_TBLPoids.GetLabelByID(m_pATPDocCreuset->m_Offre.m_IdTypePoids);

								Text += ")";
							}
						}
					}
				}
			}
			else
			{
				// on recup�re libell� de la cible pond�r�e courante
				m_pATPDocCreuset->m_MAPpItemsPonderes.MoveTo(m_pATPDocCreuset->m_IdItemSel);
				if(m_pATPDocCreuset->m_MAPpItemsPonderes.IsValid() && Text != "")
				{
					// R�cup id cible
					JUnt32 Id = m_pATPDocCreuset->m_MAPpItemsPonderes.GetItem()->GetID();
					if(Id)
					{
						// R�cup Table des cibles
						// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

						// R�cup cible correspondant � l'indice
						// IItem * pItem = pItemConst->GetItemPondere(Id);
						IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemPondere(Id);

						// Si cible trouv�, on retourne son libell�
						if(pItem)
						{
							// Recuperer libell� cible en cours
							Text += " - ";
							Text += pItem->GetLabel().AsJCharPtr();

							// A FAIRE ALAIN - HK

							// R�cup�re effectif cible courante
							JFlt64 EffectifCible = 0.0;

							// on cr�e la cl� des effectifs
							CKeyMoteur keyMoteur;
							// on initialise la cl�
							// A FAIRE JEREMY
							m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
							if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.IsValid())
								keyMoteur.Set(m_pATPDocCreuset->m_IdSrcAudienceSel, m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey(), m_pATPDocCreuset->m_IdItemSel);
							// on initialise l'effectif

							EffectifCible = m_pATPDocCreuset->m_MoteurPresse.GetEffectif(keyMoteur);

							// si effectif <> 0 ajoute info effectif
							if (EffectifCible > 0)
							{
								CString TxtEffectif;
								Text += " (";

								TxtEffectif.Format("%.0f",EffectifCible); 
								CString Fmt = _T("### ### ### ### ###");
								CFormatStr::FormatNum(Fmt, TxtEffectif);
								Text += TxtEffectif;

								Text += " ";
								//on ajoute le libell� du type de poids s�lectionn�
								const JSRCTables  & srcTables	= JSRCTables::GetInstance();
								Text += srcTables.m_TBLPoids.GetLabelByID(m_pATPDocCreuset->m_Offre.m_IdTypePoids);

								Text += ")";
							}
						}
					}
				}
			}
		}
	}
	return Text;
}
