// ATPResultPlan.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "ATPResultPlan.h"
#include "PrintAnyCWnd.h"



// CATPResultPlan

IMPLEMENT_DYNCREATE(CATPResultPlan, ETSLayoutFormView)

CATPResultPlan::CATPResultPlan()
	: ETSLayoutFormView(CATPResultPlan::IDD)
{
}

CATPResultPlan::~CATPResultPlan()
{
}

void CATPResultPlan::DoDataExchange(CDataExchange* pDX)
{
	ETSLayoutFormView::DoDataExchange(pDX);
	
	DDX_Control(pDX, IDC_CR_BTNPLUS, m_BtnPlus);
	DDX_Control(pDX, IDC_CR_BTNMOINS, m_BtnMoins);
	DDX_Control(pDX, IDC_CR_BTNAUTO, m_BtnAuto);

	DDX_Control(pDX, IDC_CR_TTL, m_Ttl);
	DDX_Control(pDX, IDC_CR_TTL2, m_lblRes);

	DDX_Control(pDX, IDC_CR_RES, m_ResultTbl);
	DDX_Control(pDX, IDC_CR_VS, m_VSRes);
	b_ini = false;
}

BEGIN_MESSAGE_MAP(CATPResultPlan, ETSLayoutFormView)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CR_BTNPLUS, OnBtnPlus)
	ON_BN_CLICKED(IDC_CR_BTNMOINS, OnBtnMoins)
	ON_BN_CLICKED(IDC_CR_BTNAUTO, OnBtnAuto)
	ON_WM_CREATE()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()




// Gestionnaires de messages CATPResultPlan
void CATPResultPlan::OnInitialUpdate()
{
	ETSLayoutFormView::OnInitialUpdate();
	Init();
}

void CATPResultPlan::Init()
{
	// on lie les ascenseurs
	if(m_ResultTbl.GetSafeHwnd())
	{
		m_ResultTbl.SetVertScroll(&m_VSRes, TRUE);
		SetColors();
	}
}

