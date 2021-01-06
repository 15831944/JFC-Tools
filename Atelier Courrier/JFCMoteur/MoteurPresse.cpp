#include "StdAfx.h"
#include ".\moteurpresse.h"
#include "Results.h"
#include "ItemsConstruitsManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMoteurPresse::CMoteurPresse(void)
{
	m_type = ABAQUE_ETAL;

	m_TerrainManager = JMoteurTerrainManager::GetInstance();
	m_TblCibles.Reset();
	m_TblEffectifs.Reset();
	m_TblAudiences.Reset();
	m_Options.Reset();
	
	ctxSelected = false;
	
	m_bInit = false;

	// Init Cible courante
	m_ItemCourant = 0;
}

CMoteurPresse::~CMoteurPresse(void)
{
}

bool CMoteurPresse::Init(JATPOffre& offre, JUnt32 filtreAud, JBool bLDP, JBool bLNM, JBool bLDP30J, JBool bReprise )
{
	m_Options.SetLDPQuotidiens(bLDP);
	m_Options.SetLNMMagazines(bLNM);
	m_Options.SetLDP30JrsBimestriels(bLDP30J);
	m_Options.SetReprises(bReprise);

	//Chargement des terrains et des cibles pour le moteur
	m_bInit = SetOffre(offre, filtreAud);

	return m_bInit;
}

