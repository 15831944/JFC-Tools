// on inclut les définitions nécesaires
#include "stdafx.h"
#include "ATPCreusetHypo_Metriques.h"
#include "ATPCreusetHypo_C2.h"
#include "ATPCreusetHypo.h"
#include ".\atpcreusethypo_c2.h"
#include "AjoutInsertion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define IsSHIFTpressed() ( (GetKeyState(VK_SHIFT) & (1 << (sizeof(SHORT)*8-1))) != 0   )
#define IsCTRLpressed()  ( (GetKeyState(VK_CONTROL) & (1 << (sizeof(SHORT)*8-1))) != 0 )

//////////////////
// le constructeur

CATPCreusetHypo_C2::CATPCreusetHypo_C2()
{
	// on initialise le document
	m_pATPDocCreuset = 0;
	m_prev_i = -1; 
	m_prev_ii = -1;
	m_prev_j = -1;

	// Init ligne et colonne édition cellule
	m_RowCellEdit = -1;
	m_ColCellEdit = -1;

	// Init Nombre insertions current edit cellule
	m_NbInsCurEdit = 0;

	// Format courant (1er format)
	m_NoColFormatCur = 1;
}

///////////////////////////////////////
// la fonction pour définir le document

JVoid CATPCreusetHypo_C2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// Ce grid utilisera le fonctionnement editbox
	this->SetAccesEditBox(true);

	// on met à jour le composant
	this->OnUpdate();
}

JVoid CATPCreusetHypo_C2::SetParent(CATPCreusetHypo* pATPCreusetHypo)
{
	m_pATPCreusetHypo = pATPCreusetHypo;
}
//////////////////////////////////////////////
// la fonction pour mettre à jour le composant

JVoid CATPCreusetHypo_C2::Update()
{
	// on met à jour le composant
	this->OnUpdate();
}

/////////////////
// le destructeur

CATPCreusetHypo_C2::~CATPCreusetHypo_C2()
{
	// on ne fait rien
}

BEGIN_MESSAGE_MAP(CATPCreusetHypo_C2, JFCGridMultiCell)
	//{{AFX_MSG_MAP(CATPCreusetHypo_C2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



////////////////////////////////////////////////////
// les fonctions pour capter les clicks de la souris