void CATPResultPlan::SetColors()
{
	/*
	m_BtnGRPJour.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET) );
	m_BtnGRPJour.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET) );
	m_BtnGRPJour.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET) );

	m_BtnGRPCumul.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET) );
	m_BtnGRPCumul.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET) );
	m_BtnGRPCumul.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET) );

	m_BtnCouvCumul.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET) );
	m_BtnCouvCumul.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET) );
	m_BtnCouvCumul.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET) );

	m_BtnCouvJour.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET) );
	m_BtnCouvJour.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET) );
	m_BtnCouvJour.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET) );
	*/

	m_BtnPlus.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnPlus.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnPlus.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_BtnMoins.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnMoins.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnMoins.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_BtnAuto.SetColor( CButtonST::BTNST_COLOR_BK_IN, CATPColors::GetColorLight(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnAuto.SetColor( CButtonST::BTNST_COLOR_BK_OUT, CATPColors::GetColorMedium(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );
	m_BtnAuto.SetColor( CButtonST::BTNST_COLOR_BK_FOCUS, CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET );

	m_Ttl.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_Ttl.SetTextColor(CATPColors::GetColorSuperDark(CATPColors::COLORCREUSET));
	m_Ttl.SetFontName(CATPColors::GetFontLabelName());
	m_Ttl.SetFontSize(CATPColors::GetFontLabelSize());
	m_Ttl.SetBorder(false);

	m_lblRes.SetBkColor(CATPColors::GetColorDark(CATPColors::COLORCREUSET),CATPColors::COLORCREUSET,CATPColors::DARK_DEGRAD);
	m_lblRes.SetTextColor(CATPColors::GetColorWhite());
	m_lblRes.SetFontName(CATPColors::GetFontLabelName());
	m_lblRes.SetBorder(false);

	m_ResultTbl.Invalidate();
	
	m_BtnPlus.SetIcon(IDI_CR_FBAS);
	m_BtnMoins.SetIcon(IDI_CR_FHAUT);
	
	m_grphGRPJour.Update();
	m_grphCouvCumul.Update();

	Invalidate();
}

void CATPResultPlan::OnSize(UINT nType, int cx, int cy)
{
	
	if (b_ini == false)
	{
		UpdateLayout(
		pane(VERTICAL)<<item (IDC_CR_TTL, ABSOLUTE_VERT)
					  <<(
		
						pane(HORIZONTAL)<<(
							pane(VERTICAL, ABSOLUTE_HORZ)	<<item (IDC_CR_TTL2, NORESIZE)
															<<(pane(HORIZONTAL)	<<item (IDC_CR_RES, GREEDY)
																				<<item (IDC_CR_VS, ABSOLUTE_HORZ))
											)
							
							<<(pane(VERTICAL)				<<(pane(HORIZONTAL)	<<item (IDC_CR_MIRE, GREEDY)
																				<<(pane(VERTICAL)<<item(IDC_CR_BTNMOINS,NORESIZE)
																								<<item(IDC_CR_BTNAUTO,NORESIZE)
																								<<item(IDC_CR_BTNPLUS,NORESIZE))
																								
																					)
																				)
					
							/*
							<<(pane(VERTICAL)				<<(pane(HORIZONTAL)	<<item (IDC_CR_MIRE, GREEDY)
																				<<(pane(VERTICAL)<<item(IDC_CR_BTNMOINS,NORESIZE)
																								<<item(IDC_CR_BTNAUTO,NORESIZE)
																								<<item(IDC_CR_BTNPLUS,NORESIZE))
																								
																				)
															<<(pane(HORIZONTAL)	<<item(IDC_CR_BTN1GRPJ,ABSOLUTE_VERT)
																				<<item(IDC_CR_BTN2GRPC,ABSOLUTE_VERT)
																				<<item(IDC_CR_BTN3COUVC,ABSOLUTE_VERT)
																				<<item(IDC_CR_BTN4COUVJ,ABSOLUTE_VERT) )
																				)
							*/
						)
		);
		b_ini = true;
	}
	else
	{
		ETSLayoutFormView::OnSize(nType, cx, cy);
	}


	CRect rect;
	CWnd *wnd = CFormView::GetDlgItem(IDC_CR_MIRE);
	if (wnd)
	{
		wnd->GetWindowRect(&rect);
		ScreenToClient(rect);

		m_grphGRPJour.SetWindowPos(this, rect.left, rect.top, rect.Width(), rect.Height(),  SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
		m_grphCouvCumul.SetWindowPos(this, rect.left, rect.top, rect.Width(), rect.Height(),  SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
		m_grphGRPCumul.SetWindowPos(this, rect.left, rect.top, rect.Width(), rect.Height(),  SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
		m_grphCouvJour.SetWindowPos(this, rect.left, rect.top, rect.Width(), rect.Height(),  SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOOWNERZORDER);
	}
}

void CATPResultPlan::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// N'appelez pas CFormView::OnPaint() pour la peinture des messages
	RECT Rect;
	// on récupère la zone cliente
	this->GetClientRect(&Rect);

	CBrush Brush(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));

	// on dessine le fond
	dc.FillRect(&Rect, &Brush);
}

JVoid CATPResultPlan::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	m_pATPDocCreuset = pATPDocCreuset;

	m_grphGRPJour.SetDocument(pATPDocCreuset);
	m_grphCouvCumul.SetDocument(pATPDocCreuset);
	m_grphGRPCumul.SetDocument(pATPDocCreuset);
	m_grphCouvJour.SetDocument(pATPDocCreuset);

	m_ResultTbl.SetDocument(pATPDocCreuset);
}

JVoid CATPResultPlan::Update()
{
	/*
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphGRPJour())
		CheckRadioButton(IDC_CR_BTN1GRPJ, IDC_CR_BTN4COUVJ, IDC_CR_BTN1GRPJ);
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphGRPCumul())
		CheckRadioButton(IDC_CR_BTN1GRPJ, IDC_CR_BTN4COUVJ, IDC_CR_BTN2GRPC);
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphCouvCumul())
		CheckRadioButton(IDC_CR_BTN1GRPJ, IDC_CR_BTN4COUVJ, IDC_CR_BTN3COUVC);
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphCouvJour())
		CheckRadioButton(IDC_CR_BTN1GRPJ, IDC_CR_BTN4COUVJ, IDC_CR_BTN4COUVJ);
	*/

	CString str;
	if (m_pATPDocCreuset->m_AFFDistributionC)
		str.FormatMessage(IDS_CR_DISTRIBCUMUL);
	if (m_pATPDocCreuset->m_AFFDistributionN)
		str.FormatMessage(IDS_CR_DISTRIBNETTE);
	if (m_pATPDocCreuset->m_AFFVentilation.IsVisible())
		str.FormatMessage(IDS_CR_VENTIL);
	if (m_pATPDocCreuset->m_AFFMemoDememo)
		str.FormatMessage(IDS_CR_MEMODEMEMO);
	if(str=="")
		str.FormatMessage(IDS_CR_PERF);
	m_lblRes.SetText(str);


	/*
	CString str1;
	if (m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
	{
		str.FormatMessage(IDS_CR_GRPJOUR);
		str1.FormatMessage(IDS_CR_GRPCUMUL);

		m_BtnCouvCumul.EnableWindow(FALSE);
		m_BtnCouvJour.EnableWindow(FALSE);
	}
	else
	{
		str.FormatMessage(IDS_CR_XGRPJOUR);
		str1.FormatMessage(IDS_CR_XGRPCUMUL);
		m_BtnCouvCumul.EnableWindow(TRUE);
		m_BtnCouvJour.EnableWindow(TRUE);
	}

	m_BtnGRPJour.SetWindowText(str);
	m_BtnGRPCumul.SetWindowText(str1);
	*/

	m_grphGRPJour.Update();
	m_grphCouvCumul.Update();
	m_grphGRPCumul.Update();
	m_grphCouvJour.Update();

	if(m_ResultTbl.GetSafeHwnd())
		m_ResultTbl.Update();

	CString Text = GetLabel();
	// on écrit le titre
	if (Text=="")
		Text.LoadString(IDS_CR_RESULTATS);

	// Ecriture titre des graphiques
	m_Ttl.SetFontBold(true); 


	// Texte info graphique
	CString StrGraph = "";

	// Graphique GRP Jour
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphGRPJour())
	{
		if (m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
			StrGraph.FormatMessage(IDS_CR_GRPJOUR);
		else
			StrGraph.FormatMessage(IDS_CR_XGRPJOUR);
	}
	// Graphique GRP Cumulé
	else if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphGRPCumul()) 
	{
		if (m_pATPDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
			StrGraph.FormatMessage(IDS_CR_GRPCUMUL);
		else
			StrGraph.FormatMessage(IDS_CR_XGRPCUMUL);
	}
	// Graphique Couv Cumulée
	else if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphCouvCumul())
	{
		StrGraph.FormatMessage(IDS_CR_COUVCUM);
	}
	// Graphique Apport C% Jour
	else if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphCouvJour())
	{
		StrGraph.FormatMessage(IDS_CR_APPCOUVJOUR);			
	}

	// Ajoute info type de graphique
	if (StrGraph != "")
	{
		Text += " - ";
		Text += StrGraph;
	}

	m_Ttl.SetText(Text);
	

}

CString CATPResultPlan::GetLabel()
{
	CString Text = "";
	// on vérifie la validité du document
	if (m_pATPDocCreuset->IsValid())
	{
		// on récupère libellé filtre d'audience
		CString TxtFiltre = "";
		JSRCPresse* m_srcPresse;
		m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();
		if(m_pATPDocCreuset->GetIdFiltreAudienceSel() != 1)
		{
			TxtFiltre = m_srcPresse->m_TBLFiltresAudience.GetLabelByID(m_pATPDocCreuset->m_IdFiltreAudienceSel);
			//Text += TxtFiltre;
			//Text += " - ";
		}

		// on récupère libellé de la source
		CTerrainManager* tm = CTerrainManager::GetInstance();
		CKeyTerrain key;
		//key.Set(m_pATPDocCreuset->m_IdSrcAudienceSel);
		//tm->GetTerrainNoRef(key)->m_sTerrainName;
		if (m_pATPDocCreuset && m_pATPDocCreuset->m_IdSrcAudienceSel)
		{
			// Récup Table
			const JSRCTables  & srcTables	= JSRCTables::GetInstance();

			// Récup la source audience courante
			JUnt32 SrcID			= m_pATPDocCreuset->m_IdSrcAudienceSel;  

			// Récup le nouveau source
			const JSource * pt		= srcTables.m_TBLSources.GetItem(SrcID);

			// Recuperer dans table source
			//Text += pt->m_Libelle.AsJCharPtr();
		}

		if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetCount() > 0)
		{

			if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetCount() > 1)
			{
				// Multi-terrains
				//Text += " ";
				//Text += "Multi-terrains";
			}
			else
			{
				// Mono-terrain
				m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();

				// Ident terrain sélectionné
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
					// Récup Table
					const JSRCTables  & srcTables	= JSRCTables::GetInstance();

					// Récup la source audience courante
					JUnt32 SrcID			= m_pATPDocCreuset->m_IdSrcAudienceSel;  

					// Récup le nouveau terrain
					const JTerrain * pt			= srcTables.m_TBLTerrains.GetItem(SrcID,IdTerrainSel);

					// Recuperer dans table terrain
					//Text += " ";
					//Text += pt->m_Libelle.AsJCharPtr();
				}
			}

			// Cas d'une cible normale sélectionnée
			if (!m_pATPDocCreuset->m_CiblesPondsActives)
			{
				// on recupère libellé de la cible courante
				m_pATPDocCreuset->m_MAPpItems.MoveTo(m_pATPDocCreuset->m_IdItemSel);
				if(m_pATPDocCreuset->m_MAPpItems.IsValid())
				{
					// Récup id cible
					JUnt32 Id = m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID();
					if(Id)
					{
						// Récup Table des cibles
						// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

						// Récup cible correspondant à l'indice
						// IItem * pItem = pItemConst->GetItemConstruit(Id);
						IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(Id);

						// Si cible trouvé, on retourne son libellé
						if(pItem)
						{
							// Recuperer libellé cible en cours
							// Text += " - ";
							Text += pItem->GetLabel().AsJCharPtr();

							// Récupère effectif cible courante
							JFlt64 EffectifCible = 0.0;
							
							// on crée la clé des effectifs
							CKeyMoteur keyMoteur;
							// on initialise la clé
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
								//on ajoute le libellé du type de poids sélectionné
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
				// on recupère libellé de la cible pondérée courante
				m_pATPDocCreuset->m_MAPpItemsPonderes.MoveTo(m_pATPDocCreuset->m_IdItemSel);
				if(m_pATPDocCreuset->m_MAPpItemsPonderes.IsValid())
				{
					// Récup id cible
					JUnt32 Id = m_pATPDocCreuset->m_MAPpItemsPonderes.GetItem()->GetID();
					if(Id)
					{
						// Récup Table des cibles
						// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

						// Récup cible pondérée correspondant à l'indice
						// IItem * pItem = pItemConst->GetItemPondere(Id);
						IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemPondere(Id);

						// Si cible trouvé, on retourne son libellé
						if(pItem)
						{
							// Recuperer libellé cible en cours
							// Text += " - ";
							Text += pItem->GetLabel().AsJCharPtr();

							// A FAIRE ALAIN - HK

							// Récupère effectif cible courante
							JFlt64 EffectifCible = 0.0;

							// on crée la clé des effectifs
							CKeyMoteur keyMoteur;
							// on initialise la clé
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
								//on ajoute le libellé du type de poids sélectionné
								const JSRCTables  & srcTables	= JSRCTables::GetInstance();
								Text += srcTables.m_TBLPoids.GetLabelByID(m_pATPDocCreuset->m_Offre.m_IdTypePoids);

								Text += ")";
							}

						}
					}
				}
			}

			// Les segments
			// + nom segment (si +, seg.multiples)
			if (m_pATPDocCreuset->m_Offre.m_TBLIdSegmentsAudience.GetCount() > 1)
				// Ici indique plusieurs segments
				Text.Append(" - Multi Segments");
			else
			{
				// Source Table
				const JSRCTables & Sources = JSRCTables::GetInstance ();

				// Id Segment de ciblage
				JUnt32 IdSeg;
				m_pATPDocCreuset->m_Offre.m_TBLIdSegmentsAudience.MoveFirst();
				IdSeg = m_pATPDocCreuset->m_Offre.m_TBLIdSegmentsAudience.GetItem();

				// Affiche ssi segment différent de ensemble
				if (IdSeg != 1)
				{
					const JSegment* Seg = Sources.m_TBLSegments.GetItem(IdSeg);
					Text += " - Segment : ";
					Text += Seg->m_Libelle.AsJCharPtr();
				}
			}
		}
	}
	return Text;
}

