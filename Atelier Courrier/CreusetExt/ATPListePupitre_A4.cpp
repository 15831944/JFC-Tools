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
// la fonction pour d�finir le document
JVoid CATPListePupitre_A4::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour
JVoid CATPListePupitre_A4::OnUpdate()
{
	// Reset contenu
	this->ResetContent(); 

	// on redessine la fen�tre
	InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////////////////////
// la fonction pour mettre � jour le composant
JVoid CATPListePupitre_A4::Update()
{
	// remplit la liste des cibles
	if (m_pATPDocCreuset)
	{
		// Init Liste des cibles utilisables
		this->ResetContent(); 

		// Selon mode cible (cibles de l'offre ou cibles pond�r�es)
		if (!m_pATPDocCreuset->m_CiblesPondsActives)
		{
			// Boucle sur toutes les cibles s�lectionn�es de l'offre
			int IndexCur = 0;
			for (m_pATPDocCreuset->m_MAPpItems.MoveFirst();
				m_pATPDocCreuset->m_MAPpItems.IsValid();
				m_pATPDocCreuset->m_MAPpItems.MoveNext())
			{
				// Libell� de la cible
				CString TxtCible;

				// Avertissement si cible non autoris� sur tous les terrains s�lectionn�es
				CString TxtAvertissement;

				// Ident de la cible
				JUnt32 IdCible = m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID(); 

				// formate libell� cible 
				if (!AuMoins1TerrainNonAutorise(IdCible, TxtAvertissement))
				{
					// La cible est valide sur tous les terrains s�lectionn�s
					TxtCible = m_pATPDocCreuset->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr();
				}
				else
				{
					// Indique par (*) la non validit� de la cible sur certains terrains
					TxtCible.Format("%s - %s","(*)", m_pATPDocCreuset->m_MAPpItems.GetItem()->GetLabel().AsJCharPtr());
				}

				// Ins�re libell� cible
				this->AddString(TxtCible);

				// R�cup id cible
				this->SetItemData(IndexCur,m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID()); 

				// Passe � la cible suivante
				IndexCur++;
			}

			// S�lection de la cible courante
			if(m_pATPDocCreuset->m_IdItemSel)
			{
				int IndexSel = 0;
				for (m_pATPDocCreuset->m_MAPpItems.MoveFirst();
					m_pATPDocCreuset->m_MAPpItems.IsValid();
					m_pATPDocCreuset->m_MAPpItems.MoveNext())
				{
					// Si c'est la cible s�lectonn�e en cours
					JUnt32 IdItemSel = m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID();
					if	(m_pATPDocCreuset->m_IdItemSel == m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID())
					{
						// Marque la cible s�lectionn�e
						this->SetCurSel(IndexSel);

						// On sort - on n'est en mono-s�lection
						break;
					}

					// Passe � la suivante
					IndexSel++;
				}
			}
			else if (m_pATPDocCreuset->m_MAPpItems.GetCount() > 0 && this->GetCount() > 0) 
			{				
				// on s�lectionne la 1ere
				m_pATPDocCreuset->m_MAPpItems.MoveFirst(); 
				m_pATPDocCreuset->m_IdItemSel = m_pATPDocCreuset->m_MAPpItems.GetItem()->GetID();

				// Indique au moteur
				m_pATPDocCreuset->m_MoteurPresse.SetItemCourant(m_pATPDocCreuset->m_IdItemSel);

				// Marque la cible s�lectionn�e
				this->SetCurSel(0);
			}
		}

		////////////////////// Chargement des cibles pond�r�es utilisables sur les terrains actifs ////////////////////////
		else
		{
			// Liste des terrains en cours
			JList<CTerrain*> LstTerrains;

			// Init liste des terrains
			LstTerrains.Reset(); 

			// R�cup�re les terrains
			for(m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.MoveFirst(); 
				m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.IsValid(); 
				m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.MoveNext())
			{
				// Ajoute le terrain
				CKeyTerrain KeyTerrain(m_pATPDocCreuset->m_Offre.m_IdSourceAudience, m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.GetItem());
				LstTerrains.AddTail() = CTerrainManager::GetInstance()->GetTerrain(KeyTerrain);
			}

			// Affichage des cibles pond�r�es utilisables
			// CItemsConstruits * pItemsConstruits = CItemsConstruits::GetInstance();

			// Chargement des items existants selon les terrains
			JList<IItem const *> vItems;
			if (LstTerrains.GetCount())
			{
				// R�cup�re les items ind�pendants (sans les cibles pond�r�es)
				// pItemsConstruits->GetItemsPonderes(vItems, LstTerrains);
				CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemsPonderes(vItems, LstTerrains);

				// Boucle sur tous les items
				int IndexCur = 0;
				for (vItems.MoveFirst(); vItems.IsValid(); vItems.MoveNext())
				{
					// R�cup�re l'item pond�r�
					IItem * pItemPond = (IItem*)vItems.GetItem();

					// Ident de l'item pond�r�
					JInt32 IdItemPond = pItemPond->GetID();

					// Libell� de la cible
					CString TxtCible;

					// Avertissement si cible non autoris� sur tous les terrains s�lectionn�es
					CString TxtAvertissement;

					// formate libell� cible pond�r� selon que la cible pond�r�e peut �tre utiliser ou pas
					if (m_pATPDocCreuset->m_MoteurPresse.CiblePondAutorise(IdItemPond, m_pATPDocCreuset->m_Offre.m_IdSourceAudience, m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()))
					{
						// La cible est valide sur tous les terrains s�lectionn�s
						TxtCible = pItemPond->GetLabel().AsJCharPtr();
					}
					else
					{
						// Indique par (*) la non validit� de la cible sur tous les terrains
						TxtCible.Format("%s - %s","(*)", pItemPond->GetLabel().AsJCharPtr());
					}

					// Ins�re libell� cible pond�r�e
					this->AddString(TxtCible);
					
					// Ins�re libell� cible pond�r�e
					// this->AddString(pItemPond->GetLabel().AsJCharPtr());

					// R�cup id cible pond�r�e
					this->SetItemData(IndexCur, IdItemPond); 

					// Incr�mente pour l'item pond�r� suivant
					IndexCur++;
				}

				// S�lection de la cible courante
				JUnt32 Nb = m_pATPDocCreuset->m_MAPpItemsPonderes.GetCount();
				if(m_pATPDocCreuset->m_IdItemSel)
				{
					int IndexSel = 0;
					for (m_pATPDocCreuset->m_MAPpItemsPonderes.MoveFirst();
						 m_pATPDocCreuset->m_MAPpItemsPonderes.IsValid();
						 m_pATPDocCreuset->m_MAPpItemsPonderes.MoveNext())
					{
						// Si c'est la cible pond�r�e s�lectonn�e en cours
						JUnt32 IdItemSel = m_pATPDocCreuset->m_MAPpItemsPonderes.GetItem()->GetID();
						if	(m_pATPDocCreuset->m_IdItemSel == m_pATPDocCreuset->m_MAPpItemsPonderes.GetItem()->GetID())
						{
							// Marque la cible s�lectionn�e
							this->SetCurSel(IndexSel);

							// On sort - on n'est en mono-s�lection
							break;
						}

						// Passe � la suivante
						IndexSel++;
					}
				}
				else if (m_pATPDocCreuset->m_MAPpItemsPonderes.GetCount() > 0 && this->GetCount() > 0) 
				{				
					// on s�lectionne la 1ere
					m_pATPDocCreuset->m_MAPpItemsPonderes.MoveFirst(); 
					m_pATPDocCreuset->m_IdItemSel = m_pATPDocCreuset->m_MAPpItemsPonderes.GetItem()->GetID();

					// Indique au moteur
					m_pATPDocCreuset->m_MoteurPresse.SetItemCourant(m_pATPDocCreuset->m_IdItemSel);

					// Marque la cible s�lectionn�e
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
	// R�cup index s�lection cible
	JUnt32 IndexCible = this->GetCurSel();

	// Id Cible s�lectionn�
	JUnt32 IdCible = this->GetItemData(IndexCible);

	// Aucun changement
	if (m_pATPDocCreuset->m_IdItemSel == IdCible) return FALSE;


	// Avertissement si cible non autoris� sur tous les terrains s�lectionn�es
	CString TxtAvertissementTerrain = "";
	CString TxtAvertissement = "";

	// Les items construits
	// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

	// R�cup cible correspondant � l'indice
	// IItem * pItem = pItemConst->GetIItemByID(IdCible);
	IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(IdCible);
	
	// Test si ce n'est pas un item pond�r�
	if (!pItem->IsItemPondere()) 
	{
		// Affiche message si cible non autoris�e 
		if (AuMoins1TerrainNonAutorise(IdCible, TxtAvertissementTerrain))
		{
			CString TxtCible;
			
			// this->GetText(this->GetCurSel(), TxtCible);

			// R�cup cible correspondant � l'indice
			// IItem * pItem = CItemsConstruits::GetInstance()->GetItemConstruit(IdCible);
			IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetItemConstruit(IdCible);

			// Affiche le message de pr�vention
			// TxtAvertissement.Format("%s %s %s\n%s","Cible", TxtCible, "invalide sur terrains", TxtAvertissementTerrain);
			TxtAvertissement.Format("%s %s %s\n\n%s","Le nombre de cas de la cible", pItem->GetLabel().AsJCharPtr(), "est trop faible sur le(s) terrain(s) ci-dessous : ", TxtAvertissementTerrain);
			MessageBox(TxtAvertissement,"Avertissement seuil", MB_ICONEXCLAMATION);
		}
	}
	else
	{
		// A REMETTRE CIBLE POND
		// Test validit� pour l'item pond�r�
		// formate libell� cible pond�r� selon que la cible pond�r�e peut �tre utiliser ou pas
		if (!m_pATPDocCreuset->m_MoteurPresse.CiblePondAutorise(pItem->GetID(), m_pATPDocCreuset->m_Offre.m_IdSourceAudience, m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()))
		{
			// Boucle sur les infos seuils terrains
			JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

			// R�cup le source
			const JSource * ptSrc		= pSrcTables->m_TBLSources.GetItem(m_pATPDocCreuset->m_Offre.m_IdSourceAudience);

			// Ident terrain s�lectionn�
			m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()->MoveFirst(); 
			if (m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()->IsValid())
			{
				// Id Terrain s�lectionn�
				JUnt32 TerID = m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()->GetKey();

				// R�cup infos terrain
				const JTerrain * pTerrain = pSrcTables->m_TBLTerrains.GetItem(m_pATPDocCreuset->m_Offre.m_IdSourceAudience, TerID);

				TxtAvertissement.Format("%s %s %s\n%s","Cible", pItem->GetLabel().AsJCharPtr(), "invalide sur terrain", pTerrain->m_Libelle.AsJCharPtr());
				MessageBox(TxtAvertissement,"Avertissement seuil", MB_ICONEXCLAMATION);
			}
		}
		//*
	}

	// Nouvel id cible, ajout au map s�lection
	m_pATPDocCreuset->SetModifiedFlag();
	m_pATPDocCreuset->m_IdItemSel = IdCible;

	// Indique au moteur
	m_pATPDocCreuset->m_MoteurPresse.SetItemCourant(m_pATPDocCreuset->m_IdItemSel);

	// Puis on remet � jour la table des pond�rations via la cible s�lectionn�es (en se positionnant sur terrain en cours)
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
// Requ�te pour savoir si cible valide sur tous les terrains s�lectionn�s
JBool CATPListePupitre_A4::AuMoins1TerrainNonAutorise(JUnt32 IdCible, CString & TxtAvertissement)
{
	// Initialise la balise test
	JBool AuMoins1TerrainNonAutorise = false;

	// Boucle sur les infos seuils terrains
	JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

	// R�cup le source
	const JSource * ptSrc		= pSrcTables->m_TBLSources.GetItem(m_pATPDocCreuset->m_Offre.m_IdSourceAudience);

	// Chaines de tous les terrains avc avertissement
	TxtAvertissement = "";

	// Boucle sur les terrains s�lectionn�s
	for (m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()->MoveFirst(); 
		 m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()->IsValid();
		 m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()->MoveNext())
	{
		// R�cup ident terrain
		const JUnt32 IdTerrain = m_pATPDocCreuset->GetMapIdTerrainsAudienceSel()->GetKey();

		if (!m_pATPDocCreuset->m_CiblesPondsActives)
		{
			// Test sur les cibles de l'offre
			if (m_pATPDocCreuset->m_Offre.CibleAutorisee(IdCible, IdTerrain) == false)  
			{
				// Ce terrain n'est pas autoris� pour cette cible
				AuMoins1TerrainNonAutorise = true;

				// R�cup infos terrain
				const JTerrain * pTerrain = pSrcTables->m_TBLTerrains.GetItem(m_pATPDocCreuset->m_Offre.m_IdSourceAudience, IdTerrain);
				CString TxtTerrain;
				TxtTerrain.Format("%s%s %s\n","  - ",ptSrc->m_Libelle.AsJCharPtr(), pTerrain->m_Libelle.AsJCharPtr());	

				// Ajoute au message le terrain
				TxtAvertissement += TxtTerrain;
			}
		}
	}

	// Renvoi si au moins un terrain non autoris�
	return AuMoins1TerrainNonAutorise;
}

/////////////////////////////////////////////////////
// le destructeur
CATPListePupitre_A4::~CATPListePupitre_A4()
{
	// on ne fait rien
}