JVoid CATPCreusetHypo_C2::OnLButtonDown(LONG i, LONG ii, LONG j)
{
	//Selon l'état des btn SHIFT ou ctrl pas le mm nombre d'insertion
	int NbAjoutIns = 1;

	// Avec la touche Shift, 5 insertions ajoutées d'un coup 
	if (IsSHIFTpressed())
		NbAjoutIns = 5;

	// Avec la touche Ctrl, 1 insertions ajoutées d'un coup 
	if (IsCTRLpressed())
		// NbAjoutIns = 10;
		NbAjoutIns = 1;

	// Ajoute insertion
	AjoutInsertionCellule(i,ii,j,NbAjoutIns);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ajoute nb insertion dans la cellule correspondante
void CATPCreusetHypo_C2::AjoutInsertionCellule(LONG NoHypo, LONG NoFormat, LONG NoSupport, int NbAjoutIns)
{
	// Ne rien faire si aucune insertion à ajouter
	if (NbAjoutIns == 0) return;

	// Test si limite nb hypothèse atteinte
	if (NoHypo + 1 > m_pATPDocCreuset->m_MaxHypothese) return;

	// si on ajoute un spot à la dernière hypothèse, on en ajoute une nouvelle
	if (NoHypo + 1 == m_pATPDocCreuset->m_TblHypotheses.GetCount()) 
		m_pATPDocCreuset->AddNewHypo();

	// Récup ident format courant sur cette cellule
	JUnt32 IdFormatCur = ConvertICellToFormat(NoFormat);
	m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(IdFormatCur);

	// Voir si hypothèse non valide on la crée
	m_pATPDocCreuset->m_TblHypotheses.MoveTo(NoHypo + 1);
	if (m_pATPDocCreuset->m_TblHypotheses.IsValid() && m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
	{
		// Mise à jour hypothèse courante
		m_pATPDocCreuset->m_IdxHypotheseCourant = NoHypo + 1;

		// Cle du support
		CKeySupport KeySupp;
		JUnt32 IdSupport;

		//// on récupère l'identifiant du Support
		// Mode plateau
		KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(NoSupport);
		IdSupport = KeySupp.m_IdSupport;
		
		if(KeySupp.m_IdParent)
			return;

		// Support actif et ligne active des support
		m_pATPCreusetHypo->m_IdSupportActif    = IdSupport;
		//m_pATPCreusetHypo->m_NoLigSupportActif = m_pATPDocCreuset->OnRecupLigneSuppActif(m_pATPCreusetHypo->m_IdSupportActif); 


		// Ajouter une insertion sur ce support et cette hypothèse
		// 1 / On ajoutera d'abord sur un emplacement vide sur la période d'action
		// 2 / Sinon on empile sur la dernière insertion possible déjà existante

		JUnt32 Ret = 0;

		// Ajoute le nombre d'insertions
		for (int k = 0; k< NbAjoutIns; k++)
		{
			// Ajout insertion pour ce format
			if (CAjoutInsertion::AjoutInsertionSuccessif(IdSupport,IdFormatCur,NoHypo + 1,m_pATPDocCreuset))
			{ 
				// Support actif et récup ligne correspondante support actif
				m_pATPCreusetHypo->m_IdSupportActif = IdSupport;

				// Récup ligne active selon mode visualisation (offre ou plateau)
				//m_pATPDocCreuset->OnRecupLigneSuppActif(IdSupport);

				// si on ajoute un spot à la dernière hypothèse, on en ajoute une nouvelle
				if (m_pATPDocCreuset->m_IdxHypotheseCourant <  m_pATPDocCreuset->m_MaxHypothese
					&& m_pATPDocCreuset->m_IdxHypotheseCourant  == m_pATPDocCreuset->m_TblHypotheses.GetCount() - 1) 
				{
					m_pATPDocCreuset->AddNewHypo();
				}
				// Modif active
				Ret = 1;
			}
		}

		// Si insertion(s) effectuée(s) on met à jour le document
		if (Ret)
		{
			m_pATPDocCreuset->SetModifiedFlag(TRUE); 
			m_pATPDocCreuset->m_AFFCreuset.SetPlanActif(false);
			m_pATPDocCreuset->m_AFFCreuset.SetHypoActif(true);
			//Update();
			Invalidate();
			UpdateWindow();
			//
			// m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO);   /// A OPTIMISER !!!!! OPTIM 2.7

			m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO_LIGHT);   

			////////////////////////////////////////////////////////////////////////////////////////////
			// Init Apports
			// on récupère le nombre de Supportx
			JInt32 NbSupports = m_pATPDocCreuset->m_EchelleSupports.GetCount();

			// Voir si hypothèse courante valide
			m_pATPDocCreuset->m_ITRHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);

			// on boucle sur tous les Supportx
			for (JInt32 IdxSupport = 0; IdxSupport < NbSupports; IdxSupport += 1)
			{
				// on récupère l'identifiant du Support
				const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(IdxSupport);

				// on init l'apport
				m_pATPDocCreuset->m_ITREchelle.SetApport(IdSupport,-1);
			}
			m_pATPDocCreuset->m_ApportARecalculer = true;
			////////////////////////////////////////////////////////////////////////////////////////////

			// Calcul nouvelle contribution
			ComputeContrib(NoHypo, NoFormat, NoSupport);
		}
	}	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Ajout ou supp insertions par bloc de lignes
void CATPCreusetHypo_C2::AjoutOuSuppInsertionBlocRow(LONG Col, LONG RowDeb, LONG RowFin, LONG NoFormatCur, int NbFinalIns)
{
	JUnt32 Ret = 0;

	// Flag Suppression effectuée
	JBool FlagSuppInsertion = false;

	for (LONG Row = RowDeb; Row <= RowFin; Row++)
	{
		// Recup no format
		LONG NoFormat = NoFormatCur;

		// Recup nb insertion précédent sur cette cellule
		JUnt32 NbInsCell = GetNbInsertion(Col, NoFormat, Row);

		// Voir si on en ajoute ou si on en supprime
		if (NbInsCell <= NbFinalIns)
		{	
			// Il faut ajouter des insertions
			JUnt32 NbAjoutIns = NbFinalIns - NbInsCell; 

			// AjoutInsertionCellule(Col, NoFormat, Row, NbAjoutIns);

			// Ne rien faire si aucune insertion à ajouter
			if (NbAjoutIns == 0) continue;

			// Test si limite nb hypothèse atteinte
			if (Col + 1 > m_pATPDocCreuset->m_MaxHypothese) return;

			// si on ajoute un spot à la dernière hypothèse, on en ajoute une nouvelle
			if (Col + 1 == m_pATPDocCreuset->m_TblHypotheses.GetCount()) 
				m_pATPDocCreuset->AddNewHypo();

			// Récup ident format courant sur cette cellule
			JUnt32 IdFormatCur = ConvertICellToFormat(NoFormat);
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(IdFormatCur);

			// Voir si hypothèse non valide on la crée
			m_pATPDocCreuset->m_TblHypotheses.MoveTo(Col + 1);
			if (m_pATPDocCreuset->m_TblHypotheses.IsValid() && m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
			{
				// Mise à jour hypothèse courante
				m_pATPDocCreuset->m_IdxHypotheseCourant = Col + 1;

				// Cle du support
				CKeySupport KeySupp;
				JUnt32 IdSupport;

				//// on récupère l'identifiant du Support
				// Mode plateau
				KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(Row);
				IdSupport = KeySupp.m_IdSupport;

				if(KeySupp.m_IdParent)
					return;

				// Support actif et ligne active des support
				m_pATPCreusetHypo->m_IdSupportActif    = IdSupport;
				//m_pATPCreusetHypo->m_NoLigSupportActif = m_pATPDocCreuset->OnRecupLigneSuppActif(m_pATPCreusetHypo->m_IdSupportActif); 


				// Ajouter une insertion sur ce support et cette hypothèse
				// 1 / On ajoutera d'abord sur un emplacement vide sur la période d'action
				// 2 / Sinon on empile sur la dernière insertion possible déjà existante

				// Ajoute le nombre d'insertions
				for (int k = 0; k< NbAjoutIns; k++)
				{
					// Ajout insertion pour ce format
					if (CAjoutInsertion::AjoutInsertionSuccessif(IdSupport,IdFormatCur,Col + 1,m_pATPDocCreuset))
					{ 
						// Support actif et récup ligne correspondante support actif
						m_pATPCreusetHypo->m_IdSupportActif = IdSupport;

						// Récup ligne active selon mode visualisation (offre ou plateau)
						//m_pATPDocCreuset->OnRecupLigneSuppActif(IdSupport);

						// si on ajoute un spot à la dernière hypothèse, on en ajoute une nouvelle
						if (m_pATPDocCreuset->m_IdxHypotheseCourant <  m_pATPDocCreuset->m_MaxHypothese
							&& m_pATPDocCreuset->m_IdxHypotheseCourant  == m_pATPDocCreuset->m_TblHypotheses.GetCount() - 1) 
						{
							m_pATPDocCreuset->AddNewHypo();
						}
						// Modif active
						Ret = 1;
					}
				}
			}
		}
		else
		{
			// Il faut supprimer des insertions
			JUnt32 NbSupp = NbInsCell - NbFinalIns; 
			// SuppInsertionCellule(Col, NoFormat, Row, NbSupp);
			
			// Ne rien faire si aucune insertion à supprimer
			if (NbSupp == 0) return;

			// On se positionne sur format courant
			JUnt32 IdFormatCur = ConvertICellToFormat(NoFormat);
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(IdFormatCur);

			// Voir si hypothèse non valide on la crée
			m_pATPDocCreuset->m_TblHypotheses.MoveTo(Col + 1);
			if (m_pATPDocCreuset->m_TblHypotheses.IsValid() && m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
			{
				// Mise à jour hypothèse courante
				m_pATPDocCreuset->m_IdxHypotheseCourant = Col + 1;

				// on récupère l'identifiant du Support
				CKeySupport KeySupp;
				JUnt32 IdSupport;

				// Mode plateau
				KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(Row);
				IdSupport = KeySupp.m_IdSupport;

				if(KeySupp.m_IdParent)
					return;

				// Supprimer une insertion sur ce support et cette hypothèse
				// On supprime la dernière insertion de chacun des formats (voir via liste chrono insertions)
				// On boucle sur tous les formats actifs 
				// Selon l'état des btn SHIFT ou ctrl pas le mm nombre d'insertion
				for (int k = 0; k < NbSupp; k++)
				{
					// Supprime l'insertion
					CAjoutInsertion::SupprLastInsertion(IdSupport,IdFormatCur,Col+1,m_pATPDocCreuset);
					FlagSuppInsertion = true;

					// Support actif
					m_pATPCreusetHypo->m_IdSupportActif = IdSupport;
					m_pATPCreusetHypo->m_NoLigSupportActif = m_pATPDocCreuset->OnRecupLigneSuppActif(IdSupport);
				}

				// Si suppression insertion, test si hypothese avec au moins 1 insertion, sinon décalage
				// m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO); 
				m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO_LIGHT); 

				if (FlagSuppInsertion)
				{
					// on récupère le plan de l'hypothèse courante après suppression élémnt
					m_pATPDocCreuset->m_TblHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);
					CPlanInsertions & PLNInsert = m_pATPDocCreuset->m_TblHypotheses.GetItem();

					// Test si encore des insertions pour ce plan
					if (PLNInsert.GetCount() == 0 &&
						m_pATPDocCreuset->m_TblHypotheses.GetCount() > 2 &&
						m_pATPDocCreuset->m_IdxHypotheseCourant == m_pATPDocCreuset->m_TblHypotheses.GetCount()-2)
					{
						// Suppression de l'avant dernière hypothèse si vide / garde une seule hypothèse vierge
						m_pATPDocCreuset->UpdateAllViews(UPDATE_SUPP_HYPOTHESECUR);
					}
				}

				/*
				// on met à jour le document
				m_pATPDocCreuset->SetModifiedFlag(TRUE);

				// On recalcule les contributions
				ComputeContrib(NoHypo, NoFormat, NoSupport);
				*/
			}
		}
	}


	// Si insertion(s) effectuée(s) on met à jour le document
	if (Ret)
	{
		m_pATPDocCreuset->SetModifiedFlag(TRUE); 
		m_pATPDocCreuset->m_AFFCreuset.SetPlanActif(false);
		m_pATPDocCreuset->m_AFFCreuset.SetHypoActif(true);
		//Update();
		Invalidate();
		UpdateWindow();
		//
		// m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO);   /// A OPTIMISER !!!!! OPTIM 2.7

		m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO_LIGHT);   

		////////////////////////////////////////////////////////////////////////////////////////////
		// Init Apports
		// on récupère le nombre de Supportx
		JInt32 NbSupports = m_pATPDocCreuset->m_EchelleSupports.GetCount();

		// Voir si hypothèse courante valide
		m_pATPDocCreuset->m_ITRHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);

		// on boucle sur tous les Supportx
		for (JInt32 IdxSupport = 0; IdxSupport < NbSupports; IdxSupport += 1)
		{
			// on récupère l'identifiant du Support
			const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(IdxSupport);

			// on init l'apport
			m_pATPDocCreuset->m_ITREchelle.SetApport(IdSupport,-1);
		}
		m_pATPDocCreuset->m_ApportARecalculer = true;
		////////////////////////////////////////////////////////////////////////////////////////////
	}

	if (Ret || FlagSuppInsertion)
	{
		// Calcul nouvelle contribution
		for (LONG Row = RowDeb; Row <= RowFin; Row++)
		{
			ComputeContrib(Col, NoFormatCur, Row);
		}
	}
}

