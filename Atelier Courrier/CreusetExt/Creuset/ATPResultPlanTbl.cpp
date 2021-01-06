// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATPResultHypo_Metriques.h"
#include "ATPResult_Grandeurs.h"
#include "ATPResultPlanTbl.h"
#include "resource.h"
#include "Courbes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////
// le constructeur

CATPResultPlanTbl::CATPResultPlanTbl()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

///////////////////////////////////////
// la fonction pour d�finir le document

JVoid CATPResultPlanTbl::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;
	m_ResultGrandeurs.SetDocument(m_pATPDocCreuset);
	// on met � jour le composant
	//this->OnUpdate();

}

//////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPResultPlanTbl::Update()
{
	// on met � jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPResultPlanTbl::~CATPResultPlanTbl()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPResultPlanTbl, JFCGrid)
	//{{AFX_MSG_MAP(CATPResultPlanTbl)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////
// la fonction pour dessiner l'int�rieur des cellules

JVoid CATPResultPlanTbl::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	CBrush bruchRed;
	bruchRed.CreateSolidBrush(RGB(250,150,150));

	// Attention on n'affiche pas la couverture si p�riode tablier != p�riode d'action
	JDate DateD = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
	JDate DateF = m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF();

	JDate DateDT = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD();
	JDate DateFT = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF();

	// on cr�e le texte
	CString Text;
	if (i == 0)
	{
		// on calcule le rectangle
		RECT Rect; Rect.left = rect.left - 3; Rect.right = rect.right + 3; Rect.top = rect.top - 2; Rect.bottom = rect.bottom + 1;

		// R�sultats g�n�raux (Couverture, GRP, Budget etc....)
		if (!m_pATPDocCreuset->m_AFFDistributionN && !m_pATPDocCreuset->m_AFFDistributionC)
		{
			// Libell� Budget KEuros
			if (m_ResultGrandeurs.IsBudget(j))
				Text.LoadString(IDS_CR_BUDGETACTIF);

			if (m_ResultGrandeurs.IsBudgetFormat(j))
			{
				m_pATPDocCreuset->m_Offre.m_TBLIdFormats.MoveTo(j-1);
				const JSRCPresse  & srcpresse = JSRCPresse::GetInstance();
				
				srcpresse.m_TBLFormats.MoveTo(m_pATPDocCreuset->m_Offre.m_TBLIdFormats.GetItem()); // R�cup item correspondant � l'indice
				if(srcpresse.m_TBLFormats.IsValid())
				{
					JFormat *pElem = (JFormat *)srcpresse.m_TBLFormats.GetItem();
					if(pElem)
					{
						Text.LoadString(IDS_CR_BUDGET);
						Text += " ";
						Text += pElem->GetLabelCourt().AsJCharPtr();  
					}
				}
			}

			// Libell� Nb indivs touch�s
			if (m_ResultGrandeurs.IsNbIndividus(j))
				Text.LoadString(IDS_CR_AUDIENCE);
			// Libell� Nb de contacts distribu�es
			if (m_ResultGrandeurs.IsNbContacts(j))
				Text.LoadString(IDS_CR_CONTACTS);
			// Libell� Nb de contacts plus
			if (m_ResultGrandeurs.IsNbCtsPlus(j))
				Text.LoadString(IDS_CR_CONTACTSPLUS_ABR);
			// Libell� GRP
			if (m_ResultGrandeurs.IsGRP(j))
				Text.LoadString(IDS_CR_GRP);
			// Libell� X-GRP
			if (m_ResultGrandeurs.IsXGRP(j))
				Text.LoadString(IDS_CR_GRPACTIF);
			// Libell� Couverture
			if (m_ResultGrandeurs.IsCouverture(j))
				Text.LoadString(IDS_CR_COUVERTURE);
			// Libell� R�p�tition
			if (m_ResultGrandeurs.IsRepetition(j))
				Text.LoadString(IDS_CR_REPET);
			// Libell� R�p�tition[+]
			if (m_ResultGrandeurs.IsRepetPlus(j))
				Text.LoadString(IDS_CR_REPETPLUS);
			// Libell� Couverture m�moris�e
			if (m_ResultGrandeurs.IsCouvMem(j))
				Text.LoadString(IDS_CR_COUVMEM);
			// Libell� Cout GRP
			if (m_ResultGrandeurs.IsCoutGRP(j))
				Text.LoadString(IDS_CR_CTGRP);
			// Libell� Cout Couverture
			if (m_ResultGrandeurs.IsCoutCouv(j))
				Text.LoadString(IDS_CR_CTCOUV);
			// Libell� Cout / 1000 Indiv
			if (m_ResultGrandeurs.IsCout1000Ind(j))
				Text.LoadString(IDS_CR_CT1000I);
			// Libell� Cout GRP 1000 Contact
			if (m_ResultGrandeurs.IsCout1000Cts(j))
				Text.LoadString(IDS_CR_CT1000C);
			
		}

		if(m_pATPDocCreuset->m_AFFDistributionC)
		{
			// Distribution de contacts
			if (j == 20)
				// Libell� des non touch�s
				Text.LoadString(IDS_CR_DISTRIBNT);

			else 
			{
				// touch�s au moins 1 fois (1 fois, 2 fois etc ... jusqu'� 20+)
				if (j == 0)
					Text.LoadString(IDS_CR_DISTRIB1ABR);
				else if (j < 19) 
					Text.FormatMessage(IDS_CR_DISTRIBNABR,j+1);
				else
					Text.FormatMessage(IDS_CR_DISTRIBPABR,j+1);
			}
		}

		if(m_pATPDocCreuset->m_AFFDistributionN)
		{
			// Distribution de contacts
			if (j == 20)
				// Libell� des non touch�s
				Text.LoadString(IDS_CR_DISTRIBNTABR);

			else 
			{
				// touch�s au moins 1 fois (1 fois, 2 fois etc ... jusqu'� 20+)
				if (j == 0)
					Text.LoadString(IDS_CR_DISTRIBNETTE1);
				else if (j < 19) 
					Text.FormatMessage(IDS_CR_DISTRIBNETTENABR,j+1);
				else
					Text.FormatMessage(IDS_CR_DISTRIBNETTEPABR,j+1);
			}
		}
		// on �crit le texte
		dc.DrawText(Text, &rect, DT_LEFT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);	

	}
	if (i==1)
	{
		if (m_pATPDocCreuset->IsValid())
		{
			// on recherche l'interrogation
			m_pATPDocCreuset->m_ITRHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseActif);

			// on teste si on a trouv� l'hypoth�se
			if (m_pATPDocCreuset->m_ITRHypotheses.IsValid() && m_pATPDocCreuset->m_ITRHypotheses.GetItem().IsValid() )
			{
				// on r�cup�re l'hypoth�se
				const JITPDATHypoFormat & DATHypoFormat = m_pATPDocCreuset->m_ITRHypotheses.GetItem();
				const JITPDATHypothese  & DATHypothese	= DATHypoFormat.GetDATHypoFormatActif();
				
				if (!m_pATPDocCreuset->m_AFFDistributionN && !m_pATPDocCreuset->m_AFFDistributionC)
				{
					// -------------------- Budget global ---------------------
					if (m_ResultGrandeurs.IsBudget(j))
					{
						// on r�cup�re le budget
						JFlt64 Budget = DATHypothese.m_Budget;

						if (Budget)
						{
							CString Fmt;
							if(Budget > 1000000)
							{
								Text.Format("%.0f", Budget/1000.0); 
								// on formate le budget
								Fmt = _T("### ### ### ### ### K� ");
							}
							else
							{	Text.Format("%.0f", Budget); 
								// on formate le budget
								Fmt = _T("### ### ### ### ### � ");
							}
							CFormatStr::FormatNum(Fmt, Text);
						}
						else
							Text.Format("* ");

						if(! DATHypothese.m_budgetOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}

					}
					// ------------------------ Budget par format --------------------------
					else if (m_ResultGrandeurs.IsBudgetFormat(j))
					{
						//m_nbFormat = m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(;

						m_pATPDocCreuset->m_Offre.m_TBLIdFormats.MoveTo(j-1);
						JInt32 idFormat = m_pATPDocCreuset->m_Offre.m_TBLIdFormats.GetItem();
						JFlt64 Budget =  DATHypoFormat.GetDATHypoByFormat(idFormat).m_Budget;
						
				
						// on r�cup�re le budget
						//JFlt64 Budget = DATHypothese.m_BudgetFormat.Item(j-1);
						if (Budget)
						{
							CString Fmt;
							if(Budget > 1000000)
							{
								Text.Format("%.0f", Budget/1000.0); 
								// on formate le budget
								Fmt = _T("### ### ### ### ### K� ");
							}
							else
							{	Text.Format("%.0f", Budget); 
								// on formate le budget
								Fmt = _T("### ### ### ### ### � ");
							}
							CFormatStr::FormatNum(Fmt, Text);
						}
						else
							Text.Format("* ");

						//if(! DATHypothese.m_budgetOK)
						if(! DATHypoFormat.GetDATHypoByFormat(idFormat).m_budgetOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
					}

					// ------------------------ GRP --------------------------
					else if (m_ResultGrandeurs.IsGRP(j))
					{
						// on r�cup�re le nombre de contacts et l'effectif
						JFlt64 Audience = DATHypothese.m_AudienceRef;
						JFlt64 Effectif   = DATHypoFormat.m_Effectif;

						// on teste si on peut calculer le GRP
						if (Effectif > 0.0 && Audience > 0.0)
							// on formate le nombre de contacts
							Text.Format(_T("%.1f "), (100.0 * Audience) / Effectif);
						else
							// on formate une �toile
							Text.Format(_T("* "));

						CString Fmt = _T("### ### ### ### ###,#");
						CFormatStr::FormatNum(Fmt, Text);

						if(! DATHypothese.m_ResultOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
					}
					
					// ------------------------ X - GRP --------------------------
					else if (m_ResultGrandeurs.IsXGRP(j))
					{
						// on r�cup�re le nombre de contacts et l'effectif
						JFlt64 VolLecture = DATHypothese.m_VolLecture;
						JFlt64 Effectif   = DATHypoFormat.m_Effectif;

						// on teste si on peut calculer le GRP
						if (Effectif > 0.0 && VolLecture > 0.0)
							// on formate le nombre de contacts
							Text.Format(_T("%.1f "), (100.0 * VolLecture) / Effectif);
						else
							// on formate une �toile
							Text.Format(_T("* "));

						CString Fmt = _T("### ### ### ### ###,#");
						CFormatStr::FormatNum(Fmt, Text);
						if(! DATHypothese.m_ResultOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
					}

					// -------------------- Couverture -----------------------
					else if (m_ResultGrandeurs.IsCouverture(j))
					{
						// on formate la couverture	
						if(DATHypothese.m_Couverture > 0.0 && DATHypoFormat.m_Effectif>0.0 && DateD == DateDT && DateF == DateFT)
							Text.Format(_T("%.1f %% "), 100.0 * DATHypothese.m_Couverture / DATHypoFormat.m_Effectif);
						else
							Text.Format(_T("* "));
						CString Fmt = _T("### ### ### ### ###,#");
						CFormatStr::FormatNum(Fmt, Text);

						if(! DATHypothese.m_ResultOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
					}

					// -------------------- Couverture M�moris�e -----------------------
					else if (m_ResultGrandeurs.IsCouvMem(j))
					{
						int beta = AfxGetApp()->GetProfileInt("Moteur", "CourbeReponse", 0);
						CCourbes * courbes = CCourbes::GetInstance();
						double vol = 0.0;
						double DistrFreq;

						// on r�cup�re le nombre de jours
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
						if(vol> 0.0 && DATHypoFormat.m_Effectif>0.0 && DateD == DateDT && DateF == DateFT)
							Text.Format(_T("%.1f %% "), 100.0 * vol / DATHypoFormat.m_Effectif);
						else
							Text.Format(_T("* "));
						CString Fmt = _T("### ### ### ### ###,#");
						CFormatStr::FormatNum(Fmt, Text);
						if(! DATHypothese.m_ResultOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
					}

					// -------------------- Co�t GRP -----------------------
					else if (m_ResultGrandeurs.IsCoutGRP(j))
					{
						// on r�cup�re le budget, nombre de contacts et l'effectif
						JFlt64 Budget     = DATHypothese.m_Budget;
						JFlt64 Audience = DATHypothese.m_AudienceRef;
						JFlt64 Effectif   = DATHypoFormat.m_Effectif;

						// on teste si on peut calculer le co�t / GRP
						if (Audience > 0.0 && Effectif > 0.0 && Budget > 0.0)
						{
							// on formate le Co�t GRP	
							Text = CFormatStr::FormatCout((Budget * Effectif) / (100.0 * Audience), m_pATPDocCreuset->m_NbDecCout);
							//Text.Format(_T("%.0f"), (Budget * Effectif) / (100.0 * Audience));
							//CString Fmt = _T("### ### ### ### ### � ");
							//CFormatStr::FormatNum(Fmt, Text);
						}
						else
							// on formate une �toile
							Text.Format(_T("* "));
						if(! DATHypothese.m_budgetOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
						if(! DATHypothese.m_ResultOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
					}

					// -------------------- Co�t Couverture -----------------------
					else if (m_ResultGrandeurs.IsCoutCouv(j))
					{
						// on r�cup�re le budget, nombre de contacts et l'effectif
						JFlt64 Budget     = DATHypothese.m_Budget;
						JFlt64 Couv = DATHypothese.m_Couverture;
						JFlt64 Effectif   = DATHypoFormat.m_Effectif;

						// on teste si on peut calculer le co�t couverture
						if (Couv > 0.0 && Effectif > 0.0 && Budget > 0.0 && DateD == DateDT && DateF == DateFT)
						{
							// on formate le Co�t couverture	
							Text = CFormatStr::FormatCout((Budget * Effectif) / (100.0 * Couv), m_pATPDocCreuset->m_NbDecCout);
							//Text.Format(_T("%.0f"), (Budget * Effectif) / (100.0 * Couv));
							//CString Fmt = _T("### ### ### ### ### � ");
							//CFormatStr::FormatNum(Fmt, Text);
						}
						else
							// on formate une �toile
							Text.Format(_T("* "));
						if(! DATHypothese.m_budgetOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
						if(! DATHypothese.m_ResultOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
					}

					// -------------------- Co�t /1000 contacts -----------------------
					else if (m_ResultGrandeurs.IsCout1000Cts(j))
					{
						// on r�cup�re le budget, nombre de contacts et l'effectif
						JFlt64 Budget     = DATHypothese.m_Budget;
						JFlt64 VolLecture = DATHypothese.m_VolLecture;

						// on teste si on peut calculer le Co�t /1000 contacts
						if (VolLecture > 0.0 && Budget > 0.0)
						{
							// on formate le Co�t /1000 contacts
							Text = CFormatStr::FormatCout((1000.0 * Budget) / VolLecture, m_pATPDocCreuset->m_NbDecCout);
							//Text.Format(_T("%.2f � "), (1000.0 * Budget) / VolLecture);
							//CString Fmt = _T("### ### ### ### ###,##");
							//CFormatStr::FormatNum(Fmt, Text);
						}
						else
							// on formate une �toile
							Text.Format(_T("* "));

						if(! DATHypothese.m_budgetOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
						if(! DATHypothese.m_ResultOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
					}

					// -------------------- Co�t /1000 indiv -----------------------
					else if (m_ResultGrandeurs.IsCout1000Ind(j))
					{
						// on r�cup�re le budget, nombre de contacts et l'effectif
						JFlt64 Budget     = DATHypothese.m_Budget;
						// on r�cup�re la couverture cumul�e � 1+
						JFlt64 Couv = DATHypothese.m_Couverture;


						// on teste si on peut calculer le Co�t /1000 indiv
						if (Couv > 0.0 && Budget > 0.0)
						{
							// on formate le Co�t /1000 indiv
							Text = CFormatStr::FormatCout((1000.0 * Budget) / Couv, m_pATPDocCreuset->m_NbDecCout);
							//Text.Format(_T("%.2f � "), (1000.0 * Budget) / Couv);
							//CString Fmt = _T("### ### ### ### ###,##");
							//CFormatStr::FormatNum(Fmt, Text);
						}
						else
							// on formate une �toile
							Text.Format(_T("* "));

						if(! DATHypothese.m_budgetOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
						if(! DATHypothese.m_ResultOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
					}

					// -------------------- R�p�tition -----------------------
					else if (m_ResultGrandeurs.IsRepetition(j))
					{
						// on r�cup�re le nombre de contacts et l'effectif
						JFlt64 Audience = DATHypothese.m_AudienceRef;
						//JFlt64 Effectif   = DATHypoFormat.m_Effectif;

						// on r�cup�re la couverture cumul�e � 1+
						JFlt64 Couv =  DATHypothese.m_Couverture;

						// on teste si on peut calculer la r�p�tition
						if (Couv > 0.0  && DateD == DateDT && DateF == DateFT)
							// on formate la repetition	
							Text.Format(_T("%.2f "), Audience / Couv);
						else
						{
							// on formate une �toile
							Text.Format(_T("* "));
						}
						if(! DATHypothese.m_ResultOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
					}

					// -------------------- R�p�tition [+] -----------------------
					else if (m_ResultGrandeurs.IsRepetPlus(j))
					{
						// on r�cup�re le nombre de contacts et l'effectif
						JFlt64 VolLecture = DATHypothese.m_VolLecture;
						//JFlt64 Effectif   = DATHypoFormat.m_Effectif;

						// on r�cup�re la couverture cumul�e � 1+
						JFlt64 Couv =  DATHypothese.m_Couverture;

						// on teste si on peut calculer la r�p�tition[+]
						if (Couv > 0.0 && DateD == DateDT && DateF == DateFT)
							// on formate la repetition	[+]
							Text.Format(_T("%.2f "), VolLecture / Couv);
						else
						{
							// on formate une �toile
							Text.Format(_T("* "));
						}
						if(! DATHypothese.m_ResultOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
					}

					// -------------------- Audience (Nb Individus touch�s)-----------------------
					else if (m_ResultGrandeurs.IsNbIndividus(j))
					{

						// on r�cup�re la couverture cumul�e � 1+
						JFlt64 Couv = DATHypothese.m_Couverture;

						// on formate la couverture	
						if (Couv > 0.0 && DateD == DateDT && DateF == DateFT)
							Text.Format(_T("%.0f"), Couv /1000.0);
						else
							Text.Format(_T("*"));

						CString Fmt = _T("### ### ### ### ### ");
						CFormatStr::FormatNum(Fmt, Text);
						if(! DATHypothese.m_ResultOK)
						{
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
					}

					// -------------------- Nombre de contact distribu� -----------------------
					else if (m_ResultGrandeurs.IsNbContacts(j))
					{
						// on r�cup�re le nombre de contacts
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
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
					}
					// -------------------- Nombre de contact distribu� -----------------------
					else if (m_ResultGrandeurs.IsNbCtsPlus(j))
					{
						// on r�cup�re le nombre de contacts
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
							dc.FillRect(&rect,&bruchRed);
							dc.SetTextColor(RGB(255,255,255));
						}
					}
				}

				if(m_pATPDocCreuset->m_AFFDistributionN)
				{
					
					// -------------------- Distribution de fr�quence Nette-----------------------
					// on r�cup�re le nombre de jours
     //               JInt32 idxF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF() - m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
	

					//// on r�cup�re la distribution de fr�quence
					//JFlt64 DistrFreq = 0.0; 
					//if (idxF > 0) 
					//{
					//	if (j == 20)
					//		DistrFreq = DATHypoFormat.m_Effectif - DATHypothese.m_DistributionJour.Item(idxF).Item(1).AsJFlt64()  ;
					//	if (j == 19)
					//		DistrFreq = DATHypothese.m_DistributionJour.Item(idxF).Item(j+1).AsJFlt64()  ;
					//	if (j < 19)
					//		DistrFreq = (DATHypothese.m_DistributionJour.Item(idxF).Item(j+1).AsJFlt64() - DATHypothese.m_DistributionJour.Item(idxF).Item(j+2).AsJFlt64()) ;
					//	DistrFreq *= 100.0/ DATHypoFormat.m_Effectif;
					//}

					// en attente valeur distr. fr�q
					if (j != 20)
					{
						int beta = AfxGetApp()->GetProfileInt("Moteur", "CourbeReponse", 0);
						CCourbes * courbes = CCourbes::GetInstance();
						courbes->GetProba(j+1,beta);

						Text.Format(_T("%.0f"), 100.0 * courbes->GetProba(j+1, beta));
					}
					//dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);				

				}

				if(m_pATPDocCreuset->m_AFFDistributionC)
				{
					
					// -------------------- Distribution de fr�quence Cumul�e-----------------------
					// on r�cup�re le nombre de jours
                    JInt32 idxF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF() - m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();

					// on r�cup�re la distribution de fr�quence
					JFlt64 DistrFreq = 0.0; 
					if (idxF > 0) 
					{
						if (j == 20)
							DistrFreq = 1.0 - (DATHypothese.m_DistributionJour.Item(idxF).Item(1).AsJFlt64()  / DATHypoFormat.m_Effectif);
						else
							DistrFreq = DATHypothese.m_DistributionJour.Item(idxF).Item(j+1).AsJFlt64() / DATHypoFormat.m_Effectif;
						DistrFreq *= 100.0;
					}

					// en attente valeur distr. fr�q
					if (DistrFreq*10 > 1)
						Text.Format(_T("%.1f %% "), DistrFreq);

					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&rect,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
				}

				// on �crit le texte
				dc.DrawText(Text, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);				
			}
		}
	}
	if (i==2)
	{
		if (m_pATPDocCreuset->IsValid())
		{
			// on recherche l'interrogation
			m_pATPDocCreuset->m_ITRHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseActif);

			// on teste si on a trouv� l'hypoth�se
			if (m_pATPDocCreuset->m_ITRHypotheses.IsValid() && m_pATPDocCreuset->m_ITRHypotheses.GetItem().IsValid())
			{
				// on r�cup�re l'hypoth�se
				const JITPDATHypoFormat & DATHypoFormat = m_pATPDocCreuset->m_ITRHypotheses.GetItem();
				const JITPDATHypothese & DATHypothese = DATHypoFormat.GetDATHypoFormatActif();
				if(m_pATPDocCreuset->m_AFFDistributionN)
				{
					
					// -------------------- Distribution de fr�quence Nette-----------------------
					// on r�cup�re le nombre de jours
                    JInt32 idxF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF() - m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD();
	

					// on r�cup�re la distribution de fr�quence
					JFlt64 DistrFreq = 0.0; 
					if (idxF > 0) 
					{
						if (j == 20)
							DistrFreq = DATHypoFormat.m_Effectif - DATHypothese.m_DistributionJour.Item(idxF).Item(1).AsJFlt64()  ;
						if (j == 19)
							DistrFreq = DATHypothese.m_DistributionJour.Item(idxF).Item(j+1).AsJFlt64()  ;
						if (j < 19)
							DistrFreq = (DATHypothese.m_DistributionJour.Item(idxF).Item(j+1).AsJFlt64() - DATHypothese.m_DistributionJour.Item(idxF).Item(j+2).AsJFlt64()) ;
						DistrFreq *= 100.0/ DATHypoFormat.m_Effectif;
					}

					// en attente valeur distr. fr�q
					//int beta = AfxGetApp()->GetProfileInt("Moteur", "CourbeReponse", 0);
					//CCourbes * courbes = CCourbes::GetInstance();
					//courbes->GetProba(j+1,beta);

					//if (DistrFreq*10 > 1)
					//	Text.Format(_T("%.0f %.1f %% "), 100.0 * courbes->GetProba(j+1, beta), DistrFreq);
					//else
					//	Text.Format(_T("%.0f"), 100.0 * courbes->GetProba(j+1, beta));
					
					if (DistrFreq*10 > 1)
						Text.Format(_T("%.1f %% "), DistrFreq);

					if(! DATHypothese.m_ResultOK)
					{
						dc.FillRect(&rect,&bruchRed);
						dc.SetTextColor(RGB(255,255,255));
					}
					dc.DrawText(Text, &rect, DT_RIGHT | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);	
				}
			}
		}
	}





}

////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPResultPlanTbl::OnLButtonDown(LONG i, LONG j)
{
}

JVoid CATPResultPlanTbl::OnRButtonDown(LONG i, LONG j)
{
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour

JVoid CATPResultPlanTbl::OnUpdate()
{
	// on initialise le nombre d'�l�ments
	LONG xGrandeur	= 0;
	LONG yGrandeur	= 0;

	// on v�rifie la validit� du document
	if (m_pATPDocCreuset != 0)
	{
		// Les r�sultats g�n�raux (Couverture, GRP, Budget etc....)
		yGrandeur = m_ResultGrandeurs.GetNbGrandeurs();
		xGrandeur = 2;
		if (m_pATPDocCreuset->m_AFFDistributionN)
		{
			// Distribution de contacts
			yGrandeur = m_ResultGrandeurs.GetNbGrandeursDF();
			xGrandeur = 3;
		}
		if (m_pATPDocCreuset->m_AFFDistributionC)
		{
			// Distribution de contacts
			yGrandeur = m_ResultGrandeurs.GetNbGrandeursDF();
			xGrandeur = 2;
		}
	}

	// on met � jour le composant
	this->SetHorzCount(xGrandeur, TRUE);
	RECT Rect; this->GetClientRect(&Rect);
	this->SetHorzItemSize((Rect.right  - Rect.left)  /  xGrandeur, TRUE, TRUE);
	this->SetVertCount(yGrandeur, TRUE);
}

///////////////////////////////////////
// CATPResultPlanTbl message handlers

void CATPResultPlanTbl::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGrid::PreSubclassWindow();

	// on r�cup�re les dimensions du composant
	RECT Rect; this->GetClientRect(&Rect);

	// on initialise la taille des cellules
	this->SetHorzItemSize(((Rect.right  - Rect.left) +  2) /  2, TRUE, FALSE);
	this->SetVertItemSize(CATPResultHypo_Metriques::CY_x1(), TRUE, FALSE);

	//this->SetVertItemSize(((Rect.bottom - Rect.top ) + 10) / 11, TRUE, FALSE);

	// on initialise le nombre de cellules
	//this->SetVertCount(21, FALSE);
	//this->SetHorzCount(3,  FALSE);
}
