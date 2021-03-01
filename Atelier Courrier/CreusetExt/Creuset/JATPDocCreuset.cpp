//
// Fichier: JATPDocCreuset.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    18/08/2003
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "resource.h"
#include "JATPDocCreuset.h"
#include "JATPEchelle_Tri.h"
#include "ATPDocSerializer.h"
#include "DialogChoixOffre.h"
#include "AjoutInsertion.h"
#include "ItemsConstruitsManager.h"

////////////////////
// les constructeurs

JATPDocCreuset::JATPDocCreuset()
:JATPDocument(), m_ITREchelle(this), m_ITRHypotheses(this), m_MaxHypothese(12)
{
	//***************************** SELECTIONS ****************************
	//
	// Source d'Audience Courante
	m_IdSrcAudienceSel		= 0;

	// Source de Ciblage Courante
	m_IdSrcCiblageSel		= 0;
	
	// Terrain Ciblage Selectionné
	m_IdTerrainCiblageSel	= 0;

	// Segment d'Audience Selectionné
	m_IdSegmentAudienceSel	= 1;

	// Segment de Ciblage Selectionné
	m_IdSegmentCibleSel		= 1;

	// Le filtre d'audience
	m_IdFiltreAudienceSel	= 1;

	// List des terrains audience sélectionnés
	m_MapIdTerrainsAudienceSel.Reset();
	
	// Liste de Formats Actifs  (Les formats pour insertion)
	m_MapIdFormatsSel.Reset();
	
	// Init Recalcul des apports
	m_ApportARecalculer = true;

	///////////////////////// INIT ELMTS SPECIF DOCUMENT CREUSET //////////////////////////
	// Item Selectionné
	m_IdItemSel					= 0;

	// Init Décalage affichage format
	m_DecalAffichFormat			= 0;

	// Initialise le hypothese courant
	m_IdxHypotheseCourant		= 1;
	m_IdxHypotheseActif			= 1;

	// Initialise les 2 filtres d'audience et l'état boite dialogue filtre audience
	// m_AFFCtxLecture				= false;
	m_AFFCourbeReponse			= false;

	// Display Hypothese 0 ou Pas
	m_AFFPlanEtendu			= 0;
	m_AFFPlanMaitre			= 0;

	// Option bouton Fenêtre Hypothèse Plan
	m_AFFDistributionN			= 0;
	m_AFFDistributionC			= 0;
	m_AFFVentilation            .SetVisible(false);
	m_AFFVentilation.SetDocument(this);

	m_AFFContribution_New            .SetVisible(false);
	m_AFFContribution_New.SetDocument(this);

	m_AFFMemoDememo	            = 0;

	m_AFFMargeErreur            = 0;
	
	m_AFFColonneTotal			= 0;
	
	// Option affichage creuset plan / grille calend  (totalité offre ou plateau par défaut)
	m_AFFPlateauGrille			= 1;

	// Option Affichage creuset hypo / test de plans  (plateau par défaut  ou plan) 
	m_AFFOffreHypos				= 0;
	m_AFFPlateauHypos			= 1;

	// Affichage de la chromie sur les insertions
	m_AFFChromie				= 0;		
	// Affichage du statut des insertions
	m_AFFStatutInsert			= 0;		

	// Largeur des cellules de la grille calendaire
	m_ZoomGrille				= 7;

	// ******************************** OPTION CALCUL ****************************
	//
	// Prise en compte des reprises en main
	m_bRepriseEnMain			= AfxGetApp()->GetProfileInt("Moteur\\CR", "Reprise", 1);

	// Privilégier l'audience LNM pour les magazines
	m_bLNMMag					= AfxGetApp()->GetProfileInt("Moteur\\CR", "LNM_Mag", 0);

	// Privilégier l'audience LDP pour les quotidiens
	m_bLDPQtd					= AfxGetApp()->GetProfileInt("Moteur\\CR", "LDP_Qtd", 0);	

	// Privilégier l'audience LDP 30 jours pour les bimestriels
	m_bLDP30J					= AfxGetApp()->GetProfileInt("Moteur\\CR", "LDP_30J", 0);	

	//int type = AfxGetApp()->GetProfileInt("Moteur", "Type", CMoteurPresse::PROBA);
	///m_MoteurPresse.SetType((CMoteurPresse::TYPEMOTEUR)type);
	m_MoteurPresse.SetType(CMoteurPresse::PROBA);

	m_bCalculsDiff = false;

	m_bSomethingToPaste = false;

	// Initialisation map des tarifs manuels
	m_MapTarifManuel.Reset(); 

	// Init des apports
	m_PrctApport = 0;

	// Par défaut les cibles pondérées ne sont pas activées
    m_CiblesPondsActives = false;

	m_ListSupportsChange = false;
}