JBool CATPCreusetHypo_C2::AjoutSupportPlateau(CKeySupport KeySup)
{
	// on teste les exceptions
	try
	{
		// on ajoute le Support au plateau
		m_pATPDocCreuset->m_PlateauSupports.Add(KeySup);
	}
	catch (JException* pException)
	{
		// on affiche le message
		::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		return false;
	}
	
	// on positionne l'indicateur de modification
	m_pATPDocCreuset->SetModifiedFlag(TRUE);

	// on met à jour les vues du document
	m_pATPDocCreuset->UpdateAllViews(UPDATE_KEY_PLATEAU);

	return true;

}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Suppression insertion sur l'hypothese en cours
JVoid CATPCreusetHypo_C2::OnRButtonDown(LONG i, LONG ii, LONG j)
{
	//Selon l'état des btn SHIFT ou ctrl >> nombre d'insertion à supprimer
	int NbSuppIns = 0;

	// Avec la touche Shift, 5 insertions supprimées d'un coup 
	if (IsSHIFTpressed())
		NbSuppIns = 5;

	// Avec la touche Ctrl, 10 insertions supprimées d'un coup 
	if (IsCTRLpressed())
		NbSuppIns = 1;

	// Suppression n insertion
	if (NbSuppIns > 0)
		SuppInsertionCellule(i, ii, j, NbSuppIns);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Suppression nb insertion dans la cellule correspondante
void CATPCreusetHypo_C2::SuppInsertionCellule(LONG NoHypo, LONG NoFormat, LONG NoSupport, int NbSuppIns)
{
	// Ne rien faire si aucune insertion à supprimer
	if (NbSuppIns == 0) return;

	// On se positionne sur format courant
	JUnt32 IdFormatCur = ConvertICellToFormat(NoFormat);
	m_pATPDocCreuset->m_MapIdFormatsSel.MoveTo(IdFormatCur);

	// Voir si hypothèse non valide on la crée
	m_pATPDocCreuset->m_TblHypotheses.MoveTo(NoHypo + 1);
	if (m_pATPDocCreuset->m_TblHypotheses.IsValid() && m_pATPDocCreuset->m_MapIdFormatsSel.IsValid())
	{
		// Mise à jour hypothèse courante
		m_pATPDocCreuset->m_IdxHypotheseCourant = NoHypo + 1;
	
		// on récupère l'identifiant du Support
		CKeySupport KeySupp;
		JUnt32 IdSupport;

		// Mode plateau
		KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(NoSupport);
		IdSupport = KeySupp.m_IdSupport;
		
		if(KeySupp.m_IdParent)
			return;

		// Recherche nb insertions
		//JBool  SuppPossible;
		//JUnt32 NbInsertions = OnGetNbInsertions(IdSupport,i+1,SuppPossible);

		//if (NbInsertions)

		// Flag Suppression effectuée
		JBool FlagSuppInsertion = false;

		// Supprimer une insertion sur ce support et cette hypothèse
		// On supprime la dernière insertion de chacun des formats (voir via liste chrono insertions)
		// On boucle sur tous les formats actifs 
		// Selon l'état des btn SHIFT ou ctrl pas le mm nombre d'insertion
		
		for (int k = 0; k < NbSuppIns; k++)
		{

			// Supprime l'insertion
			CAjoutInsertion::SupprLastInsertion(IdSupport,IdFormatCur,NoHypo+1,m_pATPDocCreuset);
			FlagSuppInsertion = true;

			// Support actif
			m_pATPCreusetHypo->m_IdSupportActif = IdSupport;
			m_pATPCreusetHypo->m_NoLigSupportActif = m_pATPDocCreuset->OnRecupLigneSuppActif(IdSupport);
		}

		// Si suppression insertion, test si hypothese avec au moins 1 insertion, sinon décalage
		m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO); 
		if (FlagSuppInsertion)
		{
			// on récupère le plan de l'hypothèse courante après suppression élémnt
			m_pATPDocCreuset->m_TblHypotheses.MoveTo(m_pATPDocCreuset->m_IdxHypotheseCourant);
			CPlanInsertions & PLNInsert = m_pATPDocCreuset->m_TblHypotheses.GetItem();

			// Test si encore des insertions pour ce plan
			if (PLNInsert.GetCount() == 0 &&
				m_pATPDocCreuset->m_TblHypotheses.GetCount() > 2 &&
				m_pATPDocCreuset->m_IdxHypotheseCourant == m_pATPDocCreuset->m_TblHypotheses.GetCount()-2)
			{
				// Suppression de l'avant dernière hypothèse si vide / garde une seule hypothèse vierge
				m_pATPDocCreuset->UpdateAllViews(UPDATE_SUPP_HYPOTHESECUR);
			}
		}

		// on met à jour le document
		m_pATPDocCreuset->SetModifiedFlag(TRUE);

		// On recalcule les contributions
		ComputeContrib(NoHypo, NoFormat, NoSupport);
	}
}

BOOL CATPCreusetHypo_C2::PreTranslateMessage(MSG* pMsg)
{
	// TODO : ajoutez ici votre code spécialisé et/ou l'appel de la classe de base
	InitToolTip();
	
	m_tool.RelayEvent(pMsg);

	return JFCGridMultiCell::PreTranslateMessage(pMsg);
}

////////////////////////////////////////////////////////
// la fonction pour capter les déplacements de la souris

