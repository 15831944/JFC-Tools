// on inclut les définitions nécessaires
#include "stdafx.h"
#include "ATPResultHypo_Metriques.h"
#include "ATPResult_Grandeurs.h"
#include "ATPResultHypo_B2.h"
#include "resource.h"
#include "Courbes.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPResultHypo_B2::CATPResultHypo_B2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPResultHypo_B2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;
	m_ResultGrandeurs.SetDocument(m_pATPDocCreuset);
	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPResultHypo_B2::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPResultHypo_B2::~CATPResultHypo_B2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPResultHypo_B2, JFCGridMultiCell)
	//{{AFX_MSG_MAP(CATPResultHypo_B2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'intérieur des cellules

JVoid CATPResultHypo_B2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG ii, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	CBrush bruchRed;
	bruchRed.CreateSolidBrush(RGB(250,150,150));

	CBrush bruchNotDefined;
	bruchNotDefined.CreateSolidBrush(RGB(250,150,150));

	Color clr;
	clr.SetFromCOLORREF(RGB(155,155,155));
	HatchBrush BrushConfetti(HatchStyleSmallConfetti, clr, Color(0,255,0,0));

	CRect recttmp = rect;
	recttmp.top    += 2;
	recttmp.bottom -= 1;
	recttmp.left   += 3;
	recttmp.right  -= 3;

	// on crée le texte
	CString Text;

	// on vérifie la validité de l'hypothèse
	if (m_pATPDocCreuset->IsValid())
	{

		// si hypothèse active, couleur différente
		if (i+1 == m_pATPDocCreuset->m_IdxHypotheseActif)
		{
			// on crée le pinceau pour les insertions en arrière plan
			CBrush BrushB;
			BrushB.CreateSolidBrush(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
			
			// on dessine le fond de la cellule
			dc.FillRect(&rect, &BrushB);
		}

		// on recherche l'interrogation
		m_pATPDocCreuset->m_ITRHypotheses.MoveTo(i+1);

		// on teste si on a trouvé l'hypothèse
		if (m_pATPDocCreuset->m_ITRHypotheses.IsValid() && m_pATPDocCreuset->m_ITRHypotheses.GetItem().IsValid())
		{
			// on récupère l'hypothèse
			//const JITPDATHypothese & DATHypothese = m_pATPDocCreuset->m_ITRHypotheses.GetItem();
			int idFormat = ConvertICellToFormat(ii);
			const JITPDATHypoFormat & DATHypoFormat = m_pATPDocCreuset->m_ITRHypotheses.GetItem();
			const JITPDATHypothese & DATHypothese = ((idFormat!=-1) ? DATHypoFormat.GetDATHypoByFormat(idFormat) : DATHypoFormat.GetDATHypoTotal());

			CFont* pOldFont = NULL;
			if(idFormat == -1 && i+1 == m_pATPDocCreuset->m_IdxHypotheseActif)
			{
				/* TEST DEGRADE
				CBrush BrushB;
				BrushB.CreateSolidBrush(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));
				*/

				// on dessine le fond de la cellule
				// dc.FillRect(&rect, &BrushB);
				// dc.FillRect(&rect, &m_BrushMedium);

				// const int NbGrad = 10;

				/*
				static COLORREF clr_degrad[CATPColors::m_NbGrad];
				CATPColors::GetColorMediumGraduation(CATPColors::COLORCREUSET, clr_degrad, CATPColors::m_NbGrad);

				CATPColors::FillRectGraduate(dc, rect, clr_degrad, CATPColors::m_NbGrad,CATPColors::STYLE_DEGRAD::FROM_TOP_DEGRAD);
				*/

				CATPColors::FillRectModeGraduate(dc, rect, m_CATPColorsApp, CATPColors::MEDIUM_DEGRAD);
			}

			if (!m_pATPDocCreuset->m_AFFDistributionN
				&& !m_pATPDocCreuset->m_AFFDistributionC
				&& !m_pATPDocCreuset->m_AFFVentilation.IsVisible()
				&& !m_pATPDocCreuset->m_AFFContribution_New.IsVisible())
			{
				// -------------------- Budget global ---------------------
				if (m_ResultGrandeurs.IsBudget(j))
				{
					// on récupère le budget
					JFlt64 Budget = DATHypothese.m_Budget;
					if (Budget)
					{
						CString Fmt;
						if(Budget > 1000000)
						{
							Text.Format("%.0f", Budget/1000.0); 
							// on formate le budget
							Fmt = _T("### ### ### ### ### K€ ");
						}
						else
						{
							Text.Format("%.0f", Budget); 
							// on formate le budget
							Fmt = _T("### ### ### ### ### € ");
						}
						CFormatStr::FormatNum(Fmt, Text);
					}
					else
						Text.Format("* ");

					if(! DATHypothese.m_budgetOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}

				}

				// ------------------------ GRP --------------------------
				else if (m_ResultGrandeurs.IsGRP(j))
				{
					// on récupère le nombre de contacts et l'effectif
					JFlt64 Audience = DATHypothese.m_AudienceRef;
					JFlt64 Effectif   = DATHypoFormat.m_Effectif;

					// on teste si on peut calculer le GRP
					if (Effectif > 0.0 && Audience > 0.0)
						// on formate le nombre de contacts
						Text.Format(_T("%.1f "), (100.0 * Audience) / Effectif);
					else
						// on formate une étoile
						Text.Format(_T("* "));

					CString Fmt = _T("### ### ### ### ###,#");
					CFormatStr::FormatNum(Fmt, Text);
	
					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}

				}
				
				// ------------------------ X - GRP --------------------------
				else if (m_ResultGrandeurs.IsXGRP(j))
				{
					// on récupère le nombre de contacts et l'effectif
					JFlt64 VolLecture = DATHypothese.m_VolLecture;
					JFlt64 Effectif   = DATHypoFormat.m_Effectif;

					// on teste si on peut calculer le GRP
					if (Effectif > 0.0 && VolLecture > 0.0)
						// on formate le nombre de contacts
						Text.Format(_T("%.1f "), (100.0 * VolLecture) / Effectif);
					else
						// on formate une étoile
						Text.Format(_T("* "));

					CString Fmt = _T("### ### ### ### ###,#");
					CFormatStr::FormatNum(Fmt, Text);
					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
				}

				// -------------------- Couverture -----------------------
				else if (m_ResultGrandeurs.IsCouverture(j))
				{
					// on récupère la couverture cumulée à 1+
					JFlt64 Couv = DATHypothese.m_Couverture;
					JFlt64 Effectif   = DATHypoFormat.m_Effectif;

					// on formate la couverture	
					if(Couv > 0.0 && Effectif > 0.0)
						Text.Format(_T("%.1f %% "), 100.0 * Couv / Effectif);
					else
						Text.Format(_T("* "));
					CString Fmt = _T("### ### ### ### ###,#");
					CFormatStr::FormatNum(Fmt, Text);
					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}

				}

				// -------------------- Couverture Mémorisée -----------------------
				else if (m_ResultGrandeurs.IsCouvMem(j))
				{
					int beta = AfxGetApp()->GetProfileInt("Moteur", "CourbeReponse", 0);
					CCourbes * courbes = CCourbes::GetInstance();
					double vol = 0.0;
					double DistrFreq;
					// on récupère le nombre de jours
					JInt32 idxD = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD() - m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD()-1;
					JInt32 idxF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF() - m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
					for (int k = 0; k<20; k++)
					{
						if( idxD>=0 )
						{
								if (k == 19)
									DistrFreq = DATHypothese.m_DistributionJour.Item(idxF).Item(k+1).AsJFlt64() - DATHypothese.m_DistributionJour.Item(idxD).Item(k+1).AsJFlt64();
								if (k < 19)
									DistrFreq = (DATHypothese.m_DistributionJour.Item(idxF).Item(k+1).AsJFlt64() - DATHypothese.m_DistributionJour.Item(idxF).Item(k+2).AsJFlt64())
									- (DATHypothese.m_DistributionJour.Item(idxD).Item(k+1).AsJFlt64() - DATHypothese.m_DistributionJour.Item(idxD).Item(k+2).AsJFlt64());
						}
						else
						{
							if (k == 19)
								DistrFreq = DATHypothese.m_DistributionJour.Item(idxF).Item(k+1).AsJFlt64();
							if (k < 19)
								DistrFreq = DATHypothese.m_DistributionJour.Item(idxF).Item(k+1).AsJFlt64() - DATHypothese.m_DistributionJour.Item(idxF).Item(k+2).AsJFlt64();
						}

						vol += DistrFreq * courbes->GetProba(k+1,beta);
					}
					// on formate la couverture	
					if(vol> 0.0 && DATHypoFormat.m_Effectif>0.0)
						Text.Format(_T("%.1f %% "), 100.0 * vol / DATHypoFormat.m_Effectif);
					else
						Text.Format(_T("* "));
					CString Fmt = _T("### ### ### ### ###,#");
					CFormatStr::FormatNum(Fmt, Text);
					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
				}

				// -------------------- Coût GRP -----------------------
				else if (m_ResultGrandeurs.IsCoutGRP(j))
				{
					// on récupère le budget, nombre de contacts et l'effectif
					JFlt64 Budget     = DATHypothese.m_Budget;
					JFlt64 Audience = DATHypothese.m_AudienceRef;
					JFlt64 Effectif   = DATHypoFormat.m_Effectif;

					// on teste si on peut calculer le coût / GRP
					if (Audience > 0.0 && Effectif > 0.0 && Budget > 0.0)
					{
						// on formate le Coût GRP	
						Text = CFormatStr::FormatCout((Budget * Effectif) / (100.0 * Audience), m_pATPDocCreuset->m_NbDecCout);
						//Text.Format(_T("%.0f"), (Budget * Effectif) / (100.0 * Audience));
						//CString Fmt = _T("### ### ### ### ### € ");
						//CFormatStr::FormatNum(Fmt, Text);
					}
					else
						// on formate une étoile
						Text.Format(_T("* "));
					if(! DATHypothese.m_budgetOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
				}

				// -------------------- Coût Couverture -----------------------
				else if (m_ResultGrandeurs.IsCoutCouv(j))
				{
					// on récupère le budget, nombre de contacts et l'effectif
					JFlt64 Budget     = DATHypothese.m_Budget;
					JFlt64 Couv = DATHypothese.m_Couverture;
					JFlt64 Effectif   = DATHypoFormat.m_Effectif;

					// on teste si on peut calculer le coût couverture
					if (Couv > 0.0 && Effectif > 0.0 && Budget > 0.0)
					{
						// on formate le Coût couverture	
						Text = CFormatStr::FormatCout((Budget * Effectif) / (100.0 * Couv), m_pATPDocCreuset->m_NbDecCout);
						//Text.Format(_T("%.0f"), (Budget * Effectif) / (100.0 * Couv));
						//CString Fmt = _T("### ### ### ### ### € ");
						//CFormatStr::FormatNum(Fmt, Text);
					}
					else
						// on formate une étoile
						Text.Format(_T("* "));
					if(! DATHypothese.m_budgetOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
				}
				// -------------------- Coût /1000 contacts -----------------------
				else if (m_ResultGrandeurs.IsCout1000Cts(j))
				{
					// on récupère le budget, nombre de contacts et l'effectif
					JFlt64 Budget     = DATHypothese.m_Budget;
					JFlt64 VolLecture = DATHypothese.m_VolLecture;

					// on teste si on peut calculer le Coût /1000 contacts
					if (VolLecture > 0.0 && Budget > 0.0)
					{
						// on formate le Coût /1000 contacts
						Text = CFormatStr::FormatCout((1000.0 * Budget) / VolLecture, m_pATPDocCreuset->m_NbDecCout);
						//Text.Format(_T("%.2f € "), (1000.0 * Budget) / VolLecture);
						//CString Fmt = _T("### ### ### ### ###,##");
						//CFormatStr::FormatNum(Fmt, Text);
					}
					else
						// on formate une étoile
						Text.Format(_T("* "));
					if(! DATHypothese.m_budgetOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
				}

				// -------------------- Coût /1000 indiv -----------------------
				else if (m_ResultGrandeurs.IsCout1000Ind(j))
				{
					// on récupère le budget, nombre de contacts
					JFlt64 Budget     = DATHypothese.m_Budget;
					// on récupère la couverture cumulée à 1+
					JFlt64 Couv = DATHypothese.m_Couverture;

					// on teste si on peut calculer le Coût /1000 indiv
					if (Couv > 0.0 && Budget > 0.0)
					{
						// on formate le Coût /1000 indiv
						Text = CFormatStr::FormatCout((1000.0 * Budget) / Couv, m_pATPDocCreuset->m_NbDecCout);
						//Text.Format(_T("%.2f € "), (1000.0 * Budget) / Couv);
						//CString Fmt = _T("### ### ### ### ###,##");
						//CFormatStr::FormatNum(Fmt, Text);
					}
					else
						// on formate une étoile
						Text.Format(_T("* "));
					if(! DATHypothese.m_budgetOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
				}

				// -------------------- Répétition -----------------------
				else if (m_ResultGrandeurs.IsRepetition(j))
				{
					// on récupère le nombre de contacts et l'effectif
					JFlt64 Audience = DATHypothese.m_AudienceRef;
					JFlt64 Effectif   = DATHypoFormat.m_Effectif;
					
					// on récupère la couverture cumulée à 1+
					JFlt64 Couv = DATHypothese.m_Couverture;

					// on teste si on peut calculer la répétition
					if (Effectif > 0.0 && Couv > 0.0)
						// on formate la repetition	
						Text.Format(_T("%.2f "), Audience / Couv);
					else
					{
						// on formate une étoile
						Text.Format(_T("* "));
					}
					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
				}

				// -------------------- Répétition [+]-----------------------
				else if (m_ResultGrandeurs.IsRepetPlus(j))
				{
					// on récupère le nombre de contacts et l'effectif
					JFlt64 VolLecture = DATHypothese.m_VolLecture;
					JFlt64 Effectif   = DATHypoFormat.m_Effectif;
					
					// on récupère la couverture cumulée à 1+
					JFlt64 Couv = DATHypothese.m_Couverture;

					// on teste si on peut calculer la répétition [+]
					if (Effectif > 0.0 && Couv > 0.0)
						// on formate la repetition	[+]
						Text.Format(_T("%.2f "), VolLecture / Couv);
					else
					{
						// on formate une étoile
						Text.Format(_T("* "));
					}
					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
				}

				// -------------------- Audience (Nb Individus touchés)-----------------------
				else if (m_ResultGrandeurs.IsNbIndividus(j))
				{
					// on récupère la couverture cumulée à 1+
					JFlt64 Couv = DATHypothese.m_Couverture;

					// on formate la couverture	
					if (Couv > 0.0)
						Text.Format(_T("%.0f"), Couv /1000.0);
					else
						Text.Format(_T("*"));

					CString Fmt = _T("### ### ### ### ### ");
					CFormatStr::FormatNum(Fmt, Text);
					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
				}

				// -------------------- Nombre de contact distribué -----------------------
				else if (m_ResultGrandeurs.IsNbContacts(j))
				{
					// on récupère le nombre de contacts
					JFlt64 Audience = DATHypothese.m_AudienceRef;

					// on formate le nombre de contacts
					if(Audience > 0.0)
						Text.Format(_T("%.0f"), Audience / 1000.0);
					else
						Text.Format(_T("*"));

					CString Fmt = _T("### ### ### ### ### ");
					CFormatStr::FormatNum(Fmt, Text);
					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
				}
				// -------------------- Nombre de contact plus -----------------------
				else if (m_ResultGrandeurs.IsNbCtsPlus(j))
				{
					// on récupère le nombre de contacts
					JFlt64 VolLecture = DATHypothese.m_VolLecture;

					// on formate le nombre de contacts
					if(VolLecture > 0.0)
						Text.Format(_T("%.0f"), VolLecture / 1000.0);
					else
						Text.Format(_T("*"));

					CString Fmt = _T("### ### ### ### ### ");
					CFormatStr::FormatNum(Fmt, Text);
					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
				}
			}

			if(m_pATPDocCreuset->m_AFFDistributionN)
			{
				
				// -------------------- Distribution de fréquence Nette-----------------------
				// on récupère le nombre de jours
				JInt32 idxF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF() - m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();

				// on récupère la distribution de fréquence
				JFlt64 DistrFreq = 0.0; 
				if (idxF > 0) 
				{
					if (j == 20)
						DistrFreq = 1.0 - (DATHypothese.m_DistributionJour.Item(idxF).Item(1).AsJFlt64()  / DATHypoFormat.m_Effectif);
					if (j == 19)
						DistrFreq = DATHypothese.m_DistributionJour.Item(idxF).Item(j+1).AsJFlt64()  / DATHypoFormat.m_Effectif;
					if (j < 19)
						DistrFreq = (DATHypothese.m_DistributionJour.Item(idxF).Item(j+1).AsJFlt64() - DATHypothese.m_DistributionJour.Item(idxF).Item(j+2).AsJFlt64()) / DATHypoFormat.m_Effectif;
					DistrFreq *= 100.0;
				}

				// en attente valeur distr. fréq
				if (DistrFreq*10 > 1)
					Text.Format(_T("%.1f %% "), DistrFreq);
				if(! DATHypothese.m_ResultOK)
				{
					dc.FillRect(&recttmp,&bruchRed);
					dc.SetTextColor(RGB(255,255,255));
				}


			}

			if(m_pATPDocCreuset->m_AFFDistributionC)
			{
				
				// -------------------- Distribution de fréquence Cumulée-----------------------
				// on récupère le nombre de jours
				JInt32 idxF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF() - m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();

				// on récupère la distribution de fréquence
				JFlt64 DistrFreq = 0.0; 
				if (idxF > 0) 
				{
					if (j == 20)
						DistrFreq = 1.0 - (DATHypothese.m_DistributionJour.Item(idxF).Item(1).AsJFlt64()  / DATHypoFormat.m_Effectif);
					else
						DistrFreq = DATHypothese.m_DistributionJour.Item(idxF).Item(j+1).AsJFlt64() / DATHypoFormat.m_Effectif;
					DistrFreq *= 100.0;
				}

				// en attente valeur distr. fréq
				if (DistrFreq*10 > 1)
					Text.Format(_T("%.1f %% "), DistrFreq);

				if(! DATHypothese.m_ResultOK)
				{
					dc.FillRect(&recttmp,&bruchRed);
					dc.SetTextColor(RGB(255,255,255));
				}
			}

			if (m_pATPDocCreuset->m_AFFVentilation.IsVisible())
			{
				
				Text = m_pATPDocCreuset->m_AFFVentilation.GetValue(i+1, idFormat, j);


				//if (j%6 == 0 && m_pATPDocCreuset->m_AFFVentilation.GetGRP(i+1,idFormat, j/6)>0.0)
				//	Text.Format(_T("%.1f  "), m_pATPDocCreuset->m_AFFVentilation.GetGRP(i+1,idFormat, j/6));
				//if (j%6 == 1 && m_pATPDocCreuset->m_AFFVentilation.GetIndGRP(i+1,idFormat, j/6)>0.0)
				//	Text.Format(_T("%.0f "), m_pATPDocCreuset->m_AFFVentilation.GetIndGRP(i+1,idFormat, j/6));
				//if (j%6 == 2 && m_pATPDocCreuset->m_AFFVentilation.GetCouv(i+1,idFormat, j/6)>0.0)
				//	Text.Format(_T("%.1f %% "), m_pATPDocCreuset->m_AFFVentilation.GetCouv(i+1,idFormat, j/6));
				//if (j%6 == 3 && m_pATPDocCreuset->m_AFFVentilation.GetIndCouv(i+1,idFormat, j/6)>0.0)
				//	Text.Format(_T("%.0f "), m_pATPDocCreuset->m_AFFVentilation.GetIndCouv(i+1,idFormat, j/6));
				//if (j%6 == 4 && m_pATPDocCreuset->m_AFFVentilation.GetRepet(i+1,idFormat, j/6)>0.0)
				//	Text.Format(_T("%.1f "), m_pATPDocCreuset->m_AFFVentilation.GetRepet(i+1,idFormat, j/6));
				if(! DATHypothese.m_ResultOK)
				{
					dc.FillRect(&recttmp,&bruchRed);
					dc.SetTextColor(RGB(255,255,255));
				}
			}

			// Modif Version 2.7
			if (m_pATPDocCreuset->m_AFFContribution_New.IsVisible())
			{
				Text = m_pATPDocCreuset->m_AFFContribution_New.GetValue(i+1, idFormat, j);

				if (m_pATPDocCreuset->m_AFFContribution_New.NbValue() != 0)
				{
					// Nombre de supports sélectionnés
					JUnt32 NbSupportsPlateau = m_pATPDocCreuset->m_PlateauSupports.GetCount();
					JUnt32 NbSupportsActif = m_pATPDocCreuset->OnNbLigPlanActif();

					// Nombre de grandeurs sélectionnées
					JUnt32 NbGrandeursActives = m_pATPDocCreuset->m_AFFContribution_New.NbValue();

					// Init Texte
					Text = "";

					// Affichage des contributions par bloc de grandeurs identiques
					if (m_pATPDocCreuset->m_AFFContribution_New.GetModeAffich() == m_pATPDocCreuset->m_AFFContribution_New.MODEAFFICH::AFFICH_BLOC_SUPPORTS)
					{
						// Cas de plusieurs grandeurs contribs sélectionnées
						int IndSupportActif = j % (NbSupportsActif + 1);

						// Indice support
						int IndSupport = m_pATPDocCreuset->GetIndexSuppPlateauViaIndexSuppActif(IndSupportActif);

						if (IndSupport < NbSupportsPlateau && IndSupportActif < NbSupportsActif)
						{
							JMap<JUnt32, CString> & MapResultContrib = m_pATPDocCreuset->m_AFFContribution_New.GetMapValue(i+1, idFormat, IndSupport);
						
							// Indice Position Grandeur Sel
							JUnt32 IndPosGrdSel = (j / (NbSupportsActif + 1));

							// Init Indice Grandeur
							JUnt32 IndiceGrd = 0;

							// Resultat Couv ???
							if (m_pATPDocCreuset->m_AFFContribution_New.CanCouv())
							{
								if (IndiceGrd == IndPosGrdSel)
								{
									MapResultContrib.MoveTo(m_pATPDocCreuset->m_AFFContribution_New.TYPE::COUV);
									if (MapResultContrib.IsValid())
									{
										Text = MapResultContrib.GetItem();
									}
									else
										Text = "-1";
								}
								// 1 grandeur en + de sélectionner
								IndiceGrd++;

							}

							// Resultat GRP ???
							if (m_pATPDocCreuset->m_AFFContribution_New.CanGRP())
							{
								if (IndiceGrd == IndPosGrdSel)
								{
									MapResultContrib.MoveTo(m_pATPDocCreuset->m_AFFContribution_New.TYPE::GRP);
									if (MapResultContrib.IsValid())
									{
										Text = MapResultContrib.GetItem();
									}
									else
										Text = "-1";
								}

								// 1 grandeur en + de sélectionner
								IndiceGrd++;
							}

							// Resultat PART GRP ???
							if (m_pATPDocCreuset->m_AFFContribution_New.CanPGRP())
							{
								if (IndiceGrd == IndPosGrdSel)
								{
									MapResultContrib.MoveTo(m_pATPDocCreuset->m_AFFContribution_New.TYPE::PARTGRP);
									if (MapResultContrib.IsValid())
									{
										Text = MapResultContrib.GetItem();
									}
									else
										Text = "-1";
								}
								// 1 grandeur en + de sélectionner
								IndiceGrd++;
							}

							// Resultat BUDGET ???
							if (m_pATPDocCreuset->m_AFFContribution_New.CanBudg())
							{
								if (IndiceGrd == IndPosGrdSel)
								{
									MapResultContrib.MoveTo(m_pATPDocCreuset->m_AFFContribution_New.TYPE::BUDGET);
									if (MapResultContrib.IsValid())
									{
										Text = MapResultContrib.GetItem();
									}
									else
										Text = "-1";
								}
								// 1 grandeur en + de sélectionner
								IndiceGrd++;
							}

							// Resultat BUDGET ???
							if (m_pATPDocCreuset->m_AFFContribution_New.CanPBudg())
							{
								if (IndiceGrd == IndPosGrdSel)
								{
									MapResultContrib.MoveTo(m_pATPDocCreuset->m_AFFContribution_New.TYPE::PARTBUDGET);
									if (MapResultContrib.IsValid())
									{
										Text = MapResultContrib.GetItem();
									}
									else
										Text = "-1";

									// Surlignage si budget non OK
									if (!DATHypothese.m_budgetOK)
									{
										dc.FillRect(&recttmp,&bruchRed);
										dc.SetTextColor(RGB(255,255,255));
									}
								}
								// 1 grandeur en + de sélectionner
								IndiceGrd++;
							}
				
							// Resultat BUDGET ???
							if (m_pATPDocCreuset->m_AFFContribution_New.CanCtCouv())
							{
								if (IndiceGrd == IndPosGrdSel)
								{
									MapResultContrib.MoveTo(m_pATPDocCreuset->m_AFFContribution_New.TYPE::CTCOUV);
									if (MapResultContrib.IsValid())
									{
										Text = MapResultContrib.GetItem();
									}
									else
										Text = "-1";
								}
								// 1 grandeur en + de sélectionner
								IndiceGrd++;
							}

						}
					}
					else
					{
						// Affichage résultat contributions en regroupant les grandeurs sélectionnés
						// Cas de plusieurs grandeurs contribs sélectionnées
						int IndSupportActif = j / (NbGrandeursActives + 1);

						// Indice support
						int IndSupport = m_pATPDocCreuset->GetIndexSuppPlateauViaIndexSuppActif(IndSupportActif);
						if (IndSupport < NbSupportsPlateau && IndSupportActif < NbSupportsActif)
						{
							JMap<JUnt32, CString> & MapResultContrib = m_pATPDocCreuset->m_AFFContribution_New.GetMapValue(i+1, idFormat, IndSupport);

							// Indice Position Grandeur Sel
							JUnt32 IndPosGrdSel = (j %(NbGrandeursActives + 1));

							// Init Indice Grandeur
							JUnt32 IndiceGrd = 0;

							// Resultat Couv ???
							if (m_pATPDocCreuset->m_AFFContribution_New.CanCouv())
							{
								if (IndiceGrd == IndPosGrdSel)
								{
									MapResultContrib.MoveTo(m_pATPDocCreuset->m_AFFContribution_New.TYPE::COUV);
									if (MapResultContrib.IsValid())
									{
										Text = MapResultContrib.GetItem();
									}
								}
								// 1 grandeur en + de sélectionner
								IndiceGrd++;
							}

							// Resultat GRP ???
							if (m_pATPDocCreuset->m_AFFContribution_New.CanGRP())
							{
								if (IndiceGrd == IndPosGrdSel)
								{
									MapResultContrib.MoveTo(m_pATPDocCreuset->m_AFFContribution_New.TYPE::GRP);
									if (MapResultContrib.IsValid())
									{
										Text = MapResultContrib.GetItem();
									}
								}
								// 1 grandeur en + de sélectionner
								IndiceGrd++;
							}

							// Resultat PART GRP ???
							if (m_pATPDocCreuset->m_AFFContribution_New.CanPGRP())
							{
								if (IndiceGrd == IndPosGrdSel)
								{
									MapResultContrib.MoveTo(m_pATPDocCreuset->m_AFFContribution_New.TYPE::PARTGRP);
									if (MapResultContrib.IsValid())
									{
										Text = MapResultContrib.GetItem();
									}
								}

								// 1 grandeur en + de sélectionner
								IndiceGrd++;
							}

							// Resultat BUDGET ???
							if (m_pATPDocCreuset->m_AFFContribution_New.CanBudg())
							{
								if (IndiceGrd == IndPosGrdSel)
								{
									MapResultContrib.MoveTo(m_pATPDocCreuset->m_AFFContribution_New.TYPE::BUDGET);
									if (MapResultContrib.IsValid())
									{
										Text = MapResultContrib.GetItem();
									}
								}

								// 1 grandeur en + de sélectionner
								IndiceGrd++;
							}

							// Resultat BUDGET ???
							if (m_pATPDocCreuset->m_AFFContribution_New.CanPBudg())
							{
								if (IndiceGrd == IndPosGrdSel)
								{
									MapResultContrib.MoveTo(m_pATPDocCreuset->m_AFFContribution_New.TYPE::PARTBUDGET);
									if (MapResultContrib.IsValid())
									{
										Text = MapResultContrib.GetItem();
									}

									// Surlignage si budget non OK
									if (!DATHypothese.m_budgetOK)
									{
										dc.FillRect(&recttmp,&bruchRed);
										dc.SetTextColor(RGB(255,255,255));
									}
								}

								// 1 grandeur en + de sélectionner
								IndiceGrd++;
							}

							// Resultat Cout Couv ???
							if (m_pATPDocCreuset->m_AFFContribution_New.CanCtCouv())
							{
								if (IndiceGrd == IndPosGrdSel)
								{
									MapResultContrib.MoveTo(m_pATPDocCreuset->m_AFFContribution_New.TYPE::CTCOUV);
									if (MapResultContrib.IsValid())
									{
										Text = MapResultContrib.GetItem();
									}
								}

								// 1 grandeur en + de sélectionner
								IndiceGrd++;
							}
						}
					}
				}
				
				if(! DATHypothese.m_ResultOK)
				{
					dc.FillRect(&recttmp,&bruchRed);
					dc.SetTextColor(RGB(255,255,255));
				}

				if (!m_pATPDocCreuset->m_AFFContribution_New.IsVisible())
				{
					if ((m_pATPDocCreuset->m_AFFContribution_New.m_type == m_pATPDocCreuset->m_AFFContribution_New.BUDGET 
						||m_pATPDocCreuset->m_AFFContribution_New.m_type == m_pATPDocCreuset->m_AFFContribution_New.PARTBUDGET 
						||m_pATPDocCreuset->m_AFFContribution_New.m_type == m_pATPDocCreuset->m_AFFContribution_New.CTCOUV )
						&&(! DATHypothese.m_budgetOK))
					{
						dc.FillRect(&recttmp,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
				}
			}

			// on écrit le texte
			if (Text != "*")
			{
				dc.DrawText(Text, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);				
				dc.SetTextColor(RGB(0,0,0));
			}
			else
			{
				Graphics grf(dc);
				grf.FillRectangle(&BrushConfetti, rect.left, rect.top, rect.right-rect.left-1, rect.bottom-rect.top-1);
				dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);	
				dc.SetTextColor(RGB(150,200,30));
			}
			
			if(pOldFont!=NULL)
				dc.SelectObject(pOldFont);
	
		}
	}
}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPResultHypo_B2::OnLButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

JVoid CATPResultHypo_B2::OnRButtonDown(LONG i, LONG j)
{
	// on ne fait rien
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPResultHypo_B2::OnUpdate()
{

	// on initialise le nombre d'éléments
	LONG xHyp		= 0;
	LONG yGrandeur	= 0;
	int nbFormat = 0;

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0 && m_pATPDocCreuset->IsValid())
	{
		// on récupère le nombre d'éléments
		xHyp		= m_pATPDocCreuset->m_TblHypotheses.GetCount()-1;
		m_mapICellFormat.Reset();
		nbFormat = 0;
		for(m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
		{
			if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
			{
				nbFormat++;
				m_mapICellFormat.Add(nbFormat) = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
			}
		}
		////Colonne Total
		if(m_pATPDocCreuset->m_AFFColonneTotal)
		{
			nbFormat++;
			m_mapICellFormat.Add(nbFormat) = -1;
		}

		// on récupère le nombre de grandeurs
		if (!m_pATPDocCreuset->m_AFFDistributionN
			&& !m_pATPDocCreuset->m_AFFDistributionC
			&& !m_pATPDocCreuset->m_AFFVentilation.IsVisible()
			&& !m_pATPDocCreuset->m_AFFContribution_New.IsVisible())
		{
			// Les résultats généraux (Couverture, GRP, Budget etc....)
			yGrandeur	= m_ResultGrandeurs.GetNbGrandeurs();
		}
		if (m_pATPDocCreuset->m_AFFDistributionN || m_pATPDocCreuset->m_AFFDistributionC )
		{
			// Distribution de contacts
			yGrandeur  += m_ResultGrandeurs.GetNbGrandeursDF();
		}
		if (m_pATPDocCreuset->m_AFFVentilation.IsVisible())
		{
			// Ventilation
			yGrandeur  = (m_pATPDocCreuset->m_AFFVentilation.NbValue()+1) * m_pATPDocCreuset->m_AFFVentilation.GetNbItem();
		}

		if (m_pATPDocCreuset->m_AFFContribution_New.IsVisible())
		{
			// en mode plateau
			// yGrandeur = m_pATPDocCreuset->m_PlateauSupports.GetCount();

			if (m_pATPDocCreuset->m_AFFContribution_New.NbValue() == 0)
			{
				// Aucune grandeur contrib sélectionnée
				// yGrandeur = m_pATPDocCreuset->m_PlateauSupports.GetCount();
				yGrandeur = m_pATPDocCreuset->OnNbLigPlanActif();
			}
			else
			{
				// Cas de plusieurs grandeurs contribs sélectionnées
				// yGrandeur = m_pATPDocCreuset->m_PlateauSupports.GetCount() * m_pATPDocCreuset->m_AFFContribution_New.NbValue();
				yGrandeur = m_pATPDocCreuset->OnNbLigPlanActif() * m_pATPDocCreuset->m_AFFContribution_New.NbValue();

				// On ajoute les lignes espaces entre bloc de résultats
				if (m_pATPDocCreuset->m_AFFContribution_New.GetModeAffich() == m_pATPDocCreuset->m_AFFContribution_New.MODEAFFICH::AFFICH_BLOC_SUPPORTS)
				{
					// Mode bloc par supports
					yGrandeur += m_pATPDocCreuset->m_AFFContribution_New.NbValue() - 1;
				}
				else
				{
					// Mode bloc par grandeurs
					// yGrandeur += m_pATPDocCreuset->m_PlateauSupports.GetCount() - 1;
					 yGrandeur += m_pATPDocCreuset->OnNbLigPlanActif() - 1;
				}
			}
		}
	}

	// on met à jour le composant
	this->SetHorzCellCount(nbFormat, FALSE);
	if(nbFormat!=0)
	{
		this->SetHorzCount(xHyp, FALSE);
		this->SetHorzItemSize(nbFormat*CATPResultHypo_Metriques::CX_Bx(), TRUE, FALSE);
	}
	else
	{
		this->SetHorzCount(0, FALSE);
		this->SetHorzItemSize(CATPResultHypo_Metriques::CX_Bx(), TRUE, FALSE);
	}
	this->SetVertCount(yGrandeur, TRUE);



}

///////////////////////////////////////
// CATPResultHypo_B2 message handlers

void CATPResultHypo_B2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPResultHypo_Metriques::CX_Bx(), TRUE, FALSE);
	this->SetVertItemSize(CATPResultHypo_Metriques::CY_x1(), TRUE, FALSE);
}

int CATPResultHypo_B2::ConvertICellToFormat(int iCell)
{
	m_mapICellFormat.MoveTo(iCell);
	if(m_mapICellFormat.IsValid())
		return m_mapICellFormat.GetItem();

	return -1;

}