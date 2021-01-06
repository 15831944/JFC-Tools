////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//											LISTE DES TERRAINS   											  //	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "ATPListePupitre_A2.h"
#include "ItemsConstruitsManager.h"

/////////////////////////////////////////////////////
// Constructeur
CATPListePupitre_A2::CATPListePupitre_A2(void)
{
	// on initialise le document
	m_pATPDocCreuset = 0;
}

/////////////////////////////////////////////////////
// la fonction pour d�finir le document
JVoid CATPListePupitre_A2::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	// on m�morise le document
	m_pATPDocCreuset = pATPDocCreuset;

	// on met � jour le composant
	this->OnUpdate();
}

//////////////////////////////////////////////
// la fonction pour appliquer les mises � jour
JVoid CATPListePupitre_A2::OnUpdate()
{
	// Reset contenu
	this->ResetContent(); 

	// on redessine la fen�tre
	InvalidateRect(NULL, FALSE);
}

/////////////////////////////////////////////////////
// la fonction pour mettre � jour le composant

JVoid CATPListePupitre_A2::Update()
{
	// remplit la liste des terrains
	if (m_pATPDocCreuset)
	{
		// Init Liste
		this->ResetContent(); 

		// R�cup Table
		const JSRCTables  & srcTables	= JSRCTables::GetInstance();

		// R�cup la source audience courante
		JUnt32 SrcID			= m_pATPDocCreuset->m_IdSrcAudienceSel;  

		// Flag au moins 1 terrain s�lectionn�
		bool FlagTerSel = false;

		// Balaye tous les terrains de l'offre
		JUnt32 IndexCur = 0;
        for (int IndexTerrain = 0;
             IndexTerrain < m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.GetCount();
			 IndexTerrain++)
		{
			// On se positionne sur chaque terrain
			m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.MoveTo(IndexTerrain);
			if(m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.IsValid())
			{
				// R�cup le nouveau terrain
				JUnt32 TerID				= m_pATPDocCreuset->m_Offre.m_TBLIdTerrainsAudience.GetItem();
				const JTerrain * pt			= srcTables.m_TBLTerrains.GetItem(SrcID,TerID);

				// Ajoute terrain � la liste
				this->AddString(pt->m_Libelle.AsJCharPtr());

				// R�cup id cible
				this->SetItemData(IndexCur,TerID); 

				// Passe � la cible suivante
				IndexCur++;

				// Test si terrain s�lectionn� ou non
				m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveTo(TerID);
				if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.IsValid())
				{
					// Marque la s�lection
					this->SetCurSel(IndexTerrain);
					FlagTerSel = true;
				}
			}
		}

		this->RedrawWindow(); 
	}
}

/////////////////////////////////////////////////////
// Changement de cible
JBool CATPListePupitre_A2::ChangeSelection()
{
	// R�cup index s�lection
	JUnt32 IndexTerrain = this->GetCurSel();

	// Id Terrain s�lectionn�
	JUnt32 TerID = this->GetItemData(IndexTerrain);

	m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
	if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.IsValid())
	{
		// Aucun changement
		if (m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.GetKey() == TerID) return FALSE;
	}
	else
		return FALSE;

	if (m_pATPDocCreuset)
	{	
		// Avertissement si cible non autoris� sur tous les terrains s�lectionn�es
		JBool AuMoins1TerrainNonAutorise = false; 
		CString TxtAvertissement;

		// Boucle sur les infos seuils terrains
		JSRCTables * pSrcTables = (JSRCTables*)& JSRCTables::GetInstance();

		// R�cup le source
		const JSource * ptSrc		= pSrcTables->m_TBLSources.GetItem(m_pATPDocCreuset->m_Offre.m_IdSourceAudience);

		// Chaines de tous les terrains avc avertissement
		CString TxtAvertissementTerrain = "";

		// Les items construits
		// CItemsConstruits * pItemConst = CItemsConstruits::GetInstance();

		// R�cup cible correspondant � l'indice
		// IItem * pItem = pItemConst->GetIItemByID(m_pATPDocCreuset->m_IdItemSel);
		IItem * pItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(m_pATPDocCreuset->m_IdItemSel);

		// Attention si c'est une cible pond�r�e
		if (!pItem->IsItemPondere())
		{
			if (m_pATPDocCreuset->m_Offre.CibleAutorisee(m_pATPDocCreuset->m_IdItemSel, TerID) == false)  
			{
				// Ce terrain n'est pas autoris� pour cette cible
				AuMoins1TerrainNonAutorise = true;

				// R�cup infos terrain
				const JTerrain * pTerrain = pSrcTables->m_TBLTerrains.GetItem(m_pATPDocCreuset->m_Offre.m_IdSourceAudience, TerID);
				CString TxtTerrain;
				TxtTerrain.Format("%s%s %s\n","  - ",ptSrc->m_Libelle.AsJCharPtr(), pTerrain->m_Libelle.AsJCharPtr());	

				// Ajoute au message le terrain
				TxtAvertissementTerrain += TxtTerrain;
			}


			// Affiche message si cible non autoris�e 
			if (AuMoins1TerrainNonAutorise)
			{
				

				// Si cible trouv�, on retourne son libell�
				if(pItem)
				{
					// Recuperer libell� cible en cours
					// TxtAvertissement.Format("%s %s %s\n%s","Cible", pItem->GetLabel().AsJCharPtr(), "invalide sur terrains :", TxtAvertissementTerrain);
					TxtAvertissement.Format("%s %s %s\n\n%s","Le nombre de cas de la cible", pItem->GetLabel().AsJCharPtr(), "est trop faible sur le(s) terrain(s) ci-dessous :", TxtAvertissementTerrain);
					MessageBox(TxtAvertissement,"Avertissement seuil", MB_ICONEXCLAMATION);
				}
			}
		}
		else
		{
			// Test validit� pour l'item pond�r�
			// Map avec 1 seul terrain
			JMap <JUnt32, JUnt32> MapTerrain;
			MapTerrain.Add(TerID) = 0;

			// formate libell� cible pond�r� selon que la cible pond�r�e peut �tre utiliser ou pas
			if (!m_pATPDocCreuset->m_MoteurPresse.CiblePondAutorise(pItem->GetID(), m_pATPDocCreuset->m_Offre.m_IdSourceAudience, &MapTerrain))
			{
				// R�cup infos terrain
				const JTerrain * pTerrain = pSrcTables->m_TBLTerrains.GetItem(m_pATPDocCreuset->m_Offre.m_IdSourceAudience, TerID);
				
				TxtAvertissement.Format("%s %s %s\n%s","Cible", pItem->GetLabel().AsJCharPtr(), "invalide sur certains terrains", pTerrain->m_Libelle.AsJCharPtr());
				MessageBox(TxtAvertissement,"Avertissement seuil", MB_ICONEXCLAMATION);
			}
		}
	}

	// Nouvel id terrain, ajout au map s�lection
	m_pATPDocCreuset->SetModifiedFlag();
	m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.Reset();
	m_pATPDocCreuset->m_MapIdTerrainsAudienceSel.Add(TerID) = TerID;

	return TRUE;
}

/////////////////////////////////////////////////////
// le destructeur
CATPListePupitre_A2::~CATPListePupitre_A2()
{
	// on ne fait rien
}