JVoid CATPCreusetHypo_C2::OnMouseMove(LONG i, LONG ii, LONG j)
{
	//on s'épargne le calcul si la souris ne change pas de cellule
	if (j == m_prev_j && i == m_prev_i && ii == m_prev_ii)
		return;
	
	m_pATPDocCreuset->m_AFFContribution.Reset();

	m_tool.Activate(FALSE);
	if (i == -1)
		return;

	CString strTipText;
	JInt32 IdFormatCur = ConvertICellToFormat(ii);
	CKeySupport KeySupp;
	JUnt32 IdSupport;
	//if (m_pATPDocCreuset->m_AFFOffreHypos) 
	//	// en mode offre globale
	//	//const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);
	//	IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);
	//else
	//{
		// Mode plateau
		KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(j);
		IdSupport = KeySupp.m_IdSupport;
	//}

	m_pATPDocCreuset->SubstitutFormat(IdSupport, IdFormatCur);

	CTarifs * pTarifs	= CTarifs::GetInstance();

	JUnt32 Tarif = 0;

	// Recherche nouveau tarif par date, en testant les tarifs manuels s'ils existent
	if(pTarifs && pTarifs->GetTarifByVersion(IdSupport, IdFormatCur, m_pATPDocCreuset->m_MapTarifManuel,  m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(), Tarif) )
	{
		strTipText.Format("%d",Tarif); 
		CString Fmt = _T("### ### ### ### ### € ");
		CFormatStr::FormatNum(Fmt, strTipText);
	}
	else
		strTipText = "*";

	JInt32 idfmt = 0;
	if(m_pATPDocCreuset->SubstitutFormat(IdSupport,idfmt))
		strTipText.AppendFormat(" - %s", JSRCPresse::GetInstance().m_TBLFormats.GetLabelCourtByID(idfmt));
	//strTipText.Format("i=%d ii=%d j=%d", i,ii,j);


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




	if(i == -1 || j == -1 || ii == -1)
	{
		m_pATPDocCreuset->m_AFFContribution.Reset();
		m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CONTRIB);
		m_prev_i = i;
		m_prev_ii = ii;
		m_prev_j = j;
		return;
	}

	m_prev_i = i;
	m_prev_ii = ii;
	m_prev_j = j;

	ComputeContrib(i, ii, j);
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour

JVoid CATPCreusetHypo_C2::OnUpdate()
{
	// Nombre d'éléments
	LONG xHyp  = 0;
	LONG ySupp = 0;
	int nbFormat = 0;

	// on vérifie la validité du document
	if (m_pATPDocCreuset != 0)
	{
		// on vérifie le document
		if (m_pATPDocCreuset->IsValid())
		{
			// on récupère le nombre d'éléments hypothèse
			xHyp = m_pATPDocCreuset->m_TblHypotheses.GetCount()-1;
			//xHyp = xHyp*m_pATPDocCreuset->m_MapIdFormatsSel.GetCount();

			//nbFormat = m_pATPDocCreuset->m_MapIdFormatsSel.GetCount();
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
			////

			// on récupère le nombre de supports
			//if (m_pATPDocCreuset->m_AFFOffreHypos)
			//	// via offre globale
			//	ySupp = m_pATPDocCreuset->m_EchelleSupports.GetCount();
			//else
				// via plateau
				ySupp = m_pATPDocCreuset->m_PlateauSupports.GetCount();
		}
	}

	this->SetHorzCellCount(nbFormat, FALSE);
	if(nbFormat!=0)
	{
		this->SetHorzCount(xHyp, FALSE);
		this->SetHorzItemSize(nbFormat*CATPCreusetHypo_Metriques::CX_Cx(), TRUE, FALSE);
	}
	else
	{
		this->SetHorzCount(0, FALSE);
		this->SetHorzItemSize(CATPCreusetHypo_Metriques::CX_Cx(), TRUE, FALSE);
	}

	this->SetVertCount(ySupp, TRUE);

}

/*
JVoid CATPCreusetHypo_C2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG ii, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	// si hypothèse active ou support actif , couleur différente
	if ( (i+1 == m_pATPDocCreuset->m_IdxHypotheseActif)
	{
		// on crée le pinceau pour les insertions en arrière plan
		CBrush BrushB;
		CRect recttmp = rect;
		if ((i+1 == m_pATPDocCreuset->m_IdxHypotheseActif)
		{
			// les autres cellules insertions
			BrushB.CreateSolidBrush(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
		}

		// on dessine le fond de la cellule
		dc.FillRect(&recttmp, &BrushB);
	}

	// on initialise le nombre d'insertions
	JInt32 NbInsertions = 0;
	JInt32 NbInsertCpl = 0;

	// on récupère la période d'action de l'étude
	JDate DateD = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD();
	JDate DateF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF();
	bool bCpl = false;

	// on vérifie la validité de la période d'action
	if (DateD.IsValid() && DateF.IsValid())
	{
		// on récupère l'hypothèse courante
		m_pATPDocCreuset->m_TblHypotheses.MoveTo(i+1);

		// on vérifie la validité de l'hypothèse
		if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
		{
			// on récupère l'identifiant du Support
			CKeySupport Support;
			Support.m_IdSupport = 0;

			// on récupère le plan de l'hypothèse courante
			CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();


			int idFormat = ConvertICellToFormat(ii);
			CString Text;

			if (idFormat != -1)
				PLNInsertions.GetNbInsertSupport(Support, idFormat, DateD, DateF, NbInsertions, NbInsertCpl, bCpl);

			if(idFormat == -1 )
			{
				if(i+1 == m_pATPDocCreuset->m_IdxHypotheseActif)
				{
					CBrush BrushB;
					BrushB.CreateSolidBrush(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));
					// on dessine le fond de la cellule
					dc.FillRect(&rect, &BrushB);
				}
				PLNInsertions.GetNbInsertSupport(Support, DateD, DateF, NbInsertions, NbInsertCpl, bCpl);
			}

			// on formate le nombre d'insertions
			if(NbInsertions)
			{
				if (bCpl)
					Text.Format(_T("(%d)"), NbInsertions);
				else
				{
					if (NbInsertCpl)
						Text.Format(_T("%d (%d)"), NbInsertions, NbInsertCpl);
					else
						Text.Format(_T("%d"), NbInsertions);
				}
			}

			// on écrit le nombre d'insertions
			dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);

		}
	}
}
*/