LONG CATPResultPlan::GetCXMin()
{
	// on renvoie la largeur
	return 178;
}

LONG CATPResultPlan::GetCYMin()
{
	// on renvoie la hauteur
	return 50;
}

void CATPResultPlan::OnBtn1GRPJour()
{
	//CPrintAnyCWnd::MetaFile(&m_grphGRPJour, "c:\\toto.emf");
	m_pATPDocCreuset->m_AFFResultPlan.SetGraphGRPJour();
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULT_PLAN);
}

void CATPResultPlan::OnBtn2GRPCumul()
{
	m_pATPDocCreuset->m_AFFResultPlan.SetGraphGRPCumul();
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULT_PLAN);
}

void CATPResultPlan::OnBtn3CouvCumul()
{
	m_pATPDocCreuset->m_AFFResultPlan.SetGraphCouvCumul();
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULT_PLAN);
}

void CATPResultPlan::OnBtn4CouvJour()
{
	m_pATPDocCreuset->m_AFFResultPlan.SetGraphCouvJour();
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_RESULT_PLAN);
}

void CATPResultPlan::OnBtnPlus()
{
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphGRPJour())
		m_grphGRPJour.IncrMax();
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphGRPCumul())
		m_grphGRPCumul.IncrMax();
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphCouvCumul())
		m_grphCouvCumul.IncrMax();
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphCouvJour())
		m_grphCouvJour.IncrMax();
}
void CATPResultPlan::OnBtnMoins()
{
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphGRPJour())
		m_grphGRPJour.DecrMax();
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphGRPCumul())
		m_grphGRPCumul.DecrMax();
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphCouvCumul())
		m_grphCouvCumul.DecrMax();
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphCouvJour())
		m_grphCouvJour.DecrMax();
}