JATPDocCreuset::JATPDocCreuset(const JATPDocCreuset & Source)
:JATPDocument(), m_ITREchelle(this), m_ITRHypotheses(this), m_MaxHypothese(12)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JATPDocCreuset & JATPDocCreuset::operator =(const JATPDocCreuset & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool JATPDocCreuset::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_KEYEtude.GetDateD().IsValid() && m_KEYEtude.GetDateF().IsValid() );
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Source d'Audience Courante
JUnt32 JATPDocCreuset::GetIdSrcAudienceSel()
{
	return m_IdSrcAudienceSel;
}
void JATPDocCreuset::SetIdSrcAudienceSel(JUnt32 Id)
{
	m_IdSrcAudienceSel = Id;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Source de Ciblage Courante (Id ou liste d'Id)	
JUnt32 JATPDocCreuset::GetIdSrcCiblageSel()
{
	return m_IdSrcCiblageSel;
}

void JATPDocCreuset::SetIdSrcCiblageSel(JUnt32 Id)
{
	m_IdSrcCiblageSel = Id;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Terrain Ciblage Selectionné (Id ou liste d'Id)
JUnt32 JATPDocCreuset::GetIdTerrainCiblageSel()
{
	// A FAIRE
	return m_IdTerrainCiblageSel;
}

void JATPDocCreuset::SetIdTerrainCiblageSel(JUnt32 Id)
{
	m_IdTerrainCiblageSel = Id;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Segment d'Audience Selectionné (Id ou liste d'Id)
JUnt32 JATPDocCreuset::GetIdSegmentAudienceSel()
{
	// A FAIRE
	return m_IdSegmentAudienceSel;
}

void JATPDocCreuset::SetIdSegmentAudienceSel(JUnt32 Id)
{
	m_IdSegmentAudienceSel = Id;
}
	
///////////////////////////////////////////////////////////////////////////////////////////////
// Segment de Ciblage Selectionné (Id ou liste d'Id)
JUnt32 JATPDocCreuset::GetIdSegmentCibleSel()
{
	// A FAIRE
	return m_IdSegmentCibleSel;
}

void JATPDocCreuset::SetIdSegmentCibleSel(JUnt32 Id)
{
	m_IdSegmentCibleSel = Id;
}
	
///////////////////////////////////////////////////////////////////////////////////////////////
// Le filtre d'audience (Id ou liste d'Id)
JUnt32 JATPDocCreuset::GetIdFiltreAudienceSel()
{
	// A FAIRE
	return m_IdFiltreAudienceSel;
}

void JATPDocCreuset::SetIdFiltreAudienceSel(JUnt32 Id)
{
	m_IdFiltreAudienceSel = Id;
}
	
///////////////////////////////////////////////////////////////////////////////////////////////
// List des terrains audience sélectionnés (Map ou liste de Maps)
JMap<JUnt32,JUnt32> * JATPDocCreuset::GetMapIdTerrainsAudienceSel()
{
	return & m_MapIdTerrainsAudienceSel;
}

void JATPDocCreuset::SetMapIdTerrainsAudienceSel(JMap<JUnt32,JUnt32> &MapId)
{
	// A FAIRE
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Liste de Formats Actifs  (Map ou liste de Maps)
void JATPDocCreuset::GetMapIdFormatsSel(JMap<JUnt32,JUnt32> &MapId)
{
	// A FAIRE
}

void JATPDocCreuset::SetMapIdFormatsSel(JMap<JUnt32,JUnt32> &MapId)
{
	// A FAIRE
}

///////////////////////////////////////////////////////////////////////////////////////////////
// Synchronisation supports Echelle et supports Plateau avec insertion et supports sélectionnés
JVoid JATPDocCreuset::SynchroSupports()
{
	// on récupère les Supportx 
	JMap<CKeySupport, JBool> RESPlateaux;
	JMap<JUnt32, JBool> RESEchelles;

	// Sauvegarde ordre des élmts plateaux (sinon ils réapparaissent dans l'ordre des identifiants supports)
	// Et on veut bien sûr conserver l'ordre plateau saisie client
	JMap<JUnt32,CKeySupport> MapOrdreSupportPlateau;

	// on récupère le nombre de Support du plateau
	JInt32 NbSupports = m_PlateauSupports.GetCount();

	// on boucle sur tous les Supportx
	for (JInt32 IdxSupport = 0; IdxSupport < NbSupports; IdxSupport += 1)
	{
		// on récupère le Support courant
		const CKeySupport & KeySupport = m_PlateauSupports.Item(IdxSupport);

		// on ajoute le Support
		RESPlateaux.MoveTo(KeySupport);
		if (!RESPlateaux.IsValid())
		{
			// Ajoute élmt au plateau
			RESPlateaux.Add(KeySupport);

			// Conserve ordre de l'élmt sur le plateau
			MapOrdreSupportPlateau.Add(IdxSupport) = KeySupport;
		}

		// No couplage supports dans l'echelle
		if(!KeySupport.m_IdParent) 
		{
			RESEchelles.MoveTo(KeySupport.m_IdSupport);
			if (!RESEchelles.IsValid())
				RESEchelles.Add(KeySupport.m_IdSupport);
		}
	}

	// on récupère le nombre de Supportx
	NbSupports = m_EchelleSupports.GetCount();

	// on boucle sur tous les Supportx
	for (JInt32 IdxSupport = 0; IdxSupport < NbSupports; IdxSupport += 1)
	{
		// on récupère le Support courant
		const JUnt32 & IdSupport = m_EchelleSupports.Item(IdxSupport);

		// on ajoute le Support
		RESEchelles.MoveTo(IdSupport); 
		if (!RESEchelles.IsValid()) 
			RESEchelles.Add(IdSupport);
	}

	// on boucle sur toutes les hypothèses
	for (m_TblHypotheses.MoveFirst(); m_TblHypotheses.IsValid(); m_TblHypotheses.MoveNext())
	{
		// on récupère les insertions de l'hypothèse courante
		CPlanInsertions & PLNInsertions = m_TblHypotheses.GetItem();

		// on boucle sur toutes les insertions
		for (PLNInsertions.MoveFirst_SFD(); PLNInsertions.IsValid_SFD(); PLNInsertions.MoveNext_SFD())
		{
			// on récupère l'insertion courante
			const CInsertion * pInsertion = PLNInsertions.GetItem_SFD();

			CKeySupport KeySup;
			KeySup.m_IdSupport = pInsertion->m_IdSupport;

			// on ajoute le Support 
			if (m_TblHypotheses.GetIndex() != 0)
			{
				// Ajoute au plateau si fait partie d'une hypothése réelle (pas 0)
				RESPlateaux.MoveTo(KeySup);
				if (!RESPlateaux.IsValid())
				{
					// Ajoute si insertion fait partie de la période action
					JDate DateD = m_KEYPeriodeAction.GetDateD();
					if (pInsertion->m_DateD + pInsertion->m_Duree - 1 > m_KEYPeriodeAction.GetDateD() &&
						pInsertion->m_DateD <= m_KEYPeriodeAction.GetDateF())
						RESPlateaux.Add(KeySup);
				}
			}

			RESEchelles.MoveTo(KeySup.m_IdSupport); 
			if (!RESEchelles.IsValid())
				RESEchelles.Add(KeySup.m_IdSupport);
		}
	}

	// TODO :: Vérifier les supports avec Tarifs

	// on construit la liste des Supportx
	JList<JUnt32> ListIdSupports;
	for (RESEchelles.MoveFirst(); RESEchelles.IsValid(); RESEchelles.MoveNext())
		ListIdSupports.AddTail() = RESEchelles.GetKey();

	// on définit les Supportx de l'échelle
	m_EchelleSupports.SetIdSupports(ListIdSupports, true);

	// on construit la liste des Supportx
	JList<CKeySupport> ListKeySupports;

	// En conservant ordre d'entrée des supports lors de la saisie plateau
	for (MapOrdreSupportPlateau.MoveFirst(); 
		 MapOrdreSupportPlateau.IsValid();  
		 MapOrdreSupportPlateau.MoveNext())
	{
		// Récup Clé support
		const CKeySupport & KeySupport = MapOrdreSupportPlateau.GetItem();

		// On se positionne sur les élmts plateau
		RESPlateaux.MoveTo(KeySupport);

		if (RESPlateaux.IsValid()) 
			// Récup ident support dans liste
			ListKeySupports.AddTail() = RESPlateaux.GetKey();
	}

	// Résupère liste des supports sur plateau
	m_PlateauSupports.SetKeySupports(ListKeySupports);

}

///////////////////////////////////////////////////
// la fonction pour préparer le contenu du document

BOOL JATPDocCreuset::OnPrepareContents()
{
	// on initialise l'indicateur
	BOOL Ret = FALSE;

	// on teste les exceptions
	try
	{
		// Synchronisation des listes supports (Echelle +¨Plateau)
		this->SynchroSupports(); 

		// on ajoute un plan master + un premier hypothese
		if (m_TblHypotheses.GetCount() == 0)
		{
			AddNewHypo();
			AddNewHypo();
			m_IdxHypotheseCourant = 1;
		}
		m_TblHypotheses.MoveFirst();
		m_TblHypotheses.GetItem().SetLabel("Plan maître");


		m_MapAffHypothese.Init(m_MaxHypothese+1);
		m_MapAffHypothese.Set(m_IdxHypotheseCourant);

		// on teste la sélection de l'hypothèse
		if (m_IdxHypotheseCourant >= m_TblHypotheses.GetCount())
		{
			m_IdxHypotheseCourant = m_TblHypotheses.GetCount()-1;
		}
		// si hypothèse autre que 0, ce sera l'hypothèse active
		if (m_IdxHypotheseCourant != 0)
			m_IdxHypotheseActif = m_IdxHypotheseCourant;

		// on teste la validité de la période d'action
		if (!m_KEYPeriodeAction.GetDateD().IsValid() || !m_KEYPeriodeAction.GetDateF().IsValid())
		{
			// on initialise la période d'action
			m_KEYPeriodeAction.SetPeriode(m_KEYEtude.GetDateD(), m_KEYEtude.GetDateF());
			
		}

		// sauve pour la période d'action hypothèse
		m_KEYPeriodeActionHyp.SetPeriode(m_KEYPeriodeAction.GetDateD(),m_KEYPeriodeAction.GetDateF());

		// Voir avec Jeremy / ALAIN
		if (!m_KEYPeriodeActionTablier.GetDateD().IsValid() || !m_KEYPeriodeActionTablier.GetDateF().IsValid())
		{
			m_KEYPeriodeActionTablier.SetPeriode(m_KEYPeriodeActionHyp.GetDateD(),m_KEYPeriodeActionHyp.GetDateF());
		}

		// on récupère les sources

		//m_mapSubstFormat.Reset();
		// on corrige l'indicateur
		Ret = TRUE;
	}
	catch (JException * pException)
	{
		// on affiche le message
		::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
	}

	// on renvoie l'indicateur
	return (Ret);
}

///////////////////////////////////////////
// Update AVANT
///////////////////////////////////////////
/////////////////////////////////////////////
// la fonction pour mettre à jour le document

VOID JATPDocCreuset::OnUpdate(const JFCUpdate & Message)
{
	// on modifie le curseur
	//CWaitCursor Wait;


	if (Message == UPDATE_OFFRE || Message == UPDATE_INITIAL)
	{
		bool CurSelCibleChange				= false;
		bool CurSelTerrainChange			= false;
		bool CurSelIdFiltreAudienceChange	= false;

		CWaitCursor Wait;
		// on crée les éléments
		JITPTBLHypotheses ITRHypotheses(this);
		JITPTBLEchelle    ITREchelle(this);

		// on teste les exceptions
		try
		{
			JUnt32 ExTerrainSel = 0;
			m_MapIdTerrainsAudienceSel.MoveFirst();
			if (m_MapIdTerrainsAudienceSel.IsValid())
			{
				ExTerrainSel = m_MapIdTerrainsAudienceSel.GetKey();
			}

			// Save ex m_IdFiltreAudienceSel
			JUnt32 ExIdFiltreAudienceSel = m_IdFiltreAudienceSel;

			// on charge les terrains que pour update initial
			if (Message == UPDATE_INITIAL)
			{
				this->OnLoadTerrains();
				m_ListSupportsChange = true;
			}

			if (Message == UPDATE_OFFRE)
				this->OnChangeTerrains();

			// A t-on changé le terrain en cours de sélection
			m_MapIdTerrainsAudienceSel.MoveFirst();
			if (m_MapIdTerrainsAudienceSel.IsValid())
			{
				if (m_MapIdTerrainsAudienceSel.GetKey() != ExTerrainSel || Message == UPDATE_INITIAL)
				{
					CurSelTerrainChange = true;
				}
			}

			// on charge les Items Cibles
			JUnt32 ExItemSel = m_IdItemSel;
			this->OnLoadItems();

			// Voir si Cible en cours de sélection reste la même
			if (ExItemSel != m_IdItemSel || Message == UPDATE_INITIAL)
				CurSelCibleChange = true;


			// A VOIR ALAIN - HK
			// on charge les items pondérés
			this->OnLoadItemsPonderes(); 

			// on charge les Supports
			if (Message == UPDATE_INITIAL)
				this->OnLoadSupports();

			if (Message == UPDATE_OFFRE)
				this->OnUpdateSupports();

			// on charge les formats
			this->OnLoadFormats();

			// on désactive la colone Total des formats
			if (m_MapIdFormatsSel.GetCount() <= 1)
				m_AFFColonneTotal = false;

			//On initialise le moteur
			if (Message == UPDATE_INITIAL)
				this->InitMoteur();


			// Test si Filtre Audience Sel change
			if (ExIdFiltreAudienceSel != m_IdFiltreAudienceSel)
				CurSelIdFiltreAudienceChange = true;

			if (Message == UPDATE_OFFRE)
			{
				// if (CurSelTerrainChange || CurSelCibleChange || CurSelIdFiltreAudienceChange)
				//if (CurSelTerrainChange || CurSelCibleChange || m_ListSupportsChange)
				this->m_MoteurPresse.SetOffre(m_Offre, m_IdFiltreAudienceSel);
			}

			// Positionne pondérations cible en cours  (sinon problèe avec les cibles pondérés pré-chargées)
			this->m_MoteurPresse.SetPonderations(m_IdItemSel, m_Offre.m_IdSourceAudience, this->m_MapIdTerrainsAudienceSel.GetKey());

			CKeyTerrain keyTerrain;
			m_MapIdTerrainsAudienceSel.MoveFirst();
			keyTerrain.Set(m_IdSrcAudienceSel, m_MapIdTerrainsAudienceSel.GetKey());
			if (!m_MoteurPresse.CanAbaque(keyTerrain))
				m_MoteurPresse.SetType(CMoteurPresse::PROBA);

			//Mise à jour des tarifs des insertions
			UpdateTarifs();

			if (CurSelTerrainChange || CurSelCibleChange || m_ListSupportsChange)
			{
				// on calcule l'interrogation pour toutes les hypothèses
				for (JInt32 i=0; i<m_TblHypotheses.GetCount(); i++)
				{
					this->OnMakeHypotheses(i, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel,m_TblHypotheses, ITRHypotheses, true);
				}

				// on calcule l'interrogation des échelles
				// this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, ITRHypotheses);

				
				// Modif 2.7 Dec 2009
				if (Message == UPDATE_OFFRE)
					// this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, ITRHypotheses, true);
					this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, ITRHypotheses, false);
				else
					this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, ITRHypotheses, false);

				// Attention on permute avant pour calcul apport
				m_ITRHypotheses.Swap(ITRHypotheses);

				// on effectue le tri des échelles
				this->OnSortEchelles(m_AFFEchelle, ITREchelle, m_EchelleSupports);

				// on permute les éléments
				// m_ITRHypotheses.Swap(ITRHypotheses);
				m_ITREchelle   .Swap(ITREchelle);
			}
			else
			{
				// on effectue le tri des échelles
				this->OnSortEchelles(m_AFFEchelle, ITREchelle, m_EchelleSupports);
			}
		}
		catch (JException* )
		{
			// on affiche le message
			//::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_KEY_PERIODE_ACTION)
	{
		CWaitCursor Wait;
		// on crée les éléments
		JITPTBLHypotheses ITRHypotheses(this);

		// A VOIR ALAIN
		ITRHypotheses.Swap(m_ITRHypotheses);

		// on teste les exceptions
		try
		{
			// on calcule l'interrogation pour toutes les hypothèses
			for (JInt32 i=0; i<m_TblHypotheses.GetCount(); i++)
			{
				this->OnMakeHypotheses(i, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel, m_TblHypotheses, ITRHypotheses, true);
			}				

			// on permute les éléments
			m_ITRHypotheses.Swap(ITRHypotheses);
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_SEL_TERRAIN)
	{
		CWaitCursor Wait;
		// on crée les éléments
		JITPTBLHypotheses ITRHypotheses(this);
		JITPTBLEchelle    ITREchelle(this);

		// A VOIR ALAIN
		ITRHypotheses.Swap(m_ITRHypotheses);

		// on teste les exceptions
		try
		{
			// A FAIRE ALAIN
			m_MapIdTerrainsAudienceSel.MoveFirst();
			if (m_MapIdTerrainsAudienceSel.IsValid())
			{
				// Ident Terrain
				JUnt32 IdTerrainSel;
				IdTerrainSel = m_MapIdTerrainsAudienceSel.GetKey();
				//

				CKeyTerrain keyTerrain;
				keyTerrain.Set(m_IdSrcAudienceSel, IdTerrainSel);
				if (!m_MoteurPresse.CanAbaque(keyTerrain))
					m_MoteurPresse.SetType(CMoteurPresse::PROBA);

				// on calcule l'interrogation pour toutes les hypothèses
				for (JInt32 i=0; i<m_TblHypotheses.GetCount(); i++)
					this->OnMakeHypotheses(i, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel, m_TblHypotheses, ITRHypotheses, true);

				// on calcule l'interrogation des échelles
				this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, ITRHypotheses, true);
			}

			// on effectue le tri des échelles
			this->OnSortEchelles(m_AFFEchelle, ITREchelle, m_EchelleSupports);

			// on permute les éléments
			m_ITRHypotheses.Swap(ITRHypotheses);
			m_ITREchelle   .Swap(ITREchelle);
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_SEL_CIBLE)
	{
		CWaitCursor Wait;
		// on crée les éléments
		JITPTBLHypotheses ITRHypotheses(this);
		JITPTBLEchelle    ITREchelle(this);

		// A VOIR ALAIN
		ITRHypotheses.Swap(m_ITRHypotheses);

		// on teste les exceptions
		try
		{
			m_MapIdTerrainsAudienceSel.MoveFirst ();
			if (m_MapIdTerrainsAudienceSel.IsValid())
			{
				JUnt32 IdTerrainSel = m_MapIdTerrainsAudienceSel.GetKey();
				for (JInt32 i=0; i<m_TblHypotheses.GetCount(); i++)
					this->OnMakeHypotheses(i, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel, m_TblHypotheses, ITRHypotheses, true);

				// on calcule l'interrogation des échelles
				// Modif 09/12/2009
				// this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, ITRHypotheses, true);
				this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, ITRHypotheses);
			}

			// on effectue le tri des échelles
			this->OnSortEchelles(m_AFFEchelle, ITREchelle, m_EchelleSupports);

			// on permute les éléments
			m_ITRHypotheses.Swap(ITRHypotheses);
			m_ITREchelle   .Swap(ITREchelle);
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox("Cible non valide sur ce terrain", MB_ICONERROR | MB_OK);
			// ::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_OPTIONSMOTEUR)
	{
		CWaitCursor Wait;
		// on crée les éléments
		JITPTBLHypotheses ITRHypotheses(this);
		JITPTBLEchelle    ITREchelle(this);

		// A VOIR ALAIN
		ITRHypotheses.Swap(m_ITRHypotheses);

		m_MoteurPresse.SetOptions(m_Offre, m_bLDPQtd, m_bLNMMag, m_bLDP30J,  m_bRepriseEnMain);
		
		// on teste les exceptions
		try
		{
			m_MapIdTerrainsAudienceSel.MoveFirst ();
			if (m_MapIdTerrainsAudienceSel.IsValid())
			{
				JUnt32 IdTerrainSel = m_MapIdTerrainsAudienceSel.GetKey();

				for (JInt32 i=0; i<m_TblHypotheses.GetCount(); i++)
					this->OnMakeHypotheses(i, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel, m_TblHypotheses, ITRHypotheses, true);

				// on calcule l'interrogation des échelles
				this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, ITRHypotheses, true);
			}

			// on effectue le tri des échelles
			this->OnSortEchelles(m_AFFEchelle, ITREchelle, m_EchelleSupports);

			// on permute les éléments
			m_ITRHypotheses.Swap(ITRHypotheses);
			m_ITREchelle   .Swap(ITREchelle);
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_FILTREAUDIENCE)
	{
		CWaitCursor Wait;
		// on crée les éléments
		JITPTBLHypotheses ITRHypotheses(this);
		JITPTBLEchelle    ITREchelle(this);

		// A VOIR ALAIN
		ITRHypotheses.Swap(m_ITRHypotheses);

		m_MoteurPresse.SetFiltreAudience(m_Offre, m_IdFiltreAudienceSel);
		
		// on teste les exceptions
		try
		{
			m_MapIdTerrainsAudienceSel.MoveFirst ();
			if (m_MapIdTerrainsAudienceSel.IsValid())
			{
				JUnt32 IdTerrainSel = m_MapIdTerrainsAudienceSel.GetKey();

				for (JInt32 i=0; i<m_TblHypotheses.GetCount(); i++)
					this->OnMakeHypotheses(i, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel, m_TblHypotheses, ITRHypotheses, true);

				// on calcule l'interrogation des échelles
				this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, ITRHypotheses, true);
			}

			// on effectue le tri des échelles
			this->OnSortEchelles(m_AFFEchelle, ITREchelle, m_EchelleSupports);

			// on permute les éléments
			m_ITRHypotheses.Swap(ITRHypotheses);
			m_ITREchelle   .Swap(ITREchelle);
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_CTXLECTURE)
	{
		CWaitCursor Wait;
		// on crée les éléments
		JITPTBLHypotheses ITRHypotheses(this);
		JITPTBLEchelle    ITREchelle(this);

		// A VOIR ALAIN
		ITRHypotheses.Swap(m_ITRHypotheses);

		// on teste les exceptions
		try
		{
			m_MapIdTerrainsAudienceSel.MoveFirst ();
			if (m_MapIdTerrainsAudienceSel.IsValid())
			{
				JUnt32 IdTerrainSel = m_MapIdTerrainsAudienceSel.GetKey();

				for (JInt32 i=0; i<m_TblHypotheses.GetCount(); i++)
					this->OnMakeHypotheses(i, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel, m_TblHypotheses, ITRHypotheses, true);

				// on calcule l'interrogation des échelles
				this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, ITRHypotheses, true);
			}

			// on effectue le tri des échelles
			this->OnSortEchelles(m_AFFEchelle, ITREchelle, m_EchelleSupports);

			// on permute les éléments
			m_ITRHypotheses.Swap(ITRHypotheses);
			m_ITREchelle   .Swap(ITREchelle);
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_SEL_HYPOTHESE)
	{
		// Désormais il faut recalculer les apports dans Echelle
		// on teste les exceptions
		try
		{
			JITPTBLEchelle    ITREchelle(this);

			// puis on recalcul les échelles pour les apports
			this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, m_ITRHypotheses);

			m_ITRHypotheses.MoveTo(m_IdxHypotheseCourant);
			m_ITREchelle   .Swap(ITREchelle);
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_SEL_HYPOTHESE_WITHOUTCALCUL)
	{
		
	}
	else if (Message == UPDATE_ETUDE)
	{
		CWaitCursor wait;
		// Chargement étude , reconstruction du tableau Hypotheses
		// on teste les exceptions
		JITPTBLHypotheses ITRHypotheses(this);
		try
		{
			for (JInt32 i=0; i<m_TblHypotheses.GetCount(); i++)
				this->OnMakeHypotheses(i, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel, m_TblHypotheses, ITRHypotheses, true);
			m_ITRHypotheses.Swap(ITRHypotheses);
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_TARIFS_QP)
	{
		m_ITREchelle.UpdateTarifs(m_KEYEtude);
		UpdateTarifs();
		m_ITRHypotheses.UpdateTarif(m_KEYPeriodeAction, m_KEYPeriodeActionTablier);
	}
	else if (Message == UPDATE_TARIFS)
	{
		UpdateTarifs();
		// on crée les éléments
		JITPTBLHypotheses ITRHypotheses(this);
		JITPTBLEchelle    ITREchelle(this);
		
		
		// A VOIR ALAIN
		ITRHypotheses.Swap(m_ITRHypotheses);

		// on teste les exceptions
		try
		{
			// on calcule l'interrogation pour toutes les hypothèses
			for (JInt32 i=0;i<m_TblHypotheses.GetCount();i++)
				this->OnMakeHypotheses(i, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel, m_TblHypotheses, ITRHypotheses, true);

			// on calcule l'interrogation des échelles
			this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, ITRHypotheses, true);

			// on permute les éléments
			m_ITRHypotheses.Swap(ITRHypotheses);
			m_ITREchelle   .Swap(ITREchelle);
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_SEL_FORMAT)
	{
		// on crée les éléments
		JITPTBLHypotheses ITRHypotheses(this);
		JITPTBLEchelle    ITREchelle(this);

		// A VOIR ALAIN
		//ITRHypotheses.Swap(m_ITRHypotheses);

		// on teste les exceptions
		try
		{
			for (JInt32 i=0;i<m_TblHypotheses.GetCount();i++)
			{
				m_TblHypotheses.MoveTo(i);
				if(!m_TblHypotheses.IsValid()) throw JInvalidCall::GetInstance();

				// on récupère le plan de l'hypothèse courante
				CPlanInsertions & PLNInsertions = m_TblHypotheses.GetItem();
				m_ITRHypotheses.MoveTo(i);
				if(m_ITRHypotheses.IsValid())
				{
					//JITPDATHypoFormat & dat =  m_ITRHypotheses.GetItem();
					m_ITRHypotheses.CalcDatHypoFActif(m_KEYPeriodeAction, m_KEYPeriodeActionTablier,PLNInsertions,m_IdItemSel);
					//m_ITRHypotheses.GetItem().Swap(dat);
				}

			}

			//for (JInt32 i=0;i<m_TblHypotheses.GetCount();i++)
			//	this->OnMakeHypotheses(i, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel, m_TblHypotheses, ITRHypotheses, true);

			// on calcule l'interrogation des échelles
			// this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, ITRHypotheses, true);

			// on calcule l'interrogation des échelles (sans recalcul des apports)
			this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, ITRHypotheses, false);

			// on permute les éléments
			//m_ITRHypotheses.Swap(ITRHypotheses);
			m_ITREchelle   .Swap(ITREchelle);
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_AFF_CREUSET)
	{
	}
	else if (Message == UPDATE_CURRENT_HYPO)
	{
		// on teste les exceptions
		try
		{
			JITPTBLEchelle    ITREchelle(this);

			// on calcule l'interrogation pour toutes les hypothèses
			if(m_bCalculsDiff)
				this->OnMakeHypotheses(m_IdxHypotheseCourant, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel, m_TblHypotheses, m_ITRHypotheses, false);
			else
				this->OnMakeHypotheses(m_IdxHypotheseCourant, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel, m_TblHypotheses, m_ITRHypotheses, true);

			// puis on recalcul les échelles pour les apports
			this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, m_ITRHypotheses);
			m_ITREchelle   .Swap(ITREchelle);
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_CURRENT_HYPO_LIGHT)
	{
		// on teste les exceptions
		try
		{
			// on calcule l'interrogation pour toutes les hypothèses
			if(m_bCalculsDiff)
				this->OnMakeHypotheses(m_IdxHypotheseCourant, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel, m_TblHypotheses, m_ITRHypotheses, false);
			else
				this->OnMakeHypotheses(m_IdxHypotheseCourant, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel, m_TblHypotheses, m_ITRHypotheses, true);
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_KEY_PLATEAU)
	{
		// Récup Nb élmt
		JUnt32 Count  = m_PlateauSupports.GetCount();

		// Map tempo positionnement des supports plateau
		JMap <JUnt32,bool> tmp;

		// On boucle sur tous les élmts supports du plateau courant
		for(JUnt32 Idx=0; Idx < Count; Idx++)
		{
			const CKeySupport & Key = m_PlateauSupports.Item(Idx);

			// Si c'est un support 1er niveau
			if(!Key.m_IdParent)
			{
				// on regarde s'il est dans le map courant
				m_MapIdCouplageExpanded.MoveTo(Key.m_IdSupport);

				if(m_MapIdCouplageExpanded.IsValid())
					// si c'est le cas on l'ajoute au map ptemporaire
					tmp.Add(Key.m_IdSupport) = m_MapIdCouplageExpanded.GetItem();
			}
		}

		// Puis on récupère les élmts dans le Map courant
		m_MapIdCouplageExpanded.Swap(tmp);

		// Mise à jour du plateau
		UpdatePlateau();

	}
	else if (Message == UPDATE_AFF_PUPITRE)
	{
	}
	else if (Message == UPDATE_CALC_ECHELLE)
	{
		// on teste les exceptions
		try
		{
			// Refaire les échelles avec recalcul des apports
			JITPTBLEchelle    ITREchelle(this);

			// Pour le moment on recalcule les apports (car pas de stockage précédent apport !!!)
			this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, m_ITRHypotheses, true);
			m_ITREchelle   .Swap(ITREchelle);

			// on effectue le tri des échelles
			this->OnSortEchelles(m_AFFEchelle, m_ITREchelle, m_EchelleSupports);

			// Les apports ne sont plus à recalculer
			m_ApportARecalculer = false;
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_CALC_ECHELLE_SANS_CALCAPPORT)
	{
		// on teste les exceptions
		try
		{
			/*
			// Refaire les échelles avec recalcul des apports
			JITPTBLEchelle    ITREchelle(this);

			// Pour le moment on recalcule les apports (car pas de stockage précédent apport !!!)
			this->OnMakeEchelles(m_EchelleSupports, m_KEYEtude, m_KEYPeriodeAction, m_IdItemSel, ITREchelle, m_ITRHypotheses);
			m_ITREchelle   .Swap(ITREchelle);

			// on effectue le tri des échelles
			this->OnSortEchelles(m_AFFEchelle, m_ITREchelle, m_EchelleSupports);

			// Les apports ne sont plus à recalculer
			m_ApportARecalculer = false;
			*/

			// on récupère le nombre de Supportx
			JInt32 NbSupports = m_EchelleSupports.GetCount();

			// Voir si hypothèse courante valide
			m_ITRHypotheses.MoveTo(m_IdxHypotheseCourant);

			// on boucle sur tous les Supportx
			for (JInt32 IdxSupport = 0; IdxSupport < NbSupports; IdxSupport += 1)
			{
				// on récupère l'identifiant du Support
				const JUnt32 & IdSupport = m_EchelleSupports.Item(IdxSupport);

				// on init l'apport
				m_ITREchelle.SetApport(IdSupport,-1);
			}

			m_ApportARecalculer = true;
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_AFF_ECHELLE)
	{
		// on teste les exceptions
		try
		{
			// on effectue le tri des échelles
			this->OnSortEchelles(m_AFFEchelle, m_ITREchelle, m_EchelleSupports);
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_AFF_ZOOM_PLAN)
	{
	}
	else if (Message == UPDATE_AFF_PANEL)
	{
		// A FAIRE
	}
	else if (Message == UPDATE_AFF_PANEL)
	{
		// A FAIRE
	}
	else if (Message == UPDATE_AFF_BTN_CREUSETPLAN)
	{
		// Gestion des boutons sous la grille calendaire (Plateau/ Plan / Effacer)
	}
	else if (Message == UPDATE_AFF_BTN_CREUSETHYPO)
	{

		if (m_AFFPlateauHypos)
		{
			// A FAIRE
			// Affichage sélection mode plateau dans test de plan
		}

		if (m_AFFOffreHypos)
		{
			// A FAIRE
			// Affichage sélection mode offre dans test de plan
			
		}

		// Gestion passage visu plateau grille plan / plan grille plan
		if (m_AFFPlateauGrille)
		{
			// Affichage du plateau pour grille calendaire
		}
		
	}
	else if (Message == UPDATE_AFF_RESULTBAR)
	{
		// Traitement des options barre hypothèses
		if (m_AFFVentilation.IsVisible())
		{
		}
		else if (m_AFFMemoDememo)
		{
		}
	}
	else if (Message == UPDATE_AFF_BAR_PLAN)
	{
		// Traitement sélection passe plan étendu à période action
		if(m_AFFPlanEtendu)
		{
			m_IdxHypotheseCourant = 0;

			// Copy plan master sur hypothèse 0 = hypothese plan étendu
			if(CopyMasterPlanToHypothese(m_IdxHypotheseCourant))
			{
				// Copie du plan master sur hypothèses
				m_TblHypotheses.MoveTo(m_IdxHypotheseCourant);
				ReplacePlanWithHypothese(&m_TblHypotheses.GetItem(),m_IdxHypotheseActif,
					                     m_KEYPeriodeActionHyp.GetDateD(),
					                     m_KEYPeriodeActionHyp.GetDateF());

				// Récup les bornes dates de l'étude
				m_KEYPeriodeAction.SetPeriode(m_KEYEtude.GetDateD(),m_KEYEtude.GetDateF());

				// Récupère le plateau lié au plan étendu
				OnPlateauSupport(true);
			}
		}
		else
		{
			// Reset Hypothèse 0 (enlève toutes les insertions)
			m_TblHypotheses.MoveTo(0);
			CPlanInsertions &PlanHyp0 = m_TblHypotheses.GetItem();
			PlanHyp0.Reset(); 

			// Récupère l'hypothèse courante
			m_IdxHypotheseCourant = m_IdxHypotheseActif;

			// Reprend les bornes de travail période action hypothèse
			m_KEYPeriodeAction.SetPeriode(m_KEYPeriodeActionHyp.GetDateD(),
										  m_KEYPeriodeActionHyp.GetDateF());

		}
		JITPTBLHypotheses ITRHypotheses(this);

		// A VOIR // ALAIN
		ITRHypotheses.Swap(m_ITRHypotheses); 

		// on calcule l'interrogation des hypothèses
		try
		{
			// on calcule l'interrogation pour toutes les hypothèses
			// A FAIRE ALAIN
			/*
			// for (JInt32 i=1;i<m_TblHypotheses.GetCount();i++)
			for (JInt32 i=0;i<m_TblHypotheses.GetCount();i++)
				OnMakeHypotheses(i, m_KEYPeriodeAction, m_IdItemSel, m_IdTerrainAudienceSel, m_TblHypotheses, ITRHypotheses, true);
			*/
			for (JInt32 i=0;i<m_TblHypotheses.GetCount();i++)
				OnMakeHypotheses(i, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_IdItemSel, m_TblHypotheses, ITRHypotheses, true);

			// on permute les éléments
			m_ITRHypotheses.Swap(ITRHypotheses);
		}
		catch (JException* pException)
		{
			// on affiche le message
			::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
		}
	}
	else if (Message == UPDATE_SUPP_HYPOTHESECUR)
	{
		// Suppression de l'hypothèse courante
		OnSuppHypothese(m_IdxHypotheseCourant);

		// Attention à hypothese active
		if (m_IdxHypotheseCourant <	m_IdxHypotheseActif &&	m_IdxHypotheseActif > 1)
			m_IdxHypotheseActif--;

		// Nouvelle hypothèse active
		m_MapAffHypothese.Set(m_IdxHypotheseCourant, false);
		m_MapAffHypothese.ResetSet(m_IdxHypotheseActif);
		m_MapAffHypothese.Set(m_IdxHypotheseActif);
		//m_MapAffHypothese.MoveTo(m_IdxHypotheseActif);
		//if(!m_MapAffHypothese.IsValid())
		//	m_MapAffHypothese.Add(m_IdxHypotheseActif) = true;
		//else
		//	m_MapAffHypothese.GetItem() = true;
	}

	//dlg.DestroyWindow();
	m_AFFVentilation.Update(Message);
	m_AFFContribution_New.Update(Message);
}

//////////////////////////////////////////
// les fonctions pour vérifier le document

JBool JATPDocCreuset::OnCheckEtude(const JATPKEYEtude & KEYEtude) const
{
	// on vérifie la période de l'étude
	if (!KEYEtude.GetDateD().IsValid()) return (false);
	if (!KEYEtude.GetDateF().IsValid()) return (false);

	// on quitte
	return (true);
}

JBool JATPDocCreuset::OnCheckPeriodeAction(const JATPKEYEtude & KEYEtude, const JATPKEYPeriodeAction & KEYPeriodeAction) const
{
	// on récupère la période de l'étude
	JDate DateED = KEYEtude.GetDateD();
	JDate DateEF = KEYEtude.GetDateF();

	// on vérifie la période d'action
	JDate DatePD = KEYPeriodeAction.GetDateD(); if (DatePD.IsValid()) { if (DatePD < DateED || DatePD > DateEF) return (false); }
	JDate DatePF = KEYPeriodeAction.GetDateF(); if (DatePF.IsValid()) { if (DatePF < DateED || DatePF > DateEF) return (false); }

	// on quitte
	return (true);
}

JBool JATPDocCreuset::OnCheckHypotheses(const JATPKEYEtude & KEYEtude, const JATPEchelleSupports & EchelleSupports, const JATPPlateauSupports & PlateauSupports, LIST_HYPOTHESES & PLNHypotheses)
{
	// on récupère la période de l'étude
	JDate DateED = KEYEtude.GetDateD();
	JDate DateEF = KEYEtude.GetDateF();

	// on boucle sur toutes les hypothèses
	for (PLNHypotheses.MoveFirst(); PLNHypotheses.IsValid(); PLNHypotheses.MoveNext())
	{
		// on récupère les insertions de l'hypothèse courante
		CPlanInsertions & PLNInsertions = PLNHypotheses.GetItem();

		// on boucle sur toutes les insertions
		for (PLNInsertions.MoveFirst_SFD(); PLNInsertions.IsValid_SFD(); PLNInsertions.MoveNext_SFD())
		{
			// on récupère l'insertion courante
			const CInsertion * pInsertion = PLNInsertions.GetItem_SFD();

			// on récupère la durée de l'insertion
			JInt32 Duree = pInsertion->m_Duree;

			// on calcule la période de l'insertion
			JDate DatePD = pInsertion->m_DateD;
			JDate DatePF = DatePD + (Duree - 1);

			// on teste les dates de l'insertion
			if (DatePD < DateED || DatePD > DateEF) return (false);
			if (DatePF < DateED || DatePF > DateEF) return (false);
		}
	}
	// on quitte
	return (true);
}

////////////////////////////////////////////////////////////////////
// Chargement des terrains
JBool JATPDocCreuset::OnChangeTerrains()
{
	// Set the current selected Source Audience ID
	m_IdSrcAudienceSel = m_Offre.m_IdSourceAudience;

	JMap <JUnt32,JUnt32> MapTmp;

	if (m_MapIdTerrainsAudienceSel.GetCount() > 0)
	{
		for (m_MapIdTerrainsAudienceSel.MoveFirst();
			 m_MapIdTerrainsAudienceSel.IsValid();
			 m_MapIdTerrainsAudienceSel.MoveNext())
		{
			JUnt32 IdTerrainSel = m_MapIdTerrainsAudienceSel.GetKey();

			if(IdTerrainSel)
			{
				// Check if we still have the terrain
				for(m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
					m_Offre.m_TBLIdTerrainsAudience.IsValid();
					m_Offre.m_TBLIdTerrainsAudience.MoveNext())
				{
					if(m_Offre.m_TBLIdTerrainsAudience.GetItem() == IdTerrainSel)
					{	
						JUnt32 IdTerrain = m_Offre.m_TBLIdTerrainsAudience.GetItem();
						MapTmp.Add(IdTerrain); 
					}	
				}	
			}
		}

		if (MapTmp.GetCount() == 0)
		{
			// dans ce cas on prend le 1er terrain
			m_Offre.m_TBLIdTerrainsAudience.MoveFirst();

			// Ajoute ce terrain au map terrains sélectionnés
			if (m_Offre.m_TBLIdTerrainsAudience.IsValid())
			{
				JUnt32 IdTerrain = m_Offre.m_TBLIdTerrainsAudience.GetItem();
				MapTmp.Add(IdTerrain);  
			}
		}

		// Puis on swap avec nouveau map id terrains sélectionnés
		m_MapIdTerrainsAudienceSel.Swap(MapTmp);

	}
	else
	{
		// Select the First terrain in the Map
		m_Offre.m_TBLIdTerrainsAudience.MoveFirst();

		// Ajoute ce terrain au map terrains sélectionnés
		if (m_Offre.m_TBLIdTerrainsAudience.IsValid())
		{
			JUnt32 IdTerrain = m_Offre.m_TBLIdTerrainsAudience.GetItem();
			m_MapIdTerrainsAudienceSel.Add(IdTerrain) = IdTerrain;  
		}
		else
			// problème sélection terrain
			return false;
	}		
	
	return true;
}

JBool JATPDocCreuset::OnLoadTerrains() 
{
	// Flag modification de l'audience source
	bool bSource = false;
	bSource = (m_Offre.m_IdSourceAudience ==  m_IdSrcAudienceSel); 

	JSRCTables * srcTables	= (JSRCTables*)& JSRCTables::GetInstance();
	//Si la source n'est pas disponible on doit sortir de l'appli
	if( srcTables->m_TBLSources.GetItem(m_Offre.m_IdSourceAudience) == NULL && IsValid())
	{
		m_Offre.m_IdSourceAudience = 0;
		m_Offre.m_TBLIdTerrainsAudience.Reset();
		m_Offre.m_IdTerrainCiblage = 0;

		m_IdSrcAudienceSel = 0;
		m_IdSrcCiblageSel = 0;
		m_MapIdTerrainsAudienceSel.Reset();
		m_IdTerrainCiblageSel = 0;
		this->SetModifiedFlag();

		::AfxMessageBox(IDS_CR_ERRSRC, MB_ICONEXCLAMATION | MB_OK);
		//exit(0);
		return false;
	}

	// Mise à jour du Map terrains courant 
	bool bRem = false;
	m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
	while(m_Offre.m_TBLIdTerrainsAudience.IsValid())
	{
		// La source d'audience en cours
		JInt32 SrcID				= m_Offre.m_IdSourceAudience;  
		// Récup le nouveau terrain
		JInt32 TerID				= m_Offre.m_TBLIdTerrainsAudience.GetItem();
		const JTerrain * pt = srcTables->m_TBLTerrains.GetItem(SrcID,TerID);

		//on n'a pas le terrain
		if (!pt)
		{
			m_MapIdTerrainsAudienceSel.MoveTo(TerID);
			if (m_MapIdTerrainsAudienceSel.IsValid())
				m_MapIdTerrainsAudienceSel.Remove();
			
			m_Offre.m_TBLIdTerrainsAudience.Remove(+1);
			this->SetModifiedFlag();
			bRem = true;
		}
		else
			m_Offre.m_TBLIdTerrainsAudience.MoveNext();
	}

	if (bRem)
		::AfxMessageBox(IDS_CR_ERRTER, MB_ICONEXCLAMATION | MB_OK);


	// Set the current selected Source Audience ID
	m_IdSrcAudienceSel = m_Offre.m_IdSourceAudience;

	JMap <JUnt32,JUnt32> MapTmp;
	if (m_MapIdTerrainsAudienceSel.GetCount() > 0)
	{
		for (m_MapIdTerrainsAudienceSel.MoveFirst();
			 m_MapIdTerrainsAudienceSel.IsValid();
			 m_MapIdTerrainsAudienceSel.MoveNext())
		{
			JUnt32 IdTerrainSel = m_MapIdTerrainsAudienceSel.GetKey();

			if(IdTerrainSel)
			{
				// Check if we still have the terrain
				for(m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
					m_Offre.m_TBLIdTerrainsAudience.IsValid();
					m_Offre.m_TBLIdTerrainsAudience.MoveNext())
				{
					if(m_Offre.m_TBLIdTerrainsAudience.GetItem() == IdTerrainSel)
					{	
						JUnt32 IdTerrain = m_Offre.m_TBLIdTerrainsAudience.GetItem();
						MapTmp.Add(IdTerrain); 
					}	
				}	
			}
		}

		if (MapTmp.GetCount() == 0)
		{
			// dans ce cas on prend le 1er terrain
			m_Offre.m_TBLIdTerrainsAudience.MoveFirst();

			// Ajoute ce terrain au map terrains sélectionnés
			if (m_Offre.m_TBLIdTerrainsAudience.IsValid())
			{
				JUnt32 IdTerrain = m_Offre.m_TBLIdTerrainsAudience.GetItem();
				MapTmp.Add(IdTerrain);  
			}
		}

		// Puis on swap avec nouveau map id terrains sélectionnés
		m_MapIdTerrainsAudienceSel.Swap(MapTmp);

	}
	else
	{
		// Select the First terrain in the Map
		m_Offre.m_TBLIdTerrainsAudience.MoveFirst();

		// Ajoute ce terrain au map terrains sélectionnés
		if (m_Offre.m_TBLIdTerrainsAudience.IsValid())
		{
			JUnt32 IdTerrain = m_Offre.m_TBLIdTerrainsAudience.GetItem();
			m_MapIdTerrainsAudienceSel.Add(IdTerrain) = IdTerrain;  
		}
		else
			// problème sélection terrain on sort de l'appli
			//exit(0);
		{
			m_Offre.m_IdTerrainCiblage = 0;
			return false;
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////
// Chargement des items cibles
JBool JATPDocCreuset::OnLoadItems()
{
	// Flag modification de l'audience source
	JMap<JUnt32 , IItem * >	TmpItems;
	m_MAPpItems.Reset();

	// Mise à jour du Map Items courant 
	bool bRem = false;
	CString msg;
	m_Offre.m_TBLIdItems.MoveFirst();
	while (m_Offre.m_TBLIdItems.IsValid())
	{
		// Récupére le Item de l'offre
		JUnt32 IdItem = m_Offre.m_TBLIdItems.GetItem();
		// Récup Table des items
		CItemsConstruitsBase * pItemConst = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

		// Récup item correspondant à l'indice
		IItem * pItem = pItemConst->GetIItemByID(IdItem);

		bool itemValid = false;
		if(pItem)
		{
			
			itemValid = true;
			//Test des items sur tous les terrains.
			for(m_Offre.m_TBLIdTerrainsAudience.MoveFirst();m_Offre.m_TBLIdTerrainsAudience.IsValid();m_Offre.m_TBLIdTerrainsAudience.MoveNext())
			{
				CTerrain * pTerrain = CTerrainManager::GetInstance()->GetTerrain(m_Offre.m_IdSourceAudience, m_Offre.m_TBLIdTerrainsAudience.GetItem());
				if (!pItem->IsValid(pTerrain))
					itemValid = false;
			}
		}
		if(itemValid)
		{
			if( pItem->IsItemConstruit() )
				TmpItems.Add(IdItem) = pItem;
			if( pItem->IsPalette())
			{
				CPalettes *pPal = (CPalettes *)pItem->AsPalette();
				JList<JInt32> idList;
				pPal->GetItems(idList);
				for (idList.MoveFirst(); idList.IsValid(); idList.MoveNext())
				{
					IItem * pItemfromPal = pItemConst->GetIItemByID(idList.GetItem());
					TmpItems.Add(pItemfromPal->GetID()) = pItemfromPal;
				}
			}
			m_Offre.m_TBLIdItems.MoveNext();
		}
		else
		{
			m_Offre.m_mapLblItem.MoveTo(m_Offre.m_TBLIdItems.GetItem());
			msg.Append(m_Offre.m_mapLblItem.GetItem().AsJCharPtr());
			msg += "\r\n";
			m_Offre.m_mapLblItem.Remove();
			m_Offre.m_TBLIdItems.Remove(+1);
			this->SetModifiedFlag();
			bRem = true;
		}
	}
	if (bRem)
	{
		CString str;
		str.FormatMessage(IDS_CR_ERRITEM, msg);
		::AfxMessageBox(str, MB_ICONEXCLAMATION | MB_OK);
	}

	// No Items loaded
	if(!TmpItems.GetCount())
	{
		if (IsValid())
		{
			//on reselectionne l'item ensemble
			CItemsConstruitsBase * pItemConst = CItemsConstruitsManager::GetInstance()->GetItemsInstance();
			IItem * pItem = pItemConst->GetIItemByID(pItemConst->GetDefaultItemId());
			if(pItem)
				TmpItems.Add(pItemConst->GetDefaultItemId()) = pItem;
		}
		else
			return false;
	}

	// m_MAPpItemsPonderes

	// Swap the Item maps
	m_MAPpItems.Swap(TmpItems);

	// Check if there is a Currently Selected item
	if(m_IdItemSel)
	{
		// Check if we still have the item
		m_MAPpItems.MoveTo(m_IdItemSel);
		if(!m_MAPpItems.IsValid())
			// The item is no longer present so take the first
			m_MAPpItems.MoveFirst();
	}
	else
		// Select the First item in the Map
		m_MAPpItems.MoveFirst();

	// Attention si c'est une cible pondérée
	// Voir si c'est une cible pondérée
	// CItemPondere * pItemPondere = CItemsConstruits::GetInstance()->GetItemPondere(m_IdItemSel);
	CItemPondere * pItemPondere = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemPondere(m_IdItemSel);

	if (!pItemPondere)
	{
		// Set the Selected Item
		m_IdItemSel = m_MAPpItems.GetItem()->GetID();

		// Indique au moteur
		this->m_MoteurPresse.SetItemCourant(m_IdItemSel); 
	}
	
	// Réinitialise les pondérations
	this->m_MapIdTerrainsAudienceSel.MoveFirst();
	if (this->m_MapIdTerrainsAudienceSel.IsValid())
		this->m_MoteurPresse.SetPonderations(m_IdItemSel, m_Offre.m_IdSourceAudience, this->m_MapIdTerrainsAudienceSel.GetKey());

	return true;
}

////////////////////////////////////////////////////////////////////
// Chargement des items cibles pondérées
JBool JATPDocCreuset::OnLoadItemsPonderes()
{
	// A FAIRE ALAIN - HK
	// Récup Table des items pondérés
	CItemsConstruitsBase * pItems = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

	// Flag modification de l'audience source
	m_MAPpItemsPonderes.Reset();

	// Comptabilise validité des items pondérés sur les terrains en cours de sélection
	JMap <JUnt32, JUnt32> MapValidTerrains;
	MapValidTerrains.Reset(); 	

	// Nombre de terrains sur lesquels les ites pondérés doivent être valides
	JUnt32 NbTerrainSel = m_Offre.m_TBLIdTerrainsAudience.GetCount();

	//Test des items sur tous les terrains.
	for(m_Offre.m_TBLIdTerrainsAudience.MoveFirst();m_Offre.m_TBLIdTerrainsAudience.IsValid();m_Offre.m_TBLIdTerrainsAudience.MoveNext())
	{
		CTerrain * pTerrain = CTerrainManager::GetInstance()->GetTerrain(m_Offre.m_IdSourceAudience, m_Offre.m_TBLIdTerrainsAudience.GetItem());

		// Récupère les items indépendants (sans les cibles pondérées)
		JList<IItem const *> vItems;
		// pItems->GetItemsPonderes(vItems, pTerrain);
		CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemsPonderes(vItems, pTerrain);

		// Boucle sur tous les items
		for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
		{
			// Récupère l'item
			IItem * pItem = (IItem*)vItems.GetItem();

			// Ident de l'item
			JInt32 IdItem		= pItem->GetID();

			// Test si cet item pondéré est valid sur ce terrain
			if (pItem->IsValid(pTerrain))
			{
				MapValidTerrains.MoveTo(IdItem);
				if (MapValidTerrains.IsValid())
				{
					// Incrémente le nombre de terrains sur lequel cet item pondéré est valide
					MapValidTerrains.GetItem()++;
				}	
				else
				{
					// Ajoute un nouvel item pour les validités
					MapValidTerrains.Add(IdItem) = 1;
				}
			}
		}
	}

	// Boucle sur les items pondérés trouvés sur chacun des terrains
	for (MapValidTerrains.MoveFirst(); MapValidTerrains.IsValid();MapValidTerrains.MoveNext())
	{
		// Récup nombre de validité terrains
		JUnt32 NbValid = MapValidTerrains.GetItem();

		// Si valide sur tous les terrains on le stocke dans map des items pondérés ctifs
		if (NbValid == NbTerrainSel)
		{
			// Récup ident item
			JUnt32 IdItemPond = MapValidTerrains.GetKey(); 

			// Récup l'item
			IItem * pItem = pItems->GetIItemByID(IdItemPond);

			// Ajoute au map ds items pondérés actifs
			m_MAPpItemsPonderes.Add(IdItemPond) = pItem;
		}
	}

	// Selon mode cible (cibles de l'offre ou cibles pondérées)
	if (m_CiblesPondsActives)
	{
		// Check if there is a Currently Selected item
		if(m_IdItemSel)
		{
			// Check if we still have the item
			m_MAPpItemsPonderes.MoveTo(m_IdItemSel);
			if(!m_MAPpItemsPonderes.IsValid())
				// The item is no longer present so take the first
				m_MAPpItemsPonderes.MoveFirst();
		}
		else
			// Select the First item in the Map
			m_MAPpItemsPonderes.MoveFirst();

		// Set the Selected Item
		m_IdItemSel = m_MAPpItemsPonderes.GetItem()->GetID();

		// Indique au moteur
		this->m_MoteurPresse.SetItemCourant(m_IdItemSel);

		// Réinitialise les pondérations
		this->m_MapIdTerrainsAudienceSel.MoveFirst();
		if (this->m_MapIdTerrainsAudienceSel.IsValid())
			this->m_MoteurPresse.SetPonderations(m_IdItemSel, m_Offre.m_IdSourceAudience, this->m_MapIdTerrainsAudienceSel.GetKey());

	}
	else
	{
		// Test si cible active OK
		m_MAPpItems.MoveTo(m_IdItemSel);
		if (!m_MAPpItems.IsValid())
		{
			// Par défaut on prend la cible ensmeble si aucun
			m_MAPpItems.MoveFirst();
			if (m_MAPpItems.IsValid())
			{
				// Set the Selected Item
				m_IdItemSel = m_MAPpItems.GetItem()->GetID();

				// Indique au moteur
				this->m_MoteurPresse.SetItemCourant(m_IdItemSel);
			}
		}
	}

	return true;
}

////////////////////////////////////////////////////////////////////
// Chargement des Supports
JBool JATPDocCreuset::OnLoadSupports()
{
	// Verify changes to the supports list
	CRegroupements * pReg = CRegroupements::GetInstance();

	
	//Chargement des titres Apparies (titres sans GRP)
	for(m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
		m_Offre.m_TBLIdTerrainsAudience.IsValid();
		m_Offre.m_TBLIdTerrainsAudience.MoveNext())
		pReg->GetTitresApparies(m_Offre.m_IdSourceAudience, m_Offre.m_TBLIdTerrainsAudience.GetItem());
	
		
	if(!m_Offre.m_TBLIdSupports.GetCount())
		return false;

	JMap<JUnt32,bool> TmpMap;

	m_Offre.m_TBLIdSupports.MoveFirst();
	while( m_Offre.m_TBLIdSupports.IsValid())
	{
		// Récup id support
		IElem *pElem = pReg->GetIElemByID(m_Offre.m_TBLIdSupports.GetItem());
		if (!pElem)
		{
			m_Offre.m_TBLIdSupports.Remove(+1);
			continue;
		}
		if(pElem->IsLot())
		{
			// Crée liste des supports lot
			JList<JUnt32> ListSup;
			ILot * pLot = pElem->AsLot();
			if(pLot)
			{
				pLot->GetFils(ListSup);
				// On ajoute les supports enfants au map supports
				for(ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
				{
					// Check if the support is already in the map
					TmpMap.MoveTo(ListSup.GetItem());
					if(!TmpMap.IsValid())
						TmpMap.Add(ListSup.GetItem()) = true;
				}
			}
		}
		else
		{
			// Check if the support is already in the map
			TmpMap.MoveTo(m_Offre.m_TBLIdSupports.GetItem());
			if(!TmpMap.IsValid())
				// on ajout le support au map
				TmpMap.Add(m_Offre.m_TBLIdSupports.GetItem()) = true; 
		}
		m_Offre.m_TBLIdSupports.MoveNext();
	}

	///on verifie la syncho avec le m_EchelleSupports
	bool bRem = false;
	CString msg;
	JList<JUnt32>	lstSupportsInval;
	for (JInt32 i = 0; i<m_EchelleSupports.GetCount(); i++)
	{
		JUnt32 id = m_EchelleSupports.Item(i);
		IElem *pElem = pReg->GetIElemByID(id);
		TmpMap.MoveTo(id);

		//si on a un support de l'echelle mais il n'est pas dans la map ça veut dire qu'il vient d'une famille supprimée
		if(pElem)
		{
			if (!TmpMap.IsValid())
			{
				TmpMap.Add(id) = true;
				m_Offre.m_TBLIdSupports.AddTail() = id;
			}
		}
		else
		{
			//il faut retirer les insertions de ce support
			m_Offre.m_mapLblSupport.MoveTo(id);
			if(m_Offre.m_mapLblSupport.IsValid())
			{
				msg.Append(m_Offre.m_mapLblSupport.GetItem().AsJCharPtr());
				m_Offre.m_mapLblSupport.Remove();
			}
			msg += "\r\n";
	
			lstSupportsInval.AddTail() = id;

			bRem = true;
			SetModifiedFlag();
		}
	}
	if(bRem)
	{
		CString str;
		str.FormatMessage(IDS_CR_ERRSUPPORT, msg);
		::AfxMessageBox(str, MB_ICONEXCLAMATION | MB_OK);
	}
	

	
	// on boucle sur toutes les hypothèses pour retirer les insertions des supports invalidés

	for (m_TblHypotheses.MoveFirst(); m_TblHypotheses.IsValid(); m_TblHypotheses.MoveNext())
	{
		// on récupère les insertions de l'hypothèse courante
		CPlanInsertions & PLNInsertions = m_TblHypotheses.GetItem();
		for (lstSupportsInval.MoveFirst(); lstSupportsInval.IsValid(); lstSupportsInval.MoveNext())
		{
			for(m_Offre.m_TBLIdFormats.MoveFirst(); m_Offre.m_TBLIdFormats.IsValid(); m_Offre.m_TBLIdFormats.MoveNext())
			{
				PLNInsertions.SuppAllInsertion(lstSupportsInval.GetItem(), m_Offre.m_TBLIdFormats.GetItem(), m_KEYEtude.GetDateD(), m_KEYEtude.GetDateF());
			}
		}
	}


	// Fill list with all supports
	JList<JUnt32> TmpList;
	for(TmpMap.MoveFirst(); TmpMap.IsValid(); TmpMap.MoveNext())
		TmpList.AddTail() = TmpMap.GetKey();

	// Update the Echelle
	JATPEchelleSupports TmpEchelle;
	TmpEchelle.SetIdSupports(TmpList);
	m_EchelleSupports.Swap(TmpEchelle);
	
	UpdatePlateau();

	return true;
}


JBool JATPDocCreuset::OnUpdateSupports()
{

	// Verify changes to the supports list
	CRegroupements * pReg = CRegroupements::GetInstance();

	if(!m_Offre.m_TBLIdSupports.GetCount())
		return false;

	JMap<JUnt32,bool> TmpMap;

	m_Offre.m_TBLIdSupports.MoveFirst();
	while( m_Offre.m_TBLIdSupports.IsValid())
	{
		// Récup id support
		IElem *pElem = pReg->GetIElemByID(m_Offre.m_TBLIdSupports.GetItem());
		if (!pElem)
		{
			m_Offre.m_TBLIdSupports.Remove(+1);
			continue;
		}
		if(pElem->IsLot())
		{
			// Crée liste des supports lot
			JList<JUnt32> ListSup;
			ILot * pLot = pElem->AsLot();
			if(pLot)
			{
				pLot->GetFils(ListSup);
				// On ajoute les supports enfants au map supports
				for(ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
				{
					// Check if the support is already in the map
					TmpMap.MoveTo(ListSup.GetItem());
					if(!TmpMap.IsValid())
						TmpMap.Add(ListSup.GetItem()) = true;
				}
			}
		}
		else
		{
			// Check if the support is already in the map
			TmpMap.MoveTo(m_Offre.m_TBLIdSupports.GetItem());
			if(!TmpMap.IsValid())
				// on ajout le support au map
				TmpMap.Add(m_Offre.m_TBLIdSupports.GetItem()) = true; 
		}
		m_Offre.m_TBLIdSupports.MoveNext();
	}

	
	///on verifie la syncho avec le m_EchelleSupports
	CString msg;
	JList<JUnt32>	lstSupportsInval;
	for (JInt32 i = 0; i<m_EchelleSupports.GetCount(); i++)
	{
		JUnt32 id = m_EchelleSupports.Item(i);
		TmpMap.MoveTo(id);

		if (!TmpMap.IsValid())
		{
			lstSupportsInval.AddTail() = id;
			SetModifiedFlag();
		}

	}
	

	//if(lstSupportsInval.GetCount())
	//{
		CString ttl, qst;
		ttl.LoadString(IDR_CR_ATPFRAME);
		qst.LoadString(IDS_CR_SUPPR_SUPPORTECH);
		if (lstSupportsInval.GetCount() &&  AfxMessageBox(qst, MB_ICONQUESTION|MB_YESNO) == IDNO)
		{
			for(lstSupportsInval.MoveFirst(); lstSupportsInval.IsValid(); lstSupportsInval.MoveNext())
				m_Offre.m_TBLIdSupports.AddTail() = lstSupportsInval.GetItem();
			return true;
		}
		else
		{

			// on boucle sur toutes les hypothèses pour retirer les insertions des supports invalidés
			SetModifiedFlag();
			for (m_TblHypotheses.MoveFirst(); m_TblHypotheses.IsValid(); m_TblHypotheses.MoveNext())
			{
				// on récupère les insertions de l'hypothèse courante
				CPlanInsertions & PLNInsertions = m_TblHypotheses.GetItem();
				for (lstSupportsInval.MoveFirst(); lstSupportsInval.IsValid(); lstSupportsInval.MoveNext())
				{
					for(m_Offre.m_TBLIdFormats.MoveFirst(); m_Offre.m_TBLIdFormats.IsValid(); m_Offre.m_TBLIdFormats.MoveNext())
					{
						PLNInsertions.SuppAllInsertion(lstSupportsInval.GetItem(), m_Offre.m_TBLIdFormats.GetItem(), m_KEYPeriodeAction.GetDateD(), m_KEYPeriodeAction.GetDateF());
					}
				}
			}


			// Fill list with all supports
			JList<JUnt32> TmpList;
			for(TmpMap.MoveFirst(); TmpMap.IsValid(); TmpMap.MoveNext())
				TmpList.AddTail() = TmpMap.GetKey();

			// Update the Echelle
			JATPEchelleSupports TmpEchelle;
			TmpEchelle.SetIdSupports(TmpList);
			m_EchelleSupports.Swap(TmpEchelle);
			
			UpdatePlateau();
		}
	//}

	return true;
}
////////////////////////////////////////////////////////////////////
//// Mise à jour du plateau support selon plan étendu ou hypothèses


//////////////////////////////////////////////////////////////////
// Mise à jour du plateau support selon plan étendu ou hypothèses
JVoid JATPDocCreuset::OnPlateauSupport(JBool OptionPlanEtendu)
{
	if (OptionPlanEtendu)
	{
		// Init Plateau supports zone étendu (toute la période étude)
		JATPPlateauSupports TmpPlateauEtendu;

		// On ajoute déjà d'office les élmts du plateau hypothèses
		for (JInt32 InxSupp = 0; InxSupp < m_PlateauSupports.GetCount(); InxSupp++)
		{
			// Récup clé support dans plateau support
			CKeySupport KeySup;
			KeySup = m_PlateauSupports.Item(InxSupp); 

			// Ajoute au plateau temporaire
			TmpPlateauEtendu.Add(KeySup);

		}

		// Va sur l'hypothèse lié au plan étendu (hypothèse 0) pour trouver les supports supplémentaires à ajouter
		m_TblHypotheses.MoveTo(0);

		// on vérifie la validité de l'hypothèse
		if (m_TblHypotheses.IsValid())
		{
			// on récupère le plan de l'hypothèse courante
			CPlanInsertions & PLNInsertions = m_TblHypotheses.GetItem();

			if (PLNInsertions.GetCount())
			{
				// Validation de tous les élmts de l'offre pour le plateau
				for (JInt32 InxElmt = 0; InxElmt < m_EchelleSupports.GetCount();InxElmt++) 
				{		
					// on récupère l'identifiant du Support
					const JUnt32 & IdSupport = m_EchelleSupports.Item(InxElmt);

					// on crée la clé pour recherche insertion
					CInsKey Key;
					Key.m_IdSupport 	=   IdSupport;
					Key.m_IdParent		=	0; 
					Key.m_DateD			=	m_KEYEtude.GetDateD();
					Key.m_IdFormat		=	0;

					// on cherche insertion à partir du début période étude (si mode hypothèses)
					if(PLNInsertions.FindPAInsertions(Key,m_KEYEtude.GetDateD(),m_KEYEtude.GetDateF()))
					{

						// on ajoute le Support au plateau si non présent
						CKeySupport KeySup;
						KeySup.m_IdSupport  = Key.m_IdSupport;
						KeySup.m_IdParent   = Key.m_IdParent;
						if(TmpPlateauEtendu.FindItem(KeySup) < 0)
							// Support absent du plateau support, mais présent sur plan étendu
							TmpPlateauEtendu.Add(KeySup);
					
					}
				}	
			}
		}

		// Récup les supports dans plateau étendu
		m_PlateauSupportsPlanEtendu.Swap(TmpPlateauEtendu);
	}
}

//////////////////////////////////////////////////////////////
// Update plateau suppors hypothese suite à nouvelle vague
JVoid JATPDocCreuset::MajPlateauHypotheseNewVague()
{
	// Init Plateau supports hypotheses
	JATPPlateauSupports TmpPlateauHyp;

	// Balaye toutes les hypothèses
	for (m_TblHypotheses.MoveFirst(); m_TblHypotheses.IsValid(); m_TblHypotheses.MoveNext())
	{
		// on récupère le plan de l'hypothèse courante
		CPlanInsertions & PLNInsertions = m_TblHypotheses.GetItem();

		if (PLNInsertions.GetCount())
		{
			// Validation de tous les élmts de l'offre pour le plateau
			for (JInt32 InxElmt = 0; InxElmt < m_EchelleSupports.GetCount();InxElmt++) 
			{		
				// on récupère l'identifiant du Support
				const JUnt32 & IdSupport = m_EchelleSupports.Item(InxElmt);

				// on crée la clé pour recherche insertion
				CInsKey Key;
				Key.m_IdSupport 	=   IdSupport;
				Key.m_IdParent		=	0; 
				Key.m_DateD			=	m_KEYEtude.GetDateD();
				Key.m_IdFormat		=	0;

				// on cherche insertion à partir du début période étude (si mode hypothèses)
				if(PLNInsertions.FindPAInsertions(Key,m_KEYPeriodeActionHyp.GetDateD(),m_KEYPeriodeActionHyp.GetDateF()))
				{
					// Ajout dans plateau car insertion(s) présente(s) sur période action
					try
					{
						// on ajoute le Support si non présent
						CKeySupport KeySup;
						KeySup.m_IdSupport  = Key.m_IdSupport;
						KeySup.m_IdParent   = Key.m_IdParent;
						JInt32 IdSupp       = TmpPlateauHyp.FindItem(KeySup);
						if (IdSupp < 0)
						{
							// Support absent du plateau support, mais présent sur plan étendu
							TmpPlateauHyp.Add(KeySup);
						}
					}
					catch (JException* pException)
					{
						// on affiche le message
						::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
					}
				}
			}	
		}
	}

	// Récup les supports dans plateau courant
	m_PlateauSupports.Swap(TmpPlateauHyp);

	// on positionne l'indicateur de modification
	SetModifiedFlag(TRUE);

	// on met à jour les vues du document
	UpdateAllViews(UPDATE_ETUDE);

}


//////////////////////////////////////
// Chargement des formats disponibles
JVoid JATPDocCreuset::OnLoadFormats()
{
	// Récupère les id formats
	JMap<JUnt32, JUnt32> mapFormat;
	for (m_Offre.m_TBLIdFormats.MoveFirst();m_Offre.m_TBLIdFormats.IsValid();m_Offre.m_TBLIdFormats.MoveNext())
	{
		JUnt32 IdFormat = m_Offre.m_TBLIdFormats.GetItem();
		m_MapIdFormatsSel.MoveTo(IdFormat);
		if (!m_MapIdFormatsSel.IsValid())
			mapFormat.Add(IdFormat) = 0;
		else
			mapFormat.Add(IdFormat) = m_MapIdFormatsSel.GetItem();
	}
	m_MapIdFormatsSel.Swap(mapFormat);
	
	for(m_MapIdFormatsSel.MoveFirst();m_MapIdFormatsSel.IsValid(); m_MapIdFormatsSel.MoveNext())
	{
		if(m_MapIdFormatsSel.GetItem() >=2)
			return;
	}
	m_MapIdFormatsSel.MoveFirst();
	m_MapIdFormatsSel.GetItem() = 3;
}

JVoid JATPDocCreuset::UpdatePlateau()
{
	// Update the Plateau
	JList <CKeySupport> TmpListPlateau;

	// Temporary Supports for the extended map
	JMap<JUnt32,bool> TmpExpanded;

	// on récupère le nombre de Supports dans le plateau
	JInt32 NbSupports = m_PlateauSupports.GetCount();

	// on boucle sur tous les Supports du plateau
	for (JInt32 IdxSupport = 0; IdxSupport < NbSupports; IdxSupport++)
	{
		// on récupère l'identifiant du Support
		const CKeySupport & KeySupport = m_PlateauSupports.Item(IdxSupport);
		if(!KeySupport.m_IdParent)
		{
			// Support Simple ou Couplage Parent
			//m_MapIdSupportsSel.MoveTo(KeySupport.m_IdSupport);
			if (m_EchelleSupports.FindItem(KeySupport.m_IdSupport) != -1)

			// Support dans l'offre
			//if(m_MapIdSupportsSel.IsValid())
			{
				// Add support to the plateau temporaire
				TmpListPlateau.AddTail() = KeySupport;
				m_MapIdCouplageExpanded.MoveTo(KeySupport.m_IdSupport);
				if(!m_MapIdCouplageExpanded.IsValid())
					TmpExpanded.Add(KeySupport.m_IdSupport) = false;
				else
				{
					TmpExpanded.Add(KeySupport.m_IdSupport) = m_MapIdCouplageExpanded.GetItem();
					// Check if couplage expanded
					if(m_MapIdCouplageExpanded.GetItem())
					{
						// Add all children
						CRegroupements * pReg = CRegroupements::GetInstance();

						// Récup id support
						IElem *pElem = pReg->GetIElemByID(KeySupport.m_IdSupport);

						// Test si c'est un couplage
						if(pElem->IsCouplage())
						{
							// Crée liste des supports couplage
							JList<JUnt32> ListSupCoupl;

							pElem->AsCouplage()->GetFils(ListSupCoupl);

							// On ajoute les supports enfants au plateau
							for(ListSupCoupl.MoveFirst(); ListSupCoupl.IsValid(); ListSupCoupl.MoveNext())
							{
								CKeySupport KeySup;
								KeySup.m_IdSupport = ListSupCoupl.GetItem();
								KeySup.m_IdParent = KeySupport.m_IdSupport;
								TmpListPlateau.AddTail() = KeySup;
							}
						}
					}
				}
			}
		}
	}

	m_MapIdCouplageExpanded.Swap(TmpExpanded);

	// Update the Plateau
	JATPPlateauSupports TmpPlateau;
	TmpPlateau.SetKeySupports(TmpListPlateau);
	m_PlateauSupports.Swap(TmpPlateau);
	
}

JVoid JATPDocCreuset::UpdateTarifs()
{
	bool bModif = false;
	CTarifs* tarifs = CTarifs::GetInstance();
	for(m_TblHypotheses.MoveFirst(); m_TblHypotheses.IsValid(); m_TblHypotheses.MoveNext())
	{
		if(UpdateTarifsPlan(m_TblHypotheses.GetItem()))
			bModif = true;
	}

	if (bModif)
	{
		SetModifiedFlag();
		//CString str;
		//str.LoadString(IDS_CR_MAJTARIFS);
		//::AfxMessageBox(str, MB_ICONEXCLAMATION | MB_OK);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Mise à jour des tarif insertions   (attention bug corrigé Oct 2008  voir MoveFirst plus bas)
bool JATPDocCreuset::UpdateTarifsPlan(CPlanInsertions & plan)
{
	bool bModif = false;
	CTarifs* tarifs = CTarifs::GetInstance();
	
	// Clef traité juste avant si modif
	CInsKeySDF LastInsKeySDF;

	for(plan.MoveFirst_SDF(); plan.IsValid_SDF(); plan.MoveNext_SDF())
	{
		CInsertion* ins = plan.GetItem_SDF();

		JDate date1;
		JDate date2;
		JUnt32 duree;
		JUnt32 trf;

		//on cherche si une insertion correspond dans les 2 directions
		JInt32 idFormat = ins->m_IdFormat;
		
		SubstitutFormat(ins->m_IdSupport, idFormat);
		
		// Recherche nouveau tarif par date, en testant les tarifs manuels s'ils existent
		int ret1 = tarifs->GetTarifByDate(ins->m_IdSupport, idFormat, m_MapTarifManuel, ins->m_DateD, m_KEYPeriodeAction.GetDateD(), date1, duree, trf);
		int ret2 = tarifs->GetTarifByDate(ins->m_IdSupport, idFormat, m_MapTarifManuel, ins->m_DateD, m_KEYPeriodeAction.GetDateF(), date2, duree, trf);

		// Attention si c'est un tarif entré manuellement
		CKeyTarifManuel KeyTarifManuel;
		KeyTarifManuel.m_IdSupport = ins->m_IdSupport;
		KeyTarifManuel.m_IdFormat  = idFormat;  
		m_MapTarifManuel.MoveTo(KeyTarifManuel);
		
		// Pas de mise à jour, sauf si précédemment tarif manuel
		if (!m_MapTarifManuel.IsValid() && ins->m_Data.m_TarifP == trf )
		{	
			if(ret1>=100 || ret2>=100)
			{
				// Sauve clef insertion précédente si elle existe
				LastInsKeySDF.m_DateD		= ins->m_DateD;
				LastInsKeySDF.m_IdFormat	= ins->m_IdFormat;
				LastInsKeySDF.m_IdParent	= ins->m_IdParent;
				LastInsKeySDF.m_IdSupport	= ins->m_IdSupport;
				continue;
			}
		}

		//on verifie si la date est bonne (ssi non tarif manuel utilisateur)
		if (!m_MapTarifManuel.IsValid() && date1 != date2)
		{
			if (ins->m_DateD != date1 || ins->m_DateD != date2)
			{
				CInsertion * pIns = new CInsertion(*ins);
				
				if(ins->m_DateD-date1 < date2-ins->m_DateD)
					pIns->m_DateD = date1;
				else
					pIns->m_DateD = date2;

				pIns->m_Data.m_TarifP = trf;
				pIns->m_Data.m_Duree = duree;
				plan.DeleteInsertion(*ins, m_KEYPeriodeAction.GetDateD(), m_KEYPeriodeAction.GetDateF());
				plan.AddInsertion(*pIns, m_KEYPeriodeAction.GetDateD(), m_KEYPeriodeAction.GetDateF());

				// Correctif bug sinon ça boucle !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
				//c pour revenir à la permiere insertion c plus sur (probleme d'iterateur interne)
				// plan.MoveFirst_SDF(); ????

				// On se recale	sur la précédente insertion traité
				plan.MoveTo_SDF(LastInsKeySDF);
				if (plan.IsValid_SDF()) 
					plan.MoveNext_SDF();
				else
					plan.MoveFirst_SDF();

				// Sauve clef insertion finalement traité
				LastInsKeySDF.m_DateD		= plan.GetItem_SDF()->m_DateD;
				LastInsKeySDF.m_IdFormat	= plan.GetItem_SDF()->m_IdFormat;
				LastInsKeySDF.m_IdParent	= plan.GetItem_SDF()->m_IdParent;
				LastInsKeySDF.m_IdSupport	= plan.GetItem_SDF()->m_IdSupport;

				bModif = true;
			}
			else
			{
				// Sauve clef insertion précédente si elle existe
				LastInsKeySDF.m_DateD		= ins->m_DateD;
				LastInsKeySDF.m_IdFormat	= ins->m_IdFormat;
				LastInsKeySDF.m_IdParent	= ins->m_IdParent;
				LastInsKeySDF.m_IdSupport	= ins->m_IdSupport;
			}
		}
		else
		{
			//juste une mise à jour de durée ou de tarif
			if( ins->m_Data.m_TarifP != trf || ins->m_Duree != duree)
			{
				if(!ins->IsCouplage())
				{
					ins->m_Duree = duree;
					ins->m_Data.m_Duree = duree;
				}

				ins->m_Data.m_TarifP = trf;
				bModif = true;
			}

			// Sauve clef insertion précédente si elle existe
			LastInsKeySDF.m_DateD		= ins->m_DateD;
			LastInsKeySDF.m_IdFormat	= ins->m_IdFormat;
			LastInsKeySDF.m_IdParent	= ins->m_IdParent;
			LastInsKeySDF.m_IdSupport	= ins->m_IdSupport;
		}
	}
	return bModif;
}


JVoid JATPDocCreuset::OnMakeHypotheses(const JUnt32 & IdxHypothese, const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTravail, JUnt32 IdItem, LIST_HYPOTHESES & PLNHypotheses, JITPTBLHypotheses & ITRHypotheses, JBool bCompute)
{
	// Test si période Action valide
	if (!KEYPeriodeAction.GetDateD().IsValid() || !KEYPeriodeAction.GetDateF().IsValid())   
		return;

	if (!KEYPeriodeTravail.GetDateD().IsValid() || !KEYPeriodeTravail.GetDateF().IsValid())   
		return;

	// on recherche l'hypothèse
	ITRHypotheses.MoveTo(IdxHypothese);
	if(bCompute)
	{
		if(ITRHypotheses.IsValid())
			ITRHypotheses.Remove();
		ITRHypotheses.Add(IdxHypothese, KEYPeriodeAction, KEYPeriodeTravail, PLNHypotheses);
	}
	else
	{
		if(ITRHypotheses.IsValid())
			ITRHypotheses.GetItem().Reset();
		//else
		//	ITRHypotheses.Add(
	}

	//if (Replace && ITRHypotheses.IsValid())

	//// on teste si on a trouvé l'hypothèse
	//if (!ITRHypotheses.IsValid())
	//{
	//	// on ajoute l'hypothèse
	//	ITRHypotheses.Add(IdxHypothese, KEYPeriodeAction, KEYPeriodeTravail, PLNHypotheses);
	//}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calcul apport via l'hypothèse courante
JVoid JATPDocCreuset::CalculApport(const JITPTBLHypotheses & ITRHypotheses, CInsertion & Insertion, JFlt64 & Apport)
{
	// Init apport
	Apport = -1;

	// Récup couverture de départ
	JFlt64 CurCouverture = 0.0;
	if(ITRHypotheses.IsValid())
	{
		const JITPDATHypothese & CurDATHypo = ITRHypotheses.GetItem().GetDATHypoFormatActif();
		CurCouverture = CurDATHypo.m_Couverture; 
	}

	// Passe par hypothese tempo
	JITPTBLHypotheses New_ITRHypotheses(this); 
	New_ITRHypotheses.Add(m_IdxHypotheseCourant, m_KEYPeriodeAction, m_KEYPeriodeActionTablier, m_TblHypotheses); 
	
	// Récupérer le plan de l'hypothèse en cours
	New_ITRHypotheses.MoveFirst();
	if (New_ITRHypotheses.IsValid())
	{
		// Récupère le plan hypothèse en cours
		m_TblHypotheses.MoveTo(m_IdxHypotheseCourant);
		if(!m_TblHypotheses.IsValid()) throw JInvalidCall::GetInstance();

		// on récupère le plan de l'hypothèse courante
		CPlanInsertions & PLNInsertions = m_TblHypotheses.GetItem();

		// Recopie dans nouveau plan
		CPlanInsertions NewPLNInsertions;
		NewPLNInsertions = PLNInsertions;

		// Ajoute le support
		CAjoutInsertion::AjoutInsertionSuccessif(Insertion.m_IdSupport, Insertion.m_IdFormat, NewPLNInsertions, m_KEYPeriodeActionTablier.GetDateD(), m_KEYPeriodeActionTablier.GetDateF(),1,this);

		// Récupère audiences avec le support ajouté
		New_ITRHypotheses.CalcDatHypoFActif(m_KEYPeriodeAction, m_KEYPeriodeActionTablier,NewPLNInsertions,m_IdItemSel);
		JITPDATHypothese& NewDATHypo = New_ITRHypotheses.GetItem().GetDATHypoFormatActif();

		// Couverture nouvelle
		JFlt64 NewCouverture = NewDATHypo.m_Couverture; 

		// Récupère les audiences plan en cours
		// ITRHypotheses.CalcDatHypoFActif(m_KEYPeriodeAction, m_KEYPeriodeActionTablier,PLNInsertions,m_IdItemSel);
		// JITPDATHypothese& CurDATHypo = ITRHypotheses.GetItem().GetDATHypoFormatActif();

		// Calcul de l'apport via ce support ajouté sur le plan courant
		Apport = NewCouverture - CurCouverture; 

	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calcul apport via l'hypothèse courante
/*
JVoid JATPDocCreuset::InitApport(const JITPTBLHypotheses & ITRHypotheses, CInsertion & Insertion, JFlt64 & Apport)
{
	// Init apport
	Apport = -1;

	
}
*/

// Ajout des nouveaux supports de l'offre
JVoid JATPDocCreuset::AjouteSupportsPlateau()
{
	// Invalide en mode plan étendu
	if (m_AFFPlanEtendu)
		return; 

	// Flag si au moins 1 ajout support dans plateau
	JBool AjoutSupport = false;

	// Validation de tous les élmts de l'offre pour le plateau
	for (JInt32 InxElmt = 0; InxElmt < m_EchelleSupports.GetCount();InxElmt++) 
	{		
		// on récupère l'identifiant du Support
		const JUnt32 & IdSupport = m_EchelleSupports.Item(InxElmt);

		CKeySupport KeySup;
		KeySup.m_IdSupport = IdSupport;

		// on recherche le Support dans le plateau
		JInt32 IdxSupport = m_PlateauSupports.FindItem(KeySup);

		// on teste si support non présent dans plateau
		if (IdxSupport < 0)
		{
			// on teste les exceptions
			try
			{
				// on ajoute le Support au plateau
				m_PlateauSupports.Add(KeySup);
				AjoutSupport = true;
			}
			catch (JException* pException)
			{
				// on affiche le message
				::AfxMessageBox(pException->GetMessage(), MB_ICONERROR | MB_OK);
			}
		}
	}

	if (AjoutSupport)
	{
		// on positionne l'indicateur de modification
		SetModifiedFlag(TRUE);

		// on met à jour les vues du document
		UpdateAllViews(UPDATE_ETUDE);
	}
}

JVoid JATPDocCreuset::OnMakeEchelles(const JATPEchelleSupports & EchelleSupports, const JATPKEYEtude & KEYEtude, const JATPKEYPeriodeAction & KEYPeriodeAction, JUnt32 IdItem, JITPTBLEchelle & ITREchelle, JITPTBLHypotheses & ITRHypotheses, JBool RecalculApport)
{
	// on récupère le nombre de Supportx
	JInt32 NbSupports = EchelleSupports.GetCount();

	// Voir si hypothèse courante valide
	ITRHypotheses.MoveTo(m_IdxHypotheseCourant);
	
	// on boucle sur tous les Supportx
	for (JInt32 IdxSupport = 0; IdxSupport < NbSupports; IdxSupport += 1)
	{
		// on récupère l'identifiant du Support
		const JUnt32 & IdSupport = EchelleSupports.Item(IdxSupport);

		// on calcule l'échelle
		ITREchelle.Add(IdSupport, KEYEtude, KEYPeriodeAction, ITRHypotheses, false, RecalculApport);
	}

	// Calcul apport effectué
	if (RecalculApport)
	{
		m_ApportARecalculer = false;
	}		
	else
	{
		m_ApportARecalculer = true;
	}		
}

/* Avant
JVoid JATPDocCreuset::OnMakeEchelles(const JATPEchelleSupports & EchelleSupports, const JATPKEYEtude & KEYEtude, const JATPKEYPeriodeAction & KEYPeriodeAction, JUnt32 IdItem, JITPTBLEchelle & ITREchelle) const
{
	// on récupère le nombre de Supportx
	JInt32 NbSupports = EchelleSupports.GetCount();

	// on boucle sur tous les Supportx
	for (JInt32 IdxSupport = 0; IdxSupport < NbSupports; IdxSupport += 1)
	{
		// on récupère l'identifiant du Support
		const JUnt32 & IdSupport = EchelleSupports.Item(IdxSupport);

		// on calcule l'échelle
		ITREchelle.Add((IdSupport, KEYEtude, KEYPeriodeAction, false, false);
	}
}
*/

//////////////////////////////////////
// la fonction pour trier les échelles

JVoid JATPDocCreuset::OnSortEchelles(const JATPAFFEchelle & AFFEchelle,const JITPTBLEchelle & ITREchelle, JATPEchelleSupports & EchelleSupports) const
{
	// Tri par libellé support
	if      (AFFEchelle.IsSortBySupportsUp())		JATPEchelle_Tri::SortBySupportsUp		(ITREchelle, EchelleSupports);
	else if (AFFEchelle.IsSortBySupportsDn())		JATPEchelle_Tri::SortBySupportsDn		(ITREchelle, EchelleSupports);

	// Tri par périodicité
	else if (AFFEchelle.IsSortByPeriodiciteUp())     JATPEchelle_Tri::SortByPeriodiciteUp    (ITREchelle, EchelleSupports);
	else if (AFFEchelle.IsSortByPeriodiciteDn())     JATPEchelle_Tri::SortByPeriodiciteDn    (ITREchelle, EchelleSupports);

	// Tri par tarif
	else if (AFFEchelle.IsSortByTarifsUp())			JATPEchelle_Tri::SortByTarifsUp			(ITREchelle, EchelleSupports);
	else if (AFFEchelle.IsSortByTarifsDn())			JATPEchelle_Tri::SortByTarifsDn			(ITREchelle, EchelleSupports);

	// Tri par GRP
	else if (AFFEchelle.IsSortByGRPUp())			JATPEchelle_Tri::SortByGRPUp			(ITREchelle, EchelleSupports);
	else if (AFFEchelle.IsSortByGRPDn())			JATPEchelle_Tri::SortByGRPDn			(ITREchelle, EchelleSupports);

	// Tri par Couverture
	else if (AFFEchelle.IsSortByCouverturesUp())	JATPEchelle_Tri::SortByCouverturesUp	(ITREchelle, EchelleSupports);
	else if (AFFEchelle.IsSortByCouverturesDn())	JATPEchelle_Tri::SortByCouverturesDn	(ITREchelle, EchelleSupports);

	// Tri par Cout GRP
	else if (AFFEchelle.IsSortByCoutsGRPUp())		JATPEchelle_Tri::SortByCoutsGRPUp		(ITREchelle, EchelleSupports);
	else if (AFFEchelle.IsSortByCoutsGRPDn())		JATPEchelle_Tri::SortByCoutsGRPDn		(ITREchelle, EchelleSupports);

	// Tri par Répétition
	else if (AFFEchelle.IsSortByRepetitionsUp())	JATPEchelle_Tri::SortByRepetitionsUp	(ITREchelle, EchelleSupports);
	else if (AFFEchelle.IsSortByRepetitionsDn())	JATPEchelle_Tri::SortByRepetitionsDn	(ITREchelle, EchelleSupports);

	// Tri par Nb Individus
	else if (AFFEchelle.IsSortByNbIndividusUp())	JATPEchelle_Tri::SortByNbIndividusUp	(ITREchelle, EchelleSupports);
	else if (AFFEchelle.IsSortByNbIndividusDn())	JATPEchelle_Tri::SortByNbIndividusDn	(ITREchelle, EchelleSupports);

	// Tri par Nb Contacts
	else if (AFFEchelle.IsSortByNbContactsUp())		JATPEchelle_Tri::SortByNbContactsUp		(ITREchelle, EchelleSupports);
	else if (AFFEchelle.IsSortByNbContactsDn())		JATPEchelle_Tri::SortByNbContactsDn		(ITREchelle, EchelleSupports);

	// Tri par Indice Affinité
	else if (AFFEchelle.IsSortByIndAffiniteUp())	JATPEchelle_Tri::SortByIndAffiniteUp	(ITREchelle, EchelleSupports);
	else if (AFFEchelle.IsSortByIndAffiniteDn())	JATPEchelle_Tri::SortByIndAffiniteDn	(ITREchelle, EchelleSupports);

	// Tri par % Affinité
	else if (AFFEchelle.IsSortByPrcAffiniteUp())	 JATPEchelle_Tri::SortByPrcAffiniteUp	(ITREchelle, EchelleSupports);
	else if (AFFEchelle.IsSortByPrcAffiniteDn())	JATPEchelle_Tri::SortByPrcAffiniteDn	(ITREchelle, EchelleSupports);

	// Tri par LDP
	else if (AFFEchelle.IsSortByLDPUp())			JATPEchelle_Tri::SortByLDPUp			(ITREchelle, EchelleSupports);
	else if (AFFEchelle.IsSortByLDPDn())			JATPEchelle_Tri::SortByLDPDn			(ITREchelle, EchelleSupports);

	// Tri par Reprise en main
	else if (AFFEchelle.IsSortByRepriseUp())		JATPEchelle_Tri::SortByRepriseUp		(ITREchelle, EchelleSupports);
	else if (AFFEchelle.IsSortByRepriseDn())		JATPEchelle_Tri::SortByRepriseDn		(ITREchelle, EchelleSupports);

	// Tri par Apport
	else if (AFFEchelle.IsSortByApportUp())			JATPEchelle_Tri::SortByApportUp			(ITREchelle, EchelleSupports);
	else if (AFFEchelle.IsSortByApportDn())			JATPEchelle_Tri::SortByApportDn			(ITREchelle, EchelleSupports);

}

// la fonction pour savoir si l'offre change la sélection terrain et cible en cours
JBool JATPDocCreuset::OffreCurSelTerrainCurSelCibleChange()
{
	// La sélection cible actuelle est-elle toujours dans l'offre
	bool CibleOk = false;
	for (m_Offre.m_TBLIdItems.MoveFirst(); m_Offre.m_TBLIdItems.IsValid(); m_Offre.m_TBLIdItems.MoveNext())
	{
		if (m_Offre.m_TBLIdItems.GetItem() == m_IdItemSel)
		{
			CibleOk = true;
			break;
		}			
	}
	
	// La sélection terrain actuelle est-elle toujours dans l'offre
	bool TerrainOk = false;
	for (m_Offre.m_TBLIdTerrainsAudience.MoveFirst(); m_Offre.m_TBLIdTerrainsAudience.IsValid(); m_Offre.m_TBLIdTerrainsAudience.MoveNext())
	{
		if (m_Offre.m_TBLIdTerrainsAudience.GetItem() == m_IdTerrainCiblageSel)
		{
			TerrainOk = true;
			break;
		}			
	}	

	if (!CibleOk || !TerrainOk)
		return true;
	else
		return false;
}

// la fonction pour supprimer les insertions (hypothèse courante / formats actifs / période action)
JVoid JATPDocCreuset::OnSuppInsertion(JUnt32 idxHypo, bool FormatActif)
{

	// Suppression sur la période d'action ???? A VOIR
	JDate DateD = m_KEYPeriodeAction.GetDateD();
	JDate DateF = m_KEYPeriodeAction.GetDateF();

	// on initialise le code
	JInt32 Ret = 0;

	// Va sur l'hypothèse courante
	m_TblHypotheses.MoveTo(idxHypo);
	if (m_TblHypotheses.IsValid())
	{
		// on récupère les insertions de l'hypothèse courante
		CPlanInsertions & PLNInsertions = m_TblHypotheses.GetItem();
		CPlanInsertions Tmp;

		// Nb insertions
		if (PLNInsertions.GetCount())
		{
			// on balaye toutes les insertions
			for (PLNInsertions.MoveFirst_SFD(); PLNInsertions.IsValid_SFD(); PLNInsertions.MoveNext_SFD())
			{
				// on récupère l'insertion courante
				CInsertion * pInsertion = PLNInsertions.GetItem_SFD();

				// Test si au moins 1 insertion
				if (pInsertion->HasInsertions())
				{	
					// On boucle sur tous les formats actifs
					for (m_MapIdFormatsSel.MoveFirst();m_MapIdFormatsSel.IsValid();m_MapIdFormatsSel.MoveNext())
					{
						// Récupére état du format
						JUnt32 Etat = m_MapIdFormatsSel.GetItem();

						// Id format courant
						JUnt32 IdFormatCur = m_MapIdFormatsSel.GetKey();

						// on balaye tous les supports du plateau
						for (JInt32 i = 0;i < m_PlateauSupports.GetCount();i++)
						{
							// Le support fait partie du plateau courant, donc insertion(s) existante(s) possible(s)
							const CKeySupport & KeySupport = m_PlateauSupports.Item(i);

							// Test si format courant
							if (pInsertion->m_IdFormat != IdFormatCur)
								continue;

							// Insertion pour ce support n'appartenant pas à la période d'action, supp impossible
							if (pInsertion->m_DateD < DateD || pInsertion->m_DateD > DateF ) 
								if (pInsertion->m_IdSupport	== KeySupport.m_IdSupport && 
 									pInsertion->m_IdParent  == KeySupport.m_IdParent)
								Tmp.AddInsertion(*pInsertion,DateD,DateF);

							// Voir si insertion est du niveau parent
							if(pInsertion->m_IdParent == 0 )
							{
								// On ajoute dans plan temporaire que si insertions sur format non actif
								if (pInsertion->m_IdSupport	== KeySupport.m_IdSupport && 
									pInsertion->m_IdParent  == KeySupport.m_IdParent  &&
									pInsertion->m_IdFormat  == IdFormatCur            &&  
									((FormatActif&&Etat == 2) || Etat == 0)			  &&
  									pInsertion->HasInsertions())
								{
									// Insertion non supprimée , on ajoute au map insertion tmp
									Tmp.AddInsertion(*pInsertion,DateD,DateF);
								}
							}
						}
					}
				}
			}

			// Puis on remet en place la mise à jour plan insertions
			PLNInsertions.Swap(Tmp); 

			// Test si encore des insertions pour ce plan et si toutes les hypothèses derrière vide
			if (PLNInsertions.GetCount() == 0  && 
				m_TblHypotheses.GetCount() > 2 &&
				idxHypo == m_TblHypotheses.GetCount()-2)
			{
				// Suppression de l'hypothese
				OnSuppHypothese(idxHypo);

				// Attention à hypothese active
				if (idxHypo <	m_IdxHypotheseActif &&	m_IdxHypotheseActif > 1)
					m_IdxHypotheseActif--;

				// Nouvelle hypothèse active
				m_MapAffHypothese.Set(idxHypo, false);
				m_MapAffHypothese.ResetSet(m_IdxHypotheseActif);

				//m_MapAffHypothese.MoveTo(m_IdxHypotheseActif);
				//if(!m_MapAffHypothese.IsValid())
				//	m_MapAffHypothese.Add(m_IdxHypotheseActif) = true;
				//else
				//	m_MapAffHypothese.GetItem() = true;

				// le plan creuset est modifié
				SetModifiedFlag(TRUE); 
				m_AFFCreuset.SetHypoActif(false);
				UpdateAllViews(UPDATE_AFF_CREUSET);
			}	
		}
	}

	// on teste l'affichage du zoom
	if (m_AFFZoomPlan.IsVisible())
		// on met à jour l'affichage du zoom
		m_AFFZoomPlan.Reset();

}

// Suppression d'une hypothese
JVoid JATPDocCreuset::OnSuppHypothese(JUnt32 IdxHypothese)
{
	// Si ce n'est pas la dernière hypothèse, on décale les hypothèses
	if (IdxHypothese < m_TblHypotheses.GetCount())
	{

		// Crée un nouveau tableau de plans hypotheses
		JList<CPlanInsertions> TblHypo;

		// Démarre la recopie des hypothèses à partir de la fin
		JBool HypoActive = false;
		for (JInt32 i = m_TblHypotheses.GetCount()-1;i>=0;i--)
		{
			if (IdxHypothese != i )
			{
				// On se positionne sur l'hypothèse
				m_TblHypotheses.MoveTo(i);

				// on récupère les insertions de l'hypothèse courante
				CPlanInsertions & PLNInsertions = m_TblHypotheses.GetItem();

				// on ajoute aux hypothèses temporaires
				if (PLNInsertions.GetCount() > 0 || HypoActive == true) 
				{
					// modif 1/03/2004
					TblHypo.AddTail() =  PLNInsertions;
					HypoActive = true;
				}
			}
		}
	

		// on inverse le tableau
		JList<CPlanInsertions> TblHypo2;

		for (JInt32 i =TblHypo.GetCount()-1;i>=0;i--)
		{
			// On se positionne sur l'hypothèse
			TblHypo.MoveTo(i);

			// on récupère les insertions de l'hypothèse courante
			CPlanInsertions & PLNInsertions = TblHypo.GetItem();
			TblHypo2.AddTail() =  PLNInsertions;
		}		

		// Si dernière hypothèse non vide et < Maxhypothese on ajoute une hypothèse vide
		if (TblHypo2.GetCount() == 0)
			TblHypo2.AddTail();

		// Complète avec hypothèse vide s'il y a lieu
		if (TblHypo2.GetCount() > 1)
		{
			TblHypo2.MoveTo(TblHypo2.GetCount()-1); 
			if (TblHypo2.IsValid())
			{
				CPlanInsertions & PLNInsertions = TblHypo2.GetItem();
				if (PLNInsertions.GetCount() != 0 && TblHypo2.GetCount() < m_MaxHypothese)
					// on ajoute une hypothèse vide
					TblHypo2.AddTail();
			}
		}
		else
			// ici seulement hypothese de base, il faut en rajouter une
			TblHypo2.AddTail();

		// Swap all Hypotheses
		m_TblHypotheses.Swap(TblHypo2);


		// Réinitialiser l'hypothèse courante
		if (m_IdxHypotheseCourant >= m_TblHypotheses.GetCount())
			m_IdxHypotheseCourant = m_TblHypotheses.GetCount()-1;

	}

}

// Récup ligne active (plan de test) selon mode visualisation (offre ou plateau)
JUnt32 JATPDocCreuset::OnRecupLigneSuppActif(JUnt32 IdSupport)
{
	if (m_AFFOffreHypos)
	{
		// Recherche n° ligne via liste plateau supports
		for (JInt32 i=0;i<m_EchelleSupports.GetCount();i++)
		{
			const JUnt32 &IdSupp = m_EchelleSupports.Item(i);
			// Si même ident, n° ligne trouvé on quitte
			if (IdSupp == IdSupport)
				return (i+1);
		}
	}
	else
	{
		// Recherche n° ligne via liste offre supports
		for (JInt32 i=0;i<m_PlateauSupports.GetCount();i++)
		{
			// Récup infos support dans plateau
			const CKeySupport & KeySupport = m_PlateauSupports.Item(i);
				
			// Si même ident, n° ligne trouvé on quitte
			if (KeySupport.m_IdSupport == IdSupport)
				return (i+1);
		}
	}

	// pour tous les autres cas
	return 0;
}	

// Recup nb lignes supports actifs toutes hypothèses
JUnt32 JATPDocCreuset::OnNbLigPlanActif()
{
	JUnt32 NbLigActif = 0;

	// Dates pour la recherche
	JDate DateD;
	JDate DateF;

	if (m_IdxHypotheseCourant != 0)
	{
		// on prendra les dates de la période action
		DateD = m_KEYPeriodeAction.GetDateD();
		DateF = m_KEYPeriodeAction.GetDateF();
	}
	else
	{
		// Cas du plan étendu, on prend les dates de l'étude
		DateD = m_KEYEtude.GetDateD();
		DateF = m_KEYEtude.GetDateF();
	}

	// balaye le plateau des supports surla période d'action
	JMap <JUnt32, bool> MapSuppActif;
	MapSuppActif.Reset();

	for (JUnt32 IdxHypothese = 1; IdxHypothese < m_TblHypotheses.GetCount(); IdxHypothese++)
	{
		for (JInt32 j = 0;j < m_PlateauSupports.GetCount();j++)
		{
			if (OnGetNbInsertions(m_PlateauSupports.Item(j).m_IdSupport, DateD, DateF, IdxHypothese))
			{
				MapSuppActif.MoveTo(m_PlateauSupports.Item(j).m_IdSupport);
				if (!MapSuppActif.IsValid())
				{
					MapSuppActif.Add(m_PlateauSupports.Item(j).m_IdSupport) = true;
				}
			}
		}
	}


	// Renvoi le nombre de lignes plateau avec insertions sur la période demandé
	NbLigActif = MapSuppActif.GetCount();
	return NbLigActif;
}

JUnt32 JATPDocCreuset::GetIndexSuppPlateauViaIndexSuppActif(JUnt32 IndSupportActif)
{
	// Dates pour la recherche
	JDate DateD;
	JDate DateF;

	if (m_IdxHypotheseCourant != 0)
	{
		// on prendra les dates de la période action
		DateD = m_KEYPeriodeAction.GetDateD();
		DateF = m_KEYPeriodeAction.GetDateF();
	}
	else
	{
		// Cas du plan étendu, on prend les dates de l'étude
		DateD = m_KEYEtude.GetDateD();
		DateF = m_KEYEtude.GetDateF();
	}

	JUnt32 InxSuppActif = 0;
	for (JUnt32 j = 0; j < m_PlateauSupports.GetCount(); j++)
	{
		for (JUnt32 IdxHypothese = 1; IdxHypothese < m_TblHypotheses.GetCount(); IdxHypothese++)
		{
			if (OnGetNbInsertions(m_PlateauSupports.Item(j).m_IdSupport, DateD, DateF, IdxHypothese))
			{
				InxSuppActif++;
				break;
			}
		}
		
		if ((InxSuppActif-1) == IndSupportActif)
		{
			return j;
		}
	}

	return IndSupportActif;
}

// Récup nb lignes active du plateau courant
JUnt32 JATPDocCreuset::OnNbLigPlanPlateau(JBool PlanEtendu)
{
	// Init nb de lignes plan (plaetau compacté)
	JUnt32 NbLigPlan = 0;

	// Dates pour la recherche
	JDate DateD;
	JDate DateF;

	if (m_IdxHypotheseCourant != 0)
	{
		// on prendra les dates de la période action
		DateD = m_KEYPeriodeAction.GetDateD();
		DateF = m_KEYPeriodeAction.GetDateF();
	}
	else
	{
		// Cas du plan étendu, on prend les dates de l'étude
		DateD = m_KEYEtude.GetDateD();
		DateF = m_KEYEtude.GetDateF();
	}

	if (PlanEtendu)
	{
		// balaye le plateau étendu
		for (JInt32 j = 0;j < m_PlateauSupportsPlanEtendu.GetCount();j++)
		{
			if (OnGetNbInsertions(m_PlateauSupportsPlanEtendu.Item(j).m_IdSupport,DateD,DateF,0))
			{
				NbLigPlan++;
			}
		}
	}
	else
	{
		// balaye le plateau des supports surla période d'action
		for (JInt32 j = 0;j < m_PlateauSupports.GetCount();j++)
		{
			if (OnGetNbInsertions(m_PlateauSupports.Item(j).m_IdSupport,DateD,DateF,m_IdxHypotheseCourant))
			{
				NbLigPlan++;
			}
		}
	}

	// Renvoi le nombre de lignes plateau avec insertions sur la période demandé
	return NbLigPlan;
}

// Récup Index Plateau via no ligne plan
JInt32 JATPDocCreuset::OnIndexPlateauViaIndexPlan(JUnt32 IndexSupportPlan)
{
	
	// Dates pour la recherche
	JDate DateD;
	JDate DateF;

	if (m_AFFPlanEtendu)
	{
		// balaye le plateau plan étendu
		// Cas du plan étendu, on prend les dates de l'étude
		DateD = m_KEYEtude.GetDateD();
		DateF = m_KEYEtude.GetDateF();
		JUnt32 InxCurPlan = -1;
		for (JInt32 j = 0;j < m_PlateauSupportsPlanEtendu.GetCount();j++)
		{
			if (OnInsertionSupport(j,DateD,DateF,m_IdxHypotheseCourant))
			{
				// Ce support a des insertions, il appartient donc au plan
				InxCurPlan ++;
				if (InxCurPlan == IndexSupportPlan)
				{
					// on se trouve sur le bon support plan, renvoi index support plateau
					return j;
				}
			}
		}
	}
	else
	{
		// balaye le plateau hypothèses
		// on prendra les dates de la pérode action
		DateD = m_KEYPeriodeAction.GetDateD();
		DateF = m_KEYPeriodeAction.GetDateF();
		JUnt32 InxCurPlan = -1;
		for (JInt32 j = 0;j < m_PlateauSupports.GetCount();j++)
		{
			if (OnInsertionSupport(j,DateD,DateF,m_IdxHypotheseCourant))
			{
				// Ce support a des insertions, il appartient donc au plan
				InxCurPlan ++;
				if (InxCurPlan == IndexSupportPlan)
				{
					// on se trouve sur le bon support plan, renvoi index support plateau
					return j;
				}
			}
		}
	}
	// Index plan non trouvé
	return -1;

}

JUnt32 JATPDocCreuset::OnGetNbInsertions(JUnt32 IdSupport,JDate DateDebut, JDate DateFin, JUnt32 IdHypothese)
{
	// on initialise le nombre d'insertions
	JUnt32 NbInsertions = 0;

	// on vérifie la validité de la période d'action
	if (DateDebut.IsValid() && DateFin.IsValid())
	{

		// Va sur l'hypothese 
		m_TblHypotheses.MoveTo(IdHypothese);
		if (m_TblHypotheses.IsValid())
		{
			// on récupère les insertions de l'hypothèse courante
			CPlanInsertions & PLNInsertions = m_TblHypotheses.GetItem();

			// Récup Index Support dans plateau
			JInt32 j;
			JInt32 InxSuppPlateau = -1;
			for (j = 0;j < m_PlateauSupports.GetCount();j++)
			{
				if (m_PlateauSupports.Item(j).m_IdSupport == IdSupport)
				{
					// Ce support fait partie du plateau, en index j
					InxSuppPlateau = j;
					break;
				}
			}

			if (InxSuppPlateau >= 0)
			{
				// Le support fait partie du plateau courant, donc insertion(s) existante(s) possible(s)
				// const CKeySupport & Support = m_PlateauSupports.Item(j);
				const CKeySupport & Support = m_PlateauSupports.Item(InxSuppPlateau);

				// on crée la clé de l'insertion
				CInsKey Key;
				Key.m_IdSupport		=	Support.m_IdSupport;
				Key.m_IdParent		=	Support.m_IdParent; 
				Key.m_DateD			=	DateDebut;
				Key.m_IdFormat		=	0;

				JList<CInsKey> List;

				if(PLNInsertions.FindInsertions(Key,DateFin,List))
				{
					// Filtre avec les formats actifs
					JList<CInsKey> Tmp;
					for(List.MoveFirst(); List.IsValid(); List.MoveNext())
					{
						m_MapIdFormatsSel.MoveTo(List.GetItem().m_IdFormat);
						if(m_MapIdFormatsSel.IsValid())
						{
							// Récupére état du format
							JUnt32 Etat = m_MapIdFormatsSel.GetItem();
							if (Etat & 0x0003)
								Tmp.AddTail() = List.GetItem();
						}
					}
					// on met à jour l'affichage du zoom
					if(Tmp.GetCount())
					{
						for(Tmp.MoveFirst(); Tmp.IsValid(); Tmp.MoveNext())
						{
							CInsKey Key = Tmp.GetItem();
							CInsertion *pInsertion = PLNInsertions.FindInsertion(Key);
							if(pInsertion == 0)
								continue;
							NbInsertions += pInsertion->m_NbInsertions;
						}
					}
				}
			}
		}
	}

	// on renvoie le nombre d'insertions
	return (NbInsertions);
}

JBool JATPDocCreuset::OnInsertionSupport(JUnt32 InxSupportPlateau,JDate DateDebut, JDate DateFin, JUnt32 IdHypothese)
{
	// on vérifie la validité de la période d'action
	if (DateDebut.IsValid() && DateFin.IsValid())
	{

		// Va sur l'hypothese 
		m_TblHypotheses.MoveTo(IdHypothese);
		if (m_TblHypotheses.IsValid())
		{
			//// on récupère les insertions de l'hypothèse courante
			CPlanInsertions &PLNInsertions = m_TblHypotheses.GetItem();

			if (InxSupportPlateau >= 0)
			{
				// Le support fait partie du plateau courant, donc insertion(s) existante(s) possible(s)
				const CKeySupport & Support = m_PlateauSupports.Item(InxSupportPlateau);

				// on crée la clé de l'insertion
				CInsKey Key;
				Key.m_IdSupport		=	Support.m_IdSupport;
				Key.m_IdParent		=	Support.m_IdParent; 
				Key.m_DateD			=	DateDebut;
				Key.m_IdFormat		=	0;

				JList<CInsKey> List;

				// Récupère les insertions pour cette clé
				if(PLNInsertions.FindInsertions(Key,DateFin,List))
				{
					// Filtre avec les formats actifs
					JList<CInsKey> Tmp;
					for(List.MoveFirst(); List.IsValid(); List.MoveNext())
					{
						m_MapIdFormatsSel.MoveTo(List.GetItem().m_IdFormat);
						if(m_MapIdFormatsSel.IsValid())
						{
							// Récupére état du format
							JUnt32 Etat = m_MapIdFormatsSel.GetItem();
							if (Etat & 0x0003)
							{
								// Format actif
								CInsKey Key = List.GetItem();
								CInsertion *pInsertion = PLNInsertions.FindInsertion(Key);
								if(pInsertion != 0)
									// Au moins 1 insertion trouvée
									return true;
							}
						}
					}
				}
			}
		}
	}

	// pas d'insertions pour ce support
	return false;
}


// la fonction pour supprimer les insertions (toutes hypothèses  période action)
JVoid JATPDocCreuset::OnSuppInsertionAllHypo()
{
	for (JUnt32 idx = m_TblHypotheses.GetCount(); idx>0; idx--)
		OnSuppInsertion(idx, false);
	m_MapAffHypothese.ResetSet(1);
}

//////////////////////////////////////////

JVoid JATPDocCreuset::Send(JStream & Stream) 
{
	CATPDocSerializer::Send(Stream, this);
}

JVoid JATPDocCreuset::Recv(JStream & Stream)
{
	CATPDocSerializer::Recv(Stream, this);
}

//////////////////////////////////////
// la fonction pour permuter l'élément
JVoid JATPDocCreuset::Swap(JATPDocCreuset & Source)
{
	// Swap the elements
	JUnt32	IdTmp;
	m_KEYEtude					.Swap(Source.m_KEYEtude);
	m_KEYPeriodeAction			.Swap(Source.m_KEYPeriodeAction);
	m_KEYPeriodeActionHyp		.Swap(Source.m_KEYPeriodeActionHyp);
	m_KEYPeriodeActionTablier	.Swap(Source.m_KEYPeriodeActionTablier);


	IdTmp						=	m_IdItemSel;
	m_IdItemSel					=	Source.m_IdItemSel;

	// Indique au moteur
	m_MoteurPresse.SetItemCourant(m_IdItemSel);

	Source.m_IdItemSel			=	IdTmp;

	IdTmp						=	m_IdSrcAudienceSel;
	m_IdSrcAudienceSel			=	Source.m_IdSrcAudienceSel;
	Source.m_IdSrcAudienceSel	=	IdTmp;

	IdTmp						=	m_IdSrcCiblageSel;
	m_IdSrcCiblageSel			=	Source.m_IdSrcCiblageSel;
	Source.m_IdSrcCiblageSel	=	IdTmp;

	m_MapIdTerrainsAudienceSel	.Swap(Source.m_MapIdTerrainsAudienceSel);  

	IdTmp							=	m_IdTerrainCiblageSel;
	m_IdTerrainCiblageSel			=	Source.m_IdTerrainCiblageSel;
	Source.m_IdTerrainCiblageSel	=	IdTmp;

	m_MapIdFormatsSel				.Swap(Source.m_MapIdFormatsSel);

	IdTmp							=	m_IdSegmentAudienceSel;
	m_IdSegmentAudienceSel			=	Source.m_IdSegmentAudienceSel; 
	Source.m_IdSegmentAudienceSel	=	IdTmp;

	IdTmp							=   m_IdSegmentCibleSel;	
	m_IdSegmentCibleSel				=	Source.m_IdSegmentCibleSel;
	Source.m_IdSegmentCibleSel		=	IdTmp;

	// Permut infos filtre audience
	IdTmp							=	m_IdFiltreAudienceSel;
	m_IdFiltreAudienceSel			=	Source.m_IdFiltreAudienceSel;
	Source.m_IdFiltreAudienceSel	=	IdTmp;


	m_EchelleSupports			.Swap(Source.m_EchelleSupports);
	m_PlateauSupports			.Swap(Source.m_PlateauSupports);
	m_PlateauSupportsPlanEtendu	.Swap(Source.m_PlateauSupportsPlanEtendu);

	m_Offre							.Swap(Source.m_Offre);

	IdTmp							=	m_IdxHypotheseCourant;
	m_IdxHypotheseCourant			=	Source.m_IdxHypotheseCourant;
	Source.m_IdxHypotheseCourant	=	IdTmp;

	IdTmp							=	m_IdxHypotheseActif;
	m_IdxHypotheseActif				=	Source.m_IdxHypotheseActif;
	Source.m_IdxHypotheseActif		=	IdTmp;

	m_MapAffHypothese			.Swap(Source.m_MapAffHypothese);
	m_TblHypotheses				.Swap(Source.m_TblHypotheses);
	m_MasterPlan				.Swap(Source.m_MasterPlan);
	m_TblPlanArchive			.Swap(Source.m_TblPlanArchive);  	

	// Les items cibles 
	m_MAPpItems					.Swap(Source.m_MAPpItems);
	
	// Les items cibles pondérées
	m_MAPpItemsPonderes			.Swap(Source.m_MAPpItemsPonderes);   

	m_MapIdCouplageExpanded		.Swap(Source.m_MapIdCouplageExpanded);

	m_ITRHypotheses				.Swap(Source.m_ITRHypotheses);
	m_ITREchelle				.Swap(Source.m_ITREchelle);

	m_listFormatsQP.			Swap(Source.m_listFormatsQP);

	m_mapSubstFormat.Swap(Source.m_mapSubstFormat);

	// Swap de la table des tarifs manuels courants
	m_MapTarifManuel.Swap(Source.m_MapTarifManuel);  

}

/////////////////////////////////////////////////////////////////
// Copy Hypothese dans pressePapier(interne au document)
void JATPDocCreuset::CopierPlanClipBoard()
{
	m_PlanClipBoard.Reset();

	// Va sur hypo source
	m_TblHypotheses.MoveTo(m_IdxHypotheseActif);
	if(!m_TblHypotheses.IsValid())
		return;

	//Récup plan de cette hypothèse
	for(m_TblHypotheses.GetItem().MoveFirst_SDF(); m_TblHypotheses.GetItem().IsValid_SDF(); m_TblHypotheses.GetItem().MoveNext_SDF())
	{
		
		CInsertion *pData =	m_TblHypotheses.GetItem().GetItem_SDF();
		//on ne doit copier que les insertions des formats actifs
		m_MapIdFormatsSel.MoveTo(pData->m_IdFormat);
		if(m_MapIdFormatsSel.GetItem() >= 2)
			m_PlanClipBoard.AddInsertion(*pData,m_KEYPeriodeAction.GetDateD(),m_KEYPeriodeAction.GetDateF());
	}

	m_bSomethingToPaste = true;

}
void JATPDocCreuset::CollerPlanClipBoard()
{
	// Va sur hypo destination
	m_TblHypotheses.MoveTo(m_IdxHypotheseActif);
	if(!m_TblHypotheses.IsValid())
		return;

	m_TblHypotheses.GetItem() = m_PlanClipBoard;

	UpdateAllViews(UPDATE_CURRENT_HYPO);
	
	// Ajout hypothèse vierge
	if(m_IdxHypotheseActif < m_MaxHypothese && m_IdxHypotheseActif == m_TblHypotheses.GetCount()-1)
	{
		AddNewHypo();
		UpdateAllViews(UPDATE_AFF_CREUSET);
	}


	SetModifiedFlag();
}

void JATPDocCreuset::AjouterPlanClipBoard()
{
	// Va sur hypo destination
	m_TblHypotheses.MoveTo(m_IdxHypotheseActif);
	if(!m_TblHypotheses.IsValid())
		return;

//	m_TblHypotheses.GetItem() = m_PlanClipBoard;
	for(m_PlanClipBoard.MoveFirst_SDF(); m_PlanClipBoard.IsValid_SDF(); m_PlanClipBoard.MoveNext_SDF())
	{
		// Récup Key et Data insertion
		CInsertion *pData =	m_PlanClipBoard.GetItem_SDF();
		// Ajoute l'insertion au plan
		m_TblHypotheses.GetItem().AddInsertion(*pData,m_KEYPeriodeAction.GetDateD(),m_KEYPeriodeAction.GetDateF());
	}

	UpdateAllViews(UPDATE_CURRENT_HYPO);
	
	// Ajout hypothèse vierge
	if(m_IdxHypotheseActif < m_MaxHypothese && m_IdxHypotheseActif == m_TblHypotheses.GetCount()-1)
	{
		AddNewHypo();
		UpdateAllViews(UPDATE_AFF_CREUSET);
	}

	SetModifiedFlag();
}

void JATPDocCreuset::AjouterSuccessifPlanClipBoard()
{
	// Va sur hypo destination
	m_TblHypotheses.MoveTo(m_IdxHypotheseActif);
	if(!m_TblHypotheses.IsValid())
		return;

	for(m_PlanClipBoard.MoveFirst_SDF(); m_PlanClipBoard.IsValid_SDF(); m_PlanClipBoard.MoveNext_SDF())
	{
		CInsertion *pData =	m_PlanClipBoard.GetItem_SDF();
		//for(int i = 0; i<pData->m_NbInsertions;i++)
		{
			CAjoutInsertion::AjoutInsertionSuccessif(
				m_PlanClipBoard.GetItem_SDF()->m_IdSupport,
				m_PlanClipBoard.GetItem_SDF()->m_IdFormat,
				m_TblHypotheses.GetItem(),
				m_KEYPeriodeAction.GetDateD(),
				m_KEYPeriodeAction.GetDateF(),pData->m_NbInsertions,this
			);
		}
	}

	UpdateAllViews(UPDATE_CURRENT_HYPO);
	//// Ajout hypothèse vierge
	if(m_IdxHypotheseActif < m_MaxHypothese && m_IdxHypotheseActif == m_TblHypotheses.GetCount()-1)
	{
		AddNewHypo();
		UpdateAllViews(UPDATE_ETUDE);
	}

	SetModifiedFlag();
}

/////////////////////////////////////////////////////////////////
// Copy Master Plan to Hypothese
bool	JATPDocCreuset::CopyMasterPlanToHypothese(JUnt32 HypoIdx)
{
	// Do nothing if Plan Master of invalid index
	if(HypoIdx >= m_TblHypotheses.GetCount())
		return false;

	// Crée un nouveau plan idem au master
	CPlanInsertions PlnInsert = m_MasterPlan;

	// Swap insertions Master sur l'hypothèse actif
	m_TblHypotheses.MoveTo(HypoIdx);
	if(!m_TblHypotheses.IsValid())
		return false;

	m_TblHypotheses.GetItem().Swap(PlnInsert);

	// Ajout hypothèse vierge
	if( HypoIdx < m_MaxHypothese &&
		HypoIdx == m_TblHypotheses.GetCount()-1)
		AddNewHypo();

	return true;
}
////////////////////////////////////////////////////////////////
// Replace Plan with Hypothese
bool	JATPDocCreuset::ReplacePlanWithHypothese(CPlanInsertions *pDest, JUnt32 HypoIdx, JDate DateD, JDate DateF)
{
	// Get the hypothese to copy
	m_TblHypotheses.MoveTo(HypoIdx);
	if(!m_TblHypotheses.IsValid())
		return false;

	CPlanInsertions & Hypo	=	m_TblHypotheses.GetItem();
	CPlanInsertions & Plan	=	*pDest;
	CPlanInsertions Tmp;

	// Remove from the plan all the insertions within the Period d'Action
	for(Plan.MoveFirst_SDF(); Plan.IsValid_SDF(); Plan.MoveNext_SDF())
	{
		// Récup Key et Data insertion
		CInsertion *pData =	Plan.GetItem_SDF();
		// Récupére insertions plan si en dehors des bornes de la période d'action hypothese courante
		if(pData->m_DateD < DateD || pData->m_DateD > DateF)
			// Ajoute l'insertion au map temporaire
			Tmp.AddInsertion(*pData,m_KEYPeriodeAction.GetDateD(),m_KEYPeriodeAction.GetDateF());
	}
	// Insert all insertions from the Hypothese Actif into the Plan
	for(Hypo.MoveFirst_SDF(); Hypo.IsValid_SDF(); Hypo.MoveNext_SDF())
	{
		// Récup Key et Data insertion
		CInsertion * pData	=	Hypo.GetItem_SDF();

		// Garder uniquement les insertions dans la période d'action (y compris celles coupées)
		// if(!(pData->m_DateD + pData->m_Duree -1 < DateD || pData->m_DateD > DateF))
		if(!(pData->m_DateD < DateD || pData->m_DateD > DateF))
			Tmp.AddInsertion(*pData,m_KEYPeriodeAction.GetDateD(),m_KEYPeriodeAction.GetDateF());
	}
	// Crée un nouveau tableau de plans hypotheses
	// Swap new plan avec old plan
	Plan.Swap(Tmp);
	return true;
}


void JATPDocCreuset::InitMoteur()
{
	if (IsValid())
		m_MoteurPresse.Init(m_Offre, m_IdFiltreAudienceSel, m_bLDPQtd, m_bLNMMag, m_bLDP30J, m_bRepriseEnMain);
}

JATPDocCreuset::~JATPDocCreuset()
{
	// Sauve paramètre type moteur
	//AfxGetApp()->WriteProfileInt("Moteur", "Type", m_MoteurPresse.GetType());
}

void JATPDocCreuset::AddNewHypo()
{
	m_TblHypotheses.AddTail();
}

// Pour export des informations
void JATPDocCreuset::Accept(CATPIVisitor * pVisitor)
{
}


bool JATPDocCreuset::SubstitutFormat(JInt32 idSupport, JInt32 & idFormat)
{
	m_mapSubstFormat.MoveTo(idSupport);
	if(m_mapSubstFormat.IsValid())
	{
		idFormat = m_mapSubstFormat.GetItem();
		return true;
	}
	return false;
}

void JATPDocCreuset::SetSubstitutFormat(JInt32 idSupport, JInt32 idFormat/* = 0*/)
{
	m_mapSubstFormat.MoveTo(idSupport);
	if(idFormat == 0 && m_mapSubstFormat.IsValid())
	{
		m_mapSubstFormat.Remove();
		return;
	}

	if(!m_mapSubstFormat.IsValid())
		m_mapSubstFormat.Add(idSupport,true);
	 
	m_mapSubstFormat.GetItem() = idFormat;
}