JVoid CATPCreusetHypo_C2::OnDrawCell(CDC & dc, RECT & rect, LONG i, LONG ii, LONG j, CBrush & BrushA, CBrush & BrushB, CBrush & BrushC, CBrush & BrushD)
{
	if (i+1 == m_pATPDocCreuset->m_IdxHypotheseActif)
	{
		// on crée le pinceau pour les insertions en arrière plan
		CBrush BrushB;
		CRect recttmp = rect;
		if (i+1 == m_pATPDocCreuset->m_IdxHypotheseActif)
		{
			// les autres cellules insertions
			BrushB.CreateSolidBrush(CATPColors::GetColorLight(CATPColors::COLORCREUSET));
		}

		// on dessine le fond de la cellule
		dc.FillRect(&recttmp, &BrushB);
	}

	// on initialise le nombre d'insertions
	JInt32 NbInsertions = 0;
	JInt32 NbInsertCpl = 0;

	// on récupère la période d'action de l'étude
	JDate DateD = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD();
	JDate DateF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF();
	bool bCpl = false;

	// on vérifie la validité de la période d'action
	if (DateD.IsValid() && DateF.IsValid())
	{
		// on récupère l'hypothèse courante
		m_pATPDocCreuset->m_TblHypotheses.MoveTo(i+1);

		// on vérifie la validité de l'hypothèse
		if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
		{
			// on récupère l'identifiant du Support
			CKeySupport Support;
			Support.m_IdSupport = 0;
			//if (m_pATPDocCreuset->m_AFFPlateauGrille)
			//{
			//	// Mode affichage plateau (tous les supports sélectionnées)
			//	if (m_pATPDocCreuset->m_AFFPlanEtendu) 
			//		// Récupère identificateur support sur le plateau plan étendu
			//		Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(j);
			//	else
			//		// Récupère identificateur support sur le plateau hypothèses
					Support = m_pATPDocCreuset->m_PlateauSupports.Item(j);
			//}
			//else
			//{
			//	// Mode affichage plan (tous les supports sélectionnées avec au moins 1 insertion)
			//	// Récupère index plateau
			//	JInt32 InxPlateau = m_pATPDocCreuset->OnIndexPlateauViaIndexPlan(j);
			//	if (InxPlateau >= 0)
			//	{

			//		if (m_pATPDocCreuset->m_AFFPlanEtendu) 
			//			// Récupère identificateur support sur le plateau plan étendu
			//			Support = m_pATPDocCreuset->m_PlateauSupportsPlanEtendu.Item(InxPlateau);
			//		else
						// Récupère identificateur support sur le plateau hypothèse
			//			Support = m_pATPDocCreuset->m_PlateauSupports.Item(InxPlateau);
			//	}
			//}


			// on récupère le plan de l'hypothèse courante
			CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();

			
			int idFormat = ConvertICellToFormat(ii);
			CString Text;
			
			if (idFormat != -1)
				PLNInsertions.GetNbInsertSupport(Support, idFormat, DateD, DateF, NbInsertions, NbInsertCpl, bCpl);
			
			if(idFormat == -1 )
			{
				if(i+1 == m_pATPDocCreuset->m_IdxHypotheseActif)
				{
					CBrush BrushB;
					BrushB.CreateSolidBrush(CATPColors::GetColorMedium(CATPColors::COLORCREUSET));

					// on dessine le fond de la cellule
					dc.FillRect(&rect, &BrushB);
				}
				PLNInsertions.GetNbInsertSupport(Support, DateD, DateF, NbInsertions, NbInsertCpl, bCpl);
			}
		
			
			// on formate le nombre d'insertions
			if(NbInsertions)
			{
				if (bCpl)
					Text.Format(_T("(%d)"), NbInsertions);
				else
				{
					if (NbInsertCpl)
						Text.Format(_T("%d (%d)"), NbInsertions, NbInsertCpl);
					else
						Text.Format(_T("%d"), NbInsertions);
				}
			}
			
			
			
			// on écrit le nombre d'insertions
			dc.DrawText(Text, &rect, DT_CENTER | DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
			
		}
	}
}




/////////////////////////////////////
// CATPCreusetHypo_C2 message handlers

void CATPCreusetHypo_C2::PreSubclassWindow() 
{
	// on appelle le gestionnaire de base
	this->JFCGridMultiCell::PreSubclassWindow();

	// on initialise la taille des cellules
	this->SetHorzItemSize(CATPCreusetHypo_Metriques::CX_Cx(), TRUE, FALSE);
	this->SetVertItemSize(CATPCreusetHypo_Metriques::CY_x2(), TRUE, FALSE);
}

void CATPCreusetHypo_C2::ComputeContrib(LONG i, LONG ii, LONG j) 
{
	// le zoom contribution n'est pas affiché
	m_pATPDocCreuset->m_AFFContribution.Reset();
	if (!m_pATPDocCreuset->m_AFFContribution.IsVisible())
		return;
	
	CWaitCursor wait;

	m_pATPDocCreuset->m_TblHypotheses.MoveTo(i+1);
	if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
	{
		// on récupère l'identifiant du Support
		JUnt32 IdSupport;
		//if (m_pATPDocCreuset->m_AFFOffreHypos) 
		//	// en mode offre globale
		//    //const JUnt32 & IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);
		//	IdSupport = m_pATPDocCreuset->m_EchelleSupports.Item(j);
		//else
		//{
			// en mode plateau
			const CKeySupport & KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(j);
			IdSupport = KeySupp.m_IdSupport; 
		//}


		//contributuion

		//On recupere les données pour l'hypo
		m_pATPDocCreuset->m_ITRHypotheses.MoveTo(i+1);
		if (!m_pATPDocCreuset->m_ITRHypotheses.IsValid())
		{
			m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CONTRIB);
			return;
		}
		
		if(!m_pATPDocCreuset->m_ITRHypotheses.IsValid())
			return;

		
		// Id format courant
		JUnt32 IdFormatCur = ConvertICellToFormat(ii);

		m_pATPDocCreuset->m_AFFContribution.m_idHypo = i;
		m_pATPDocCreuset->m_AFFContribution.m_idFormat = IdFormatCur;
		m_pATPDocCreuset->m_AFFContribution.m_idSupport = j;


		const JITPDATHypoFormat & datFormatHypothese =  m_pATPDocCreuset->m_ITRHypotheses.GetItem();
		//const JITPDATHypothese & datHypothese =  datFormatHypothese.GetDATHypoByFormat(IdFormatCur);

		if(! datFormatHypothese.GetDATHypoByFormat(IdFormatCur).IsValid())
		{
			m_pATPDocCreuset->m_ITRHypotheses.CalcDatHypo(
						m_pATPDocCreuset->m_KEYPeriodeActionHyp,
						m_pATPDocCreuset->m_KEYPeriodeActionTablier,
						m_pATPDocCreuset->m_TblHypotheses.GetItem(), m_pATPDocCreuset->m_ITRHypotheses.GetItem(),false );
		}
		const JITPDATHypothese & datHypothese =  datFormatHypothese.GetDATHypoByFormat(IdFormatCur);


		//#####################################################################//

		CPlanInsertions planSupp =  m_pATPDocCreuset->m_TblHypotheses.GetItem();


		// Supprime l'insertion
		planSupp.SuppLastInsertion(IdSupport, IdFormatCur,m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
																		m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF() );


		JITPDATHypoFormat datFormatPlanSupp;
		m_pATPDocCreuset->m_ITRHypotheses.CalcDatHypo(
								m_pATPDocCreuset->m_KEYPeriodeActionHyp,
								m_pATPDocCreuset->m_KEYPeriodeActionTablier,
								planSupp, datFormatPlanSupp, true );

		const JITPDATHypothese & datPlanSupp =  datFormatPlanSupp.GetDATHypoByFormat(IdFormatCur);

		//#####################################################################//
		CPlanInsertions planAdd =  m_pATPDocCreuset->m_TblHypotheses.GetItem();


		// Ajout insertion pour ce format
	//	AjoutInsertionNEW(IdSupport,IdFormatCur,planAdd);
		CAjoutInsertion::AjoutInsertionSuccessif(IdSupport, IdFormatCur, planAdd, m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD(), m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF(),1,m_pATPDocCreuset);
	

		JITPDATHypoFormat datFormatPlanAdd;
		m_pATPDocCreuset->m_ITRHypotheses.CalcDatHypo(
								m_pATPDocCreuset->m_KEYPeriodeActionHyp,
								m_pATPDocCreuset->m_KEYPeriodeActionTablier,
								planAdd, datFormatPlanAdd, true );

		const JITPDATHypothese & datPlanAdd =  datFormatPlanAdd.GetDATHypoByFormat(IdFormatCur);



		//#####################################################################//

	
		JFlt64 valHypo = 0.0;
		JFlt64 valAdd = 0.0;
		JFlt64 valSupp = 0.0;


		//Contrib
		JInt32 NbJours = datHypothese.m_DistributionJour.GetCount();
		if(datFormatHypothese.m_Effectif)
		{
			valHypo = 100.0 * datHypothese.m_DistributionJour.Item(NbJours - 1).Item(1).AsJFlt64() / datFormatHypothese.m_Effectif;
			valAdd = 100.0 * datPlanAdd.m_DistributionJour.Item(NbJours - 1).Item(1).AsJFlt64() / datFormatPlanAdd.m_Effectif;
			valSupp = 100.0 * datPlanSupp.m_DistributionJour.Item(NbJours - 1).Item(1).AsJFlt64() / datFormatPlanSupp.m_Effectif;
		}
		m_pATPDocCreuset->m_AFFContribution.m_ContribAdd = valAdd - valHypo;
		m_pATPDocCreuset->m_AFFContribution.m_ContribSupp = valSupp - valHypo;



		//GRP
		valHypo = 0.0;
		valAdd = 0.0;
		valSupp = 0.0;
		if(datFormatHypothese.m_Effectif)
		{
			valHypo = datHypothese.m_VolLecture*100.0 / datFormatHypothese.m_Effectif;
			valAdd = datPlanAdd.m_VolLecture*100.0 / datFormatPlanAdd.m_Effectif;
			valSupp = datPlanSupp.m_VolLecture*100.0 / datFormatPlanSupp.m_Effectif;
		}
		m_pATPDocCreuset->m_AFFContribution.m_XGRPAdd = valAdd - valHypo;
		m_pATPDocCreuset->m_AFFContribution.m_XGRPSupp = valSupp - valHypo;

		//cout grp
		valHypo = 0.0;
		valAdd = 0.0;
		valSupp = 0.0;
		if(datFormatHypothese.m_Effectif)
		{
			if(datHypothese.m_AudienceRef)
				valHypo =  (datHypothese.m_Budget * datFormatHypothese.m_Effectif )/ (100.0 * datHypothese.m_AudienceRef);
	
			if(datPlanAdd.m_AudienceRef)
				valAdd =  (datPlanAdd.m_Budget * datFormatPlanAdd.m_Effectif) / (100.0 * datPlanAdd.m_AudienceRef);

			if(datPlanSupp.m_AudienceRef)
				valSupp =  (datPlanSupp.m_Budget * datFormatPlanSupp.m_Effectif) / (100.0 * datPlanSupp.m_AudienceRef);
		}
		m_pATPDocCreuset->m_AFFContribution.m_coutGRPAdd = valAdd - valHypo;
		m_pATPDocCreuset->m_AFFContribution.m_coutGRPSupp = valSupp - valHypo;



		//reprise
		valHypo = 0.0;
		valAdd = 0.0;
		valSupp = 0.0;
		if(datHypothese.m_Couverture)
			valHypo = datHypothese.m_VolLecture / datHypothese.m_Couverture;
		if(datPlanAdd.m_Couverture)
			valAdd = datPlanAdd.m_VolLecture / datPlanAdd.m_Couverture;
		if(datPlanSupp.m_Couverture)
			valSupp = datPlanSupp.m_VolLecture / datPlanSupp.m_Couverture;

		m_pATPDocCreuset->m_AFFContribution.m_repriseAdd = valAdd - valHypo;
		m_pATPDocCreuset->m_AFFContribution.m_repriseSupp = valSupp - valHypo;
	
		//#####################################################################//



		
		//budget
		valHypo = datHypothese.m_Budget;
		valSupp = datPlanSupp.m_Budget;
		m_pATPDocCreuset->m_AFFContribution.m_Prix =  valHypo - valSupp;
		if (valHypo != 0.0)
			m_pATPDocCreuset->m_AFFContribution.m_PartBudget = ((m_pATPDocCreuset->m_AFFContribution.m_Prix) * 100.0) / valHypo;


		//X-GRP
		if(datFormatHypothese.m_Effectif != 0.0)
		{
			valHypo = (100.0 * datHypothese.m_VolLecture) / datFormatHypothese.m_Effectif;
			valSupp = (100.0 * datPlanSupp.m_VolLecture) / datFormatPlanSupp.m_Effectif;
			if (valHypo != 0.0)
			{
				m_pATPDocCreuset->m_AFFContribution.m_XGRP = valHypo - valSupp;
				m_pATPDocCreuset->m_AFFContribution.m_PartXGRP = (m_pATPDocCreuset->m_AFFContribution.m_XGRP * 100.0) / valHypo;
			}

		}




	}
	m_pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CONTRIB);
	
}

