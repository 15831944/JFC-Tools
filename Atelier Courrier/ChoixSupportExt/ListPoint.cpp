// ListPoint.cpp: implementation of the CListPoint class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ListPoint.h"
#include "DlgMapping.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CListPoint::CListPoint()
{
	// Init pointeur document
	m_pDocChoixSupport = 0;

	// Init pointeur boite de dialogue mapping
	m_pDlgMapping = 0;

	// Libell� visible par d�faut
	LibelleVisible = true;
}

//////////////////////////////////////////////////////////////////////
// Fonction de r�cup�ration document et dialog mapping
void CListPoint::SetDocument(JATPDocChoixSupport* pDoc, CDlgMapping *pDlg)
{
	// R�cup�re pointeur sur document g�n�ral
	m_pDocChoixSupport = pDoc;

	// R�cup�ration fen�tre dialog mapping
	m_pDlgMapping = pDlg;

	// Update liste points
	this->Update(true);
}

CListPoint::~CListPoint()
{
	// on ne fait rien
}

/* VERSION SANS TENIR COMPTE DES SUPPORTS RESULTATS NON DEFINIS
///////////////////////////////////////////////////////////////////////
// Update liste des points mapping
//////////////////////////////////////////////////////////////////////
bool CListPoint::Update()
{
	// R�cup nb points � mapper // A REVOIR
	// AVANT
	//JUnt32 NbSupports = m_pDocChoixSupport->m_MapIdSupportsSel.GetCount();
	//m_Liste.SetSize(NbSupports);
	//

	// Associer au graphe mapping uniquement les supports avec r�sultats d�finis
	JUnt32 NbSuppValid = GetNbPointValid();
	m_Liste.SetSize(NbSuppValid);
	
	// Init Map des infos points
	m_MapInfoPoint.Reset();

	// D�finition de la cl� recherche r�sultat
	CKeyResultMap KeyResult;

	// Recup 1er terrain s�lectionn�
	m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
	if (m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
	{
		// au moins 1 terrain s�lectionn�
		KeyResult.m_IdTerrain = m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
	}
	else
		return false;

	// Recup 1ere cible s�lectionn�e
	m_pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
	if (m_pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
	{
		// au moins 1 cible s�lectionn�e
		KeyResult.m_IdCible = m_pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
	}
	else
		return false;

	// Gestion tarif
	CTarifs * pTarifs	= CTarifs::GetInstance();

	// R�cup�re objet regroupement
	CRegroupements * pRegroupements = CRegroupements::GetInstance();

	// R�cup info points
	JUnt32 InxSupport = 0;

	// Boucle sur tous les supports du document
	for (m_pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
		 m_pDocChoixSupport->m_MapIdSupportsSel.IsValid();
		 m_pDocChoixSupport->m_MapIdSupportsSel.MoveNext())
	{
		// D�finition de la cl� recherche r�sultat
		KeyResult.m_IdSupport = m_pDocChoixSupport->m_MapIdSupportsSel.GetKey();

		// R�cup�re le format support ligne s�lectionn� pour le mapping
		JUnt32 IdFormatSupport = 0;
		m_pDocChoixSupport->m_MapIdFormatSpecifMap.MoveTo(KeyResult.m_IdSupport);
		if (m_pDocChoixSupport->m_MapIdFormatSpecifMap.IsValid())
			IdFormatSupport = m_pDocChoixSupport->m_MapIdFormatSpecifMap.GetItem();

		// R�cup�re Tarif item selon format
		JUnt32 Tarif = 0;
		if(pTarifs)
		{
			if(!pTarifs->GetTarifByVersion(KeyResult.m_IdSupport, IdFormatSupport, m_pDocChoixSupport->m_DateTarifaire,Tarif))
				Tarif = 0;
		}

		// R�cup�ration des r�sultats
		CResultMap ResultMap;
		m_pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResult);

		// Init Abscisse et ordonn�e de chaque point mapping 
		double ResultAbs = -1;
		double ResultOrd = -1;
 
		// Si r�sultats valides et support avec tarif d�fini
		if (m_pDocChoixSupport->m_MapResultatMap.IsValid())
		{
			// R�cup�re les r�sultats
			ResultMap = m_pDocChoixSupport->m_MapResultatMap.GetItem(); 
        
			// R�cup n� colonne abscisse
			JUnt32 NoColAbs = m_pDlgMapping->RecupNoColAbscisseMap();

			// Calcul du r�sultat en abscisse (colonne m_NoColTarif)
			ResultAbs = m_pDlgMapping->RecupResult(NoColAbs,Tarif,ResultMap.m_VolLecture, 
 													 ResultMap.m_AudienceLDP,
													 ResultMap.m_Audience,
													 ResultMap.m_Audience1Ins,
													 ResultMap.m_Effectif,
													 ResultMap.m_NbCas,
													 ResultMap.m_VolAffinite,
													 ResultMap.m_AudienceAffiniteLDP,
													 ResultMap.m_AudienceAffinite,
													 ResultMap.m_EffectifAffinite);
		
			// R�cup n� colonne ordonn�e
			JUnt32 NoColOrd = m_pDlgMapping->RecupNoColOrdonneeMap();

			// Calcul du r�sultat en abscisse (colonne m_NoColTarif)
			ResultOrd = m_pDlgMapping->RecupResult(NoColOrd,Tarif,ResultMap.m_VolLecture, 
 													 ResultMap.m_AudienceLDP,
													 ResultMap.m_Audience,
													 ResultMap.m_Audience1Ins,
													 ResultMap.m_Effectif,
													 ResultMap.m_NbCas,
													 ResultMap.m_VolAffinite,
													 ResultMap.m_AudienceAffiniteLDP,
													 ResultMap.m_AudienceAffinite,
													 ResultMap.m_EffectifAffinite);
		}

		// Si point valide on le mettra sur le mapping
		if (ResultAbs >= 0 && ResultOrd >= 0)
		{
			// Posintion du support en X et Y
			this->SetdXPosElt(InxSupport, ResultAbs);
			this->SetdYPosElt(InxSupport, ResultOrd);
		}
		else
		{
			if (ResultAbs >= 0)
				// d�fini en abscisse
				this->SetdXPosElt(InxSupport, ResultAbs);
			else
				// Non d�fini
				this->SetdXPosElt(InxSupport, 0);

			if (ResultOrd >= 0)
				// d�fini en ordonn�e
				this->SetdYPosElt(InxSupport, ResultOrd);
			else
				this->SetdYPosElt(InxSupport, 0);
		}

		// Support s�lection pour plateau 0/N
		if (EtatSupprot)  // m_pDocChoixSupport->m_MapIdSupportsSel.GetItem() == true
			// Support faisant partie du plateau
			this->SetSelElt (InxSupport, true);
		else
			// Support en dehors du plateau
			this->SetSelElt (InxSupport, false);

		// Visibilit� du support dans mapping
		this->SetShowElt(InxSupport, true);

		// les libell�s
		this->SetdXPosLib(InxSupport, 0.05);
		this->SetdYPosLib(InxSupport, 0.05);
		this->SetShowLib(InxSupport, false);
		this->SetSelLib (InxSupport, false);

		// les courbes
		this->SetIsCourbeElt(InxSupport, false);

		// R�cup info Point
		IElem* pSupport = pRegroupements->GetIElemByID(KeyResult.m_IdSupport);

		// on r�cup�re le libell� support
		CString LibSupport = "-";
		if (pSupport)
		{
			// Affichage libell� support
			LibSupport = pSupport->GetLabel().AsJCharPtr();
		}
		m_MapInfoPoint.Add(InxSupport) = LibSupport;

		// Incr�mente indice support suivant
		InxSupport++;
	}

	// Mise � jour mapping Ok
	return true;
}
*/

