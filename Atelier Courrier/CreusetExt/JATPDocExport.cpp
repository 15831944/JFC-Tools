#include "StdAfx.h"

#include "JATPDocExport.h"
#include "ATPResult_Grandeurs.h"
#include "ATPIVisitor.h"
#include "Courbes.h"
#include "IDLimits.h"
#include "ItemsConstruitsManager.h"

// le constructeur
JATPDocExport::JATPDocExport(JATPDocCreuset & ATPDocCreuset) : m_ATPDocCreuset(ATPDocCreuset)
{
	// Init Infos Export
	m_LabelEtude		= "";		// Nom de l'�tude
	m_LabelGroupe		= "";		// Nom groupe associ�
	m_CodeGroupe		= "";		// Code groupe
	m_LabelAnnonceur	= "";		// Nom annonceur associ�
	m_CodeAnnonceur		= "";		// Code annonceur
	m_LabelProduit		= "";		// Nom produit associ�
	m_CodeProduit		= "";		// Code produit
	m_DateDebPeriode.Reset();		// Debut P�riode globale
	m_DateFinPeriode.Reset();		// Fin P�riode globale
	m_DateDebPeriodeAction.Reset(); // D�but P�riode d'action
	m_DateFinPeriodeAction.Reset(); // Fin P�riode d'action
	m_LabelCible			= "";	// Nom de la cible courante
	m_EffectifCible			= 0;	// Effectif cible courante
	m_EchantillonCible		= 0;	// Echantillon cible courante
	m_GRPActifValid         = true; // La validit� du GRP actif
	m_LabelRefAffinite		= "";	// Nom r�f�rence affinit�
	m_LabelFiltreAudience	= "";	// Nom Filtre d'audience
	m_LabelSource			= "";   // Nom Source
	m_LabelTerrain			= "";	// Nom Terrain
	m_LabelSegment			= "";	// Nom Segment
	m_LabelTypePoids		= "";	// Nom type de poids
	m_LabelMethodeCalcCouv	= "";   // Nom m�thode de calculs couverture
	m_ListFormatActif.Reset();		// Liste des formats actifs
	m_TInfoHypos.Reset();			// Les infos hypoth�ses
	m_InfoEchelle.Reset();			// les infos �chelles

	m_srcTables	= (JSRCTables*)& JSRCTables::GetInstance();
	m_srcPresse = (JSRCPresse*)& JSRCPresse::GetInstance();

	// R�cup�ration des infos g�n�rales
	JBool ret = RecupInfoGene();
	ASSERT(ret == true);

	// Init String Export
	m_StrExport = "";
}

// le destructeur
JATPDocExport::~JATPDocExport(void)
{
}

// R�cup�ration des informations g�n�rales
JBool JATPDocExport::RecupInfoGene()
{
	if (m_ATPDocCreuset.IsValid())
	{
		// Nom de l'�tude
		GetInfoEtude();

		// Nom et id groupe associ�
		GetInfoGroupe();

		// Nom et id annonceur associ�
		GetInfoAnnonceur();
		
		// Nom produit associ�
		GetInfoProduit();

		// P�riode globale et p�riode d'action
		GetInfoPeriodes();

		// Nom de la cible
		GetInfoCible();

		// Nom r�f�rence affinit�
		GetInfoRefAffinite();

		// Nom Filtre d'audience
		GetInfoFiltreAudience();

		// Nom Source et Terrain
		GetInfoSourceTerrain();

		// Nom Segment
		GetInfoSegment();

		// Nom type de poids
		GetInfoTypePoids();

		// Nom m�thode de calculs couverture
		GetInfoMethodeCalcul();

		// R�cup les formats actifs
		GetFormatActif();

		// R�cup�ration infos hypoth�ses
		GetInfoHypotheses();

		// Recup�ration des infos Echelle
		GetInfoEchelle();

		return true;
	}
	else
		// Probl�me lecture infos doc
		return false;
}

// R�cup�ration Label Etude
JVoid JATPDocExport::GetInfoEtude()
{
	// R�cup�re le libell� de l'�tude
	m_LabelEtude = m_ATPDocCreuset.m_KEYEtude.GetLabel();
}

// Nom et id groupe associ�
JVoid JATPDocExport::GetInfoGroupe()
{
	// Nom groupe associ�
	m_LabelGroupe = "Nom Groupe";

	// Code groupe
	m_CodeGroupe  = "X";
}

// Nom et id annonceur associ�
JVoid JATPDocExport::GetInfoAnnonceur()
{
	// Nom annonceur associ�
	m_LabelAnnonceur = "Nom Annonceur";

	// Code annonceur
	m_CodeAnnonceur  = "X";
}

// Nom produit associ�
JVoid JATPDocExport::GetInfoProduit()
{
	// Nom produit associ�
	m_LabelProduit = "Nom Produit";

	// Code produit
	m_CodeProduit  = "X";
}