//////////////////////////////////////////////////////////////////////////
// Parametrage moteur presse via sélection offre et filtre audience
bool CMoteurPresse::SetOffre(JATPOffre& offre, JUnt32 filtreAud)
{
	m_TblCibles.Reset();
	m_TblEffectifs.Reset();
	m_TblAudiences.Reset();

	//pour chaque terrain
	for(offre.m_TBLIdTerrainsAudience.MoveFirst();
		offre.m_TBLIdTerrainsAudience.IsValid();
		offre.m_TBLIdTerrainsAudience.MoveNext())
	{
		//ajout du terrain
		JUnt32 idSrc = offre.m_IdSourceAudience;
		JUnt32 idTerrain = offre.m_TBLIdTerrainsAudience.GetItem();

		AddTerrain(idSrc, idTerrain);

		JMTFR03KEYModulation keyMod;
		keyMod.m_IdSource = idSrc;
		keyMod.m_IdTerrain = idTerrain;

		m_TblModulations.MoveTo(keyMod);
		JMTFR03DATModulation & DatMod = m_TblModulations.GetItem();

		DatMod.m_FLTFiltres.SetSelItem(DatMod.m_FLTFiltres.FindItem(filtreAud));
		DatMod.m_FLTPoids.SetSelItem(DatMod.m_FLTPoids.FindItem(offre.m_IdTypePoids));

		// Nombre de segments ciblage
		//JUnt32 NbSeg  = offre.m_TBLIdSegmentsAudience.GetCount();
		//JUnt32 NbSeg2 = NbSeg;

		// Initialisation du tableau segments
		for(int i = 0; i<DatMod.m_FLTSegments.GetCount();i++)
			DatMod.m_FLTSegments.SetSelItem(i, false);

		// Définition des segments utilisés
		for(offre.m_TBLIdSegmentsAudience.MoveFirst();offre.m_TBLIdSegmentsAudience.IsValid(); offre.m_TBLIdSegmentsAudience.MoveNext())
			DatMod.m_FLTSegments.SetSelItem( DatMod.m_FLTSegments.FindItem(offre.m_TBLIdSegmentsAudience.GetItem()), true);
		
		MakeCibles(offre, idSrc, idTerrain);

		////////////////////////////////////////// Les cibles pondérées ///////////////////////////////////
		// Fabrique aussi les cibles pondérées
		// CItemsConstruits * pItems = CItemsConstruits::GetInstance();

		// Récupère les informations du terrain
		CTerrain * pTerrain = CTerrainManager::GetInstance()->GetTerrain(idSrc, idTerrain);
		
		// Récupère les items indépendants (sans les cibles pondérées)
		JList<IItem const *> vItems;
		// pItems->GetItemsPonderes(vItems, pTerrain);
		CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemsPonderes(vItems, pTerrain);

		// Liste des idents items pondérés
		JList <JUnt32> LstIdItemsPonderes;

		// Boucle sur tous les items
		for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
		{
			// Récupère l'item
			IItem * pItem = (IItem*)vItems.GetItem();

			// Ident de l'item
			JInt32 IdItem		= pItem->GetID();

			// On l'ajoute à la liste
			LstIdItemsPonderes.AddTail() =  IdItem;
		}

		// Fabrication des cibles pondérées	
		MakeCiblesPonderes(LstIdItemsPonderes, idSrc, idTerrain);

		// Calcul pour cette offre 
		MakeValues(offre, idSrc, idTerrain, true);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
// Parametrage moteur presse via sélection offre et filtre audience
bool CMoteurPresse::SetOffre(JATPOffre& offre, JUnt32 filtreAud, JMap<JUnt32,JUnt32> *MapIdSegPop)
{
	m_TblCibles.Reset();
	m_TblEffectifs.Reset();
	m_TblAudiences.Reset();

	//pour chaque terrain
	for(offre.m_TBLIdTerrainsAudience.MoveFirst();
		offre.m_TBLIdTerrainsAudience.IsValid();
		offre.m_TBLIdTerrainsAudience.MoveNext())
	{
		//ajout du terrain
		JUnt32 idSrc = offre.m_IdSourceAudience;
		JUnt32 idTerrain = offre.m_TBLIdTerrainsAudience.GetItem();

		AddTerrain(idSrc, idTerrain);

		JMTFR03KEYModulation keyMod;
		keyMod.m_IdSource = idSrc;
		keyMod.m_IdTerrain = idTerrain;

		m_TblModulations.MoveTo(keyMod);
		JMTFR03DATModulation & DatMod = m_TblModulations.GetItem();

		DatMod.m_FLTFiltres.SetSelItem(DatMod.m_FLTFiltres.FindItem(filtreAud));
		DatMod.m_FLTPoids.SetSelItem(DatMod.m_FLTPoids.FindItem(offre.m_IdTypePoids));

		// Initialisation du tableau segments
		for(int i = 0; i<DatMod.m_FLTSegments.GetCount();i++)
			DatMod.m_FLTSegments.SetSelItem(i, false);

		// Définition des segments utilisés
		for(MapIdSegPop->MoveFirst(); MapIdSegPop->IsValid(); MapIdSegPop->MoveNext())
		{
			const JUnt32 &IdSeg = MapIdSegPop->GetKey();
			JInt32 Index = DatMod.m_FLTSegments.FindItem(MapIdSegPop->GetKey());
			if (Index >= 0 && Index < DatMod.m_FLTSegments.GetCount())
				DatMod.m_FLTSegments.SetSelItem(DatMod.m_FLTSegments.FindItem(MapIdSegPop->GetKey()), true);
		}
		
		MakeCibles(offre, idSrc, idTerrain);

		MakeValues(offre, idSrc, idTerrain);
	}
	return true;
}

bool CMoteurPresse::SetFiltreAudience(JATPOffre& offre, JUnt32 filtreAud)
{
	m_TblAudiences.Reset();
	//pour chaque terrain
	for(offre.m_TBLIdTerrainsAudience.MoveFirst(); offre.m_TBLIdTerrainsAudience.IsValid(); offre.m_TBLIdTerrainsAudience.MoveNext())
	{
		JMTFR03KEYModulation keyMod;
		keyMod.m_IdSource = offre.m_IdSourceAudience;
		keyMod.m_IdTerrain = offre.m_TBLIdTerrainsAudience.GetItem();

		m_TblModulations.MoveTo(keyMod);
		JMTFR03DATModulation & DatMod = m_TblModulations.GetItem();

		DatMod.m_FLTFiltres.SetSelItem(DatMod.m_FLTFiltres.FindItem(filtreAud));
		MakeValues(offre, offre.m_IdSourceAudience, offre.m_TBLIdTerrainsAudience.GetItem());
	}

	return true;
}


bool CMoteurPresse::SetOptions(JATPOffre& offre, JBool bLDP, JBool bLNM, JBool bLDP30J, JBool bReprise)
{
	bool bConstruct = false;
	
	if(bLDP != m_Options.GetLDPQuotidiens())
	{
		m_Options.SetLDPQuotidiens(bLDP);
		bConstruct = true;
	}
	if(bLNM != m_Options.GetLNMMagazines())
	{
		m_Options.SetLNMMagazines(bLNM);
		bConstruct = true;
	}
	if(bLDP30J != m_Options.GetLDP30JrsBimestriels())
	{
		m_Options.SetLDP30JrsBimestriels(bLDP30J);
		bConstruct = true;
	}

	if(bReprise != m_Options.GetReprises())
	{
		m_Options.SetReprises(bReprise);
		bConstruct = true;
	}


	if(bConstruct)
	{
		m_TblAudiences.Reset();
		//pour chaque terrain
		for(offre.m_TBLIdTerrainsAudience.MoveFirst(); offre.m_TBLIdTerrainsAudience.IsValid(); offre.m_TBLIdTerrainsAudience.MoveNext())
			MakeContacts(offre, offre.m_IdSourceAudience, offre.m_TBLIdTerrainsAudience.GetItem());
	}

	return true;
}




bool CMoteurPresse::AddTerrain(JUnt32 IDSource, JUnt32 IDTerrain)
{
	m_TerrainManager->AddTerrain(IDSource, IDTerrain);

	JMTFR03KEYModulation keyMod;
	keyMod.m_IdSource = IDSource;
	keyMod.m_IdTerrain = IDTerrain;

	m_TblModulations.MoveTo(keyMod);
	if (!m_TblModulations.IsValid())
		m_TblModulations.Add(keyMod, m_TerrainManager->m_TblTerrains);

	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Fabrique les cibles sélectionnées via l'offre
bool CMoteurPresse::MakeCibles(JATPOffre& offre, JUnt32 IDSource, JUnt32 IDTerrain)
{
	// CItemsConstruits * pItems = CItemsConstruits::GetInstance();
	
	JMTFR03KEYCible kCible;
	kCible.m_IdSource = IDSource;
	kCible.m_IdTerrain = IDTerrain;

	//pour chaque cible
	for(offre.m_TBLIdItems.MoveFirst();
		offre.m_TBLIdItems.IsValid();
		offre.m_TBLIdItems.MoveNext()
		)
	{
		// IItem * iItem = pItems->GetIItemByID( offre.m_TBLIdItems.GetItem() );
		IItem * iItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID( offre.m_TBLIdItems.GetItem() );

		if (iItem && iItem->IsPalette())
		{
			//////////////////////////////////////////////// Cas d'un Item-Palette ///////////////////////////////////////
			//
			CPalettes * palette = iItem->AsPalette();
			JList<JInt32> idList;
			palette->GetItems(idList);
			for (idList.MoveFirst(); idList.IsValid(); idList.MoveNext())
			{
				// CItemConstruit * item = pItems->GetIItemByID( idList.GetItem() )->AsItemConstruit();
				CItemConstruit * item = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(idList.GetItem())->AsItemConstruit();

				kCible.m_IdCible = item->GetID();

				// Test si la cible est valide sur ce terrain
				offre.m_MapInterdictionsItems.MoveTo((JUnt32)kCible.m_IdCible.AsJInt32());
				if (offre.m_MapInterdictionsItems.IsValid())
				{
					// Récupération infos autorisation pour la cible
					JMap <JUnt32, JBool> & MapInterditCible = offre.m_MapInterdictionsItems.GetItem();

					// On se place sur autorisation terrain
					MapInterditCible.MoveTo(IDTerrain);
					if (MapInterditCible.IsValid())
					{
						// Cette cible n'est pas autorisée sur ce terrain, on ne l'ajoute pas au moteur
						// Cette cible est autorisée sur ce terrain, on l'ajoute en indiquant cible non autorisée
						AddCible(kCible, item, false);
					}
					else
					{
						// Ajout de la cible pour les calculs
						AddCible(kCible, item);
					}
				}
				else
				{
					// Ajout de la cible pour les calculs
					AddCible(kCible, item);
				}
			}

		}
		else
		{
			//////////////////////////////////////////////// Cas d'un Item Simple ///////////////////////////////////////
			//
			if (iItem && iItem->IsItemConstruit())
			{
				CItemConstruit * item = iItem->AsItemConstruit();
				kCible.m_IdCible = item->GetID();

				// Test si la cible est valide sur ce terrain
				offre.m_MapInterdictionsItems.MoveTo((JUnt32)kCible.m_IdCible.AsJInt32());
				if (offre.m_MapInterdictionsItems.IsValid())
				{
					// Récupération infos autorisation pour la cible
					JMap <JUnt32, JBool> & MapInterditCible = offre.m_MapInterdictionsItems.GetItem();

					// On se place sur autorisation terrain
					MapInterditCible.MoveTo(IDTerrain);
					if (MapInterditCible.IsValid())
					{
						// Cette cible est autorisée sur ce terrain, on l'ajoute en indiquant cible non autorisée
						AddCible(kCible, item, false);
					}
					else
					{
						// Cette cible est autorisée sur ce terrain, on peut l'ajouter
						AddCible(kCible, item);
					}
				}
				else
				{
					// Cette cible est autorisée sur ce terrain, on peut l'ajouter
					AddCible(kCible, item);
				}
			}
		}
	}
	
	//et aussi pour l'affinité
	// IItem * iItem = pItems->GetIItemByID( offre.m_IdItemAffinite );
	IItem * iItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID( offre.m_IdItemAffinite );

	if (iItem->IsPalette())
	{
		//on decompose la palette
		CPalettes * palette = iItem->AsPalette();
		JList<JInt32> idList;
		palette->GetItems(idList);
		for (idList.MoveFirst(); idList.IsValid(); idList.MoveNext())
		{
			// CItemConstruit * item = pItems->GetIItemByID( idList.GetItem() )->AsItemConstruit();
			CItemConstruit * item = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID( idList.GetItem() )->AsItemConstruit();

			kCible.m_IdCible = item->GetID();

			// Test si la cible est valide sur ce terrain
			offre.m_MapInterdictionsItems.MoveTo((JUnt32)kCible.m_IdCible.AsJInt32());
			if (offre.m_MapInterdictionsItems.IsValid())
			{
				// Récupération infos autorisation pour la cible
				JMap <JUnt32, JBool> & MapInterditCible = offre.m_MapInterdictionsItems.GetItem();

				// On se place sur autorisation terrain
				MapInterditCible.MoveTo(IDTerrain);
				if (MapInterditCible.IsValid())
				{
					// Cette cible est autorisée sur ce terrain, on l'ajoute en indiquant cible non autorisée
					AddCible(kCible, item, false);
				}
				else
				{
					// Cette cible est autorisée sur ce terrain, on peut l'ajouter
					AddCible(kCible, item);
				}
			}
			else
			{
				// Cette cible est autorisée sur ce terrain, on peut l'ajouter
				AddCible(kCible, item);
			}
		}

	}
	else if (iItem->IsItemConstruit())
	{
		CItemConstruit * item = iItem->AsItemConstruit();
		kCible.m_IdCible = item->GetID();

		// Test si la cible est valide sur ce terrain
		offre.m_MapInterdictionsItems.MoveTo((JUnt32)kCible.m_IdCible.AsJInt32());
		if (offre.m_MapInterdictionsItems.IsValid())
		{
			// Récupération infos autorisation pour la cible
			JMap <JUnt32, JBool> & MapInterditCible = offre.m_MapInterdictionsItems.GetItem();

			// On se place sur autorisation terrain
			MapInterditCible.MoveTo(IDTerrain);
			if (MapInterditCible.IsValid())
			{
				// Cette cible est autorisée sur ce terrain, on l'ajoute en indiquant cible non autorisée
				AddCible(kCible, item, false);
			}
			else
			{
				// Cette cible est autorisée sur ce terrain, on peut l'ajouter
				AddCible(kCible, item);
			}
		}
		else
		{
			// Cette cible est autorisée sur ce terrain, on peut l'ajouter
			AddCible(kCible, item);
		}
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Ajout de la cible pour les calculs
bool CMoteurPresse::AddCible(JMTFR03KEYCible kCible, CItemConstruit* item, JBool ItemAutorise)
{
	CTerrain * pTerrain = m_TerrainManager->GetTerrain(kCible.m_IdSource.AsJInt32(), kCible.m_IdTerrain.AsJInt32());

	CCalcItemParams Params;

	Params.SetTerrain(pTerrain);
	Params.SetEchantillonID(1);
	Params.SetUniversID(pTerrain->GetDefaultUnivers());

	JMTFR03KEYModulation keyMod;
	keyMod.m_IdSource = kCible.m_IdSource;
	keyMod.m_IdTerrain = kCible.m_IdTerrain;
	m_TblModulations.MoveTo(keyMod);
	JMTFR03DATModulation & DatMod = m_TblModulations.GetItem();

	Params.SetTypePoidsID(DatMod.m_FLTPoids.Item(DatMod.m_FLTPoids.GetSelItem()).AsJInt32());


	JList<JInt32> lstSegments;
	for(int i = 0; i<DatMod.m_FLTSegments.GetCount();i++)
	{
		if(DatMod.m_FLTSegments.GetSelItem(i))
			lstSegments.AddTail() = DatMod.m_FLTSegments.Item(i).AsJInt32();
	}

	Params.SetSegmentsIDs(lstSegments);

	CBitArray bArrPopSeg;
	JUnt32 contingent =  0xFFFF;

	//TODO recuperer le bon segment
	pTerrain->GetDefaultPopSegment(bArrPopSeg, contingent);
	Params.SetPopulationSegment(bArrPopSeg);

	item->Calculate(&Params);

	const CBitArray * bArr = Params.GetResults();
	const JFLT32VECTOR * vect = Params.GetProbas();

	m_TblCibles.MoveTo(kCible);
	if (! m_TblCibles.IsValid() )
	{
		if(vect)
		{
			JFlt64* pProba = new JFlt64[vect->GetCount()];
			int i = 0;
			for(vect->MoveFirst(); vect->IsValid(); vect->MoveNext(), i++)
				pProba[i] = vect->GetItem();

			m_TblCibles.Add(kCible, (const JFlt64*) pProba, bArr->GetSize());
			delete []pProba;
		}
		else
		{
			m_TblCibles.Add(kCible, bArr->GetData(), bArr->GetSize());
		}						
	}
	return true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
// Fabrique les cibles pondérées existantes
bool CMoteurPresse::MakeCiblesPonderes(JList <JUnt32> & LstItemsPonderes, JUnt32 IDSource, JUnt32 IDTerrain)
{
	// Récupère tous les items construits
	// CItemsConstruits * pItems = CItemsConstruits::GetInstance();

	// Définit la clé cible
	JMTFR03KEYCible KeyCiblePond;
	KeyCiblePond.m_IdSource		= IDSource;
	KeyCiblePond.m_IdTerrain	= IDTerrain;

	// Pour chaque item pondéré
	for(LstItemsPonderes.MoveFirst(); LstItemsPonderes.IsValid(); LstItemsPonderes.MoveNext())
	{
		// Récupère l'item
		// IItem * pIItem = pItems->GetIItemByID(LstItemsPonderes.GetItem());
		IItem * pIItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(LstItemsPonderes.GetItem());

		// Test si c'est bien un item pondéré
		if (pIItem)
		{
			//////////////////////////////////////////////// Cas d'un Item Pondéré ///////////////////////////////////////
			if (pIItem->IsItemPondere())
			{
				// Récupère l'item pondéré
				CItemPondere * pItemPond = pIItem->AsItemPondere();

				// Récup ident cible pondéré
				KeyCiblePond.m_IdCible = pIItem->GetID();

				// Récupère la cible de base de l'item pondéré
				JInt32 IdItemBase = pItemPond->GetIDItemBase(); 

				// Cette cible est pour le moment autorisée sur ce terrain, on l'ajoute en indiquant cible non autorisée
				AddCiblePonderee(KeyCiblePond, pItemPond);
			}
		}
	}
	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
// Ajout de la cible pour les calculs
bool CMoteurPresse::AddCiblePonderee(JMTFR03KEYCible KeyCiblePond, CItemPondere * pItemPond)
{
	// Récupère tous les items construits
	// CItemsConstruits * pItems = CItemsConstruits::GetInstance();

	// Récupère le terrain en cours
	CTerrain * pTerrain = m_TerrainManager->GetTerrain(KeyCiblePond.m_IdSource.AsJInt32(), KeyCiblePond.m_IdTerrain.AsJInt32());

	// Instance Paramètres de calcul cible
	CCalcItemParams Params;

	// Initialise paramètres Terrain, Echantillon, Univers
	Params.SetTerrain(pTerrain);
	Params.SetEchantillonID(1);
	Params.SetUniversID(pTerrain->GetDefaultUnivers());

	// Récupèration table des modulations
	JMTFR03KEYModulation keyMod;
	keyMod.m_IdSource	= KeyCiblePond.m_IdSource;
	keyMod.m_IdTerrain	= KeyCiblePond.m_IdTerrain;
	m_TblModulations.MoveTo(keyMod);
	JMTFR03DATModulation & DatMod = m_TblModulations.GetItem();

	// Positionne le type poids
	Params.SetTypePoidsID(DatMod.m_FLTPoids.Item(DatMod.m_FLTPoids.GetSelItem()).AsJInt32());

	// Positionne les segments
	JList<JInt32> lstSegments;
	for(int i = 0; i<DatMod.m_FLTSegments.GetCount();i++)
	{
		if(DatMod.m_FLTSegments.GetSelItem(i))
			lstSegments.AddTail() = DatMod.m_FLTSegments.Item(i).AsJInt32();
	}
	Params.SetSegmentsIDs(lstSegments);

	// Recuperer le bon segment population
	CBitArray bArrPopSeg;
	JUnt32 contingent =  0xFFFF;
	pTerrain->GetDefaultPopSegment(bArrPopSeg, contingent);
	Params.SetPopulationSegment(bArrPopSeg);

	/////////////////////////////////////////////////////// Item de base ////////////////////////////////////////////
	//
	// Récupère item de base
	JUnt32 IdItemBase = pItemPond->GetIDItemBase();
	// const CItemConstruit * pItemBase = pItems->GetItemConstruit(IdItemBase); 
	const CItemConstruit * pItemBase = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(IdItemBase); 

	// Calcul avec l'item de base
	pItemBase->Calculate(&Params);

	// Récupère vecteur item de base
	const CBitArray		* bArr_ItemBase	= Params.GetResults();
	const JFLT32VECTOR	* vect_ItemBase	= Params.GetProbas();

	// Sauvegarde des vecteurs de l'item de base pour cette cible pondérée
	JUnt32 NbIndivItemBase	= 0;
	JFlt64 TotPoidsItemBase = 0.0;
	m_TblCibles.MoveTo(KeyCiblePond);
	if (! m_TblCibles.IsValid() )
	{
		if(vect_ItemBase)
		{
			JFlt64* pProba = new JFlt64[vect_ItemBase->GetCount()];
			int i = 0;
			for(vect_ItemBase->MoveFirst(); vect_ItemBase->IsValid(); vect_ItemBase->MoveNext(), i++)
				pProba[i] = vect_ItemBase->GetItem();

			m_TblCibles.Add(KeyCiblePond, (const JFlt64*) pProba, bArr_ItemBase->GetSize());
			delete []pProba;
		}
		else
		{
			m_TblCibles.Add(KeyCiblePond, bArr_ItemBase->GetData(), bArr_ItemBase->GetSize());
		}						
	}
	
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Positionne les pondérations selon la cible active (à 1 si cible de l'offre)
void CMoteurPresse::SetPonderations(JUnt32 IdCible, JUnt32 IdSource, JUnt32 IdTerrain)
{
	// Récupère tous les items construits
	// CItemsConstruits * pItems = CItemsConstruits::GetInstance();

	// Récupère le terrain en cours
	CTerrain * pTerrain = m_TerrainManager->GetTerrain(IdSource, IdTerrain);

	// Récupération de l'item
	// IItem * pIItem = pItems->GetIItemByID(IdCible);
	IItem * pIItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(IdCible);

	// Si items ventilations on prend l'item cible courante
	if (IdCible!=0 && !pIItem)
	{
		// Récupération de l'item
		// pIItem = pItems->GetIItemByID(m_ItemCourant);
		pIItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(m_ItemCourant);
	}

	if (pIItem)
	{
		// Informations du terrain en cours
		JMTFR03TBLTerrains	 & TblTerrains		= GetTblTerrains();
		JMTFR03KEYTerrain KeyTerrain;
		KeyTerrain.m_IdSource  = IdSource; 
		KeyTerrain.m_IdTerrain = IdTerrain; 
		TblTerrains.MoveTo(KeyTerrain);
		if (TblTerrains.IsValid())
		{
			// LA table des modulations
			JMTFR03KEYModulation keyMod;
			keyMod.m_IdSource = IdSource;
			keyMod.m_IdTerrain = IdTerrain;
			m_TblModulations.MoveTo(keyMod);

			// Test si elmt modulation existe bien
			if (m_TblModulations.IsValid())
			{
				JMTFR03DATModulation & DatMod = m_TblModulations.GetItem();

				const JMTFR03DATTerrain & DATTerrain = TblTerrains.GetItem(); 


				JVector<JInt32> vect;
				pTerrain->GetTypesPoids(vect); 

				JInt32 IdTypePoids = DatMod.m_FLTPoids.Item(DatMod.m_FLTPoids.GetSelItem()).AsJInt32();
				if (!pTerrain->HasTypePoids(IdTypePoids))
					IdTypePoids = pTerrain->GetDefaultTypePoidsID() ;

				JInt32 IdUnivers;
				IdUnivers = pTerrain->GetDefaultUnivers();

				JMTFR03KEYPoids KEYPoids;
				KEYPoids.m_IdPoids   = IdTypePoids ;
				KEYPoids.m_IdUnivers = IdUnivers ;
				DATTerrain.m_TBLPoids.MoveTo(KEYPoids); 
				JInt32 NbPoids = DATTerrain.m_TBLPoids.GetItem().m_ItemPoids.GetCount();

				// Si cible ordinaire (non pondérée) , on initialise vecteur pondérations à 1
				if (!pIItem->IsItemPondere())
				{
					// JMTFR03 NEW
					// Initialise le vecteur pondération
					JMTFR03TBLPonderations & TblPonderations = TblTerrains.GetTblPonderations(KeyTerrain);

					// Init toutes les pondérations à 1
					TblPonderations.Reset(); 
					TblPonderations.InitPonderation(NbPoids);
				}
				else
				{
					// Instance Paramètres de calcul cible
					CCalcItemParams Params;

					// Initialise paramètres Terrain, Echantillon, Univers
					Params.SetTerrain(pTerrain);
					Params.SetEchantillonID(1);
					Params.SetUniversID(pTerrain->GetDefaultUnivers());

					// Récupèration table des modulations
					JMTFR03KEYModulation keyMod;
					keyMod.m_IdSource	= IdSource;
					keyMod.m_IdTerrain	= IdTerrain;
					m_TblModulations.MoveTo(keyMod);
					JMTFR03DATModulation & DatMod = m_TblModulations.GetItem();

					// Positionne le type poids
					Params.SetTypePoidsID(DatMod.m_FLTPoids.Item(DatMod.m_FLTPoids.GetSelItem()).AsJInt32());

					// Positionne les segments
					JList<JInt32> lstSegments;
					for(int i = 0; i<DatMod.m_FLTSegments.GetCount();i++)
					{
						if(DatMod.m_FLTSegments.GetSelItem(i))
							lstSegments.AddTail() = DatMod.m_FLTSegments.Item(i).AsJInt32();
					}
					Params.SetSegmentsIDs(lstSegments);

					// Recuperer le bon segment population
					CBitArray bArrPopSeg;
					JUnt32 contingent =  0xFFFF;
					pTerrain->GetDefaultPopSegment(bArrPopSeg, contingent);
					Params.SetPopulationSegment(bArrPopSeg);

					/////////////////////////////////////////////////////// Item de base ////////////////////////////////////////////
					//
					// Récupère l'item pondéré
					CItemPondere * pItemPond = pIItem->AsItemPondere();

					// Définit la clé cible
					JMTFR03KEYCible KeyCiblePond;
					KeyCiblePond.m_IdSource		= IdSource;
					KeyCiblePond.m_IdTerrain	= IdTerrain;

					// Récup ident cible pondéré
					KeyCiblePond.m_IdCible = pIItem->GetID();

					// Récupère item de base
					JUnt32 IdItemBase = pItemPond->GetIDItemBase();
					// const CItemConstruit * pItemBase = pItems->GetItemConstruit(IdItemBase); 
					const CItemConstruit * pItemBase = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(IdItemBase); 

					// Calcul avec l'item de base
					pItemBase->Calculate(&Params);

					// Total poids item de base
					JFlt64 TotPoidsItemBase = 0.0;

					// Récupère vecteur item de base
					const CBitArray		* bArr_ItemBase	= Params.GetResults();
					const JFLT32VECTOR	* vect_ItemBase	= Params.GetProbas();

					// C'est un item pondéré, on applique les pondérations
					// Total poids cible de base
					JInt32 Nb;
					Nb = bArr_ItemBase->GetSize();

					for (JInt32 k = 0; k < Nb; k ++)
					{
						// On cumul les poids si individu fait partie de la cible de base
						if (bArr_ItemBase->GetAt(k))
						{
							TotPoidsItemBase += DATTerrain.m_TBLPoids.GetItem().m_ItemPoids.Item(k).AsJInt32();
						}
					}

					//////////////////////////////////////// Boucle sur les composants pondérations ////////////////////////////////////
					//
					// Récupère les listes informations composants
					JList <JInt32> LstIDCompos;
					JList <JFlt64> LstCoeffCompos;
					pItemPond->GetIDItems(LstIDCompos, LstCoeffCompos); 

					// Liste des coefficients à appliquer
					JList <JFlt64> LstPrctViaBase; 

					// Le vecteur pour le complément
					CBitArray TBitComplement;
					TBitComplement.SetSize(Nb);

					// Le coefficient applicable pour les élméents complémentaires
					JFlt64 CoeffReelComplement = 100.00;
					JFlt64 CoeffPondComplement = 100.00;

					// Initialise tous les élmts à true
					for (JInt32 k = 0; k < Nb; k ++)
					{
						TBitComplement.SetAt(k, true); 	
					}

					// Boucle sur ces composants (et calcul les nouveaux poids)
					LstCoeffCompos.MoveFirst(); 
					for (LstIDCompos.MoveFirst(); LstIDCompos.IsValid(); LstIDCompos.MoveNext(), LstCoeffCompos.MoveNext())
					{
						// Récupère les informations du composant
						JUnt32 IdCompo		= LstIDCompos.GetItem();
						JFlt64 PonderCompo	= LstCoeffCompos.GetItem(); 

						// Instance paramètrage pour composant
						CCalcItemParams ParamsCompo;
						ParamsCompo.SetTerrain(pTerrain);
						ParamsCompo.SetEchantillonID(1);
						ParamsCompo.SetUniversID(pTerrain->GetDefaultUnivers());

						// Positionne les poids
						ParamsCompo.SetTypePoidsID(DatMod.m_FLTPoids.Item(DatMod.m_FLTPoids.GetSelItem()).AsJInt32());
						ParamsCompo.SetSegmentsIDs(lstSegments);

						// Positionne le segment population 
						ParamsCompo.SetPopulationSegment(bArrPopSeg);

						// Récupère l'item composant
						// CItemConstruit * pItemCompo = pItems->GetIItemByID(IdCompo)->AsItemConstruit();
						CItemConstruit * pItemCompo = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(IdCompo)->AsItemConstruit();

						// Calcul item composant via les paramètrages
						pItemCompo->Calculate(&ParamsCompo);

						// Récupère les résultats
						const CBitArray		* bArrCompo	= ParamsCompo.GetResults();
						const JFLT32VECTOR	* vectCompo	= ParamsCompo.GetProbas();

						// Calcul total poids de ce vecteur composant
						JFlt64 TotPoidsCompo = 0.0;

						// Boucle sur tous les indivs du vecteur
						JUnt32 NbIndivActif = 0;
						Nb = bArrCompo->GetSize();

						// Boucle sur tous les individus du vecteur de base
						for (JInt32 k = 0; k < Nb; k ++)
						{
							JBool ValByte = bArrCompo->GetAt(k);
							if (ValByte)
							{
								// Cumul poids si individu fait partie du vecteur composant
								TotPoidsCompo += DATTerrain.m_TBLPoids.GetItem().m_ItemPoids.Item(k).AsJInt32();
								NbIndivActif++;

								// Indique elmt non inclu dans complément
								TBitComplement.SetAt(k, false); 
							}
						}

						// Calcul % et l'ajoute à la liste
						LstPrctViaBase.AddTail()  = ((JFlt64)TotPoidsCompo / (JFlt64)TotPoidsItemBase) * 100.0;
					}

					/////////////////////////////////////////////////////////////////////////////////////////////////
					//								Application des coeffs de pondération						   //	
					/////////////////////////////////////////////////////////////////////////////////////////////////
					// Initialise le vecteur pondération
					// JMTFR03 NEW
					JMTFR03TBLPonderations & TblPonderations = TblTerrains.GetTblPonderations(KeyTerrain);

					// Init toutes les pondérations à 1
					TblPonderations.Reset(); 
					TblPonderations.InitPonderation(NbPoids);
					
					// Nombre indivs du vecteur de base
					JInt32 NbIndivBase = bArr_ItemBase->GetSize();

					// Boucle sur la liste des coefficients pondérations
					LstIDCompos.MoveFirst(); 
					LstCoeffCompos.MoveFirst();
					for (LstPrctViaBase.MoveFirst(); LstPrctViaBase.IsValid(); LstPrctViaBase.MoveNext(), LstCoeffCompos.MoveNext(), LstIDCompos.MoveNext())
					{
						// Pondération retenue
						JFlt64 PonderCompo = LstCoeffCompos.GetItem();

						// Mise à jour pondération complément
						CoeffPondComplement -= PonderCompo;

						// Coeff réel
						JFlt64 CoeffReel = LstPrctViaBase.GetItem();

						// Mise à jour coeff reel complément
						CoeffReelComplement -= CoeffReel; 

						// Calcul du coefficient à appliquer (pour repecter le coeff pondération)
						JFlt64 CoeffApplicable = 1.0;
						if (CoeffReel)
							CoeffApplicable = PonderCompo / CoeffReel;

						// Instance paramètrage pour composant
						CCalcItemParams ParamsCompo;
						ParamsCompo.SetTerrain(pTerrain);
						ParamsCompo.SetEchantillonID(1);
						ParamsCompo.SetUniversID(pTerrain->GetDefaultUnivers());

						// Positionne les poids
						ParamsCompo.SetTypePoidsID(DatMod.m_FLTPoids.Item(DatMod.m_FLTPoids.GetSelItem()).AsJInt32());
						ParamsCompo.SetSegmentsIDs(lstSegments);

						// Positionne le segment population 
						ParamsCompo.SetPopulationSegment(bArrPopSeg);

						// Récupère l'id composant
						const JInt32 IdCompo = LstIDCompos.GetItem();

						// Récupère l'item composant
						// CItemConstruit * pItemCompo = pItems->GetIItemByID(IdCompo)->AsItemConstruit();
						CItemConstruit * pItemCompo = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(IdCompo)->AsItemConstruit();

						// Calcul item composant via les paramètrages
						pItemCompo->Calculate(&ParamsCompo);

						// Récupère les résultats
						const CBitArray		* bArrCompo	= ParamsCompo.GetResults();
						const JFLT32VECTOR	* vectCompo	= ParamsCompo.GetProbas();

						// JMTFR03 NEW
						for (JInt32 IdxIndiv = 0; IdxIndiv < Nb; IdxIndiv++)
						{
							JBool ValByteF = bArrCompo->GetAt(IdxIndiv);
							if (ValByteF)
							{
								// Applique coeff de ce composant
								TblPonderations.SetPonderations(IdxIndiv, CoeffApplicable);
							}
						}
					}

					if (CoeffReelComplement > 0 && CoeffPondComplement > 0)
					{
						// Calcul coeff applicable aux éléments en dehors des composants
						JFlt64 CoeffAppliCompl = CoeffPondComplement / CoeffReelComplement;

						// Affecte coefficient aux élmts compléments
						JUnt32 NnIndiv = 0;
						for (JInt32 IdxIndiv = 0; IdxIndiv < Nb; IdxIndiv++)
						{
							// JMTFR03 NEW
							JBool ValByteCompl = TBitComplement.GetAt(IdxIndiv);
							if (ValByteCompl)
							{
								// Applique coeff de ce composant
								TblPonderations.SetPonderations(IdxIndiv, CoeffAppliCompl);
								NnIndiv++;
							}
						}
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Vérifie si la cible est valide sur une liste de terrains
bool CMoteurPresse::CiblePondAutorise(JUnt32 IdCiblePond, JUnt32 IdSource, JMap <JUnt32, JUnt32> *pMapIdSelTerrain)
{
	// Initialise le flag autorisation
	JBool CiblePondOk = true;

	// On vérifie que c'est bien un ident item pondéré
	// CItemsConstruits * pItems = CItemsConstruits::GetInstance();

	// Récupération de l'item
	// IItem * pIItem = pItems->GetIItemByID(IdCiblePond);
	IItem * pIItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(IdCiblePond);

	// Est-ce vraiment un item pondéré
	if (pIItem->IsItemPondere())
	{
		// Récup item pondéré 
		CItemPondere *pItemPond = pIItem->AsItemPondere(); 

		if (pItemPond)
		{
			// Initialise la balise test
			JBool AuMoins1TerrainNonAutorise = false;

			// Boucle sur tous les terrains sélectionnés
			for (pMapIdSelTerrain->MoveFirst(); pMapIdSelTerrain->IsValid(); pMapIdSelTerrain->MoveNext())
			{
				// Récup ident terrain
				const JUnt32 IdTerrain = pMapIdSelTerrain->GetKey();

				// Récupère le terrain en cours
				CTerrain * pTerrain = m_TerrainManager->GetTerrain(IdSource, IdTerrain);

				// Récup table des modulations
				JMTFR03KEYModulation keyMod;
				keyMod.m_IdSource = IdSource;
				keyMod.m_IdTerrain = IdTerrain;
				m_TblModulations.MoveTo(keyMod);
				JMTFR03DATModulation & DatMod = m_TblModulations.GetItem();

				JVector<JInt32> vect;
				pTerrain->GetTypesPoids(vect); 

				JInt32 IdTypePoids = DatMod.m_FLTPoids.Item(DatMod.m_FLTPoids.GetSelItem()).AsJInt32();
				if (!pTerrain->HasTypePoids(IdTypePoids))
					IdTypePoids = pTerrain->GetDefaultTypePoidsID() ;

				JInt32 IdUnivers;
				IdUnivers = pTerrain->GetDefaultUnivers();

				// Instance Paramètres de calcul cible
				CCalcItemParams Params;

				// Initialise paramètres Terrain, Echantillon, Univers
				Params.SetTerrain(pTerrain);
				Params.SetEchantillonID(1);
				Params.SetUniversID(pTerrain->GetDefaultUnivers());

				// Positionne le type poids
				Params.SetTypePoidsID(DatMod.m_FLTPoids.Item(DatMod.m_FLTPoids.GetSelItem()).AsJInt32());

				// Positionne les segments
				JList<JInt32> lstSegments;
				for(int i = 0; i<DatMod.m_FLTSegments.GetCount();i++)
				{
					if(DatMod.m_FLTSegments.GetSelItem(i))
						lstSegments.AddTail() = DatMod.m_FLTSegments.Item(i).AsJInt32();
				}
				Params.SetSegmentsIDs(lstSegments);

				// Recuperer le bon segment population
				CBitArray bArrPopSeg;
				JUnt32 contingent =  0xFFFF;
				pTerrain->GetDefaultPopSegment(bArrPopSeg, contingent);
				Params.SetPopulationSegment(bArrPopSeg);

				// Définit la clé cible
				JMTFR03KEYCible KeyCiblePond;
				KeyCiblePond.m_IdSource		= IdSource;
				KeyCiblePond.m_IdTerrain	= IdTerrain;

				// Récup ident cible pondéré
				KeyCiblePond.m_IdCible = pIItem->GetID();

				// Récupère item de base
				JUnt32 IdItemBase = pItemPond->GetIDItemBase();
				// const CItemConstruit * pItemBase = pItems->GetItemConstruit(IdItemBase); 
				const CItemConstruit * pItemBase = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(IdItemBase); 

				// Calcul avec l'item de base
				pItemBase->Calculate(&Params);

				// Total indiv item de base au delà su seuil rédhibitoire du terrain
				JInt32 TotIndivActif = 0;

				// Récupère vecteur item de base
				const CBitArray		* bArr_ItemBase	= Params.GetResults();
				// const JFLT32VECTOR	* vect_ItemBase	= Params.GetProbas();

				// Calcul du nombre d'élémnt actif
				for (JInt32 k = 0; k < bArr_ItemBase->GetSize(); k ++)
				{
					JBool ValByte = bArr_ItemBase->GetAt(k);
					if (ValByte)
					{
						TotIndivActif ++;
					}
				}

				// Test si la représentativité de la cible pondéré dépasse le seuil rédhibitoire du terrain
				if (TotIndivActif < pTerrain->GetSeuilRedhibitoire())
				{
					// Seuil non atteint, on arrête tout et renvoie cible non représentative
					return false;
				}
			} // fin boucle terrains sélectionnés

			// Pas de soucis la cible est bien représentée sur tous les terrains en cours de sélection
			return true;
		}
		else
			return false;
	}
	else
		// Ce n'est pas un item pondéré
		return false;
}


bool CMoteurPresse::AddCibleVentilation(JATPOffre& offre, JList<IItem*> &lst)
{
	if(lst.GetCount())
	{
		m_TblEffectifs.Reset();
		m_TblAudiences.Reset();
		// CItemsConstruits * pItems = CItemsConstruits::GetInstance();

		for(offre.m_TBLIdTerrainsAudience.MoveFirst();
			offre.m_TBLIdTerrainsAudience.IsValid();
			offre.m_TBLIdTerrainsAudience.MoveNext())
		{
			//ajout du terrain

			JMTFR03KEYCible kCible;
			kCible.m_IdSource = offre.m_IdSourceAudience;
			kCible.m_IdTerrain = offre.m_TBLIdTerrainsAudience.GetItem();

			//on retire les cibles et les valeurs
			for(lst.MoveFirst(); lst.IsValid(); lst.MoveNext())
			{
				// Récupération de l'item
				IItem* item = lst.GetItem();

				// Traitement item construit standard
				if (item->IsItemConstruit())
				{
					kCible.m_IdCible = item->GetID()+10000000;
					m_TblCibles.MoveTo(kCible);
					if (m_TblCibles.IsValid())
						m_TblCibles.Remove();
				}

				// Traitement de l'item pondéré
				if (item->IsItemPondere())
				{
					kCible.m_IdCible = item->GetID()+10000000;
					m_TblCibles.MoveTo(kCible);
					if (m_TblCibles.IsValid())
						m_TblCibles.Remove();
				}

				// Traitement de l'item palette
				if (item->IsPalette())
				{
					CPalettes * palette = item->AsPalette();
					JList<JInt32> idList;
					palette->GetItems(idList);
					for (idList.MoveFirst(); idList.IsValid(); idList.MoveNext())
					{
						// CItemConstruit * itempal = pItems->GetIItemByID( idList.GetItem() )->AsItemConstruit();
						CItemConstruit * itempal = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID( idList.GetItem() )->AsItemConstruit();

						kCible.m_IdCible = itempal->GetID()+10000000;
						m_TblCibles.MoveTo(kCible);
						if (m_TblCibles.IsValid())
							m_TblCibles.Remove();
					}
				}
			}

			CTerrain * pTerrain = m_TerrainManager->GetTerrain(kCible.m_IdSource.AsJInt32(), kCible.m_IdTerrain.AsJInt32());

			CCalcItemParams Param1;

			Param1.SetTerrain(pTerrain);
			Param1.SetEchantillonID(1);
			Param1.SetUniversID(pTerrain->GetDefaultUnivers());

			JMTFR03KEYModulation keyMod;
			keyMod.m_IdSource = kCible.m_IdSource;
			keyMod.m_IdTerrain = kCible.m_IdTerrain;
			m_TblModulations.MoveTo(keyMod);
			JMTFR03DATModulation & DatMod = m_TblModulations.GetItem();

			Param1.SetTypePoidsID(DatMod.m_FLTPoids.Item(DatMod.m_FLTPoids.GetSelItem()).AsJInt32());

			JList<JInt32> lstSegments;
			for(int i = 0; i<DatMod.m_FLTSegments.GetCount();i++)
			{
				if(DatMod.m_FLTSegments.GetSelItem(i))
					lstSegments.AddTail() = DatMod.m_FLTSegments.Item(i).AsJInt32();
			}

			Param1.SetSegmentsIDs(lstSegments);
			
			CBitArray bArrPopSeg;
			JUnt32 contingent =  0xFFFF;
			//TODO recuperer le bon segment

			pTerrain->GetDefaultPopSegment(bArrPopSeg, contingent);
			Param1.SetPopulationSegment(bArrPopSeg);

			lst.MoveFirst();

			// Attention si c'est un item pondéré
			IItem *pItem = lst.GetItem();
			
			if (pItem->IsItemConstruit())
			{
				// Calcul item contruit
				CItemConstruit * pitem1 = lst.GetItem()->AsItemConstruit();
				pitem1->Calculate(&Param1);
				AddVentil(Param1, kCible, pitem1);
			}
			else
			{
				// Calcul item pondéré, à partir e l'item de base
				CItemPondere * pitem1 = lst.GetItem()->AsItemPondere();

				// Récup item de base
				// CItemsConstruits * pItems = CItemsConstruits::GetInstance();

				JUnt32 IdItemBase = pitem1->GetIDItemBase();
				// IItem * pIItem = pItems->GetIItemByID(IdItemBase);  
				IItem * pIItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(IdItemBase);  

				if (pIItem->IsItemConstruit())
				{
					// Récup l'item construit associé à l'item de base
					CItemConstruit * pitem1 = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(IdItemBase);
					pitem1->Calculate(&Param1);
					AddVentil(Param1, kCible, pitem1);
				}
			}

			lst.MoveNext();
			while (lst.IsValid())
			{
				IItem * item2 = lst.GetItem();

				if (item2->IsItemConstruit())
					AddVentil(Param1, kCible, item2->AsItemConstruit());
				if (item2->IsPalette())
				{
					CPalettes * palette = item2->AsPalette();
					JList<JInt32> idList;
					palette->GetItems(idList);
					for (idList.MoveFirst(); idList.IsValid(); idList.MoveNext())
					{
						// CItemConstruit * item = pItems->GetIItemByID( idList.GetItem())->AsItemConstruit();
						CItemConstruit * item = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID( idList.GetItem())->AsItemConstruit();
						AddVentil(Param1, kCible, item);
					}
				}
				lst.MoveNext();
			}

			// Recalcul mais sans repositionner les valeurs pondérations
			MakeValues(offre, kCible.m_IdSource.AsJInt32(), kCible.m_IdTerrain.AsJInt32(), false);

		}//for
	}
	return true;
}

bool CMoteurPresse::AddVentil(CCalcItemParams &Param1, JMTFR03KEYCible kCible, CItemConstruit* item)
{
	CCalcItemParams Param2;
	Param2.CopyParams(Param1);

	item->Calculate(&Param2);
	CResults res(0, IBloc::And);

	CResults* pRes1 = res.AddResult(new CResults(&res,IBloc::Nop));
	pRes1->SetResult(Param1.GetResults());
	pRes1->SetProbas(Param1.GetProbas());

	CResults* pRes2 = res.AddResult(new CResults(&res,IBloc::Nop));
	pRes2->SetResult(Param2.GetResults());
	pRes2->SetProbas(Param2.GetProbas());
	
	res.Calculate();
	
	const CBitArray * bArr = 0;
	res.GetResult(bArr);
	const JFLT32VECTOR * vect = 0;
	res.GetProbas(vect);
	kCible.m_IdCible = item->GetID()+10000000;
	m_TblCibles.MoveTo(kCible);
	if(m_TblCibles.IsValid())
		m_TblCibles.Remove();
	if (! m_TblCibles.IsValid() )
	{
		if(vect)
		{
			JFlt64* pProba = new JFlt64[vect->GetCount()];
			int i = 0;
			for(vect->MoveFirst(); vect->IsValid(); vect->MoveNext(), i++)
				pProba[i] = vect->GetItem();

			m_TblCibles.Add(kCible, (const JFlt64*) pProba, bArr->GetSize());
			delete []pProba;
		}
		else
			m_TblCibles.Add(kCible, bArr->GetData(), bArr->GetSize());
	}

	return true;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////
// Calcul pour chaque items  des effectifs et contacts
bool CMoteurPresse::MakeValues(JATPOffre& offre, JUnt32 IdSource, JUnt32 IdTerrain, JBool CalculPonderation)
{
	CKeyMoteur key;
	key.m_IDSource = IdSource;
	key.m_IDTerrain = IdTerrain;

	// Nobre de cible
	int Nb = m_TblCibles.GetCount();

	//pour chaque item on calcule son effectif et ses contacts
	for(m_TblCibles.MoveFirst(); m_TblCibles.IsValid(); m_TblCibles.MoveNext())
	{
		key.m_IDItem = m_TblCibles.GetKey().m_IdCible.AsJInt32();
		AddEffectif(key, CalculPonderation);
		CalcContacts(offre, key);
	}

	return true;
}

bool CMoteurPresse::MakeEffectifs(JUnt32 IdSource, JUnt32 IdTerrain)
{
	CKeyMoteur key;
	key.m_IDSource = IdSource;
	key.m_IDTerrain = IdTerrain;

	//pour chaque item on calcule son effectif
	for(m_TblCibles.MoveFirst(); m_TblCibles.IsValid(); m_TblCibles.MoveNext())
	{
		key.m_IDItem = m_TblCibles.GetKey().m_IdCible.AsJInt32();
		AddEffectif(key,false);
	}

	return true;
}

bool CMoteurPresse::AddEffectif(CKeyMoteur key, JBool CalculPonderation)
{
	JMTFR03KEYEffectif kEff;
	kEff.m_IdSource = key.m_IDSource;
	kEff.m_IdTerrain = key.m_IDTerrain;
	kEff.m_IdCible = key.m_IDItem;

	if(kEff.IsValid())
	{
		m_TblEffectifs.MoveTo(kEff);
		if (!m_TblEffectifs.IsValid())
		{

			// Repositionne les valeurs pondérations si nécessaire
			// if (CalculPonderation)
			if (kEff.m_IdCible.IsValid())
				SetPonderations(kEff.m_IdCible.AsJInt32(), kEff.m_IdSource.AsJInt32(), kEff.m_IdTerrain.AsJInt32());

			// Ajoute à la table des effectifs
			m_TblEffectifs.Add(kEff, m_TerrainManager->m_TblTerrains, m_TblModulations, m_TblCibles);
		}
	}
	return true;
}

bool CMoteurPresse::MakeContacts(JATPOffre& offre, JUnt32 IdSource, JUnt32 IdTerrain)
{
	CKeyMoteur key;
	key.m_IDSource = IdSource;
	key.m_IDTerrain = IdTerrain;

	//pour chaque item on calcule ses contacts
	for(m_TblCibles.MoveFirst(); m_TblCibles.IsValid(); m_TblCibles.MoveNext())
	{
		key.m_IDItem = m_TblCibles.GetKey().m_IdCible.AsJInt32();
		CalcContacts(offre, key);
	}

	return true;
}

bool CMoteurPresse::CalcContacts(JATPOffre& offre, CKeyMoteur key)
{
	//pour tout les supports
	const JSRCPresse & srcpresse = JSRCPresse::GetInstance();
	for(offre.m_TBLIdSupports.MoveFirst();
		offre.m_TBLIdSupports.IsValid();
		offre.m_TBLIdSupports.MoveNext())
	{
		
		IElem* pElem = CRegroupements::GetInstance()->GetIElemByID(offre.m_TBLIdSupports.GetItem());
		
		if(pElem == NULL)
			continue;
			//return false;

		// Calcul contact dans le cas d'un couplage
		if(pElem->IsCouplage())
		{
			// Crée liste des supports lot
			JList<IElem*> ListSup;
			pElem->AsCouplage()->GetFils(ListSup);

			JMTFR03Couplage couplage;
			// On ajoute les supports enfants au map supports
			for(ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
			{
				// Ajout Contact
				AddContact(key, ListSup.GetItem()->AsITitre()->GetIDAudience());

				// Ajout elmt couplage
				JUnt32 IdAudience = ListSup.GetItem()->AsITitre()->GetIDAudience();
				couplage.MoveTo(IdAudience);
				if (!couplage.IsValid())
					couplage.Add(ListSup.GetItem()->AsITitre()->GetIDAudience());
			}


			//On ajoute le couplage dans la table d'audience
			JMTFR03KEYAudience kAudience;
			kAudience.m_IdSource = key.m_IDSource;
			kAudience.m_IdTerrain = key.m_IDTerrain;
			kAudience.m_IdCible = key.m_IDItem;
			kAudience.m_IdTitre = pElem->GetID();
			
			if(kAudience.IsValid())
			{
				m_TblAudiences.MoveTo(kAudience);
				if (!m_TblAudiences.IsValid() && m_Options.IsValid() )
					m_TblAudiences.Add(kAudience, m_TerrainManager->m_TblTerrains, m_TblModulations, m_TblCibles, m_Options, couplage);
			}

		}

		if(pElem->IsLot())
		{
			JList<IElem*> ListSup;
			pElem->AsLot()->GetFils(ListSup);
			// On ajoute les supports enfants au map supports
			for(ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
				AddContact(key, ListSup.GetItem()->AsITitre()->GetIDAudience());
		}
		
		// Calcul contact dans le cas d'un support simple
		if (pElem->IsTitre() && pElem->AsITitre()->GetIDAudience())
			AddContact(key, pElem->AsITitre()->GetIDAudience());
	}

	// Pour toutes les familles // balaye toutes les lignes associées aux familles
	for (offre.m_MapSelFamilles.MoveFirst(); offre.m_MapSelFamilles.IsValid(); offre.m_MapSelFamilles.MoveNext())
	{
		// Ident famille
		JUnt32 IdFamille = offre.m_MapSelFamilles.GetKey();
		IElem* pElem = CRegroupements::GetInstance()->GetIElemByID(IdFamille);
		
		if(pElem == NULL)
			continue;
		
		// Calcul contact dans le cas d'une famille
		if(pElem->IsLot())
		{
			// Récupération des élmts fils de la famille
			JList<IElem*> ListSup;
			pElem->AsLot()->GetFils(ListSup);

			// Objet famille pour calcul audience
			JMTFR03Couplage Famille;	

			// On ajoute les supports enfants au map supports
			for(ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
			{
				if (ListSup.GetItem()->IsTitre())
				{
					// Ajout des contacts élmts fils
					AddContact(key, ListSup.GetItem()->AsITitre()->GetIDAudience());

					// Ajout elmt famille
					JUnt32 IdAudience = ListSup.GetItem()->AsITitre()->GetIDAudience();
					Famille.MoveTo(IdAudience);
					if (!Famille.IsValid())
						Famille.Add(ListSup.GetItem()->AsITitre()->GetIDAudience());
				}
			}

			//On ajoute la famille dans la table d'audience
			JMTFR03KEYAudience kAudience;
			kAudience.m_IdSource	= key.m_IDSource;
			kAudience.m_IdTerrain	= key.m_IDTerrain;
			kAudience.m_IdCible		= key.m_IDItem;
			kAudience.m_IdTitre		= pElem->GetID();
			
			if(kAudience.IsValid())
			{
				m_TblAudiences.MoveTo(kAudience);
				if (!m_TblAudiences.IsValid() && m_Options.IsValid() )
					m_TblAudiences.Add(kAudience, m_TerrainManager->m_TblTerrains, m_TblModulations, m_TblCibles, m_Options, Famille);
			}
		}
	}

	return true;
}

bool CMoteurPresse::AddContact(CKeyMoteur key, JUnt32 IDSupport)
{
	JMTFR03KEYAudience kAudience;
	kAudience.m_IdSource	= key.m_IDSource;
	kAudience.m_IdTerrain	= key.m_IDTerrain;
	kAudience.m_IdCible		= key.m_IDItem;
	kAudience.m_IdTitre		= IDSupport;
	
	if(kAudience.IsValid())
	{
		m_TblAudiences.MoveTo(kAudience);
		if (!m_TblAudiences.IsValid() && m_Options.IsValid() )
		{
			m_TblAudiences.Add(kAudience, m_TerrainManager->m_TblTerrains, m_TblModulations, m_TblCibles, m_Options);
		}
	}
	return true;
}



////////////////////////////////////////////


void CMoteurPresse::SetType(TYPEMOTEUR type)
{
	m_type = type;
}

int CMoteurPresse::GetType() const
{
	return m_type;
}


///////////////////////////////////////////////


void CMoteurPresse::GetCoefAlpha(CKeyTerrain key, JInt32 SupportId, JInt32 & dureeVie, JVector<JInt32>& vect, JInt32 & diviseur)
{
	if( m_type != PROBA )
	{
		CTerrain * pTerrain = m_TerrainManager->GetTerrain(key);	
		if(pTerrain)
		{
			vect.Reset();
			pTerrain->GetCoefAlpha(SupportId, dureeVie, vect, diviseur);
		}
	}
}



///////////////////////////////////////////////////////////////////////////////////
//Les méthodes de calcul
///////////////////////////////////////////////////////////////////////////////////
bool CMoteurPresse::GetEffCas(CKeyMoteur key, JFlt64 &effectif, JInt32 &nbcas) const
{
	bool bret = false;
	JMTFR03KEYEffectif keyEffectif;
	// on initialise la clé
	keyEffectif.m_IdSource = key.m_IDSource;
	keyEffectif.m_IdTerrain = key.m_IDTerrain;
	keyEffectif.m_IdCible = key.m_IDItem;
	
	if (keyEffectif.IsValid())
	{
		// on recherche l'effectif
		m_TblEffectifs.MoveTo(keyEffectif);

		// on teste si on a trouvé l'effectif
		if (m_TblEffectifs.IsValid())
		{
			// on récupère l'effectif
			const JMTFR03DATEffectif & DATEffectif = m_TblEffectifs.GetItem();
			// on initialise l'effectif
			nbcas = DATEffectif.m_NbCas.AsJInt32();
			effectif = DATEffectif.m_Effectif.AsJFlt64();
			bret = true;
		}
	}

	return bret;
}

JFlt64 CMoteurPresse::GetEffectif(CKeyMoteur key) const
{
	JFlt64 Effectif = -1.0;
	JInt32 nbCas = -1;

	GetEffCas(key, Effectif, nbCas);

	return Effectif;
}

JInt32 CMoteurPresse::GetNbCas(CKeyMoteur key) const
{
	JFlt64 Effectif = -1.0;
	JInt32 nbCas = -1;

	GetEffCas(key, Effectif, nbCas);

	return nbCas;
}


bool CMoteurPresse::SetMailleTemporelle(CKeyMoteur key, JFCMaille & maille, JInt32 idSupport, JUnt32 nbInsert, JDate date) const
{
	JInt32 day, month, year;
	date.GetDate(day, month, year);
	JInt32 rang = year * 10000+month * 100+day;

	const JSRCPresse & srcpresse = JSRCPresse::GetInstance();
	IElem* pElem = CRegroupements::GetInstance()->GetIElemByID( idSupport );

	JUnt32 idSuppAudience;
	//titre
	idSuppAudience  = pElem->AsITitre()->GetIDAudience();

	
	JFlt64 VolLect = 0.0;
	JFlt64 AudRef = 0.0;
	JFlt64 AudProba = 0.0;
	JInt32 PtEtal = 0;
	CalcAudience(key, idSupport, VolLect, AudRef, AudProba, PtEtal);
	JFlt64 GRPRef = 0.0;
	if(GetEffectif(key) > 0.0)
		GRPRef = (100.0 * AudRef) / GetEffectif(key);


	if(idSuppAudience)
	{
		maille.MoveTo(idSupport, rang);
		if (!maille.IsValid())
			maille.Add(idSupport, rang, idSuppAudience, nbInsert, GRPRef);
		else
		{
			JInt32 nbInsertOld = maille.GetNbInsertions().AsJInt32();
			maille.Update(nbInsertOld + nbInsert);
		}
		return true;
	}
	return false;
}

bool CMoteurPresse::SetMailleEchelle(CKeyMoteur key, JFCMaille & maille, JInt32 idSupport, JUnt32 nbInsert, bool bSuperpose/* = false*/) const
{
	unsigned int rang = 1;
	const JSRCPresse & srcpresse = JSRCPresse::GetInstance();
	IElem* pElem = CRegroupements::GetInstance()->GetIElemByID( idSupport );

	if (pElem == NULL)
		return false;

	//Si on a affaire à un couplage
	if(pElem->IsCouplage())
	{
		JList<IElem *> ListSupCoupl;
		pElem->AsCouplage()->GetFils(ListSupCoupl);
		//pour chaque support
		for(ListSupCoupl.MoveFirst(); ListSupCoupl.IsValid(); ListSupCoupl.MoveNext())
		{
			
			IElem * pIElem = ListSupCoupl.GetItem();
			SetMailleEchelle(key, maille, pIElem->GetID(), pIElem->AsTitreCouplage()->GetNbInsert()  * nbInsert , bSuperpose);
		}
		return true;
	}//fin pour les couplages

	// Est-ce une famille de supports
	if(pElem->IsLot())
	{
		// Crée liste des supports lot
		JList<JUnt32> ListSup;
		ILot * pLot = pElem->AsLot();
		if(pLot)
		{
			// Récupére les id supprorts composant la famille
			pLot->GetFils(ListSup);

			// Construction de la maille avec 1 insertion pour tous les supports de la famille
			maille.Reset(); 

			for (ListSup.MoveFirst(); ListSup.IsValid(); ListSup.MoveNext())
			{
				// Récup composant famille
				JUnt32 IdCompo = ListSup.GetItem(); 

				// Ajoute support à la maille
				SetMailleEchelle(key, maille, IdCompo, nbInsert , bSuperpose);
			}
		}
		return true;
	}//fin pour les familles
	
	JUnt32 idSuppAudience;
	//titre
	idSuppAudience  = pElem->AsITitre()->GetIDAudience();

	JFlt64 VolLect = 0.0;
	JFlt64 AudRef = 0.0;
	JFlt64 AudProba = 0.0;
	JInt32 PtEtal = 0;
	CalcAudience(key, idSupport, VolLect, AudRef, AudProba, PtEtal);
	JFlt64 GRPRef = 0.0;
	if(GetEffectif(key)>0.0)
	{
		// Valeur de GetEffectif(key)
		JFlt64 ValEffectif = GetEffectif(key);
		GRPRef = (100.0 * AudRef) / GetEffectif(key);
	}

	if(idSuppAudience)
	{
		if(bSuperpose)
			maille.Add(idSupport, rang, idSuppAudience, nbInsert, GRPRef);
		else
		{
			for (;rang <= nbInsert; rang++)
				maille.Add(idSupport, rang, idSuppAudience, 1, GRPRef);
		}
		return true;
	}
	return false;
}

bool CMoteurPresse::BuildDistributions(CKeyMoteur key, JFCMaille & maille, bool bNoDistrib/* = false*/ )
{
	m_FullBinomial.Reset();
	m_BetaBinomial.Reset();

	JMTFR03KEYCible kcible;
	kcible.m_IdCible = key.m_IDItem;
	kcible.m_IdSource = key.m_IDSource;
	kcible.m_IdTerrain = key.m_IDTerrain;

	if (kcible.IsValid() && m_Options.IsValid() && m_Options.IsValid())
	{
		if (m_type == PROBA)
			m_FullBinomial.SetItems( kcible, m_TerrainManager->m_TblTerrains, m_TblModulations, m_TblCibles, m_Options, maille, bNoDistrib);
		else
			m_BetaBinomial.SetItems( kcible, m_TerrainManager->m_TblTerrains, m_TblModulations, m_TblCibles, m_Options, maille, (m_type != 0)?true:false);
		return true;
	}
	return false;
}

JFCDistribution CMoteurPresse::GetDistribution(CMoteurPresse &presse, JFlt64 VolLecture)
{
	JFCDistribution Distribution;
	if (presse.m_type == PROBA)
		presse.m_FullBinomial.MakeDistribution(Distribution);
	else
		presse.m_BetaBinomial.MakeDistribution(VolLecture, Distribution);
	return Distribution;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Construction du vecteur distribution
bool CMoteurPresse::BuildTabDistributions(CKeyMoteur key, JFCMaille & maille, JUnt32 &NbInsertions)
{
	// Init méthode
	m_FullBinomialCum.Reset();
	m_BetaBinomial.Reset();

	// Cle moteur de calcul
	JMTFR03KEYCible kcible;
	kcible.m_IdCible	= key.m_IDItem;
	kcible.m_IdSource	= key.m_IDSource;
	kcible.m_IdTerrain	= key.m_IDTerrain;

	if (kcible.IsValid() && m_Options.IsValid() && m_Options.IsValid())
	{
		if (m_type == PROBA)
		{
			// Définition des élmts pour les distributions
			m_FullBinomialCum.SetItems( kcible, m_TerrainManager->m_TblTerrains, m_TblModulations, m_TblCibles, m_Options, maille, NbInsertions);
		}
		else
		{
			return false;
			// m_BetaBinomial.SetItems( kcible, m_TerrainManager->m_TblTerrains, m_TblModulations, m_TblCibles, m_Options, maille, (bool)m_type);
		}

		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////////
// Calcul distribution niveau nb insetions
void CMoteurPresse::GetTabDistribution(CMoteurPresse &presse, JUnt32 &NbInsert, JArray <JFCDistribution> &TDistribution, JUnt32 &LimitNbInsert)
{
	// Tableau résultat distribution
	TDistribution.Reset(); 
	TDistribution.SetCount(NbInsert);

	// Calcul Tableau des distributions en mode full binomial
	if (presse.m_type == PROBA)
	{
		// Boucle sur tous les plans de 0 à NbInsert-1
		// for (JUnt32 InxInsert = 1; InxInsert <= NbInsert; InxInsert++)
		for (JUnt32 InxInsert = 1; InxInsert <= LimitNbInsert; InxInsert++)
		{
			presse.m_FullBinomialCum.MakeDistribution(TDistribution.Item(InxInsert-1), InxInsert);
		}
	}
	// Calcul Tableau des distributions en mode beta binomial
	else
	{
		// A FAIRE
		// presse.m_BetaBinomial.MakeDistribution(VolLecture, TDistribution(0));
	}
}


bool CMoteurPresse::CalcAudience(CKeyMoteur key, JInt32 idSupport, JFlt64 & VolLecture, JFlt64 & AudienceRef, JFlt64 & AudienceProba, JInt32 & PtEtalonnage, bool bDuree) const
{
	bool bret = false;
	const JSRCPresse & srcpresse = JSRCPresse::GetInstance();
	IElem* pElem = CRegroupements::GetInstance()->GetIElemByID( idSupport );

	if (pElem == NULL)
		return bret;

	JMTFR03KEYAudience kAudience;	
	kAudience.m_IdSource = key.m_IDSource;
	kAudience.m_IdTerrain = key.m_IDTerrain;
	kAudience.m_IdCible = key.m_IDItem;

	CKeyTerrain keyTerrain(key.m_IDSource, key.m_IDTerrain);

	if(pElem->IsCouplage() && CanDescribeCouplage( pElem->GetID(), keyTerrain) )
	{
		// Calcul pour un couplage
		bret = CalcAudienceCouplage(kAudience, pElem, VolLecture, AudienceRef, AudienceProba, PtEtalonnage, bDuree);
	}
	else if (pElem->IsTitreApparente() || pElem->IsTitreApparie())
	{
		// Calcul pour un titre apparenté ou un titre apparié
		bret = CalcAudienceMediaWeight(kAudience, pElem, VolLecture, AudienceRef, AudienceProba, PtEtalonnage, bDuree);
	}
	else if (pElem->IsLot() && CanDescribeFamille( pElem->GetID(), keyTerrain)) 
	{
		// Calcul pour une famille (comme pour un couplage)
		// bret = CalcAudienceFamille(kAudience, pElem, VolLecture, AudienceRef, AudienceProba, PtEtalonnage, bDuree);
		PtEtalonnage = 30;
		bret = CalcAudienceFamille(kAudience, pElem, VolLecture, AudienceRef, AudienceProba, PtEtalonnage, bDuree);
	}
	else
	{
		// Calcul pour un titre simple
		bret = CalcAudienceTitre(kAudience, pElem, VolLecture, AudienceRef, AudienceProba, PtEtalonnage, bDuree);
	}

	return bret;
}

bool CMoteurPresse::CanDescribeCouplage(JUnt32 idCouplage, CKeyTerrain key)//static
{
	const JSRCPresse & srcpresse = JSRCPresse::GetInstance();
	IElem* pElem = CRegroupements::GetInstance()->GetIElemByID( idCouplage );
	if (pElem && pElem->IsCouplageFixe())
	{
		JList<IElem*> lst;
		pElem->AsCouplageFixe()->GetFils(lst);
		for(lst.MoveFirst(); lst.IsValid(); lst.MoveNext())
		{
			CTerrain* terr = CTerrainManager::GetInstance()->GetTerrainNoRef(key);
			if (!terr->HasSupport(lst.GetItem()->AsITitre()->GetIDAudience()))
				return false;
		}
	}
	return true;
}

// Les supports famille sont-ils entièrement définis sur le terrain
bool CMoteurPresse::CanDescribeFamille(JUnt32 IdFamille, CKeyTerrain key)
{
	const JSRCPresse & srcpresse = JSRCPresse::GetInstance();
	IElem* pElem = CRegroupements::GetInstance()->GetIElemByID(IdFamille);
	if (pElem)
	{
		if (pElem->IsLot())
		{
			JList<IElem*> lst;
			pElem->AsLot()->GetFils(lst);
			for(lst.MoveFirst(); lst.IsValid(); lst.MoveNext())
			{
				CTerrain* terr = CTerrainManager::GetInstance()->GetTerrainNoRef(key);
				if (!terr->HasSupport(lst.GetItem()->AsITitre()->GetIDAudience()))
					return false;
			}
		}
	}
	return true;
}

bool CMoteurPresse::CanUseSupportInt(IElem* pElem, CKeyTerrain key) const
{
	if (pElem && pElem->IsTitre())
	{
		CTerrain* terr  =CTerrainManager::GetInstance()->GetTerrainNoRef(key);
		JUnt32 idAud = pElem->AsITitre()->GetIDAudience();
		
		JMTFR03KEYModulation keyMod;
		keyMod.m_IdSource = key.m_IDSource;
		keyMod.m_IdTerrain = key.m_IDTerrain;

		m_TblModulations.MoveTo(keyMod);
		const JMTFR03DATModulation & DatMod = m_TblModulations.GetItem();

		bool auMoinsUnSeg = false;
		for(int i = 0; i<DatMod.m_FLTSegments.GetCount();i++)
		{
			if (DatMod.m_FLTSegments.GetSelItem(i)
				&& terr->SupportBelongsToSegment(idAud, DatMod.m_FLTSegments.Item(i).AsJInt32()))
				auMoinsUnSeg = true;
		}

		return (terr->HasSupport(idAud) && auMoinsUnSeg);
	}
	return false;
}

bool CMoteurPresse::CanUseSupport(IElem* pElem, CKeyTerrain key, JATPOffre& offre)//static
{
	if (pElem && pElem->IsTitre())
	{
		CTerrain* terr  =CTerrainManager::GetInstance()->GetTerrainNoRef(key);
		JUnt32 idAud = pElem->AsITitre()->GetIDAudience();
		
		JMTFR03KEYModulation keyMod;
		keyMod.m_IdSource = key.m_IDSource;
		keyMod.m_IdTerrain = key.m_IDTerrain;

		bool auMoinsUnSeg = false;
		for(offre.m_TBLIdSegmentsAudience.MoveFirst();offre.m_TBLIdSegmentsAudience.IsValid(); offre.m_TBLIdSegmentsAudience.MoveNext())
		{
			if( terr->SupportBelongsToSegment(idAud, offre.m_TBLIdSegmentsAudience.GetItem()) )
				auMoinsUnSeg = true;
		}

		return (terr->HasSupport(idAud) && auMoinsUnSeg);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calcul pour un couplage
bool CMoteurPresse::CalcAudienceCouplage(JMTFR03KEYAudience kAudience, IElem* pElem, JFlt64 & VolLecture, JFlt64 & AudienceRef, JFlt64 & AudienceProba, JInt32 & PtEtalonnage, bool bDuree) const
{
	bool bret = false;

	JFlt64 volLectTmp		= 0.0;
	JFlt64 audienceProbaTmp = 0.0;
	AudienceRef				= 0.0;

/////////////////////  On recupere la LDP/LNM du couplage
	kAudience.m_IdTitre = pElem->GetID();
	// on recherche les contacts du Support
	if (kAudience.IsValid())
	{
		m_TblAudiences.MoveTo(kAudience);

		// on teste si on a trouvé les contacts
		if (m_TblAudiences.IsValid())
		{
			// on récupère les contacts
			const JMTFR03DATAudience & DATAudience = m_TblAudiences.GetItem();
			if (DATAudience.m_AudienceRef.AsJFlt64() == 0.0)
				AudienceRef = DATAudience.m_AudienceProba.AsJFlt64();
			else
				AudienceRef = DATAudience.m_AudienceRef.AsJFlt64();
		}
	}
///////////////////////////
			
	// Crée liste des supports couplage
	JList<IElem *> ListSupCoupl;

	pElem->AsCouplage()->GetFils(ListSupCoupl);

	//pour chaque support
	int rang = 1;
	for( rang = 1, ListSupCoupl.MoveFirst() ; ListSupCoupl.IsValid(); ListSupCoupl.MoveNext(), rang++)
	{
		IElem * pIElem = ListSupCoupl.GetItem();

		if(pIElem)
		{
			kAudience.m_IdTitre = pIElem->AsITitre()->GetIDAudience();

			if (kAudience.IsValid())
			{
				// on recherche les contacts du Support
				m_TblAudiences.MoveTo(kAudience);
				// on teste si on a trouvé les contacts
				if (m_TblAudiences.IsValid())
				{
	
					bret = true;
					// on récupère les contacts
					const JMTFR03DATAudience & DATAudience = m_TblAudiences.GetItem();


					// Mise à jour du nombre de jours étalonnage (on prend le max des composants couplage)
					if(	DATAudience.m_PtEtalonnage.AsJInt32() > PtEtalonnage)
						PtEtalonnage = DATAudience.m_PtEtalonnage.AsJInt32();

					audienceProbaTmp += DATAudience.m_AudienceRef.AsJFlt64() * (JFlt64)pIElem->AsTitreCouplage()->GetNbInsert();

					// on initialise le nombre de contacts selon le type de moteur
					if (m_type == PROBA)
						volLectTmp +=  DATAudience.m_AudienceRef.AsJFlt64() * (JFlt64)pIElem->AsTitreCouplage()->GetNbInsert();
					else
					{
						if (bDuree)
							volLectTmp +=  (JFlt64)DATAudience.m_PtEtalonnage.AsJInt32() *  DATAudience.m_VolLecture1Jr.AsJFlt64() * (JFlt64)pIElem->AsTitreCouplage()->GetNbInsert();
						else
							volLectTmp +=  DATAudience.m_VolLecture1Jr.AsJFlt64() * (JFlt64)pIElem->AsTitreCouplage()->GetNbInsert();
					}
					if (DATAudience.m_AudienceRef.AsJFlt64() == 0.0)
					{
						audienceProbaTmp +=  DATAudience.m_AudienceProba.AsJFlt64() * (JFlt64)pIElem->AsTitreCouplage()->GetNbInsert();
						volLectTmp +=  DATAudience.m_AudienceProba.AsJFlt64() * (JFlt64)pIElem->AsTitreCouplage()->GetNbInsert();
					}

				}
			}
		}
	}//for

	VolLecture		= volLectTmp;
	AudienceProba	= audienceProbaTmp;


	return bret;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Calcul pour une famille
bool CMoteurPresse::CalcAudienceFamille(JMTFR03KEYAudience kAudience, IElem* pElem, JFlt64 & VolLecture, JFlt64 & AudienceRef, JFlt64 & AudienceProba, JInt32 & PtEtalonnage, bool bDuree) const
{
	bool bret = false;

	JFlt64 volLectTmp		= 0.0;
	JFlt64 audienceProbaTmp = 0.0;
	AudienceRef				= 0.0;

	/////////////////////  On recupere la LDP/LNM de la famille
	kAudience.m_IdTitre = pElem->GetID();

	// on recherche les contacts du Support
	if (kAudience.IsValid())
	{
		m_TblAudiences.MoveTo(kAudience);

		// on teste si on a trouvé les contacts
		if (m_TblAudiences.IsValid())
		{
			// on récupère les contacts
			const JMTFR03DATAudience & DATAudience = m_TblAudiences.GetItem();
			if (DATAudience.m_AudienceRef.AsJFlt64() == 0.0)
				AudienceRef = DATAudience.m_AudienceProba.AsJFlt64();
			else
				AudienceRef = DATAudience.m_AudienceRef.AsJFlt64();
		}
	}
	///////////////////////////
			
	// Crée liste des supports famille
	JList<IElem *> ListSupFamille;
	pElem->AsLot()->GetFils(ListSupFamille);

	//pour chaque support de la famille
	for(ListSupFamille.MoveFirst() ; ListSupFamille.IsValid(); ListSupFamille.MoveNext())
	{
		// Récup elmt support
		IElem * pIElem = ListSupFamille.GetItem();

		if(pIElem)
		{
			// Récup id titre pour l'audience
			kAudience.m_IdTitre = pIElem->AsITitre()->GetIDAudience();

			if (kAudience.IsValid())
			{
				// on recherche les contacts du Support
				m_TblAudiences.MoveTo(kAudience);

				// on teste si on a trouvé les contacts
				if (m_TblAudiences.IsValid())
				{
	
					bret = true;
					// on récupère les contacts
					const JMTFR03DATAudience & DATAudience = m_TblAudiences.GetItem();

					// Mise à jour du nombre de jours étalonnage (on prend le max des composants couplage)
					if(	DATAudience.m_PtEtalonnage.AsJInt32() > PtEtalonnage)
						PtEtalonnage = DATAudience.m_PtEtalonnage.AsJInt32();

					audienceProbaTmp += DATAudience.m_AudienceRef.AsJFlt64();

					// on initialise le nombre de contacts selon le type de moteur
					if (m_type == PROBA)
						volLectTmp +=  DATAudience.m_AudienceRef.AsJFlt64();
					else
					{
						if (bDuree)
							volLectTmp +=  (JFlt64)DATAudience.m_PtEtalonnage.AsJInt32() *  DATAudience.m_VolLecture1Jr.AsJFlt64();
						else
							volLectTmp +=  DATAudience.m_VolLecture1Jr.AsJFlt64();
					}
					if (DATAudience.m_AudienceRef.AsJFlt64() == 0.0)
					{
						audienceProbaTmp +=  DATAudience.m_AudienceProba.AsJFlt64();
						volLectTmp +=  DATAudience.m_AudienceProba.AsJFlt64();
					}

				}
			}
		}
	}//for

	VolLecture		= volLectTmp;
	AudienceProba	= audienceProbaTmp;


	return bret;
}

bool CMoteurPresse::CalcAudienceMediaWeight(JMTFR03KEYAudience kAudience, IElem* pElem, JFlt64 & VolLecture, JFlt64 & AudienceRef, JFlt64 & AudienceProba, JInt32 & PtEtalonnage, bool bDuree) const
{
	bool bret = false;
	JFlt64 mediaweight = 0.0;

	kAudience.m_IdTitre = pElem->AsITitre()->GetIDAudience();

	if (kAudience.IsValid())
	{

		if(pElem->IsTitreApparente())
			mediaweight = pElem->AsTitreApparente()->GetMediaWeight();

		if(pElem->IsTitreApparie())
			mediaweight = pElem->AsTitreApparie()->GetMediaWeight();


		// on recherche les contacts du Support
		m_TblAudiences.MoveTo(kAudience);

		// on teste si on a trouvé les contacts
		if (m_TblAudiences.IsValid())
		{
			bret = true;
			// on récupère les contacts
			const JMTFR03DATAudience & DATAudience = m_TblAudiences.GetItem();

			// Nombre de jours étallonage
			PtEtalonnage = DATAudience.m_PtEtalonnage.AsJInt32();

			// on initialise le nombre de contacts
			if (m_type == PROBA)
				VolLecture =  DATAudience.m_AudienceRef.AsJFlt64() * mediaweight/100.0;
			else
			{
				if (bDuree)
					VolLecture =  (JFlt64)PtEtalonnage *  DATAudience.m_VolLecture1Jr.AsJFlt64() * mediaweight/100.0;
				else
					VolLecture =  DATAudience.m_VolLecture1Jr.AsJFlt64() * mediaweight/100.0;
			}


			AudienceRef   = DATAudience.m_AudienceRef.AsJFlt64() * mediaweight/100.0;
			AudienceProba = DATAudience.m_AudienceRef.AsJFlt64() * mediaweight/100.0;
			
			if (DATAudience.m_AudienceRef.AsJFlt64() == 0.0)
			{
				VolLecture     =  DATAudience.m_AudienceProba.AsJFlt64() * mediaweight/100.0;
				AudienceRef    = DATAudience.m_AudienceProba.AsJFlt64() * mediaweight/100.0;
				AudienceProba  = DATAudience.m_AudienceProba.AsJFlt64() * mediaweight/100.0;
			}

			bret = true;

		}

	}
	return bret;
}

bool CMoteurPresse::CalcAudienceTitre(JMTFR03KEYAudience kAudience, IElem* pElem, JFlt64 & VolLecture, JFlt64 & AudienceRef, JFlt64 & AudienceProba, JInt32 & PtEtalonnage, bool bDuree) const
{
	CKeyTerrain keyTerrain(kAudience.m_IdSource.AsJInt32(), kAudience.m_IdTerrain.AsJInt32());
	if (! CanUseSupportInt(pElem, keyTerrain))
		return false;

	bool bret = false;
	kAudience.m_IdTitre = pElem->AsITitre()->GetIDAudience();

	// on recherche les contacts du Support
	if (kAudience.IsValid())
	{
		m_TblAudiences.MoveTo(kAudience);

		// on teste si on a trouvé les contacts
		if (m_TblAudiences.IsValid())
		{
			// on récupère les contacts
			const JMTFR03DATAudience & DATAudience = m_TblAudiences.GetItem();

			// on initialise le nombre de contacts
			PtEtalonnage = DATAudience.m_PtEtalonnage.AsJInt32();
			if (m_type == PROBA)
				VolLecture =  DATAudience.m_AudienceRef.AsJFlt64();
			else
			{
				if (bDuree)
					VolLecture =  (JFlt64)PtEtalonnage *  DATAudience.m_VolLecture1Jr.AsJFlt64();
				else
					VolLecture =  DATAudience.m_VolLecture1Jr.AsJFlt64();
			}
			if (DATAudience.m_AudienceRef.AsJFlt64() == 0.0)
			{
				AudienceRef = DATAudience.m_AudienceProba.AsJFlt64();
				VolLecture  =  DATAudience.m_AudienceProba.AsJFlt64();
			}
			else
				AudienceRef = DATAudience.m_AudienceRef.AsJFlt64();

			AudienceProba = AudienceRef;
			bret = true;
		}

	}
	return bret;
}


bool CMoteurPresse::HasFiltre(CKeyTerrain key, JInt32 idFiltre) const
{
	if (m_bInit == false || key.IsValid() == false)
		return 0;

	JMTFR03KEYModulation keyMod;
	keyMod.m_IdSource = key.m_IDSource;
	keyMod.m_IdTerrain = key.m_IDTerrain;

	m_TblModulations.MoveTo(keyMod);
	if(m_TblModulations.IsValid())
		return (m_TblModulations.GetItem().m_FLTFiltres.FindItem(idFiltre) != -1);
	
	return false;
}


bool CMoteurPresse::CanAbaque(CKeyTerrain key) const
{
	JMTFR03KEYTerrain keyT;
	keyT.m_IdSource = key.m_IDSource;
	keyT.m_IdTerrain = key.m_IDTerrain;
	m_TerrainManager->m_TblTerrains.MoveTo(keyT);
	if(m_TerrainManager->m_TblTerrains.IsValid())
	{
		if(m_TerrainManager->m_TblTerrains.GetItem().m_TBLReprises.GetCount() != 0)
			return true;
	}
	return false;
}

bool CMoteurPresse::CanLDPQuot(CKeyTerrain key) const
{
	JMTFR03KEYTerrain keyT;
	keyT.m_IdSource = key.m_IDSource;
	keyT.m_IdTerrain = key.m_IDTerrain;
	m_TerrainManager->m_TblTerrains.MoveTo(keyT);
	if(m_TerrainManager->m_TblTerrains.IsValid())
	{
		int pLDP = m_TerrainManager->m_TblTerrains.GetItem().m_TBLProbabilitesLDP.GetCount();
		int pLNM = m_TerrainManager->m_TblTerrains.GetItem().m_TBLProbabilitesLNM.GetCount();
		int vLDP = m_TerrainManager->m_TblTerrains.GetItem().m_TBLVariablesLDP.GetCount();
		int vLNM = m_TerrainManager->m_TblTerrains.GetItem().m_TBLVariablesLNM.GetCount();
		
		int poids = m_TerrainManager->m_TblTerrains.GetItem().m_TBLPoids.GetCount();
			return true;
	}

	return false;
}

////////////////////////////////////////////////////////////////////////////////////
// Récupération table des terrains du moteur
const JMTFR03TBLTerrains & CMoteurPresse::GetTblTerrains() const
{
	return (this->m_TerrainManager->m_TblTerrains);  
}

// Récupération table des terrains du moteur
JMTFR03TBLTerrains & CMoteurPresse::GetTblTerrains()
{
	return (this->m_TerrainManager->m_TblTerrains);  
}

////////////////////////////////////////////////////////////////////////////////////
// Récupération table des cibles du moteur
const JMTFR03TBLCibles & CMoteurPresse::GetTblCibles() const
{
	return (this->m_TblCibles); 
}

////////////////////////////////////////////////////////////////////////////////////
// Récupération table des modulations du moteur
const JMTFR03TBLModulations	& CMoteurPresse::GetTblModulations() const
{
	return (this->m_TblModulations); 
}

////////////////////////////////////////////////////////////////////////////////////
// Récupération table des options du moteur
const JMTFR03Options & CMoteurPresse::GetOptions() const
{
	return (this->m_Options); 
}