int CATPCreusetHypo_C2::ConvertICellToFormat(int iCell)
{
	m_mapICellFormat.MoveTo(iCell);
	if(m_mapICellFormat.IsValid())
		return m_mapICellFormat.GetItem();

	return -1;

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Activation de la boite édition cellule
bool CATPCreusetHypo_C2::OnBeginEditing(CString &CurString, LONG i, LONG j)
{
	// Récup indice cellule
	m_RowCellEdit = j;
	m_ColCellEdit = i;

	// Sur quel support
	JUnt32 IdSupport;
	const CKeySupport & KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(j);
	IdSupport = KeySupp.m_IdSupport; 

	// Récupère le nombre d'insertion courant sur ce support
	// LONG NoColFormat = GetNoColFormat();
	m_NoColFormatCur = GetNoColFormat();
	LONG NoColFormat = m_NoColFormatCur;

	m_NbInsCurEdit = GetNbInsertion(i, NoColFormat, j);

	// Met à jour le texte par défaut de la boite d'édition
	CurString.Format("%d", m_NbInsCurEdit); 

	/*
	// Active nouvelle hypothèse courante
	m_pATPDocCreuset->m_IdxHypotheseCourant = i + 1;
	m_pATPDocCreuset->SetModifiedFlag(TRUE); 
	m_pATPDocCreuset->m_AFFCreuset.SetPlanActif(false);
	m_pATPDocCreuset->m_AFFCreuset.SetHypoActif(true);

	//Update();
	Invalidate();
	UpdateWindow();
	m_pATPDocCreuset->UpdateAllViews(UPDATE_CURRENT_HYPO);
	*/

	// on active l'édition des éléments
	return (true);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Fin édition cellule
void CATPCreusetHypo_C2::OnEndEditing()
{
	// Réinit indice cellule édition
	m_RowCellEdit = -1;
	m_ColCellEdit = -1;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Récupère valeur édition
bool CATPCreusetHypo_C2::OnEditItem(const TCHAR* pString, LONG i, LONG j)
{
	// Recup row & col debut du bloc sélectionné + 1er format sélectionné
	LONG Row1, Col1;
	LONG NoFormat1;
	LONG NoFormat2;
	this->GetRowColBeginSelect(Row1, Col1, NoFormat1);

	// MODIF 2010 A VOIR pour valider le contenu d'une cellule en mode édition après click avec soris sur autre cellule
	if ((Row1 != j || Col1 != i) && Col1!= -1 && Row1 != -1)
	{
		// Mise à jour cellule avec nb insertions
		LONG NoFormat = m_NoColFormatCur;

		// Nouveau nombre d'insertions
		int NbInsEdit = atoi(pString);

		// Test non dépassement maximum insertion
		if (NbInsEdit < MaxInsertion)
		{
			// Voir si on en ajoute ou si on en supprime
			if (m_NbInsCurEdit <= NbInsEdit)
			{	
				// Il faut ajouter des insertions
				JUnt32 NbAjout = NbInsEdit - m_NbInsCurEdit; 
				AjoutInsertionCellule(m_ColCellEdit, NoFormat, m_RowCellEdit, NbAjout);
			}
			else
			{
				// Il faut supprimer des insertions
				JUnt32 NbSupp = m_NbInsCurEdit - NbInsEdit; 
				SuppInsertionCellule(m_ColCellEdit, NoFormat, m_RowCellEdit, NbSupp);
			}
		}
		else
		{
			// Maximum insertion atteint
			CString TxtMess;
			TxtMess.FormatMessage(IDS_CR_MAX_INS_EDIT, MaxInsertion-1); 
			AfxMessageBox(TxtMess,MB_ICONINFORMATION);
		}
	}	


	if (Row1 > m_RowCellEdit) return true;

	if (NoFormat1 == -1) NoFormat1 = m_NoColFormatCur;
	if (Col1 == -1) Col1 = m_ColCellEdit;
	if (Row1 == -1) Row1 = m_RowCellEdit;

	// Positionnement mode édition
	if (i == Col1 && j == Row1 && NoFormat1 == m_NoColFormatCur)
		this->SetModeEdit(false);
	else
		this->SetModeEdit(true);
	
	if (i == Col1 && j == Row1 && NoFormat1 == m_NoColFormatCur)
	{
		// Mise à jour cellule avec nb insertions
		LONG NoFormat = m_NoColFormatCur;

		// Nouveau nombre d'insertions
		int NbInsEdit = atoi(pString);

		// Test non dépassement maximum insertion
		if (NbInsEdit < MaxInsertion)
		{
			// Voir si on en ajoute ou si on en supprime
			if (m_NbInsCurEdit <= NbInsEdit)
			{	
				// Il faut ajouter des insertions
				JUnt32 NbAjout = NbInsEdit - m_NbInsCurEdit; 
				AjoutInsertionCellule(m_ColCellEdit, NoFormat, m_RowCellEdit, NbAjout);
			}
			else
			{
				// Il faut supprimer des insertions
				JUnt32 NbSupp = m_NbInsCurEdit - NbInsEdit; 
				SuppInsertionCellule(m_ColCellEdit, NoFormat, m_RowCellEdit, NbSupp);
			}
		}
		else
		{
			// Maximum insertion atteint
			CString TxtMess;
			TxtMess.FormatMessage(IDS_CR_MAX_INS_EDIT, MaxInsertion-1); 
			AfxMessageBox(TxtMess,MB_ICONINFORMATION);
		}
	}
	else if (GetHorzCellCount()== 1)
	{
		// Nouveau nombre d'insertions
		int NbInsEdit = atoi(pString);

		// Test non dépassement maximum insertion
		if (NbInsEdit < MaxInsertion)
		{
			// Recup no format
			LONG NoFormat = m_NoColFormatCur;

			// Gestion d'un bloc sélectionné avec 1 seul format d'affiché
			for (LONG Col = Col1; Col <= i; Col++)
			{
				/*
				for (LONG Row = Row1; Row <= j; Row++)
				{
					// Recup no format
					LONG NoFormat = m_NoColFormatCur;

					// Recup nb insertion précédent sur cette cellule
					JUnt32 NbInsCell = GetNbInsertion(Col, NoFormat, Row);

					// Voir si on en ajoute ou si on en supprime
					if (NbInsCell <= NbInsEdit)
					{	
						// Il faut ajouter des insertions
						JUnt32 NbAjout = NbInsEdit - NbInsCell; 
						AjoutInsertionCellule(Col, NoFormat, Row, NbAjout);
					}
					else
					{
						// Il faut supprimer des insertions
						JUnt32 NbSupp = NbInsCell - NbInsEdit; 
						SuppInsertionCellule(Col, NoFormat, Row, NbSupp);
					}
				}
				*/

				AjoutOuSuppInsertionBlocRow(Col, Row1, j, NoFormat, NbInsEdit);

			}
		}
		else
		{
			// Maximum insertion atteint
			CString TxtMess;
			TxtMess.FormatMessage(IDS_CR_MAX_INS_EDIT, MaxInsertion-1); 
			AfxMessageBox(TxtMess,MB_ICONINFORMATION);
		}

		// Fin edition
		OnCancelEditing();

		// on détruit la fenêtre
		CloseEditBox();
	}
	else
	{
		// Multi-sélection et multi-formats
		// Format départ = NoFormat1 ,  Format arrivée = NoFormat2
		NoFormat2 = m_NoColFormatCur;

		// Nouveau nombre d'insertions
		int NbInsEdit = atoi(pString);

		// Test non dépassement maximum insertion
		if (NbInsEdit < MaxInsertion)
		{
			// Gestion d'un bloc sélectionné avec 1 seul format d'affiché
			for (LONG Col = Col1; Col <= i; Col++)
			{
				// Colonnes intermédiaires, on balaye tous les formats
				LONG CurFormatDeb = 1;
				LONG CurFormatFin = GetHorzCellCount();

				if (Col == Col1)
				{
					// 1ere colonne on comende au 1er format sélectionné
					CurFormatDeb = NoFormat1;
					CurFormatFin = m_NoColFormatCur;
				}
				else if (Col == i)
				{
					// Dernière colonne, on s'arrête au dernier format sélectionné
					CurFormatFin = m_NoColFormatCur;
				}

				// 1ere colonne traitée
				for (LONG CurFormat = CurFormatDeb; CurFormat <= CurFormatFin; CurFormat++)
				{
					/*
					for (LONG Row = Row1; Row <= j; Row++)
					{
						// Recup nb insertion précédent sur cette cellule
						JUnt32 NbInsCell = GetNbInsertion(Col, CurFormat, Row);

						// Voir si on en ajoute ou si on en supprime
						if (NbInsCell <= NbInsEdit)
						{	
							// Il faut ajouter des insertions
							JUnt32 NbAjout = NbInsEdit - NbInsCell; 
							AjoutInsertionCellule(Col, CurFormat, Row, NbAjout);
						}
						else
						{
							// Il faut supprimer des insertions
							JUnt32 NbSupp = NbInsCell - NbInsEdit; 
							SuppInsertionCellule(Col, CurFormat, Row, NbSupp);
						}
					}
					*/
					AjoutOuSuppInsertionBlocRow(Col, Row1, j, CurFormat, NbInsEdit);
				}
			}
		}
		else
		{
			// Maximum insertion atteint
			CString TxtMess;
			TxtMess.FormatMessage(IDS_CR_MAX_INS_EDIT, MaxInsertion-1); 
			AfxMessageBox(TxtMess,MB_ICONINFORMATION);
		}


		// Fin edition
		OnCancelEditing();

		// on détruit la fenêtre
		CloseEditBox();
	}	

	// Init dernière zone selectionnée
	this->InitZoneSelect();

	return (true);
}

/* GOOD EX VERSION
bool CATPCreusetHypo_C2::OnEditItem(const TCHAR* pString, LONG i, LONG j)
{
	// Recup row & col debut du bloc sélectionné + 1er format sélectionné
	LONG Row1, Col1;
	LONG NoFormat1;
	LONG NoFormat2;
	this->GetRowColBeginSelect(Row1, Col1, NoFormat1);

	if (NoFormat1 == -1) NoFormat1 = m_NoColFormatCur;
	if (Col1 == -1) Col1 = m_ColCellEdit;
	if (Row1 == -1) Row1 = m_RowCellEdit;

	// Positionnement mode édition
	if (i == Col1 && j == Row1 && NoFormat1 == m_NoColFormatCur)
		this->SetModeEdit(false);
	else
		this->SetModeEdit(true);

	if (i == Col1 && j == Row1 && NoFormat1 == m_NoColFormatCur)
	{
		// Mise à jour cellule avec nb insertions
		LONG NoFormat = m_NoColFormatCur;

		// Nouveau nombre d'insertions
		int NbInsEdit = atoi(pString);

		// Test non dépassement maximum insertion
		if (NbInsEdit < MaxInsertion)
		{
			// Voir si on en ajoute ou si on en supprime
			if (m_NbInsCurEdit <= NbInsEdit)
			{	
				// Il faut ajouter des insertions
				JUnt32 NbAjout = NbInsEdit - m_NbInsCurEdit; 
				AjoutInsertionCellule(m_ColCellEdit, NoFormat, m_RowCellEdit, NbAjout);
			}
			else
			{
				// Il faut supprimer des insertions
				JUnt32 NbSupp = m_NbInsCurEdit - NbInsEdit; 
				SuppInsertionCellule(m_ColCellEdit, NoFormat, m_RowCellEdit, NbSupp);
			}
		}
		else
		{
			// Maximum insertion atteint
			CString TxtMess;
			TxtMess.FormatMessage(IDS_CR_MAX_INS_EDIT, MaxInsertion-1); 
			AfxMessageBox(TxtMess,MB_ICONINFORMATION);
		}
	}
	else if (GetHorzCellCount()== 1)
	{
		// Nouveau nombre d'insertions
		int NbInsEdit = atoi(pString);

		// Test non dépassement maximum insertion
		if (NbInsEdit < MaxInsertion)
		{
			// Gestion d'un bloc sélectionné avec 1 seul format d'affiché
			for (LONG Col = Col1; Col <= i; Col++)
			{
				for (LONG Row = Row1; Row <= j; Row++)
				{
					LONG NoFormat = m_NoColFormatCur;


					// Recup nb insertion précédent sur cette cellule
					JUnt32 NbInsCell = GetNbInsertion(Col, NoFormat, Row);

					// Voir si on en ajoute ou si on en supprime
					if (NbInsCell <= NbInsEdit)
					{	
						// Il faut ajouter des insertions
						JUnt32 NbAjout = NbInsEdit - NbInsCell; 
						AjoutInsertionCellule(Col, NoFormat, Row, NbAjout);
					}
					else
					{
						// Il faut supprimer des insertions
						JUnt32 NbSupp = NbInsCell - NbInsEdit; 
						SuppInsertionCellule(Col, NoFormat, Row, NbSupp);
					}
				}
			}
		}
		else
		{
			// Maximum insertion atteint
			CString TxtMess;
			TxtMess.FormatMessage(IDS_CR_MAX_INS_EDIT, MaxInsertion-1); 
			AfxMessageBox(TxtMess,MB_ICONINFORMATION);
		}

		// Fin edition
		OnCancelEditing();

		// on détruit la fenêtre
		CloseEditBox();
	}
	else
	{
		// Multi-sélection et multi-formats
		// Format départ = NoFormat1 ,  Format arrivée = NoFormat2
		NoFormat2 = m_NoColFormatCur;

		// Nouveau nombre d'insertions
		int NbInsEdit = atoi(pString);

		// Test non dépassement maximum insertion
		if (NbInsEdit < MaxInsertion)
		{
			// Gestion d'un bloc sélectionné avec 1 seul format d'affiché
			for (LONG Col = Col1; Col <= i; Col++)
			{
				// Colonnes intermédiaires, on balaye tous les formats
				LONG CurFormatDeb = 1;
				LONG CurFormatFin = GetHorzCellCount();

				if (Col == Col1)
				{
					// 1ere colonne on comende au 1er format sélectionné
					CurFormatDeb = NoFormat1;
				}
				else if (Col == i)
				{
					// Dernière colonne, on s'arrête au dernier format sélectionné
					CurFormatFin = m_NoColFormatCur;
				}
				// 1ere colonne traitée
				for (LONG CurFormat = CurFormatDeb; CurFormat <= CurFormatFin; CurFormat++)
				{
					for (LONG Row = Row1; Row <= j; Row++)
					{
						// Recup nb insertion précédent sur cette cellule
						JUnt32 NbInsCell = GetNbInsertion(Col, CurFormat, Row);

						// Voir si on en ajoute ou si on en supprime
						if (NbInsCell <= NbInsEdit)
						{	
							// Il faut ajouter des insertions
							JUnt32 NbAjout = NbInsEdit - NbInsCell; 
							AjoutInsertionCellule(Col, CurFormat, Row, NbAjout);
						}
						else
						{
							// Il faut supprimer des insertions
							JUnt32 NbSupp = NbInsCell - NbInsEdit; 
							SuppInsertionCellule(Col, CurFormat, Row, NbSupp);
						}
					}
				}
			}
		}
		else
		{
			// Maximum insertion atteint
			CString TxtMess;
			TxtMess.FormatMessage(IDS_CR_MAX_INS_EDIT, MaxInsertion-1); 
			AfxMessageBox(TxtMess,MB_ICONINFORMATION);
		}


		// Fin edition
		OnCancelEditing();

		// on détruit la fenêtre
		CloseEditBox();
	}	

	// Init dernière zone selectionnée
	this->InitZoneSelect();

	return (true);
}
*/


/////////////////////////////////////////////////////////////////////////////////////////////////
// Annulation édition
void CATPCreusetHypo_C2::OnCancelEditing()
{
	// Réinit indice cellule édition
	m_RowCellEdit = -1;
	m_ColCellEdit = -1;
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Récupère le nombre d'insertion courant sur ce support
JUnt32 CATPCreusetHypo_C2::GetNbInsertion(LONG NoHypo, LONG NoFormat, LONG NoSupport)
{
	// On se positionne sur l'hypothèse
	m_pATPDocCreuset->m_TblHypotheses.MoveTo(NoHypo + 1);

	// on initialise le nombre d'insertions
	JInt32 NbInsertions	= 0;
	JInt32 NbInsertCpl	= 0;

	// Voir si hypothèse valide
	if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
	{
		// on récupère le plan de l'hypothèse
		CPlanInsertions & PLNInsertions = m_pATPDocCreuset->m_TblHypotheses.GetItem();

		// Le support courant
		CKeySupport Support;
		Support.m_IdSupport = 0;
		Support = m_pATPDocCreuset->m_PlateauSupports.Item(NoSupport);

		// Ident format 
		int idFormat = ConvertICellToFormat(NoFormat);
		if (idFormat != -1)
		{	
			// on récupère la période d'action de l'étude
			JDate DateD = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateD();
			JDate DateF = m_pATPDocCreuset->m_KEYPeriodeActionTablier.GetDateF();
			bool bCpl = false;

			// Récupère nb insertions en cours sur ce support 
			PLNInsertions.GetNbInsertSupport(Support, idFormat, DateD, DateF, NbInsertions, NbInsertCpl, bCpl);
		}
	}

	// Renvoi le nombre d'insertion du support en cours
	return NbInsertions;
}
	