void CATPResultPlan::OnBtnAuto()
{
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphGRPJour())
		m_grphGRPJour.SetAuto();
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphGRPCumul())
		m_grphGRPCumul.SetAuto();
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphCouvCumul())
		m_grphCouvCumul.SetAuto();
	if (m_pATPDocCreuset->m_AFFResultPlan.GetGraphCouvJour())
		m_grphCouvJour.SetAuto();
}

int CATPResultPlan::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (ETSLayoutFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// on crée les composants
	if (m_grphGRPJour.Create(NULL, WS_VISIBLE|WS_CHILD    , CRect(0, 0, CATPGraphGRP::GetCXMin(), CATPGraphGRP::GetCYMin()), this, 0) == FALSE) return (-1);
	if (m_grphCouvCumul.Create(NULL, WS_VISIBLE|WS_CHILD    , CRect(0, 0, CATPGraphCouv::GetCXMin(), CATPGraphCouv::GetCYMin()), this, 0) == FALSE) return (-1);
	if (m_grphGRPCumul.Create(NULL, WS_VISIBLE|WS_CHILD    , CRect(0, 0, CATPGraphGRPCumul::GetCXMin(), CATPGraphGRPCumul::GetCYMin()), this, 0) == FALSE) return (-1);
	if (m_grphCouvJour.Create(NULL, WS_VISIBLE|WS_CHILD    , CRect(0, 0, CATPGraphCouvJour::GetCXMin(), CATPGraphCouvJour::GetCYMin()), this, 0) == FALSE) return (-1);
	
	return 0;
}