///////////////////////////////////////////////////////////////////////
// Update liste des points mapping
//////////////////////////////////////////////InxSupport////////////////////////
bool CListPoint::Update(JBool InitPos)
{
	// Associer au graphe mapping uniquement les supports avec r�sultats d�finis
	JUnt32 NbSuppValid = GetNbPointValid();

	// Attention si on garde les positions pr�c�dentes
	if (InitPos || m_Liste.GetSize() == 0)
	{		
		CString Txt;
		Txt.Format("%d",NbSuppValid);

		// Red�finit la liste des �lmts (init compl�te)
		if (InitPos)
		{
			// Red�finition de la liste des points mapping
			m_Liste.SetSize(NbSuppValid);
		}
	}

	if (NbSuppValid != m_Liste.GetSize())
		// Red�finition de la liste des points mapping
		m_Liste.SetSize(NbSuppValid);

	// Init Map des infos points
	m_MapInfoPoint.Reset();

	// D�finition de la cl� recherche r�sultat
	CKeyResultMap KeyResult;

	// Identificateur format courant
	JUnt32 IdFormat;
	m_pDocChoixSupport->GetMapIdFormatsSel()->MoveFirst();
	if (m_pDocChoixSupport->GetMapIdFormatsSel()->IsValid())
		IdFormat		= m_pDocChoixSupport->GetMapIdFormatsSel()->GetKey();    

	// Recup 1er terrain s�lectionn�
	m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
	if (m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
	{
		// au moins 1 terrain s�lectionn�
		KeyResult.m_IdTerrain = m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
	}
	else
		return false;

	// Recup 1ere cible s�lectionn�e
	m_pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
	if (m_pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
	{
		// au moins 1 cible s�lectionn�e
		KeyResult.m_IdCible = m_pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
	}
	else
		return false;

	// Calcul indice cible affinit�
	KeyResult.m_IdCibleAff =  m_pDocChoixSupport->m_Offre.m_IdItemAffinite;

	// Calcul indice cl� segment
	KeyResult.m_IdSegment = m_pDocChoixSupport->ClefSegment(m_pDocChoixSupport->ModuleMap); 

	// Calcul indice cl� moteur
	KeyResult.m_IdMoteur  = m_pDocChoixSupport->m_MoteurPresse.GetType();

	// Gestion tarif
	CTarifs * pTarifs	= CTarifs::GetInstance();

	// R�cup�re objet regroupement
	CRegroupements * pRegroupements = CRegroupements::GetInstance();

	// R�cup info points
	JUnt32 InxSupport = 0;


	// R�cup�re map des supports valides sur les segments en cours
	JMap <JUnt32,JUnt32> MapSuppSegTerr;
	m_pDocChoixSupport->GetMapSupportValideSegTerrain(MapSuppSegTerr);

	// Boucle sur tous les supports du document
	for (m_pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
		 m_pDocChoixSupport->m_MapIdSupportsSel.IsValid();
		 m_pDocChoixSupport->m_MapIdSupportsSel.MoveNext())
	{
		// Par d�faut c'est un support
		JBool ElmtFamille = false;

		// D�finition de la cl� recherche r�sultat
		KeyResult.m_IdSupport = m_pDocChoixSupport->m_MapIdSupportsSel.GetKey();

		// Attention si c'est une famille
		m_pDocChoixSupport->m_Offre.m_MapSelFamilles.MoveTo(KeyResult.m_IdSupport);
		if (m_pDocChoixSupport->m_Offre.m_MapSelFamilles.IsValid())
		{
			// C'est un �lmt famille
			ElmtFamille = true;
		}

		// R�cup�re le format support ligne s�lectionn� pour le mapping
		JUnt32 IdFormatSupport = 0;
		m_pDocChoixSupport->m_MapIdFormatSpecifMap.MoveTo(KeyResult.m_IdSupport);
		if (m_pDocChoixSupport->m_MapIdFormatSpecifMap.IsValid())
			IdFormatSupport = m_pDocChoixSupport->m_MapIdFormatSpecifMap.GetItem();
		else
			IdFormatSupport = IdFormat;

		// R�cup�re Tarif item selon format via date d�but tarifaire
		JUnt32 Tarif = 0;
		JBool TarifManuel = false;
		if(pTarifs)
		{
			JUnt32 Duree;
			JDate DateExact;

			/*
			if(!pTarifs->GetTarifByDate(KeyResult.m_IdSupport, IdFormatSupport, m_pDocChoixSupport->m_DateTarifaire,m_pDocChoixSupport->m_DateTarifaire+365,DateExact,Duree,Tarif))
				Tarif = 0;
			*/
			Tarif = m_pDocChoixSupport->GetTarifSupport(KeyResult.m_IdSupport, IdFormatSupport, TarifManuel);

			// Si c'est une famille, calculer le tarif correspondant
			if (ElmtFamille)
			{
				JBool TarifManuel = false;
				Tarif = m_pDocChoixSupport->GetTarifFamille(KeyResult.m_IdSupport, IdFormatSupport, 
															m_pDocChoixSupport->m_DateTarifaire, DateExact,
															Duree, MapSuppSegTerr, TarifManuel);
			}
		}

		// R�cup�ration des r�sultats	
		CResultMap ResultMap;
		m_pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResult);

		// Init Abscisse et ordonn�e de chaque point mapping 
		double ResultAbs = -1;
		double ResultOrd = -1;

		// Type de grandeur en abscisse et ordonn�e (selon les grandeurs on permutera les valeurs)
		// (pour Co�t essentiellement)
		JUnt32 IdGrandeurAbs;
		JUnt32 IdGrandeurOrd;
 
		// Si r�sultats valides et support avec tarif d�fini
		if (m_pDocChoixSupport->m_MapResultatMap.IsValid())
		{
			// R�cup�re les r�sultats
			ResultMap = m_pDocChoixSupport->m_MapResultatMap.GetItem(); 
        
			// R�cup n� colonne abscisse
			JUnt32 NoColAbs = m_pDlgMapping->RecupNoColAbscisseMap();

			// Calcul du r�sultat en abscisse (colonne m_NoColTarif)
			ResultAbs = m_pDlgMapping->RecupResult(KeyResult.m_IdSupport, NoColAbs,
												   Tarif,ResultMap);

			// Recup grandeur en abscisse
			IdGrandeurAbs = m_pDlgMapping->RecupIdGrandeur(NoColAbs);
		
			// R�cup n� colonne ordonn�e
			JUnt32 NoColOrd = m_pDlgMapping->RecupNoColOrdonneeMap();

			// Calcul du r�sultat en abscisse (colonne m_NoColTarif)
			ResultOrd = m_pDlgMapping->RecupResult(KeyResult.m_IdSupport, NoColOrd, 
												   Tarif, ResultMap);

			// Recup grandeur en abscisse
			IdGrandeurOrd = m_pDlgMapping->RecupIdGrandeur(NoColOrd);
		}

		// R�cup�re �tat visibilit� support
		JBool EtatVisible =  m_pDocChoixSupport->GetSupportVisible(KeyResult.m_IdSupport); 

		// V�rifie �lment valide via le mode s�lectionn�e (mode Supports et/ou mode Familles)
		JBool VisibleSupport = m_pDocChoixSupport->VisibleSupportMode(KeyResult.m_IdSupport);

		// Si point valide on le mettra sur le mapping (en + on v�rifie qu'il n'est pas cach�)
		if (ResultAbs >= 0 && ResultOrd >= 0 && EtatVisible && VisibleSupport)
		{
			// Position du support en X
			if (IdGrandeurAbs == m_pDocChoixSupport->GrdMap_Cout000Cts ||
				IdGrandeurAbs == m_pDocChoixSupport->GrdMap_Cout000Ind ||
				IdGrandeurAbs == m_pDocChoixSupport->GrdMap_CoutCouv   ||
				IdGrandeurAbs == m_pDocChoixSupport->GrdMap_CoutGRP)
			{
				// Cas des co�ts en abscisse / on inverse le sens affichage
				this->SetdXPosElt(InxSupport, -ResultAbs);
			}
			else
				this->SetdXPosElt(InxSupport, ResultAbs);

   		    // Position du support en Y
			if (IdGrandeurOrd == m_pDocChoixSupport->GrdMap_Cout000Cts ||
				IdGrandeurOrd == m_pDocChoixSupport->GrdMap_Cout000Ind ||
				IdGrandeurOrd == m_pDocChoixSupport->GrdMap_CoutCouv   ||
				IdGrandeurOrd == m_pDocChoixSupport->GrdMap_CoutGRP)
			{
				// Cas des co�ts en ordonn�e / on inverse le sens affichage
				this->SetdYPosElt(InxSupport, -ResultOrd);
			}
			else
				this->SetdYPosElt(InxSupport, ResultOrd);

			// Element fait partie du mapping
			if (InitPos)
				this->SetShowElt(InxSupport, true);
		
			// l'identificateur de l'�l�ment
			this->SetIdent(InxSupport,KeyResult.m_IdSupport);

			// Support s�lection pour plateau 0/N
			m_pDocChoixSupport->m_MapIdSupportsSel.MoveTo(KeyResult.m_IdSupport);
			if (m_pDocChoixSupport->m_MapIdSupportsSel.IsValid())
			{
				if (m_pDocChoixSupport->m_MapIdSupportsSel.GetItem() == true)
					// Support faisant partie du plateau
					this->SetSelElt (InxSupport, true);
				else
					// Support en dehors du plateau
					this->SetSelElt (InxSupport, false);
			}

			// Recup 1er terrain s�lectionn�
			JUnt32 IdTerrainCur = 0;
			m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
			if (m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
			{
				// au moins 1 terrain s�lectionn�
				IdTerrainCur = m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
			}

			// Recherche si position libell� existe
			CKeyInfoMapping KeyPos;
			KeyPos.m_IdTerrain			= IdTerrainCur;									// Ident Terrain via liste terrains s�lectionn�s
			KeyPos.m_IdCible			= m_pDocChoixSupport->GetIdCibleSel();			// Ident Cible via liste cibles s�lectionn�s
			KeyPos.m_IdCibleAff			= m_pDocChoixSupport->m_Offre.m_IdItemAffinite; // Calcul indice cible affinit�
			KeyPos.m_IdSegment			= KeyResult.m_IdSegment;						// Ident segments en cours 
			KeyPos.m_IdSupport			= KeyResult.m_IdSupport;						// Ident Support via liste supports s�lectionn�s
			KeyPos.m_IdFormat			= m_pDocChoixSupport->GetIdFormatPrincipal();	// Ident Format via liste formats s�lectionn�s
			KeyPos.m_IdMoteur			= KeyResult.m_IdMoteur;							// Ident moteur 
			KeyPos.m_NbInsertions		= m_pDocChoixSupport->m_NbInsertions;			// Nombre d'insertions	
			KeyPos.m_IdGrandeurAbs		= m_pDocChoixSupport->m_IdGrandMapSelAbs ;		// Ident Grandeur abscisse mapping
			KeyPos.m_IdGrandeurOrd		= m_pDocChoixSupport->m_IdGrandMapSelOrd ;		// Ident Grandeur abscisse mapping
			KeyPos.m_IdFiltreAudience	= m_pDocChoixSupport->GetIdFiltreAudienceSel();	// Ident Filtre d'audience	
			KeyPos.m_IdContexteLecture	= 0;	// A FAIRE								// Ident Contexte de lecture
			KeyPos.m_bRepriseEnMain		= m_pDocChoixSupport->m_bRepriseEnMain;			// Prise en compte reprises en main
			KeyPos.m_bLNMMag			= m_pDocChoixSupport->m_bLNMMag;				// Privil�gier audience LNM pour magazines
			KeyPos.m_bLDPQtd			= m_pDocChoixSupport->m_bLDPQtd;				// Privil�gier audience LDP pour quotidiens
			KeyPos.m_bLDP30J			= m_pDocChoixSupport->m_bLDP30J;				// Privil�gier audience LDP 30 j pour les bimestriels

			// L'information position existe t-elle
			m_pDocChoixSupport->m_MapPointMapping.MoveTo(KeyPos);  
			if (m_pDocChoixSupport->m_MapPointMapping.IsValid())
			{
				// R�cup�re les positions en X et en Y
				CPointLibelle &PointLibelle = m_pDocChoixSupport->m_MapPointMapping.GetItem();

				// Remet position courante
				this->SetPosLib(InxSupport, PointLibelle.m_XLib,PointLibelle.m_YLib);
			}
			else
			{
				// Initialise position libell�
				this->SetPosLib(InxSupport,0.05,0.05);
			}

			// A VOIR 
			// this->SetShowLib(InxSupport, false);
			this->SetSelLib (InxSupport, false);
			
			// les courbes
			this->SetIsCourbeElt(InxSupport, false);

			// R�cup info Point
			IElem* pSupport = pRegroupements->GetIElemByID(KeyResult.m_IdSupport);

			// on r�cup�re le libell� support
			CString LibSupport = "-";
			if (pSupport)
			{
				// Affichage libell� support
				LibSupport = pSupport->GetLabel().AsJCharPtr();
			}

			// Ajoute libell� � la liste des �lmts
			m_MapInfoPoint.Add(InxSupport) = LibSupport;
		}
		else
		{
			// Pas de libell� pour ce support, puisque non visible sur le mapping
			m_MapInfoPoint.Add(InxSupport) = "n.d";

			// Ce support n'appartient pas au mapping
			this->SetShowElt(InxSupport, false);

		}

		// Incr�mente indice support suivant
		InxSupport++;
	}

	
	// Mise � jour des supports s�lectionn�s
	// this->MajSelElt(); 

	// Mise � jour mapping Ok
	return true;
}

//////////////////////////////////////////////////////////////////////
// D�fini le nombre de points support  afficher
/////////////////////////////////////////////////////////////////////
long CListPoint::GetNbPointValid()
{
	// Nb points valides
	JUnt32 NbPointValid = 0;

	// D�finition de la cl� recherche r�sultat
	CKeyResultMap KeyResult;

	// Recup 1er terrain s�lectionn�
	m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
	if (m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
	{
		// au moins 1 terrain s�lectionn�
		KeyResult.m_IdTerrain = m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
	}
	else
		return 0;

	// Recup 1ere cible s�lectionn�e
	m_pDocChoixSupport->GetMapIdCiblesAudienceSel()->MoveFirst();
	if (m_pDocChoixSupport->GetMapIdCiblesAudienceSel()->IsValid())
	{
		// au moins 1 cible s�lectionn�e
		KeyResult.m_IdCible = m_pDocChoixSupport->GetMapIdCiblesAudienceSel()->GetKey();  
	}
	else
		return 0;

	// Calul indice affinit�
	KeyResult.m_IdCibleAff	= m_pDocChoixSupport->m_Offre.m_IdItemAffinite;

	// Calcul indice cl� segment
	KeyResult.m_IdSegment   = m_pDocChoixSupport->ClefSegment(m_pDocChoixSupport->ModuleMap); 

	// Calcul indice cl� moteur
	KeyResult.m_IdMoteur    = m_pDocChoixSupport->m_MoteurPresse.GetType();

	// Gestion tarif
	CTarifs * pTarifs		= CTarifs::GetInstance();

	// R�cup�re objet regroupement
	CRegroupements * pRegroupements = CRegroupements::GetInstance();

	// R�cup info points
	JUnt32 InxSupport = 0;

	// Boucle sur tous les supports du document
	for (m_pDocChoixSupport->m_MapIdSupportsSel.MoveFirst();
		 m_pDocChoixSupport->m_MapIdSupportsSel.IsValid();
		 m_pDocChoixSupport->m_MapIdSupportsSel.MoveNext())
	{
		// D�finition de la cl� recherche r�sultat
		KeyResult.m_IdSupport = m_pDocChoixSupport->m_MapIdSupportsSel.GetKey();

		// R�cup�re �tat visibilit� support
		JBool EtatVisible = m_pDocChoixSupport->GetSupportVisible(KeyResult.m_IdSupport); 

		// V�rifie �lment valide via le mode s�lectionn�e (mode Supports et/ou mode Familles)
		// JBool VisibleSupport = pDocChoixSupport->VisibleSupportMode(KeyResult.m_IdSupport);

		// R�cup�re le format support ligne s�lectionn� pour le mapping
		JUnt32 IdFormatSupport = 0;
		m_pDocChoixSupport->m_MapIdFormatSpecifMap.MoveTo(KeyResult.m_IdSupport);
		if (m_pDocChoixSupport->m_MapIdFormatSpecifMap.IsValid())
			IdFormatSupport = m_pDocChoixSupport->m_MapIdFormatSpecifMap.GetItem();

		// R�cup�re Tarif item selon format
		JUnt32 Tarif = 0;
		if(pTarifs)
		{
			/*
			if(!pTarifs->GetTarifByVersion(KeyResult.m_IdSupport, IdFormatSupport, m_pDocChoixSupport->m_DateTarifaire,Tarif))
				Tarif = 0;
			*/
			JUnt32 Duree;
			JDate DateExact;
			if(!pTarifs->GetTarifByDate(KeyResult.m_IdSupport, IdFormatSupport, m_pDocChoixSupport->m_DateTarifaire,m_pDocChoixSupport->m_DateTarifaire+365,DateExact,Duree,Tarif))
				Tarif = 0;
		}

		// R�cup�ration des r�sultats
		CResultMap ResultMap;
		m_pDocChoixSupport->m_MapResultatMap.MoveTo(KeyResult);

		// Init Abscisse et ordonn�e de chaque point mapping 
		double ResultAbs = -1;
		double ResultOrd = -1;
 
		// Si r�sultats valides et support avec tarif d�fini
		if (m_pDocChoixSupport->m_MapResultatMap.IsValid() && EtatVisible)
		{
			// R�cup�re les r�sultats
			ResultMap = m_pDocChoixSupport->m_MapResultatMap.GetItem(); 
        
			// R�cup n� colonne abscisse
			JUnt32 NoColAbs = m_pDlgMapping->RecupNoColAbscisseMap();

			// Calcul du r�sultat en abscisse (colonne m_NoColTarif)
			/*
			ResultAbs = m_pDlgMapping->RecupResult(KeyResult.m_IdSupport, NoColAbs,Tarif,ResultMap.m_VolLecture, 
 												   ResultMap.m_AudienceLDP,
 												   ResultMap.m_AudienceProba,
												   ResultMap.m_Audience,
												   ResultMap.m_Audience1Ins,
												   ResultMap.m_Effectif,
												   ResultMap.m_NbCas,
												   ResultMap.m_VolAffinite,
												   ResultMap.m_AudienceAffiniteLDP,
												   ResultMap.m_AudienceAffiniteProba,
												   ResultMap.m_AudienceAffinite,
												   ResultMap.m_EffectifAffinite);
			*/
			ResultAbs = m_pDlgMapping->RecupResult(KeyResult.m_IdSupport, NoColAbs,
												   Tarif, ResultMap);
		
			// R�cup n� colonne ordonn�e
			JUnt32 NoColOrd = m_pDlgMapping->RecupNoColOrdonneeMap();

			// Calcul du r�sultat en abscisse (colonne m_NoColTarif)
			/*
			ResultOrd = m_pDlgMapping->RecupResult(KeyResult.m_IdSupport, NoColOrd, Tarif, ResultMap.m_VolLecture, 
 												   ResultMap.m_AudienceLDP,
 												   ResultMap.m_AudienceProba,
												   ResultMap.m_Audience,
												   ResultMap.m_Audience1Ins,
												   ResultMap.m_Effectif,
												   ResultMap.m_NbCas,
												   ResultMap.m_VolAffinite,
												   ResultMap.m_AudienceAffiniteLDP,
												   ResultMap.m_AudienceAffiniteProba,
												   ResultMap.m_AudienceAffinite,
												   ResultMap.m_EffectifAffinite);
			*/
			ResultOrd = m_pDlgMapping->RecupResult(KeyResult.m_IdSupport, NoColOrd,
												   Tarif, ResultMap);
		}

		// Si point valide on le mettra sur le mapping
		if (ResultAbs >= 0 && ResultOrd >= 0)
		{
			// Ce support a un r�sultat affichable dans graphe mapping 
			NbPointValid++;
		}

		// Incr�mente indice support suivant
		InxSupport++;
	}

	// return NbPointValid;
	return m_pDocChoixSupport->m_MapIdSupportsSel.GetCount();

}

//////////////////////////////////////////////////////////////////////
// R�cup Nb �l�ment liste des points
//////////////////////////////////////////////////////////////////////
long CListPoint::GetTaille()
{
	return (m_Liste.GetSize());
}

//////////////////////////////////////////////////////////////////////
// Positionne Nb �l�ment liste des points
//////////////////////////////////////////////////////////////////////
void CListPoint::SetTaille(long nb)
{
	m_Liste.SetSize(nb);
}

//////////////////////////////////////////////////////////////////////
//position d'un element
//////////////////////////////////////////////////////////////////////
double CListPoint::GetdXPosElt(long indice)
{
	double X = m_Liste[indice].m_XElt;
	return (X);
}

double CListPoint::GetdYPosElt(long indice)
{
	double Y = m_Liste[indice].m_YElt;
	return (Y);	
}

void CListPoint::SetdXPosElt(long indice, double X)
{

	m_Liste[indice].m_XElt = X;
}

void CListPoint::SetdYPosElt(long indice, double Y)
{
	m_Liste[indice].m_YElt = Y;
}

//////////////////////////////////////////////////////////////////////
// selection d'un point
//////////////////////////////////////////////////////////////////////
bool CListPoint::GetSelElt(long indice)
{
	bool etat = m_Liste[indice].m_IsSelElt;
	return (etat);
}

void CListPoint::SetSelElt(long indice, bool etat)
{
	// Marquage ou d�marquage �lmt du plateau
	m_Liste[indice].m_IsSelElt = etat;

	// R�cup�re ident s�lection
	int IdentPoint = this->GetIdent(indice);

	// on se place sur le plateau
	m_pDocChoixSupport->m_MapIdSupportsSel.MoveTo(IdentPoint);
	if (m_pDocChoixSupport->m_MapIdSupportsSel.IsValid())
	{
		// Mise � jour marquage support
		bool &EtatSupport = m_pDocChoixSupport->m_MapIdSupportsSel.GetItem();
		EtatSupport = etat;
	}
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void CListPoint::MajSelElt()
{
	// Mise � jour des supports s�lectionn�s
	// A VOIR 2007
	m_pDocChoixSupport->UpdateAllViews(UPDATE_SEL_SUPPORTS); 
}

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void CListPoint::MajLibelleElt()
{
	// Mise � jour des �tats libell�s supports
	m_pDocChoixSupport->UpdateAllViews(UPDATE_SEL_MAPSUPPORTS); 
}


////////////////////////////////////////////////////////////////
// l'identificateur de l'�l�ment
int CListPoint::GetIdent(long indice)
{
	// retourne l'ident du point
	return m_Liste[indice].m_IdentElt;
}

void CListPoint::SetIdent(long indice, int ident)
{
	// Archive de l'ident du point
	m_Liste[indice].m_IdentElt = ident;
}

//////////////////////////////////////////////////////////////////////
// visibilite d'un element
//////////////////////////////////////////////////////////////////////
bool CListPoint::GetShowElt(long indice)
{
	bool etat = m_Liste[indice].m_IsShowElt;
	return (etat);
}

void CListPoint::SetShowElt(long indice, bool etat)
{
	m_Liste[indice].m_IsShowElt = etat;
}

// Visibilit� d'un �lmt via son identifiacteur
void CListPoint::SetShowEltById(int IdElmt, bool etat)
{
	// Recherche id �lmt parmi liste des points
	for (int i=0; i < m_Liste.GetSize(); i++)
	{
		if (m_Liste[i].m_IdentElt == IdElmt)
		{
			// Ident trouv�, on positionne son �tat
			SetShowElt(i,etat);
			return;
		}
	}
}

//////////////////////////////////////////////////////////////////////
// visibilite d'un libelle
//////////////////////////////////////////////////////////////////////
bool CListPoint::GetShowLib(long indice)
{
	bool etat = m_Liste[indice].m_IsShowLib;
	return (etat);
}

void CListPoint::SetShowLib(long indice, bool etat)
{
 	m_Liste[indice].m_IsShowLib = etat;
}

//////////////////////////////////////////////////////////////////////
// selection d'un libelle
//////////////////////////////////////////////////////////////////////

bool CListPoint::GetSelLib(long indice)
{
	bool etat = m_Liste[indice].m_IsSelLib;
	return (etat);
}

void CListPoint::SetSelLib(long indice, bool etat)
{
	m_Liste[indice].m_IsSelLib = etat;
}

//////////////////////////////////////////////////////////////////////
//position d'un libell�
//////////////////////////////////////////////////////////////////////
double CListPoint::GetdXPosLib(long indice)
{
	double X = m_Liste[indice].m_XLib;
	return (X);
}

double CListPoint::GetdYPosLib(long indice)
{
	double Y = m_Liste[indice].m_YLib;
	return (Y);	
}

///////////////////////////////////////////////////////////////////////
// Nouvelle position d'un libell� 
void CListPoint::SetPosLib(long indice, double X, double Y)
{
	// position en X
	SetdXPosLib(indice,X);

	// position en Y
	SetdYPosLib(indice,Y);

	// Recup 1er terrain s�lectionn�
	JUnt32 IdTerrainCur = 0;
	m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->MoveFirst();
	if (m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->IsValid())
	{
		// au moins 1 terrain s�lectionn�
		IdTerrainCur = m_pDocChoixSupport->GetMapIdTerrainsAudienceSel()->GetKey(); 
	}

	// sauvegarde dans map des positions
	CKeyInfoMapping KeyPos;
	KeyPos.m_IdTerrain			= IdTerrainCur;									// Ident Terrain via liste terrains s�lectionn�s
	KeyPos.m_IdCible			= m_pDocChoixSupport->GetIdCibleSel();			// Ident Cible via liste cibles s�lectionn�s
	KeyPos.m_IdCibleAff			= m_pDocChoixSupport->m_Offre.m_IdItemAffinite; // Calcul indice cible affinit�
	KeyPos.m_IdSegment			= m_pDocChoixSupport->ClefSegment(m_pDocChoixSupport->ModuleMap); // Id segments en cours
	KeyPos.m_IdSupport			= this->GetIdent(indice);						// Ident Support via liste supports s�lectionn�s
	KeyPos.m_IdFormat			= m_pDocChoixSupport->GetIdFormatPrincipal();	// Ident Format via liste formats s�lectionn�s
	KeyPos.m_IdMoteur			= m_pDocChoixSupport->m_MoteurPresse .GetType();// Ident moteur 
	KeyPos.m_NbInsertions		= m_pDocChoixSupport->m_NbInsertions;			// Nombre d'insertions	
	KeyPos.m_IdGrandeurAbs		= m_pDocChoixSupport->m_IdGrandMapSelAbs ;		// Ident Grandeur abscisse mapping
	KeyPos.m_IdGrandeurOrd		= m_pDocChoixSupport->m_IdGrandMapSelOrd ;		// Ident Grandeur abscisse mapping
	KeyPos.m_IdFiltreAudience	= m_pDocChoixSupport->GetIdFiltreAudienceSel();	// Ident Filtre d'audience	
	KeyPos.m_IdContexteLecture	= 0;	// A FAIRE								// Ident Contexte de lecture
	KeyPos.m_bRepriseEnMain		= m_pDocChoixSupport->m_bRepriseEnMain;			// Prise en compte reprises en main
	KeyPos.m_bLNMMag			= m_pDocChoixSupport->m_bLNMMag;				// Privil�gier audience LNM pour magazines
	KeyPos.m_bLDPQtd			= m_pDocChoixSupport->m_bLDPQtd;				// Privil�gier audience LDP pour quotidiens
	KeyPos.m_bLDP30J			= m_pDocChoixSupport->m_bLDP30J;				// Privil�gier audience LDP 30 j pour les bimestriels

	// Test si info inexistante
	m_pDocChoixSupport->m_MapPointMapping.MoveTo(KeyPos);  
	if (m_pDocChoixSupport->m_MapPointMapping.IsValid())
	{
		// Modifie l'info position
		CPointLibelle &PointLibelle = m_pDocChoixSupport->m_MapPointMapping.GetItem();
		PointLibelle.m_XLib = X;
		PointLibelle.m_YLib = Y;
	}
	else
	{
		// Cr�e nouvelle info position
		CPointLibelle PointLibelle;
		PointLibelle.m_XLib = X;
		PointLibelle.m_YLib = Y;

		// Ajoute au map g�n�ral des positions
		m_pDocChoixSupport->m_MapPointMapping.MoveTo(KeyPos);
		if (!m_pDocChoixSupport->m_MapPointMapping.IsValid())
			m_pDocChoixSupport->m_MapPointMapping.Add(KeyPos) = PointLibelle;  
	}
}

///////////////////////////////////////////////////////////////////////
// Position courante en X d'un libell� 
double CListPoint::GetXPosLib(long indice)
{
	// position en X
	return m_Liste[indice].m_XLib;
}

///////////////////////////////////////////////////////////////////////
// Position courante en Y d'un libell� 
double CListPoint::GetYPosLib(long indice)
{
	// position en Y
	return m_Liste[indice].m_YLib;
}


void CListPoint::SetdXPosLib(long indice, double X)
{
	m_Liste[indice].m_XLib = X;
}

void CListPoint::SetdYPosLib(long indice, double Y)
{
	m_Liste[indice].m_YLib = Y;
}

//////////////////////////////////////////////////////////////////////
// appartenance a la courbe 
//////////////////////////////////////////////////////////////////////
bool CListPoint::GetIsCourbeElt(long indice)
{
	bool etat = m_Liste[indice].m_IsCbElt;
	return (etat);
}

void CListPoint::SetIsCourbeElt(long indice, bool etat)
{
	m_Liste[indice].m_IsCbElt = etat;
}

///////////////////////////////////////////////////////////////////////////////////////////
// r�cup�ration libell� du point
CString CListPoint::GetLibPoint(long indice)
{
	// Init libell� support
	CString LibSupport = "n.d.";

	// Recherche dans le map des supports affich�s
	m_MapInfoPoint.MoveTo(indice);

	// Si point valide on r�cup�re son libell�
	if (m_MapInfoPoint.IsValid())
	{
		// Libell� support existant
		LibSupport = m_MapInfoPoint.GetItem();
	}
	return LibSupport;
}

///////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration libell� grandeur en abscisse
CString CListPoint::GetLibAbscisse()
{
	return m_pDlgMapping->RecupTexteHeaderRow(0,m_pDlgMapping->RecupNoColAbscisseMap());
}

///////////////////////////////////////////////////////////////////////////////////////////
// R�cup�ration libell� grandeur en ordonn�e
CString CListPoint::GetLibOrdonnee()
{
	return m_pDlgMapping->RecupTexteHeaderRow(0,m_pDlgMapping->RecupNoColOrdonneeMap());
}

///////////////////////////////////////////////////////////////////////////////////////////
// r�cup�ration �tat affichage libell�s
bool CListPoint::GetLibAfficher()
{
	return LibelleVisible;
}

///////////////////////////////////////////////////////////////////////////////////////////
// positionne �tat d'affichage des libell�s
void CListPoint::SetLibAfficher(bool etat)
{
	LibelleVisible = etat;
}

///////////////////////////////////////////////////////////////////////////////////////////
// Affichage uniquement des points s�lectionn�s
JVoid CListPoint::ShowSelItem()
{
	// Rend tous les supports s�lectionn�s du document visible
	m_pDocChoixSupport->SetAllSupportSelVisible(true); 

	// Mise � jour des grilles
	m_pDocChoixSupport->UpdateAllViews(UPDATE_VISIBLE_SUPPORTS);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Affichage uniquement des points non s�lectionn�s
JVoid CListPoint::HideSelItem()
{
	// Rend tous les supports non s�lectionn�s du document visible
	m_pDocChoixSupport->SetAllSupportSelVisible(false); 

	// Mise � jour des grilles
	m_pDocChoixSupport->UpdateAllViews(UPDATE_VISIBLE_SUPPORTS);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Affichage de tous les points
JVoid CListPoint::ShowAllItem()
{
	// Rend tous les supports du document visible
	m_pDocChoixSupport->SetAllSupportVisible(true);

	// Mise � jour des grilles
	m_pDocChoixSupport->UpdateAllViews(UPDATE_VISIBLE_SUPPORTS);
}