// P�riode globale et p�riode d'action
JVoid JATPDocExport::GetInfoPeriodes()
{
	// on r�cup�re les dates de l'�tude
	m_DateDebPeriode		= m_ATPDocCreuset.m_KEYPeriodeAction.GetDateD();
	m_DateFinPeriode		= m_ATPDocCreuset.m_KEYPeriodeAction.GetDateF();

	// P�riode d'action
	m_DateDebPeriodeAction  = m_ATPDocCreuset.m_KEYPeriodeActionTablier.GetDateD();
	m_DateFinPeriodeAction  = m_ATPDocCreuset.m_KEYPeriodeActionTablier.GetDateF();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Info cible normale ou info cible pond�r�e
JVoid JATPDocExport::GetInfoCible()
{
	// Cas d'une cible normale s�lectionn�e
	if (!m_ATPDocCreuset.m_CiblesPondsActives)
	{
		// on recup�re libell� de la cible courante
		m_ATPDocCreuset.m_MAPpItems.MoveTo(m_ATPDocCreuset.m_IdItemSel);
		if(m_ATPDocCreuset.m_MAPpItems.IsValid())
		{
			// R�cup id cible
			JUnt32 IdCible = m_ATPDocCreuset.m_MAPpItems.GetItem()->GetID();
			if(IdCible)
			{
				// R�cup Table des cibles
				CItemsConstruitsBase * pItemConst = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

				// R�cup cible correspondant � l'indice
				IItem * pItem = pItemConst->GetItemConstruit(IdCible);

				// Si cible trouv�, on retourne son libell�
				if(pItem)
				{
					// Recuperer libell� cible en cours
					m_LabelCible =  pItem->GetLabel().AsJCharPtr();

					// on cr�e la cl� des effectifs
					CKeyMoteur keyMoteur;
					// Positionnement sur s�lection
					m_ATPDocCreuset.m_MapIdTerrainsAudienceSel.MoveFirst();
					if (m_ATPDocCreuset.m_MapIdTerrainsAudienceSel.IsValid())
					{
						// R�cup�re id terrain et id cible 
						keyMoteur.Set(m_ATPDocCreuset.m_IdSrcAudienceSel, m_ATPDocCreuset.m_MapIdTerrainsAudienceSel.GetKey(), m_ATPDocCreuset.m_IdItemSel);
						m_EffectifCible		= (JUnt32)m_ATPDocCreuset.m_MoteurPresse.GetEffectif(keyMoteur);

						// on r�cup�re l'�chantillon
						m_EchantillonCible	= (JUnt32)m_ATPDocCreuset.m_MoteurPresse.GetNbCas(keyMoteur);

						return;
					}
				}
			}
		}
	}
	else
	{
		// sinon on exporte la cible pond�r�e s�lectionn�e
		m_ATPDocCreuset.m_MAPpItemsPonderes.MoveTo(m_ATPDocCreuset.m_IdItemSel);
		if(m_ATPDocCreuset.m_MAPpItemsPonderes.IsValid())
		{
			// R�cup id cible pond�r�e
			JUnt32 IdCible = m_ATPDocCreuset.m_MAPpItemsPonderes.GetItem()->GetID();
			if(IdCible)
			{
				// R�cup Table des cibles
				CItemsConstruitsBase * pItemConst = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

				// R�cup cible correspondant � l'indice
				IItem * pItem = pItemConst->GetItemPondere(IdCible);

				// Si cible trouv�, on retourne son libell�
				if(pItem)
				{
					// Recuperer libell� cible en cours
					m_LabelCible =  pItem->GetLabel().AsJCharPtr();

					// on cr�e la cl� des effectifs
					CKeyMoteur keyMoteur;

					// Positionnement sur s�lection
					m_ATPDocCreuset.m_MapIdTerrainsAudienceSel.MoveFirst();
					if (m_ATPDocCreuset.m_MapIdTerrainsAudienceSel.IsValid())
					{
						// A FAIRE -- ALAIN HK
						// R�cup�re id terrain et id cible 
						keyMoteur.Set(m_ATPDocCreuset.m_IdSrcAudienceSel, m_ATPDocCreuset.m_MapIdTerrainsAudienceSel.GetKey(), m_ATPDocCreuset.m_IdItemSel);
						m_EffectifCible		= (JUnt32)m_ATPDocCreuset.m_MoteurPresse.GetEffectif(keyMoteur);

						// on r�cup�re l'�chantillon
						m_EchantillonCible	= (JUnt32)m_ATPDocCreuset.m_MoteurPresse.GetNbCas(keyMoteur);

						return;
					}
				}
			}
		}
	}

	// Cible non d�termin�e
	m_LabelCible		= "non d�termin�e";
	m_EffectifCible		= 0;
	m_EchantillonCible	= 0;

}

// Label r�f�rence affinit�
JVoid JATPDocExport::GetInfoRefAffinite()
{
	if (m_ATPDocCreuset.m_Offre.m_IdItemAffinite >= 10000)
	{
		// R�cup Table des cibles
		CItemsConstruitsBase * pItemConst = CItemsConstruitsManager::GetInstance()->GetItemsInstance();

		// R�cup cible affinit� correspondante
		IItem * pItem = pItemConst->GetItemConstruit(m_ATPDocCreuset.m_Offre.m_IdItemAffinite);

		// Si affinit� trouv�e, on retourne son libell�
		if(pItem)
			// Recuperer libell� cible en cours
			m_LabelRefAffinite =  pItem->GetLabel().AsJCharPtr();
		else
			// affinit� non d�termin�e
			m_LabelRefAffinite = "non d�termin�e";
	}
	else
		// affinit� non d�termin�e
		m_LabelRefAffinite = "non d�termin�e";
}

// Nom Filtre d'audience
JVoid JATPDocExport::GetInfoFiltreAudience()
{
	// on r�cup�re libell� filtre d'audience

	// Libelle filtre audience
	m_LabelFiltreAudience = m_srcPresse->m_TBLFiltresAudience.GetLabelByID(m_ATPDocCreuset.m_IdFiltreAudienceSel);
}

// Nom Source et Terrain
JVoid JATPDocExport::GetInfoSourceTerrain()
{
	// on r�cup�re libell� de la source
	if (m_ATPDocCreuset.m_IdSrcAudienceSel)
	{
		// R�cup Table

		// R�cup la source audience courante
		JUnt32 SrcID			= m_ATPDocCreuset.m_IdSrcAudienceSel;  

		// R�cup le nouveau source
		const JSource * pt		= m_srcTables->m_TBLSources.GetItem(SrcID);
		m_LabelSource = pt->m_Libelle.AsJCharPtr();
	}
	else
		// Source non d�termin�
		m_LabelSource = "non d�termin�e";
	
	// Si au moins 1 terrain
	CString Text;
	if (m_ATPDocCreuset.m_MapIdTerrainsAudienceSel.GetCount() > 0)
	{
		if (m_ATPDocCreuset.m_MapIdTerrainsAudienceSel.GetCount() > 1)
		{
			// Multi-terrains
			Text += "Multi-terrains";
		}
		else
		{
			// Mono-terrain
			m_ATPDocCreuset.m_MapIdTerrainsAudienceSel.MoveFirst();

			// Ident terrain s�lectionn�
			JUnt32 IdTerrainSel = m_ATPDocCreuset.m_MapIdTerrainsAudienceSel.GetKey();

			// Test si terrain valide
			JBool IdTerrainValid = false;
			for (m_ATPDocCreuset.m_Offre.m_TBLIdTerrainsAudience.MoveFirst();
				 m_ATPDocCreuset.m_Offre.m_TBLIdTerrainsAudience.IsValid();
				 m_ATPDocCreuset.m_Offre.m_TBLIdTerrainsAudience.MoveNext())
			{
				if (IdTerrainSel == m_ATPDocCreuset.m_Offre.m_TBLIdTerrainsAudience.GetItem())
				{
					IdTerrainValid = true;
					break;
				}
			}

			if (IdTerrainValid)
			{
				// R�cup la source audience courante
				JUnt32 SrcID			= m_ATPDocCreuset.m_IdSrcAudienceSel;  

				// R�cup le nouveau terrain
				const JTerrain * pt			= m_srcTables->m_TBLTerrains.GetItem(SrcID,IdTerrainSel);

				// Recuperer dans table terrain
				CString TxtNewTerrain;
				TxtNewTerrain.Format("%s\t",pt->m_Libelle.AsJCharPtr());
				Text += TxtNewTerrain;
			}
		}

		// Label du ou des terrains
		m_LabelTerrain = Text;
	}
	else
		// Terrain non d�termin�
		m_LabelTerrain = "non d�termin�";
}

// Nom Segment
JVoid JATPDocExport::GetInfoSegment()
{
	// on r�cup�re libell� segment

	// Libelle segment
	CString outSegment;
	if (m_ATPDocCreuset.m_Offre.m_TBLIdSegmentsAudience.GetCount() > 0)
	{						
		for(m_ATPDocCreuset.m_Offre.m_TBLIdSegmentsAudience.MoveFirst();
			m_ATPDocCreuset.m_Offre.m_TBLIdSegmentsAudience.IsValid();
			m_ATPDocCreuset.m_Offre.m_TBLIdSegmentsAudience.MoveNext())
		{
			const JSegment* Seg = m_srcTables->m_TBLSegments.GetItem(m_ATPDocCreuset.m_Offre.m_TBLIdSegmentsAudience.GetItem());
			outSegment += Seg->m_Libelle.AsJCharPtr(); 	
			outSegment +=  "\t"; 	
		}
	}
	else
		// segment non d�fini
		outSegment = "non d�fini";
	
	m_LabelSegment = outSegment; 	
}

// Nom type de poids
JVoid JATPDocExport::GetInfoTypePoids()
{
	// on r�cup�re base
	JSRCTables* srcTables = (JSRCTables*)& JSRCTables::GetInstance();

	// Libelle type de poids
	if (m_ATPDocCreuset.m_Offre.m_IdTypePoids > 0)
		m_LabelTypePoids  = srcTables->m_TBLPoids.GetLabelByID(m_ATPDocCreuset.m_Offre.m_IdTypePoids);
	else
		// type de poids non d�fini
		m_LabelTypePoids  = "non d�fini";
}

// Nom m�thode de calculs couverture
JVoid JATPDocExport::GetInfoMethodeCalcul()
{
	// Ecriture dans buffer
	CString str;
	switch (m_ATPDocCreuset.m_MoteurPresse.GetType())
	{
	case CMoteurPresse::ABAQUE:
		// M�thode macro-probabiliste (JFC)
		str.LoadString(IDS_CR_MOTABAQUE);
		m_GRPActifValid = true;
		break;
	case CMoteurPresse::ABAQUE_ETAL:
		// M�thode macro-probabiliste �talonn�e (JFC)
		str.LoadString(IDS_CR_MOTABAQUEETAL);
		m_GRPActifValid = true;
		break;
	case CMoteurPresse::PROBA:
        // M�thode probabilit�s individuelles (idem SMP-PRESSE)
		str.LoadString(IDS_CR_MOTPROBA);
		m_GRPActifValid = false;
		break;
	}
	m_LabelMethodeCalcCouv = str;

}

// R�cup�ration des formats actifs
JVoid JATPDocExport::GetFormatActif()
{
	// Initialise liste des formats actifs
	m_ListFormatActif.Reset();

	// On boucle sur tous les formats
	for (m_ATPDocCreuset.m_MapIdFormatsSel.MoveFirst();
		 m_ATPDocCreuset.m_MapIdFormatsSel.IsValid();
		 m_ATPDocCreuset.m_MapIdFormatsSel.MoveNext())
	{
		// R�cup�re �tat du format
		JUnt32 Etat = m_ATPDocCreuset.m_MapIdFormatsSel.GetItem();
		//if (Etat & 0x0003)
			// Format Actif
			m_ListFormatActif.AddTail() = m_ATPDocCreuset.m_MapIdFormatsSel.GetKey();
	}
}

// R�cup�ration des informations hypoth�ses
JVoid JATPDocExport::GetInfoHypotheses()
{
	// Dimensionnement du tableau hypoth�ses
	m_TInfoHypos.SetCount(m_ATPDocCreuset.m_TblHypotheses.GetCount()); 

	// Balaye toutes les hypoth�se pous savoir si ligne valide
	JUnt32 NoHypo			= 0;
	for (m_ATPDocCreuset.m_TblHypotheses.MoveFirst();
		 m_ATPDocCreuset.m_TblHypotheses.IsValid();
		 m_ATPDocCreuset.m_TblHypotheses.MoveNext())
	{
		// Passe la 1ere hypoth�se (lou�e au plan �tendu)
	//	if (NoHypo)
		{
			// Init totaux par hypoth�se (A VOIR)
			/*
			m_TotBudgetHyp	= 0;
			m_TotContactHyp	= 0;
			m_TotGRPHyp		= 0.0;
			*/

			// Init Tab Info Supports
			m_TInfoHypos.Item(NoHypo).m_TInfoSupports.Reset();

			// on r�cup�re le plan de l'hypoth�se courante
			CPlanInsertions & PLNInsertions = m_ATPDocCreuset.m_TblHypotheses.GetItem();

			// R�cup�re le libell� hypoth�se
			m_TInfoHypos.Item(NoHypo).m_LabelHypo = PLNInsertions.GetLabel().AsJCharPtr(); 

			// Boucle sur le plateau
			for (JUnt32 j = 0;j < m_ATPDocCreuset.m_PlateauSupports.GetCount(); j++)
			{


					 // On boucle sur tous les formats
				for (m_ATPDocCreuset.m_MapIdFormatsSel.MoveFirst();
				 	 m_ATPDocCreuset.m_MapIdFormatsSel.IsValid();
					 m_ATPDocCreuset.m_MapIdFormatsSel.MoveNext())
				{
					// Informations support
					CInfoSupport InfoSupport;

					// R�cup�re l'ident support
					CKeySupport KeySupport			= m_ATPDocCreuset.m_PlateauSupports.Item(j);
					InfoSupport.m_IdSupport			= KeySupport.m_IdSupport;
					InfoSupport.m_IdParentSupport	= KeySupport.m_IdParent; 
					// Nombre d'insertions
					InfoSupport.m_NbInsertions = 0;

					// R�cup�re �tat du format
					JUnt32 Etat = m_ATPDocCreuset.m_MapIdFormatsSel.GetItem();
					//if (Etat & 0x0003)
					{
						// Id format courant
						
						InfoSupport.m_IdFormat = m_ATPDocCreuset.m_MapIdFormatsSel.GetKey();

						// Balaye toutes les insertions
						for (PLNInsertions.MoveFirst_SFD();PLNInsertions.IsValid_SFD();PLNInsertions.MoveNext_SFD())
						{
							// on r�cup�re l'insertion
							CInsertion * pInsertion = PLNInsertions.GetItem_SFD();

							// Si insertion sur m�me support et m�me format
							if (pInsertion->m_IdSupport == InfoSupport.m_IdSupport &&
								pInsertion->m_IdFormat  == InfoSupport.m_IdFormat && 
								pInsertion->m_IdParent  == InfoSupport.m_IdParentSupport &&
								m_ATPDocCreuset.m_KEYPeriodeActionTablier.GetDateD() <= pInsertion->m_DateD &&
								pInsertion->m_DateD <= m_ATPDocCreuset.m_KEYPeriodeActionTablier.GetDateF())
							{
								
								
								// Incr�mente nb insertions m�me support m�me format m�me hypo
								InfoSupport.m_NbInsertions += pInsertion->m_NbInsertions;

								// Calcul des r�sultats par insertion
								CInfoInsertion InfoInsertion;
								
								InfoInsertion.m_NbInsertion = pInsertion->m_NbInsertions;

								// (Contacts + GRP + Co�t GRP)
								InfoInsertion.m_NbContact	= -1;
								InfoInsertion.m_GRP			= -1;
								InfoInsertion.m_CoutGRP     = -1;
								CalculResultatInsertion(pInsertion,InfoInsertion.m_NbContact,InfoInsertion.m_GRP);

								// Date d�but insertion
								InfoInsertion.m_DateParution = pInsertion->m_DateD; 

								// Calcul co�t et dur�e parution
								CTarifs * pTarifs = CTarifs::GetInstance();
								JUnt32 TarifIns;
								JDate DateTmp;
								if (pTarifs->GetTarifByDate(InfoSupport.m_IdSupport,
														    InfoSupport.m_IdFormat,
   														    pInsertion->m_DateD,
														    m_ATPDocCreuset.m_KEYEtude.GetDateD(), 
														    DateTmp,
														    InfoInsertion.m_Duree,
														    TarifIns) > 0)
								{
									// Calcul du c�ut GRP
									if (InfoInsertion.m_GRP)
										InfoInsertion.m_CoutGRP = TarifIns / InfoInsertion.m_GRP;
									else
										InfoInsertion.m_CoutGRP = -1;
								}
								else
								{
									// Probl�me tarif
									InfoInsertion.m_CoutGRP = -1;
								}
								
								//on force la dur�e r��ele de l'insertion sinon dur�e = 0 pour les couplages
								InfoInsertion.m_Duree = pInsertion->m_Duree;

								// Calcul Contribution Insertion
								InfoInsertion.m_Contribution = -1;

								// Mise � jour totaux hypoth�se en cours
								/*
								m_TotBudgetHyp  += Tarif;
								m_TotContactHyp += InfoInsertion.m_NbContact;
								m_TotGRPHyp		+= InfoInsertion.m_GRP;
								*/
								InfoSupport.m_TInfoInsertions.AddTail() = InfoInsertion;  
							}
						}

						// R�cup�re l'objet support
						CRegroupements * pRegroupements = CRegroupements::GetInstance();
						IElem* pSupport = pRegroupements->GetIElemByID(InfoSupport.m_IdSupport);

						// on r�cup�re le libell� support
						InfoSupport.m_LblSupport = "";
						if (pSupport)
							InfoSupport.m_LblSupport = pSupport->GetLabel().AsJCharPtr() ; 

						// puis les libell�s format
						InfoSupport.m_LblCompletFormat = "";
						InfoSupport.m_LblAbregeFormat  = "";

						JInt32 idFormat = InfoSupport.m_IdFormat;
						m_ATPDocCreuset.SubstitutFormat(InfoSupport.m_IdSupport, idFormat);

						// libell� long format
						InfoSupport.m_LblCompletFormat = m_srcPresse->m_TBLFormats.GetLabelByID(idFormat);
						
						// puis le libell� abr�g� format
						InfoSupport.m_LblAbregeFormat = m_srcPresse->m_TBLFormats.GetLabelCourtByID(idFormat); 

						if(idFormat == FORMAT_VIRTUEL)
						{
							InfoSupport.m_LblCompletFormat = "";
							InfoSupport.m_LblAbregeFormat  = "";
						}

						// puis	le tarif associ�
						CTarifs * pTarifs = CTarifs::GetInstance();

						// Tarif support - format courant - date insertion debut p�riode
						if(pTarifs)
						{
							// Recherche tarif via d�but p�riode �tude
							JDate DateTmp;
							JUnt32 Duree;
							JUnt32 Tarif;
							if (pTarifs->GetTarifByDate(InfoSupport.m_IdSupport,
														idFormat,
   														m_ATPDocCreuset.m_KEYEtude.GetDateD(),
														m_ATPDocCreuset.m_KEYEtude.GetDateF(), 
														DateTmp,
														Duree,
														Tarif) > 0)
							{
								// Tarif trouv�
								InfoSupport.m_Tarif = Tarif;
							}
							else
							{
								// Tarif non trouv�
								InfoSupport.m_Tarif = -1;
							}
						}

						// Ajout elmnt support
						m_TInfoHypos.Item(NoHypo).m_TInfoSupports.AddTail() = InfoSupport;
					}
				}
			}

			// R�cup des r�sultats par hypoth�se
			m_ATPDocCreuset.m_ITRHypotheses.MoveTo(NoHypo);

			// Voir si r�sultats hypoth�ses valides
			if (! m_ATPDocCreuset.m_ITRHypotheses.IsValid())
				return;
			
			const JITPDATHypoFormat  &DATHypoFormat = m_ATPDocCreuset.m_ITRHypotheses.GetItem();
			m_TInfoHypos.Item(NoHypo).m_expDATHypoFormat = &m_ATPDocCreuset.m_ITRHypotheses.GetItem();
			const JITPDATHypothese & DATHypothese = DATHypoFormat.GetDATHypoTotal();

			// on r�cup�re le nombre de jours
			JInt32 NbJours = DATHypothese.m_DistributionJour.GetCount();

			// on r�cup�re la couverture cumul�e � 1+
			JFlt64 Couv = DATHypothese.m_Couverture;
			JFlt64 VolLecture = DATHypothese.m_VolLecture;
			JFlt64 Audience = DATHypothese.m_AudienceRef;
			JFlt64 Effectif   = DATHypoFormat.m_Effectif;
			JFlt64 Budget   = DATHypothese.m_Budget;

			// ----------------------------------------- Total insertions --------------------------------------
			m_TInfoHypos.Item(NoHypo).m_TotInsertions	= OnGetNbInsertionsHyp(NoHypo);	

			// ------------------------------------------- Budget Brut -----------------------------------------
			m_TInfoHypos.Item(NoHypo).m_BudgetBrut		= Budget;

			// --------------------------------------- Audience / Nombre d'individus touch�s (/1000) ----------- 


			// Sauve Total contacts en millier
			m_TInfoHypos.Item(NoHypo).m_Audience = Couv ;

			// ------------------------------------- Contacts actifs (/1000) ----------------------------------
			// Nombre total de contacts (en milliers)
			m_TInfoHypos.Item(NoHypo).m_ContactActif = DATHypothese.m_VolLecture;

			// ------------------------------------------------ Grp -------------------------------------------
			// on teste si on peut calculer le GRP
			if (Effectif > 0.0 && Audience > 0.0)
				// on formate le nombre de contacts
				m_TInfoHypos.Item(NoHypo).m_Grp = (100.0 * Audience) / Effectif;
			else
				// Grp non d�fini
				m_TInfoHypos.Item(NoHypo).m_Grp = -1;
			
			// --------------------------------------------- Grp Actif ----------------------------------------

			// Sauve nombre de contacts
			if (Effectif > 0.0)
				m_TInfoHypos.Item(NoHypo).m_GrpActif = (100.0 * VolLecture) / Effectif;
			else
				// Grp actif non d�fini
				m_TInfoHypos.Item(NoHypo).m_GrpActif = -1;

			// -------------------------------------- Couverture cumulee � 1+% ---------------------------------

			// on r�cup�re la couverture cumul�e � 1+
			m_TInfoHypos.Item(NoHypo).m_Couverture = 0.0; 
			if (Effectif > 0) 
				m_TInfoHypos.Item(NoHypo).m_Couverture = 100.0 * DATHypothese.m_Couverture / Effectif;
			else
				// Couverture non d�fini
				m_TInfoHypos.Item(NoHypo).m_Couverture = -1;

			// ------------------------------------------- R�p�tition ------------------------------------------
	
			// Sauve R�p�tition
			if (Effectif > 0.0 && Audience > 0.0 && Couv>0.0)
 				m_TInfoHypos.Item(NoHypo).m_Repetition = Audience / Couv;
			else
				// R�p�tition non d�fini
				m_TInfoHypos.Item(NoHypo).m_Repetition = -1;

			// ------------------------------------------- R�p�tition[+] ------------------------------------------

			// Sauve R�p�tition[+]
			if (Effectif > 0.0 && Couv > 0.0)
 				m_TInfoHypos.Item(NoHypo).m_RepetitionPlus = VolLecture / Couv;
			else
				// R�p�tition non d�fini
				m_TInfoHypos.Item(NoHypo).m_RepetitionPlus = -1;

			// -------------------------------------- Couverture m�moris�e -------------------------------------
			// Non d�finie pour le moment
			m_TInfoHypos.Item(NoHypo).m_CouvMemo			= -1;
			
			int beta = AfxGetApp()->GetProfileInt("Moteur", "CourbeReponse", 0);
			CCourbes * courbes = CCourbes::GetInstance();
			double vol = 0.0;
			double DistrFreq;
			// on r�cup�re le nombre de jours
			JInt32 idxD = m_ATPDocCreuset.m_KEYPeriodeActionTablier.GetDateD() - m_ATPDocCreuset.m_KEYPeriodeAction.GetDateD()-1;
			JInt32 idxF = m_ATPDocCreuset.m_KEYPeriodeActionTablier.GetDateF() - m_ATPDocCreuset.m_KEYPeriodeAction.GetDateD();
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
			if(vol> 0.0 && Effectif>0.0)
				m_TInfoHypos.Item(NoHypo).m_CouvMemo =  100.0 * vol / Effectif;


			// ------------------------------------------- Co�t GRP --------------------------------------------
	
			// on r�cup�re le C�ut GRP
			if (Audience > 0)
				m_TInfoHypos.Item(NoHypo).m_CoutGRP = (Budget * Effectif) / (100.0 * Audience);
			else
				// Cout GRP non d�fini
				m_TInfoHypos.Item(NoHypo).m_CoutGRP = -1;

			// ------------------------------------------- Co�t/%couv ---------------------------------

			// on teste si on peut calculer le Co�t/%couv
			if (Couv > 0.0)
				m_TInfoHypos.Item(NoHypo).m_CoutCouv = (Budget * Effectif) / (100.0 * Couv);
			else
				// Co�t/%couv  non d�fini
				m_TInfoHypos.Item(NoHypo).m_CoutCouv = -1;

			// ------------------------------------------- Co�t/1000 Individus ---------------------------------

			// on teste si on peut calculer le Co�t /1000 indiv
			if (Couv > 0.0)
				m_TInfoHypos.Item(NoHypo).m_Cout1000Indiv = (1000.0 * Budget) / Couv;
			else
				// Cout/1000 indivs non d�fini
				m_TInfoHypos.Item(NoHypo).m_Cout1000Indiv  = -1;

			// ------------------------------------------- Co�t/1000 Contacts ----------------------------------

			// on teste si on peut calculer le Co�t /1000 contacts
			if (VolLecture > 0.0)
				// on formate le Co�t /1000 contacts
				m_TInfoHypos.Item(NoHypo).m_Cout1000Contact = (1000.0 * Budget) / VolLecture;
			else
				// Co�t/1000 Individus non d�fini
				m_TInfoHypos.Item(NoHypo).m_Cout1000Contact = -1;

			// ---------------------------------------- Distribution cumul�e -----------------------------------
			m_TInfoHypos.Item(NoHypo).m_DistriCum.SetCount(21);			
			for (JUnt32 InxContact = 0; InxContact<= 20; InxContact++)
			{		
				// on r�cup�re le nombre de jours
				JInt32 idxF = m_ATPDocCreuset.m_KEYPeriodeActionTablier.GetDateF() - m_ATPDocCreuset.m_KEYPeriodeAction.GetDateD();

				// on r�cup�re la distribution de fr�quence
				if (idxF > 0) 
				{
					if (InxContact == 0)
						m_TInfoHypos.Item(NoHypo).m_DistriCum.Item(InxContact) = 1.0 - (DATHypothese.m_DistributionJour.Item(idxF).Item(1).AsJFlt64()) / Effectif;
					else
					{
						JFlt64 ValCouv = DATHypothese.m_DistributionJour.Item(idxF).Item(InxContact).AsJFlt64() / Effectif;
						m_TInfoHypos.Item(NoHypo).m_DistriCum.Item(InxContact) = DATHypothese.m_DistributionJour.Item(idxF).Item(InxContact).AsJFlt64() / Effectif;
					}
				}
			}	

			// ---------------------------------------- Distribution nette -------------------------------------
			m_TInfoHypos.Item(NoHypo).m_DistriNette.SetCount(21);			
			for (JUnt32 InxContact = 0; InxContact<= 20; InxContact++)
			{		
			
				// -------------------- Distribution de fr�quence -----------------------
				// on r�cup�re le nombre de jours
				JInt32 idxF = m_ATPDocCreuset.m_KEYPeriodeActionTablier.GetDateF() - m_ATPDocCreuset.m_KEYPeriodeAction.GetDateD();

				// on r�cup�re la distribution de fr�quence
				if (idxF > 0) 
				{
					if (InxContact == 0)
						m_TInfoHypos.Item(NoHypo).m_DistriNette.Item(InxContact) = 1.0 - (DATHypothese.m_DistributionJour.Item(idxF).Item(1).AsJFlt64()) / Effectif;
					if (InxContact == 20)
						m_TInfoHypos.Item(NoHypo).m_DistriNette.Item(InxContact) = DATHypothese.m_DistributionJour.Item(idxF).Item(InxContact).AsJFlt64() / Effectif;
					if (InxContact >0 && InxContact<20)
						m_TInfoHypos.Item(NoHypo).m_DistriNette.Item(InxContact) = (DATHypothese.m_DistributionJour.Item(idxF).Item(InxContact).AsJFlt64() - DATHypothese.m_DistributionJour.Item(idxF).Item(InxContact+1).AsJFlt64()) / Effectif;
				}
			}

			// ------------------------------------- Valeurs Graphe Couverture ----------------------------------
			RecupValGrapheCouv(NoHypo);

			// ---------------------------------------- Valeurs Graphe GRP --------------------------------------
			RecupValGrapheGRP(NoHypo);

			// Test info hypo
			CInfoHypo *pInfoHypTest;
			pInfoHypTest = &m_TInfoHypos.Item(NoHypo);
			pInfoHypTest = NULL;

		}
		//else
		//{
		//	// Hypothese 0 --> Infos de tous les supports
		//	m_TInfoHypos.Item(NoHypo).m_TInfoSupports.Reset();

		//	// on r�cup�re le plan de l'hypoth�se courante
		//	CPlanInsertions & PLNInsertions = m_ATPDocCreuset.m_TblHypotheses.GetItem();

		//	// R�cup�re le libell� hypoth�se
		//	m_TInfoHypos.Item(NoHypo).m_LabelHypo = PLNInsertions.m_label.AsJCharPtr(); 

		//	// Boucle sur le plateau
		//	for (JUnt32 j = 0;j < m_ATPDocCreuset.m_PlateauSupports.GetCount(); j++)
		//	{
		//		// Informations support
		//		CInfoSupport InfoSupport;

		//		// R�cup�re l'ident support
		//		CKeySupport KeySupport			= m_ATPDocCreuset.m_PlateauSupports.Item(j);
		//		InfoSupport.m_IdSupport			= KeySupport.m_IdSupport;
		//		InfoSupport.m_IdParentSupport	= KeySupport.m_IdParent; 
		//	}
		//
		//}

		// passe � hypoth�se suivante
		NoHypo++;
	}
}

// R�cup�ration des infos Echelle
JVoid JATPDocExport::GetInfoEchelle()
{
	// Init Liste des supports Echelle
	m_InfoEchelle.Reset();

	// Boucle sur tous les supports de l'�chelle
	for (JUnt32 InxSupport = 0; InxSupport < m_ATPDocCreuset.m_EchelleSupports.GetCount();InxSupport++)
	{
		// Initialise objet Info Support Echelle
		CInfoEchSupport InfoEchSupp;

		// on r�cup�re l'identifiant du Support
		InfoEchSupp.m_IdSupport = m_ATPDocCreuset.m_EchelleSupports.Item(InxSupport);

		// on r�cup�re le Support
		CRegroupements * pReg = CRegroupements::GetInstance();
		IElem * pElem = pReg->GetIElemByID(InfoEchSupp.m_IdSupport);
		CString ttlTmp;
		if(pElem)
		{
			// on �crit le libell� du Support
			ttlTmp = pElem->GetLabel().AsJCharPtr();

			if(pElem->IsTitre() &&  pElem->AsITitre()->GetPeriodicite() == 1)
				m_ATPDocCreuset.m_bLDPQtd ? ttlTmp += " (LDP)" : ttlTmp += " (LNM)";
		}
		InfoEchSupp.m_LabelSupport = ttlTmp;

		// on recherche l'�chelle
		m_ATPDocCreuset.m_ITREchelle.MoveTo(InfoEchSupp.m_IdSupport);

		// on teste si on a trouv� l'�chelle
		if (m_ATPDocCreuset.m_ITREchelle.IsValid())
		{
			// on r�cup�re l'�chelle
			const JITPDATEchelle & ITDATEchelle = m_ATPDocCreuset.m_ITREchelle.GetItem();	

			// P�riodicit� Support (abr�g� et complet
			// par d�faut couplage
			InfoEchSupp.m_LabelPeriodicite		= _T("Couplage");
			InfoEchSupp.m_LabelAbrPeriodicite	= _T("CPL");

			// on r�cup�re le p�riodicit�
			if(ITDATEchelle.m_Periodicite)
			{
				InfoEchSupp.m_LabelPeriodicite		= m_srcPresse->m_TBLPeriodicites.GetLabelByID(ITDATEchelle.m_Periodicite);
				InfoEchSupp.m_LabelAbrPeriodicite	= m_srcPresse->m_TBLPeriodicites.GetLabelCourtByID(ITDATEchelle.m_Periodicite); 
			}

			// Tarif support d�but p�riode
			InfoEchSupp.m_Tarif					= ITDATEchelle.m_Tarif;	
	
			// LDP/LNM Support
			if (ITDATEchelle.m_AudienceRef)
				InfoEchSupp.m_LDPLNM =  ITDATEchelle.m_AudienceRef / 1000;

			// Validit� du GRP Actif
			InfoEchSupp.m_GRPActifValid = m_GRPActifValid;

			// GRP Actif Support
			if (ITDATEchelle.m_Effectif > 0.0 && ITDATEchelle.m_VolLecture > 0.0)
				InfoEchSupp.m_GRPActif =  (100.0 * ITDATEchelle.m_VolLecture) / ITDATEchelle.m_Effectif;

			// GRP Support
			if (ITDATEchelle.m_Effectif > 0.0 && ITDATEchelle.m_AudienceProba > 0.0 )
				InfoEchSupp.m_GRP =  100.0 * ITDATEchelle.m_AudienceProba / ITDATEchelle.m_Effectif;

			// Affinite Support
			if (ITDATEchelle.m_IndAffinite > 0.0)
				InfoEchSupp.m_Affinite = ITDATEchelle.m_IndAffinite;
			
			// % Affinit� support
			if (ITDATEchelle.m_PrcAffinite > 0.0)
				InfoEchSupp.m_PrctAffinite = ITDATEchelle.m_PrcAffinite;
			
			// Co�t GRP Support
			if (ITDATEchelle.m_AudienceProba > 0.0 && ITDATEchelle.m_Effectif > 0.0 && InfoEchSupp.m_Tarif >0.0)
				InfoEchSupp.m_CoutGRP = ((JFlt64)InfoEchSupp.m_Tarif * ITDATEchelle.m_Effectif) / (100.0 * ITDATEchelle.m_AudienceProba);

			// Audience Support
			InfoEchSupp.m_Audience = ITDATEchelle.m_Distribution.Item(1).AsJFlt64();
			
			// Reprises support
			if (ITDATEchelle.m_VolLecture > 0.0 && InfoEchSupp.m_Audience > 0.0)
				InfoEchSupp.m_Reprise = ITDATEchelle.m_VolLecture / InfoEchSupp.m_Audience;
			
			// Apports Support
			InfoEchSupp.m_Apport = (ITDATEchelle.m_Apport / ITDATEchelle.m_Effectif) * 100;
			
			// Contacts Support
			InfoEchSupp.m_Contact = ITDATEchelle.m_VolLecture;
		}

		// Ajoute Elmt Info Support � Echelle
		m_InfoEchelle.AddTail() = InfoEchSupp; 
	}		

}


// R�cup�ration Valeurs Graphe Couverture
JVoid JATPDocExport::RecupValGrapheCouv(JUnt32 NoHypo)
{
	// on recherche l'interrogation
	m_ATPDocCreuset.m_ITRHypotheses.MoveTo(NoHypo);

	// on teste si on a trouv� l'hypoth�se
	if (!m_ATPDocCreuset.m_ITRHypotheses.IsValid())
		return;

	// on r�cup�re l'hypoth�se
	const JITPDATHypoFormat & DATHypoFormat = m_ATPDocCreuset.m_ITRHypotheses.GetItem();
	const JITPDATHypothese & DATHypothese = DATHypoFormat.GetDATHypoTotal();
	
	for (JUnt32 NoJour = 0; NoJour < DATHypothese.m_NbContactsJour.GetCount(); NoJour++)
	{
		if (DATHypoFormat.m_JoursActifs.GetAt(NoJour))
		{
			// on v�rifie la validit� du jour
			if (NoJour < 0 || NoJour >= DATHypothese.m_NbContactsJour    .GetCount()) return;
			if (NoJour < 0 || NoJour >= DATHypothese.m_DistributionJour.GetCount()) return;

			JFlt64 A4 = 0.0;
			A4 = 100.0 * DATHypothese.m_DistributionJour.Item(NoJour).Item(1).AsJFlt64() / DATHypoFormat.m_Effectif;

			// Stocke valeur couverture
			m_TInfoHypos.Item(NoHypo).m_ValCouv.AddTail() = A4;  
		}
		else
			// Jour non actif
			m_TInfoHypos.Item(NoHypo).m_ValCouv.AddTail() = 0.0;  
	}
}

// R�cup�ration Valeurs Graphe GRP
JVoid JATPDocExport::RecupValGrapheGRP(JUnt32 NoHypo)
{
	// on recherche l'interrogation
	m_ATPDocCreuset.m_ITRHypotheses.MoveTo(NoHypo);

	// on teste si on a trouv� l'hypoth�se
	if (!m_ATPDocCreuset.m_ITRHypotheses.IsValid()) return;

	// on r�cup�re l'hypoth�se
	const JITPDATHypoFormat & DATHypoFormat = m_ATPDocCreuset.m_ITRHypotheses.GetItem();
	const JITPDATHypothese & DATHypothese = DATHypoFormat.GetDATHypoTotal();

	for (JUnt32 NoJour = 0; NoJour < DATHypothese.m_NbContactsJour.GetCount(); NoJour++)
	{

		// on v�rifie la validit� du jour
		if (NoJour < 0 || NoJour >= DATHypothese.m_NbContactsJour.GetCount()) return;

		// on r�cup�re l'effectif et le nombre de contacts
		JFlt64 Effectif   = DATHypoFormat.m_Effectif;
		JFlt64 NbContacts = DATHypothese.m_NbContactsJour.Item(NoJour);

		// on calcule le GRP
		JFlt64 GRP = 0.0;
		if (Effectif > 0.0) GRP = (100.0 * NbContacts) / Effectif;
		
		// Stocke valeur couverture
		m_TInfoHypos.Item(NoHypo).m_ValGRP.AddTail() = GRP;  
	}
}

// Nombre d'insertions active pour une hypoth�se
JInt32 JATPDocExport::OnGetNbInsertionsHyp(JUnt32 IdHypothese) const
{
	// on initialise le nombre d'insertions
	JInt32 NbInsertions = 0;

	// on r�cup�re la p�riode d'action de l'�tude
	JDate DateD = m_ATPDocCreuset.m_KEYPeriodeActionTablier.GetDateD();
	JDate DateF = m_ATPDocCreuset.m_KEYPeriodeActionTablier.GetDateF();

	// on v�rifie la validit� de la p�riode d'action
	if (DateD.IsValid() && DateF.IsValid())
	{

		// Va sur l'hypothese 
		m_ATPDocCreuset.m_TblHypotheses.MoveTo(IdHypothese);
		if (m_ATPDocCreuset.m_TblHypotheses.IsValid())
		{
			// on r�cup�re les insertions de l'hypoth�se courante
			CPlanInsertions & PLNInsertions = m_ATPDocCreuset.m_TblHypotheses.GetItem();
			PLNInsertions.GetNbInsert(DateD,DateF,NbInsertions);
		}
	}

	// on renvoie le nombre d'insertions
	return (NbInsertions);
}

///////////////////////////////////////////////////////////////////////////////////
// Calcul r�sultat pour 1 insertion
JBool JATPDocExport::CalculResultatInsertion(CInsertion *pInsertion, JFlt64 &Contact, JFlt64 &GRP)
{
	// A FAIRE JEREMY
	// on v�rifie la validit� de l'insertion
	if	(pInsertion && pInsertion->m_IdSupport && pInsertion->m_IdFormat)
	{
		// on cr�e la cl� des effectifs
		CKeyMoteur keyMoteur;

		// Initialise les r�sultats
		JFlt64			Effectif = 0.0;

		// Boucle sur tous les terrains actifs
		for(m_ATPDocCreuset.m_MapIdTerrainsAudienceSel.MoveFirst();
			m_ATPDocCreuset.m_MapIdTerrainsAudienceSel.IsValid();
			m_ATPDocCreuset.m_MapIdTerrainsAudienceSel.MoveNext())
		{

			// Terrain et cible en cours
			keyMoteur.Set(m_ATPDocCreuset.m_IdSrcAudienceSel, m_ATPDocCreuset.m_MapIdTerrainsAudienceSel.GetKey(), m_ATPDocCreuset.m_IdItemSel);

			Effectif += m_ATPDocCreuset.m_MoteurPresse.GetEffectif(keyMoteur);

			JFlt64 VolLecture = 0.0;
			JFlt64 AudienceRef = 0.0;
			JFlt64 AudienceProba = 0.0;
			JInt32 PtEtal = 0;
				
			m_ATPDocCreuset.m_MoteurPresse.CalcAudience(keyMoteur, pInsertion->m_IdSupport, VolLecture, AudienceRef, AudienceProba, PtEtal, true);

			// Nombre de contacts
			Contact		+=  VolLecture;

			// GRP
			if (Effectif > 0)
				GRP			+=  (100.0 * Contact) / Effectif;

		} // boucle sur les terrains

		// Calcul nb contact et GRP ok
		return true;
	}
	else
	{
		// Probl�me d�finition insertion
		throw JInvalidCall::GetInstance();
		return false;
	}
}

// Les exports pour exporter creuset
JBool JATPDocExport::Accept(CATPIVisitor *pIVisitor)
{
	JBool AuMoins1HypotheseValid = false;

	// Test si au moins 1 hypoth�se avec insertions
	JUnt32 i=0;
	for(;i < m_TInfoHypos.GetCount(); i++)
	{
		// Hypothese 0 = hypoth�se plan �tendu (ne pas la prendre en compte)
		if (m_TInfoHypos.Item(i).m_TotInsertions > 0)
		{
			AuMoins1HypotheseValid = true;
			break;
		}
	}

	// Au moins 1 hypoth�se avec insertions, on exporte les r�sultats
	if (AuMoins1HypotheseValid)
	{
		// Export infos entete
		pIVisitor->VisitBegin(this);

		
		// Export infos hypo
		
		int max = m_TInfoHypos.GetCount()-1;
		if(m_TInfoHypos.GetCount()-1 == m_ATPDocCreuset.m_MaxHypothese && m_TInfoHypos.Item(m_TInfoHypos.GetCount()-1).m_TotInsertions > 0 )
			max = m_TInfoHypos.GetCount();
		for(i=0; i < max; i++)
		{
			// Pour chaque hypothese on export les infos n�cessaires
			if (!m_TInfoHypos.Item(i).Accept(pIVisitor,i,this))
				return false;

			// R�cup String Export Hypoth�se
			m_StrExport += m_TInfoHypos.Item(i).m_StrExportHypo;
		}

		// Export pied de page
		pIVisitor->VisitEnd(this);

		return true;
	}
	else
		return false;
}

// L'export �chelle
JBool JATPDocExport::AcceptEch(CATPIVisitor *pIVisitor)
{
	// Au moins 1 support, on exporte l'�chelle
	if (m_InfoEchelle.GetCount() > 0)
	{
		// Export infos entete
		pIVisitor->VisitBegin(this);

		// Export infos hypo
		for(m_InfoEchelle.MoveFirst();
			m_InfoEchelle.IsValid();
			m_InfoEchelle.MoveNext())
		{
			// Pour chaque support on exporte les infos n�cessaires
			if (!m_InfoEchelle.GetItem().Accept(pIVisitor))
				return false;

			// R�cup String Export Hypoth�se
			m_StrExport += m_InfoEchelle.GetItem().m_StrExportEch;
		}
		m_StrExport += "\n";

		// Export pied de page
		pIVisitor->VisitEnd(this);
		return true;
	}
	else
		return false;
}