void CATPResultPlan::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar != NULL)
	{
		// on positionne l'indicateur
		BOOL update = TRUE;

		// on teste si l'ascenseur est actif
		if (pScrollBar->IsWindowEnabled() != FALSE)
		{
			SCROLLINFO infos;

			// on initialise les informations
			infos.cbSize = sizeof(infos);
			infos.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;

			// on récupère l'état de l'ascenseur
			pScrollBar->GetScrollInfo(&infos, infos.fMask);

			// on teste le code de notification
			switch(nSBCode)
			{
				case SB_BOTTOM:
					// on déplace le curseur en bas au maximum
					infos.nPos = infos.nMax;
					break;
				case SB_PAGEDOWN:
					// on déplace le curseur d'une page en bas
					infos.nPos += (infos.nPage > 1) ? (infos.nPage - 1) : 1;
					break;
				case SB_LINEDOWN:
					// on déplace le curseur d'une ligne en bas
					infos.nPos += 1;
					break;
				case SB_TOP:
					// on déplace le curseur en haut au maximum
					infos.nPos  = infos.nMin;
					break;
				case SB_PAGEUP:
					// on déplace le curseur d'une page en haut
					infos.nPos -= (infos.nPage > 1) ? (infos.nPage - 1) : 1;
					break;
				case SB_LINEUP:
					// on déplace le curseur d'une ligne en haut
					infos.nPos -= 1;
					break;
				case SB_THUMBTRACK:
					// on déplace le curseur à la position courante
					infos.nPos = infos.nTrackPos;
					break;
				case SB_ENDSCROLL:
					// on déplace le curseur
					infos.nPos = infos.nPos;
					break;
				default:
					// on annule l'indicateur
					update = FALSE;
					break;
			}

			// on met à jour les informations
			infos.cbSize = sizeof(infos);
			infos.fMask  = SIF_POS;

			// on met à jour l'ascenseur
			pScrollBar->SetScrollInfo(&infos, TRUE);
		}
		m_ResultTbl.InvalidateRect(NULL, FALSE);
		m_ResultTbl.UpdateWindow();
	}

	ETSLayoutFormView::OnVScroll(nSBCode, nPos, pScrollBar);

}

BOOL CATPResultPlan::PreCreateWindow(CREATESTRUCT& cs)
{
	return ETSLayoutFormView::PreCreateWindow(cs);
}