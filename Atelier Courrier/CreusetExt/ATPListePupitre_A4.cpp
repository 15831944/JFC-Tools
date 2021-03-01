////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//											LISTE DES CIBLES   											      //	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ATPListePupitre_A4.h"
#include "ItemsConstruitsManager.h"

/////////////////////////////////////////////////////
// Constructeur
CATPListePupitre_A4::CATPListePupitre_A4(void)
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

/////////////////////////////////////////////////////
// la fonction pour définir le document
JVoid CATPListePupitre_A4::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on mémorise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met à jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises à jour
JVoid CATPListePupitre_A4::OnUpdate()
{
	// Reset contenu
	this->ResetContent(); 

	// on redessine la fenêtre
	InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////////////////////
// la fonction pour mettre à jour le composant
JVoid CATPListePupitre_A4::Update()
{
	// remplit la liste des cibles
	if (m_pATPDocCreuset)
	{
		// Init Liste des cibles utilisables
		this->ResetContent(); 

		// Selon mode cible (cibles de l'offre ou cibles pondérées)
		if (!m_pATPDocCreuset->m_CiblesPondsActives)
		{
			// Boucle sur toutes les cibles sélectionnées de l'offre
			int IndexCur = 0;
			for (m_pATPDocCreuset->m_MAPpItems.MoveFirst();
				m_pATPDocCreuset->m_MAPpItems.IsValid();
				m_pATPDocCreuset->m_MAPpItems.MoveNext())
			{
				// Libellé de la cible
				CString TxtCible;

				// Avertissement si cible non autorisé sur tous les terrains sélectionnées
				CString TxtAvertissement;

				// Ident de la cible
				JUnt32 IdCible = m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID(); 

				// formate libellé cible 
				if (!AuMoins1TerrainNonAutorise(IdCible, TxtAvertissement))
				{
					// La cible est valide sur tous les terrains sélectionnés
					TxtCible = m_pATPDocCreuset->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();
				}
				else
				{
					// Indique par (*) la non validité de la cible sur certains terrains
					TxtCible.Format("%s - %s","(*)", m_pATPDocCreuset->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr());
				}

				// Insére libellé cible
				this->AddString(TxtCible);

				// Récup id cible
				this->SetItemData(IndexCur,m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID()); 

				// Passe à la cible suivante
				IndexCur++;
			}

			// Sélection de la cible courante
			if(m_pATPDocCreuset->m_IdItemSel)
			{
				int IndexSel = 0;
				for (m_pATPDocCreuset->m_MAPpItems.MoveFirst();
					m_pATPDocCreuset->m_MAPpItems.IsValid();
					m_pATPDocCreuset->m_MAPpItems.MoveNext())
				{
					// Si c'est la cible sélectonnée en cours
					JUnt32 IdItemSel = m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID();
					if	(m_pATPDocCreuset->m_IdItemSel == m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID())
					{
						// Marque la cible sélectionnée
						this->SetCurSel(IndexSel);

						// On sort - on n'est en mono-sélection
						break;
					}

					// Passe à la suivante
					IndexSel++;
				}
			}
			else if (m_pATPDocCreuset->m_MAPpItems.GetCount() > 0 && this->GetCount() > 0) 
			{				
				// on sélectionne la 1ere
				m_pATPDocCreuset->m_MAPpItems.MoveFirst(); 
				m_pATPDocCreuset->m_IdItemSel = m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID();

				// Indique au moteur
				m_pATPDocCreuset->m_MoteurPresse.SetItemCourant(m_pATPDocCreuset->m_IdItemSel);

				// Marque la cible sélectionnée
				this->SetCurSel(0);
			}
		}

		////////////////////// Chargement des cibles pondérées utilisables sur les terrains actifs ////////////////////////
		else
		{
			// Liste des terrains en cours
			JList<CTerrain*> LstTerrains;

			// Init liste des terrains
			LstTerrains.Reset(); 

			// Récupère les terrains
			for(m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.MoveFirst(); 
				m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.IsValid(); 
				m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.MoveNext())
			{
				// Ajoute le terrain
				CKeyTerrain KeyTerrain(m_pATPDocCreuset->m_Offre.m_IdSourceAudience, m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.GetItem());
				LstTerrains.AddTail() = CTerrainManager::GetInstance()->GetTerrain(KeyTerrain);
			}

			// Affichage des cibles pondérées utilisables
			// CItemsConstruits * pItemsConstruits = CItemsConstruits::GetInstance();

			// Chargement des items existants selon les terrains
			JList<IItem const *> vItems;
			if (LstTerrains.GetCount())
			{
				// Récupère les items indépendants (sans les cibles pondérées)
				// pItemsConstruits->GetItemsPonderes(vItems, LstTerrains);
				CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemsPonderes(vItems, LstTerrains);

				// Boucle sur tous les items
				int IndexCur = 0;
				for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
				{
					// Récupère l'item pondéré
					IItem * pItemPond = (IItem*)vItems.GetItem();

					// Ident de l'item pondéré
					JInt32 IdItemPond = pItemPond->GetID();

					// Libellé de la cible
					CString TxtCible;

					// Avertissement si cible non autorisé sur tous les terrains sélectionnées
					CString TxtAvertissement;

					// formate libellé cible pondéré selon que la cible pondérée peut être utiliser ou pas
					if (m_pATPDocCreuset->m_MoteurPresse.CiblePondAutorise(IdItemPond, m_pATPDocCreuset->m_Offre.m_IdSourceAudience, m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()))
					{
						// La cible est valide sur tous les terrains sélectionnés
						TxtCible = pItemPond->GetLabel().AsJCharPtr();
					}
					else
					{
						// Indique par (*) la non validité de la cible sur tous les terrains
						TxtCible.Format("%s - %s","(*)", pItemPond->GetLabel().AsJCharPtr());
					}

					// Insére libellé cible pondérée
					this->AddString(TxtCible);
					
					// Insére libellé cible pondérée
					// this->AddString(pItemPond->GetLabel().AsJCharPtr());

					// Récup id cible pondérée
					this->SetItemData(IndexCur, IdItemPond); 

					// Incrémente pour l'item pondéré suivant
					IndexCur++;
				}

				// Sélection de la cible courante
				JUnt32 Nb = m_pATPDocCreuset->m_MAPpItemsPonderes.GetCount();
				if(m_pATPDocCreuset->m_IdItemSel)
				{
					int IndexSel = 0;
					for (m_pATPDocCreuset->m_MAPpItemsPonderes.MoveFirst();
						 m_pATPDocCreuset->m_MAPpItemsPonderes.IsValid();
						 m_pATPDocCreuset->m_MAPpItemsPonderes.MoveNext())
					{
						// Si c'est la cible pondérée sélectonnée en cours
						JUnt32 IdItemSel = m_pATPDocCreuset->m_MAPpItemsPonderes.GetItem()->GetID();
						if	(m_pATPDocCreuset->m_IdItemSel == m_pATPDocCreuset->m_MAPpItemsPonderes.GetItem()->GetID())
						{
							// Marque la cible sélectionnée
							this->SetCurSel(IndexSel);

							// On sort - on n'est en mono-sélection
							break;
						}

						// Passe à la suivante
						IndexSel++;
					}
				}
				else if (m_pATPDocCreuset->m_MAPpItemsPonderes.GetCount() > 0 && this->GetCount() > 0) 
				{				
					// on sélectionne la 1ere
					m_pATPDocCreuset->m_MAPpItemsPonderes.MoveFirst(); 
					m_pATPDocCreuset->m_IdItemSel = m_pATPDocCreuset->m_MAPpItemsPonderes.GetItem()->GetID();

					// Indique au moteur
					m_pATPDocCreuset->m_MoteurPresse.SetItemCourant(m_pATPDocCreuset->m_IdItemSel);

					// Marque la cible sélectionnée
					this->SetCurSel(0);
				}
			}
		}
	}

	this->RedrawWindow(); 
}

/////////////////////////////////////////////////////
// Changement de terrain
JBool CATPListePupitre_A4::ChangeSelection()
{
	// Récup index sélection cible
	JUnt32 IndexCible = this->GetCurSel();

	// Id Cible sélectionné
	JUnt32 IdCible = this->GetItemData(IndexCible);

	// Aucun changement
	if (m_pATPDocCreuset->m_IdItemSel == IdCible) return FALSE;


	// Avertissement si cible non autorisé sur tous les terrains sélectionnées
	CString TxtAvertissementTerrain = "";
	CString TxtAvertissement = "";

	// Les items construits
	// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

	// Récup cible correspondant à l'indice
	// IItem * pItem = pItemConst->GetIItemByID(IdCible);
	IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(IdCible);
	
	// Test si ce n'est pas un item pondéré
	if (!pItem->IsItemPondere()) 
	{
		// Affiche message si cible non autorisée 
		if (AuMoins1TerrainNonAutorise(IdCible, TxtAvertissementTerrain))
		{
			CString TxtCible;
			
			// this->GetText(this->GetCurSel(), TxtCible);

			// Récup cible correspondant à l'indice
			// IItem * pItem = CItemsConstruits::GetInstance()->GetItemConstruit(IdCible);
			IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(IdCible);

			// Affiche le message de prévention
			// TxtAvertissement.Format("%s %s %s\n%s","Cible", TxtCible, "invalide sur terrains", TxtAvertissementTerrain);
			TxtAvertissement.Format("%s %s %s\n\n%s","Le nombre de cas de la cible", pItem->GetLabel().AsJCharPtr(), "est trop faible sur le(s) terrain(s) ci-dessous : ", TxtAvertissementTerrain);
			MessageBox(TxtAvertissement,"Avertissement seuil", MB_ICONEXCLAMATION);
		}
	}
	else
	{
		// A REMETTRE CIBLE POND
		// Test validité pour l'item pondéré
		// formate libellé cible pondéré selon que la cible pondérée peut être utiliser ou pas
		if (!m_pATPDocCreuset->m_MoteurPresse.CiblePondAutorise(pItem->GetID(), m_pATPDocCreuset->m_Offre.m_IdSourceAudience, m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()))
		{
			// Boucle sur les infos seuils terrains
			JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

			// Récup le source
			const JSource * ptSrc		= pSrcTables->m_TBLSources.GetItem(m_pATPDocCreuset->m_Offre.m_IdSourceAudience);

			// Ident terrain sélectionné
			m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()->MoveFirst(); 
			if (m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()->IsValid())
			{
				// Id Terrain sélectionné
				JUnt32 TerID = m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()->GetKey();

				// Récup infos terrain
				const JTerrain * pTerrain = pSrcTables->m_TBLTerrains.GetItem(m_pATPDocCreuset->m_Offre.m_IdSourceAudience, TerID);

				TxtAvertissement.Format("%s %s %s\n%s","Cible", pItem->GetLabel().AsJCharPtr(), "invalide sur terrain", pTerrain->m_Libelle.AsJCharPtr());
				MessageBox(TxtAvertissement,"Avertissement seuil", MB_ICONEXCLAMATION);
			}
		}
		//*
	}

	// Nouvel id cible, ajout au map sélection
	m_pATPDocCreuset->SetModifiedFlag();
	m_pATPDocCreuset->m_IdItemSel = IdCible;

	// Indique au moteur
	m_pATPDocCreuset->m_MoteurPresse.SetItemCourant(m_pATPDocCreuset->m_IdItemSel);

	// Puis on remet à jour la table des pondérations via la cible sélectionnées (en se positionnant sur terrain en cours)
	m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
	JUnt32 IdTerrainSel = m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey();
	m_pATPDocCreuset->m_MoteurPresse.SetPonderations(m_pATPDocCreuset->m_IdItemSel, m_pATPDocCreuset->m_Offre.m_IdSourceAudience, IdTerrainSel);

	if (m_pATPDocCreuset->m_AFFVentilation.IsVisible())
	{
		m_pATPDocCreuset->UpdateAllViews(UPDATE_OPTIONSMOTEUR);
	}

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////
// Requête pour savoir si cible valide sur tous les terrains sélectionnés
JBool CATPListePupitre_A4::AuMoins1TerrainNonAutorise(JUnt32 IdCible, CString & TxtAvertissement)
{
	// Initialise la balise test
	JBool AuMoins1TerrainNonAutorise = false;

	// Boucle sur les infos seuils terrains
	JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

	// Récup le source
	const JSource * ptSrc		= pSrcTables->m_TBLSources.GetItem(m_pATPDocCreuset->m_Offre.m_IdSourceAudience);

	// Chaines de tous les terrains avc avertissement
	TxtAvertissement = "";

	// Boucle sur les terrains sélectionnés
	for (m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()->MoveFirst(); 
		 m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()->IsValid();
		 m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()->MoveNext())
	{
		// Récup ident terrain
		const JUnt32 IdTerrain = m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()->GetKey();

		if (!m_pATPDocCreuset->m_CiblesPondsActives)
		{
			// Test sur les cibles de l'offre
			if (m_pATPDocCreuset->m_Offre.CibleAutorisee(IdCible, IdTerrain) == false)  
			{
				// Ce terrain n'est pas autorisé pour cette cible
				AuMoins1TerrainNonAutorise = true;

				// Récup infos terrain
				const JTerrain * pTerrain = pSrcTables->m_TBLTerrains.GetItem(m_pATPDocCreuset->m_Offre.m_IdSourceAudience, IdTerrain);
				CString TxtTerrain;
				TxtTerrain.Format("%s%s %s\n","  - ",ptSrc->m_Libelle.AsJCharPtr(), pTerrain->m_Libelle.AsJCharPtr());	

				// Ajoute au message le terrain
				TxtAvertissement += TxtTerrain;
			}
		}
	}

	// Renvoi si au moins un terrain non autorisé
	return AuMoins1TerrainNonAutorise;
}

/////////////////////////////////////////////////////
// le destructeur
CATPListePupitre_A4::~CATPListePupitre_A4()
{
	// on ne fait rien
}


