//
// Fichier: JITPTBLHypotheses.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    28/08/2003
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JATPDocCreuset.h"
#include "JITPTBLHypotheses.h"
#include "ItemsConstruitsManager.h"

////////////////////
// les constructeurs

JITPTBLHypotheses::JITPTBLHypotheses(JATPDocCreuset *pDoc)
{
	m_pDocCreuset = pDoc;	// on ne fait rien
}

JITPTBLHypotheses::JITPTBLHypotheses(const JITPTBLHypotheses & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();

	/*
	this->m_pDocCreuset = Source.m_pDocCreuset;				// Le doc creuset
	this->m_Hypotheses = Source.m_Hypotheses;				// les hypotheses
	*/
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JITPTBLHypotheses & JITPTBLHypotheses::operator =(const JITPTBLHypotheses & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JITPTBLHypotheses::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_Hypotheses.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JVoid JITPTBLHypotheses::Add(const JUnt32 & IdxHypothese, const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, LIST_HYPOTHESES & PLNHypotheses, JBool Move)
{
	// on v�rifie la validit� de la cl�
	if (!m_pDocCreuset) throw JInvalidCall::GetInstance();

	// on r�cup�re l'indice de l'hypoth�se
	PLNHypotheses.MoveTo(IdxHypothese);
	if(!PLNHypotheses.IsValid()) throw JInvalidCall::GetInstance();

	// on r�cup�re le plan de l'hypoth�se courante
	CPlanInsertions & PLNInsertions = PLNHypotheses.GetItem();

	// on cr�e les r�sultats temporaire
	JITPDATHypoFormat DATTemp/*(m_pDocCreuset->m_Offre.m_TBLIdFormats)*/;
	
	CalcDatHypo(KEYPeriodeAction, KEYPeriodeTablier, PLNInsertions, DATTemp, false);

	// on ajoute la nouvelle hypoth�se
	m_Hypotheses.Add(IdxHypothese, Move).Swap(DATTemp);
}

void JITPTBLHypotheses::CalcDatHypo(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, CPlanInsertions & PLNInsertions, JITPDATHypoFormat & DATTemp, bool bNoDistrib)
{
	CalcDatHypo(KEYPeriodeAction,  KEYPeriodeTablier, PLNInsertions, DATTemp, m_pDocCreuset->m_IdItemSel, bNoDistrib);
}

void JITPTBLHypotheses::CalcDatHypo(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, CPlanInsertions & PLNInsertions, JITPDATHypoFormat & DATTemp, JUnt32 idItem, bool bNoDistrib/*=false*/)
{
	JFpu Fpu;
	DATTemp.InitFormats(m_pDocCreuset->m_Offre.m_TBLIdFormats);

	// on r�cup�re la p�riode d'action de l'�tude
	JDate DateD = KEYPeriodeAction.GetDateD();
	JDate DateF = KEYPeriodeAction.GetDateF();

	for (m_pDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
		m_pDocCreuset->m_MapIdTerrainsAudienceSel.IsValid();
		m_pDocCreuset->m_MapIdTerrainsAudienceSel.MoveNext())
	{

		// Ident du terrain
		JUnt32 IdTerrain = m_pDocCreuset->m_MapIdTerrainsAudienceSel.GetKey();

		// Ident cible s�lectionn�
		JUnt32 IdCible = m_pDocCreuset->m_IdItemSel;

		// D�fini le moteur
		CKeyMoteur keyMoteur;
		keyMoteur.Set(m_pDocCreuset->m_IdSrcAudienceSel, m_pDocCreuset->m_MapIdTerrainsAudienceSel.GetKey(), idItem);

		// Attention si c'est une cible pond�r�e
		// CItemsConstruits * pItemsConstruits = CItemsConstruits::GetInstance(); 

		// Sauvegarde si cible autoris�e ou pas
		JBool CibleActive = true;

		// R�cup l'item d�fini par l'ident cible
		IItem * pIItem = CItemsConstruitsManager::GetInstance()->GetItemsInstance()->GetIItemByID(IdCible);

		// Ce n'est pas un item pond�r�
		if (!pIItem->IsItemPondere())
		{
			// Calcul de l'effectif
			if (!m_pDocCreuset->m_Offre.CibleAutorisee(IdCible,IdTerrain))
			{
				// Pas d'effectif si la cible n'est pas autoris�e (pas atteint seuil du terrain)
				DATTemp.m_Effectif = -1;
				CibleActive = false;
			}
			else
				DATTemp.m_Effectif += m_pDocCreuset->m_MoteurPresse.GetEffectif(keyMoteur);
		}
		// C'est un item pond�r�
		else
		{
			// Map avec 1 seul terrain
			JMap <JUnt32, JUnt32> MapTerrain;
			MapTerrain.Add(IdTerrain) = 0;

			// Calcul de l'effectif
			if (!m_pDocCreuset->m_MoteurPresse.CiblePondAutorise(IdCible, m_pDocCreuset->m_Offre.m_IdSourceAudience, &MapTerrain))
			{
				// Pas d'effectif si la cible pond�r�e n'est pas autoris�e (pas atteint seuil du terrain)
				DATTemp.m_Effectif = -1;
				CibleActive = false;
			}
			else
				DATTemp.m_Effectif += m_pDocCreuset->m_MoteurPresse.GetEffectif(keyMoteur);
		}
		
		// on v�rifie la validit� de la p�riode d'action
		if (DateD.IsValid() && DateF.IsValid())
		{
			// on calcule le nombre de jours de la p�riode
			JInt32 NbJours = (DateF - DateD) + 1;
			DATTemp.InitNbJours(NbJours);

			// on boucle sur toutes les insertions
			for (PLNInsertions.MoveFirst_SFD(); PLNInsertions.IsValid_SFD(); PLNInsertions.MoveNext_SFD())
			{
				// on r�cup�re l'insertion courante
				CInsertion * pInsertion = PLNInsertions.GetItem_SFD();

				//on doit d'abord tester si on peut decrire le couplage
				CKeyTerrain keyTerrain;
				keyTerrain.Set(m_pDocCreuset->m_IdSrcAudienceSel, m_pDocCreuset->m_MapIdTerrainsAudienceSel.GetKey());
				if(pInsertion->IsCouplage() && CMoteurPresse::CanDescribeCouplage(pInsertion->m_IdSupport, keyTerrain))
				{
					//on peur d�crire le couplage on l'�value comme un mini plan
					CPlanInsertions & PLNInsertCpl = pInsertion->GetCouplageInsertions();
					for (PLNInsertCpl.MoveFirst_SFD(); PLNInsertCpl.IsValid_SFD(); PLNInsertCpl.MoveNext_SFD())
					{
						// on r�cup�re l'insertion courante
						const CInsertion * pInsertCpl = PLNInsertCpl.GetItem_SFD();
						CalcDatInsert(KEYPeriodeAction, KEYPeriodeTablier, pInsertCpl, DATTemp, keyMoteur, true);
					}
					
					// On recup�re son budget
					if (KEYPeriodeTablier.GetDateD() <= (pInsertion->m_DateD + (pInsertion->m_Duree - 1)) && KEYPeriodeTablier.GetDateF() >= pInsertion->m_DateD)
					{
						m_pDocCreuset->m_MapIdFormatsSel.MoveTo(pInsertion->m_IdFormat);
						if(!m_pDocCreuset->m_MapIdFormatsSel.IsValid())
							continue;

						if(m_pDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
							DATTemp.GetDATHypoFormatActif().m_Budget += (JFlt64)pInsertion->m_Data.m_TarifP * pInsertion->m_NbInsertions;

						DATTemp.GetDATHypoByFormat(pInsertion->m_IdFormat).m_Budget += (JFlt64)pInsertion->m_Data.m_TarifP * pInsertion->m_NbInsertions;
						DATTemp.GetDATHypoTotal().m_Budget += (JFlt64)pInsertion->m_Data.m_TarifP * pInsertion->m_NbInsertions;

						if(pInsertion->m_Data.m_TarifP == 0)
						{
							if(m_pDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
								DATTemp.GetDATHypoFormatActif().m_budgetOK = false;

							DATTemp.GetDATHypoByFormat(pInsertion->m_IdFormat).m_budgetOK = false;
							DATTemp.GetDATHypoTotal().m_budgetOK = false;
						}
					}
				}
				else
				{
					CalcDatInsert(KEYPeriodeAction, KEYPeriodeTablier, pInsertion, DATTemp, keyMoteur);
				}

			} // End for Insertions

			// si on a au - 1 insert
			if(PLNInsertions.GetCount())
			{
				for(DATTemp.m_mapDATHypo.MoveFirst(); DATTemp.m_mapDATHypo.IsValid(); DATTemp.m_mapDATHypo.MoveNext())
				{
					JFlt64 NbContactsCum = 0.0;
					JFlt64 NbContactsNet = 0.0;
					JFlt64 AudienceJour = 0.0;

					JITPDATHypothese& DATHypo = DATTemp.m_mapDATHypo.GetItem();

					m_pDocCreuset->m_MoteurPresse.BuildDistributions(keyMoteur, DATHypo.m_maille, bNoDistrib);

					for (JInt32 IdxJour = 0; IdxJour < NbJours; IdxJour += 1)
					{
						// Si la cible n'est pas autoris�e, on ne r�cup�re pas le nb contacts et l'audience
						// if (m_pDocCreuset->m_Offre.CibleAutorisee(IdCible,IdTerrain))
						if (CibleActive)
						{
							// on initialise le nombre de jours actifs et de contacts cumul�s
							NbContactsNet = DATHypo.m_NbContactsJour.Item(IdxJour);
							AudienceJour = DATHypo.m_AudienceJour.Item(IdxJour);
						}

						if (NbContactsNet > 0.0)
						{
							// on met � jour le nombre de contacts cumul�s
							NbContactsCum += NbContactsNet;

							// On incr�mente le nombre de contacts du jour
							if(IdxJour >= KEYPeriodeTablier.GetDateD()-DateD && IdxJour <= KEYPeriodeTablier.GetDateF()-DateD )
							{
								DATHypo.m_VolLecture += NbContactsNet;
								DATHypo.m_AudienceRef += AudienceJour;
							}
							DATHypo.m_DistributionJour.Item(IdxJour) = CMoteurPresse::GetDistribution(m_pDocCreuset->m_MoteurPresse, NbContactsCum);
						}
						else
						{
							//demememo
							// on recopie la distribution du jour pr�c�dent
							if (IdxJour >= 1)
								DATHypo.m_DistributionJour.Item(IdxJour) = DATHypo.m_DistributionJour.Item(IdxJour - 1);
						}

					}//for jours

				}//for DATformats

			}//if getcount

		}//fin if (DateD.IsValid() && DateF.IsValid())

	}//fin for terrains


	for(DATTemp.m_mapDATHypo.MoveFirst(); DATTemp.m_mapDATHypo.IsValid(); DATTemp.m_mapDATHypo.MoveNext())
	{
		JITPDATHypothese& DATHypo = DATTemp.m_mapDATHypo.GetItem();
		if (DATHypo.m_DistributionJour.GetCount()!=0)
		{
			JInt32 idxD = KEYPeriodeTablier.GetDateD() - KEYPeriodeAction.GetDateD() - 1;
			JInt32 idxF = KEYPeriodeTablier.GetDateF() - KEYPeriodeAction.GetDateD();
			if( idxD>=0 )
				DATHypo.m_Couverture = DATHypo.m_DistributionJour.Item(idxF).Item(1).AsJFlt64() - DATHypo.m_DistributionJour.Item(idxD).Item(1).AsJFlt64();
			else
				DATHypo.m_Couverture = DATHypo.m_DistributionJour.Item(idxF).Item(1).AsJFlt64();
			
		}
	}

}


void JITPTBLHypotheses::CalcDatInsert(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, const CInsertion * pInsertion , JITPDATHypoFormat & DATTemp, CKeyMoteur & keyMoteur, bool noBudget /*= false*/)
{
	m_pDocCreuset->m_MapIdFormatsSel.MoveTo(pInsertion->m_IdFormat);
	if(! m_pDocCreuset->m_MapIdFormatsSel.IsValid())
		return;
	
	JITPDATHypothese& DATHypo = DATTemp.GetDATHypoByFormat(pInsertion->m_IdFormat);
	JITPDATHypothese& DATHypoTotal = DATTemp.GetDATHypoTotal();
	
	bool bFormatActif = false;
	if (m_pDocCreuset->m_MapIdFormatsSel.GetItem() != 0)
		bFormatActif = true;
	JITPDATHypothese& DATHypoFormatActif = DATTemp.GetDATHypoFormatActif();
		


	if(!noBudget)
	{
		if (KEYPeriodeTablier.GetDateD() <= (pInsertion->m_DateD + (pInsertion->m_Duree - 1)) && KEYPeriodeTablier.GetDateF() >= pInsertion->m_DateD)
		{
			DATHypo.m_Budget += (JFlt64)pInsertion->m_Data.m_TarifP * pInsertion->m_NbInsertions;
			DATHypoTotal.m_Budget  += (JFlt64)pInsertion->m_Data.m_TarifP * pInsertion->m_NbInsertions;
			if(bFormatActif)
				DATHypoFormatActif.m_Budget  += (JFlt64)pInsertion->m_Data.m_TarifP * pInsertion->m_NbInsertions;

			if(pInsertion->m_Data.m_TarifP == 0)
			{
				DATHypo.m_budgetOK = false;
				DATHypoTotal.m_budgetOK = false;
				if(bFormatActif)
					DATHypoFormatActif.m_budgetOK  = false;
			}
		}

	}

	IElem* pElem = CRegroupements::GetInstance()->GetIElemByID( pInsertion->m_IdSupport );
	CKeyTerrain key;
	key.Set(m_pDocCreuset->m_IdSrcAudienceSel, m_pDocCreuset->m_MapIdTerrainsAudienceSel.GetKey());
	if (!CMoteurPresse::CanUseSupport(pElem, key, m_pDocCreuset->m_Offre))
	{
		DATHypo.m_ResultOK = false;
		DATHypoTotal.m_ResultOK = false;
		if(bFormatActif)
			DATHypoFormatActif.m_ResultOK  = false;
		return;
	}


	JDate DateD = KEYPeriodeAction.GetDateD();
	JDate DateF = KEYPeriodeAction.GetDateF();
	JInt32 NbJours = (DateF - DateD) + 1;
	
	// on r�cup�re la dur�e de l'insertion
	JInt32 Duree = pInsertion->m_Duree;

	// on teste si l'insertion est incluse dans la p�riode d'action
	if (DateD <= (pInsertion->m_DateD + (Duree - 1)) && DateF >= pInsertion->m_DateD)
	{
		JFlt64 VolLecture = 0.0;
		JFlt64 AudienceRef = 0.0;
		JFlt64 AudienceProba = 0.0;
		JFlt64 AudienceJour = 0.0;
		JInt32 PtEtal = 0;

		//calcul de l'audience
		m_pDocCreuset->m_MoteurPresse.CalcAudience(keyMoteur, pInsertion->m_IdSupport, VolLecture, AudienceRef, AudienceProba, PtEtal);

		if (m_pDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
			VolLecture = VolLecture/(JFlt64)Duree;
		else
			//on applique un r�gle de 3 pour obtenir le mm volume de lecture qq soit la duree r�elle de l'insertion
			VolLecture = (VolLecture * (JFlt64)PtEtal) / (JFlt64)Duree;

		AudienceJour = AudienceProba / (JFlt64)Duree; 
			
		// on calcule l'indice du jour de d�part de l'insertion
		JInt32 PrmJour = pInsertion->m_DateD - DateD;

		JInt32 nbPeriodAlpha = 1;
		JInt32 diviseur = 100;
		JVector<JInt32> vectAlpha;
		vectAlpha.SetCount(nbPeriodAlpha);
		vectAlpha.MoveFirst();
		vectAlpha.GetItem() = 100;

		if (pElem->IsTitre())
		{
			CKeyTerrain key;
			key.Set(m_pDocCreuset->m_IdSrcAudienceSel, m_pDocCreuset->m_MapIdTerrainsAudienceSel.GetKey());
			m_pDocCreuset->m_MoteurPresse.GetCoefAlpha(key, pElem->AsITitre()->GetIDAudience(), nbPeriodAlpha, vectAlpha, diviseur);
		}

		JInt32 MaxJourAlpha = PrmJour + (Duree * nbPeriodAlpha);
		JInt32 MaxJour = PrmJour + Duree;

		// on boucle sur tous les jours de l'insertion
		for (JInt32 Jour = PrmJour; Jour < MaxJourAlpha; Jour += 1)
		{
			// on teste si le jour est dans la p�riode d'action
			if (Jour >= 0 && Jour < NbJours)
			{

				JInt32 idx = (Jour-PrmJour)/Duree ;
				vectAlpha.MoveTo(idx);

				JFlt64 alpha = (JFlt64)vectAlpha.GetItem() / (JFlt64)diviseur;

				DATHypo.m_NbContactsJour.Item(Jour) += (VolLecture * alpha * (JFlt64)pInsertion->m_NbInsertions);	
				DATHypoTotal.m_NbContactsJour.Item(Jour) += (VolLecture * alpha * (JFlt64)pInsertion->m_NbInsertions);	
				if(bFormatActif)
				{
					DATHypoFormatActif.m_NbContactsJour.Item(Jour) += (VolLecture * alpha * (JFlt64)pInsertion->m_NbInsertions);	
					DATTemp.m_JoursActifs.SetAt(Jour,true);
				}

				if (Jour < MaxJour)
				{
					DATHypo.m_AudienceJour.Item(Jour) += (AudienceJour * (JFlt64)pInsertion->m_NbInsertions);
					DATHypoTotal.m_AudienceJour.Item(Jour) += (AudienceJour * (JFlt64)pInsertion->m_NbInsertions);
					if(bFormatActif)
						DATHypoFormatActif.m_AudienceJour.Item(Jour) += (AudienceJour * (JFlt64)pInsertion->m_NbInsertions);
					
					// Table des insertions 
					if(bFormatActif)
						DATTemp.m_JoursInsertExist.SetAt(Jour,true);
				}
			}
		}

		// on ajoute l'insertion � la maille

		m_pDocCreuset->m_MoteurPresse.SetMailleTemporelle(keyMoteur, DATHypo.m_maille, pInsertion->m_IdSupport, pInsertion->m_NbInsertions, pInsertion->m_DateD);
		m_pDocCreuset->m_MoteurPresse.SetMailleTemporelle(keyMoteur, DATHypoTotal.m_maille, pInsertion->m_IdSupport, pInsertion->m_NbInsertions, pInsertion->m_DateD);
		if (bFormatActif)
			m_pDocCreuset->m_MoteurPresse.SetMailleTemporelle(keyMoteur, DATHypoFormatActif.m_maille, pInsertion->m_IdSupport, pInsertion->m_NbInsertions, pInsertion->m_DateD);

	}//insertion ds periode action

}

/////////////////////////////////////
// la fonction pour tester le curseur

JBool JITPTBLHypotheses::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_Hypotheses.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JUnt32 & JITPTBLHypotheses::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_Hypotheses.GetKey());
}

const JITPDATHypoFormat & JITPTBLHypotheses::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_Hypotheses.GetItem());
}
JITPDATHypoFormat & JITPTBLHypotheses::GetItem()
{
	// on renvoie l'�l�ment du curseur
	return (m_Hypotheses.GetItem());
}
/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JITPTBLHypotheses::MoveTo(const JUnt32 & IdHypothese, JInt32 Move) const
{
	// on v�rifie la validit� de la cl�
//	if (!IdHypothese) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_Hypotheses.MoveTo(IdHypothese, Move);
}

JVoid JITPTBLHypotheses::MoveFirst() const
{
	// on d�place le curseur
	m_Hypotheses.MoveFirst();
}

JVoid JITPTBLHypotheses::MoveNext() const
{
	// on d�place le curseur
	m_Hypotheses.MoveNext();
}

JVoid JITPTBLHypotheses::MovePrev() const
{
	// on d�place le curseur
	m_Hypotheses.MovePrev();
}

JVoid JITPTBLHypotheses::MoveLast() const
{
	// on d�place le curseur
	m_Hypotheses.MoveLast();
}

///////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment courant

JVoid JITPTBLHypotheses::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_Hypotheses.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JITPTBLHypotheses::Swap(JITPTBLHypotheses & Source)
{
	// on permute les �l�ments
	m_Hypotheses.Swap(Source.m_Hypotheses);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JITPTBLHypotheses::Reset()
{
	// on r�initialise les �l�ments
	m_Hypotheses.Reset();
}

/////////////////
// le destructeur

JITPTBLHypotheses::~JITPTBLHypotheses()
{
	// on ne fait rien
}


void JITPTBLHypotheses::CalcDatHypoFActif(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, CPlanInsertions & PLNInsertions, JUnt32 idItem)
{
	JFpu Fpu;
	// on r�cup�re la p�riode d'action de l'�tude
	JDate DateD = KEYPeriodeAction.GetDateD();
	JDate DateF = KEYPeriodeAction.GetDateF();

	JITPDATHypothese& DATHypo = GetItem().GetDATHypoFormatActif();
	DATHypo.Reset();

	for (m_pDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
		m_pDocCreuset->m_MapIdTerrainsAudienceSel.IsValid();
		m_pDocCreuset->m_MapIdTerrainsAudienceSel.MoveNext())
	{
		
		CKeyMoteur keyMoteur;
		keyMoteur.Set(m_pDocCreuset->m_IdSrcAudienceSel, m_pDocCreuset->m_MapIdTerrainsAudienceSel.GetKey(), idItem);

		// on v�rifie la validit� de la p�riode d'action
		if (DateD.IsValid() && DateF.IsValid())
		{
			// on calcule le nombre de jours de la p�riode
			JInt32 NbJours = (DateF - DateD) + 1;
			DATHypo.InitNbJours(NbJours);

				// on boucle sur toutes les insertions
			for (PLNInsertions.MoveFirst_SFD(); PLNInsertions.IsValid_SFD(); PLNInsertions.MoveNext_SFD())
			{
				// on r�cup�re l'insertion courante
				CInsertion * pInsertion = PLNInsertions.GetItem_SFD();

				//on doit d'abord tester si on peut decrire le couplage
				CKeyTerrain keyTerrain;
				keyTerrain.Set(m_pDocCreuset->m_IdSrcAudienceSel, m_pDocCreuset->m_MapIdTerrainsAudienceSel.GetKey());
				if(pInsertion->IsCouplage() && CMoteurPresse::CanDescribeCouplage(pInsertion->m_IdSupport, keyTerrain))
				{
					//on peur d�crire le couplage on l'�value comme un mini plan
					CPlanInsertions & PLNInsertCpl = pInsertion->GetCouplageInsertions();
					for (PLNInsertCpl.MoveFirst_SFD(); PLNInsertCpl.IsValid_SFD(); PLNInsertCpl.MoveNext_SFD())
					{
						// on r�cup�re l'insertion courante
						const CInsertion * pInsertCpl = PLNInsertCpl.GetItem_SFD();
						CalcDatInsertFActif(KEYPeriodeAction, KEYPeriodeTablier, pInsertCpl, keyMoteur, true);
					}
					
					//on recup�re son budget
					if (KEYPeriodeTablier.GetDateD() <= (pInsertion->m_DateD + (pInsertion->m_Duree - 1)) && KEYPeriodeTablier.GetDateF() >= pInsertion->m_DateD)
					{
						m_pDocCreuset->m_MapIdFormatsSel.MoveTo(pInsertion->m_IdFormat);
						if(!m_pDocCreuset->m_MapIdFormatsSel.IsValid())
							continue;
						if(m_pDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
							DATHypo.m_Budget += (JFlt64)pInsertion->m_Data.m_TarifP * pInsertion->m_NbInsertions;
						if(pInsertion->m_Data.m_TarifP == 0)
						{
							if(m_pDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
								DATHypo.m_budgetOK = false;
						}
					}

				}
				else
				{
					CalcDatInsertFActif(KEYPeriodeAction, KEYPeriodeTablier, pInsertion, keyMoteur);
				}

			} // End for Insertions

			// si on a au - 1 insert
			if(PLNInsertions.GetCount())
			{
				//DATTemp.GetDATHypoFormatActif
				//for(DATTemp.m_mapDATHypo.MoveFirst(); DATTemp.m_mapDATHypo.IsValid(); DATTemp.m_mapDATHypo.MoveNext())
				{
					JFlt64 NbContactsCum = 0.0;
					JFlt64 NbContactsNet = 0.0;
					JFlt64 AudienceJour = 0.0;

					m_pDocCreuset->m_MoteurPresse.BuildDistributions(keyMoteur, DATHypo.m_maille);
				
					for (JInt32 IdxJour = 0; IdxJour < NbJours; IdxJour += 1)
					{
						// on initialise le nombre de jours actifs et de contacts cumul�s
						NbContactsNet = DATHypo.m_NbContactsJour.Item(IdxJour);
						AudienceJour = DATHypo.m_AudienceJour.Item(IdxJour);
						if (NbContactsNet > 0.0)
						{
							// on met � jour le nombre de contacts cumul�s
							NbContactsCum += NbContactsNet;
							// On incr�mente le nombre de contacts du jour
							if(IdxJour >= KEYPeriodeTablier.GetDateD()-DateD && IdxJour <= KEYPeriodeTablier.GetDateF()-DateD )
							{
								DATHypo.m_VolLecture += NbContactsNet;
								DATHypo.m_AudienceRef += AudienceJour;
							}
							DATHypo.m_DistributionJour.Item(IdxJour) = CMoteurPresse::GetDistribution(m_pDocCreuset->m_MoteurPresse, NbContactsCum);
						}
						else
						{
							//demememo
							// on recopie la distribution du jour pr�c�dent
							if (IdxJour >= 1)
								DATHypo.m_DistributionJour.Item(IdxJour) = DATHypo.m_DistributionJour.Item(IdxJour - 1);
						}

					}//for jours
				}//for DATformats
			}//if getcount

		
		}//fin if (DateD.IsValid() && DateF.IsValid())

	
	}//fin for terrains


	if (DATHypo.m_DistributionJour.GetCount()!=0)
	{
		JInt32 idxD = KEYPeriodeTablier.GetDateD() - KEYPeriodeAction.GetDateD() - 1;
		JInt32 idxF = KEYPeriodeTablier.GetDateF() - KEYPeriodeAction.GetDateD();
		if( idxD>=0 )
			DATHypo.m_Couverture = DATHypo.m_DistributionJour.Item(idxF).Item(1).AsJFlt64() - DATHypo.m_DistributionJour.Item(idxD).Item(1).AsJFlt64();
		else
			DATHypo.m_Couverture = DATHypo.m_DistributionJour.Item(idxF).Item(1).AsJFlt64();
		
	}

}


void JITPTBLHypotheses::CalcDatInsertFActif(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, const CInsertion * pInsertion, CKeyMoteur & keyMoteur, bool noBudget /*= false*/)
{
	m_pDocCreuset->m_MapIdFormatsSel.MoveTo(pInsertion->m_IdFormat);
	if(! m_pDocCreuset->m_MapIdFormatsSel.IsValid())
		return;
	
	if (m_pDocCreuset->m_MapIdFormatsSel.GetItem() == 0)
		return;

	JITPDATHypothese& DATHypoFormatActif = GetItem().GetDATHypoFormatActif();
		


	if(!noBudget)
	{
		if (KEYPeriodeTablier.GetDateD() <= (pInsertion->m_DateD + (pInsertion->m_Duree - 1)) && KEYPeriodeTablier.GetDateF() >= pInsertion->m_DateD)
		{
			DATHypoFormatActif.m_Budget  += (JFlt64)pInsertion->m_Data.m_TarifP * pInsertion->m_NbInsertions;

			if(pInsertion->m_Data.m_TarifP == 0)
				DATHypoFormatActif.m_budgetOK  = false;
		}

	}

	IElem* pElem = CRegroupements::GetInstance()->GetIElemByID( pInsertion->m_IdSupport );
	CKeyTerrain key;
	key.Set(m_pDocCreuset->m_IdSrcAudienceSel, m_pDocCreuset->m_MapIdTerrainsAudienceSel.GetKey());
	if (!CMoteurPresse::CanUseSupport(pElem, key, m_pDocCreuset->m_Offre))
	{
		DATHypoFormatActif.m_ResultOK  = false;
		return;
	}


	JDate DateD = KEYPeriodeAction.GetDateD();
	JDate DateF = KEYPeriodeAction.GetDateF();
	JInt32 NbJours = (DateF - DateD) + 1;
	GetItem().m_JoursActifs.SetSize(NbJours);
	GetItem().m_JoursInsertExist.SetSize(NbJours);
	
	// on r�cup�re la dur�e de l'insertion
	JInt32 Duree = pInsertion->m_Duree;

	// on teste si l'insertion est incluse dans la p�riode d'action
	if (DateD <= (pInsertion->m_DateD + (Duree - 1)) && DateF >= pInsertion->m_DateD)
	{
		JFlt64 VolLecture = 0.0;
		JFlt64 AudienceRef = 0.0;
		JFlt64 AudienceProba = 0.0;
		JFlt64 AudienceJour = 0.0;
		JInt32 PtEtal = 0;

		//calcul de l'audience
		m_pDocCreuset->m_MoteurPresse.CalcAudience(keyMoteur, pInsertion->m_IdSupport, VolLecture, AudienceRef, AudienceProba, PtEtal);

		if (m_pDocCreuset->m_MoteurPresse.GetType() == CMoteurPresse::PROBA)
			VolLecture = VolLecture/(JFlt64)Duree;
		else
			//on applique un r�gle de 3 pour obtenir le mm volume de lecture qq soit la duree r�elle de l'insertion
			VolLecture = (VolLecture * (JFlt64)PtEtal) / (JFlt64)Duree;

		AudienceJour = AudienceProba / (JFlt64)Duree; 
			
		// on calcule l'indice du jour de d�part de l'insertion
		JInt32 PrmJour = pInsertion->m_DateD - DateD;

		JInt32 nbPeriodAlpha = 1;
		JInt32 diviseur = 100;
		JVector<JInt32> vectAlpha;
		vectAlpha.SetCount(nbPeriodAlpha);
		vectAlpha.MoveFirst();
		vectAlpha.GetItem() = 100;

		if (pElem->IsTitre())
		{
			CKeyTerrain key;
			key.Set(m_pDocCreuset->m_IdSrcAudienceSel, m_pDocCreuset->m_MapIdTerrainsAudienceSel.GetKey());
			m_pDocCreuset->m_MoteurPresse.GetCoefAlpha(key, pElem->AsITitre()->GetIDAudience(), nbPeriodAlpha, vectAlpha, diviseur);
		}

		JInt32 MaxJourAlpha = PrmJour + (Duree * nbPeriodAlpha);
		JInt32 MaxJour = PrmJour + Duree;

		// on boucle sur tous les jours de l'insertion
		for (JInt32 Jour = PrmJour; Jour < MaxJourAlpha; Jour += 1)
		{
			// on teste si le jour est dans la p�riode d'action
			if (Jour >= 0 && Jour < NbJours)
			{

				JInt32 idx = (Jour-PrmJour)/Duree ;
				vectAlpha.MoveTo(idx);

				JFlt64 alpha = (JFlt64)vectAlpha.GetItem() / (JFlt64)diviseur;

				DATHypoFormatActif.m_NbContactsJour.Item(Jour) += (VolLecture * alpha * (JFlt64)pInsertion->m_NbInsertions);	
				GetItem().m_JoursActifs.SetAt(Jour,true);

				if (Jour < MaxJour)
				{
					DATHypoFormatActif.m_AudienceJour.Item(Jour) += (AudienceJour * (JFlt64)pInsertion->m_NbInsertions);
					
					// Table des insertions 
					GetItem().m_JoursInsertExist.SetAt(Jour,true);
				}
			}
		}

		// on ajoute l'insertion � la maille

		m_pDocCreuset->m_MoteurPresse.SetMailleTemporelle(keyMoteur, DATHypoFormatActif.m_maille, pInsertion->m_IdSupport, pInsertion->m_NbInsertions, pInsertion->m_DateD);

	}//insertion ds periode action

}

void JITPTBLHypotheses::UpdateTarif(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier)
{
	//m_Hypotheses.GetItem().m_mapDATHypo.GetItem().m_Budget
	
	for(m_Hypotheses.MoveFirst();m_Hypotheses.IsValid();m_Hypotheses.MoveNext())
	{
		//m_Hypotheses.GetKey();
		m_pDocCreuset->m_TblHypotheses.MoveTo(m_Hypotheses.GetKey());
		CPlanInsertions PLNInsertions = m_pDocCreuset->m_TblHypotheses.GetItem();
		// on boucle sur toutes les insertions
		for(m_Hypotheses.GetItem().m_mapDATHypo.MoveFirst();
			m_Hypotheses.GetItem().m_mapDATHypo.IsValid();
			m_Hypotheses.GetItem().m_mapDATHypo.MoveNext())
		{
			m_Hypotheses.GetItem().m_mapDATHypo.GetItem().m_budgetOK = true;
			m_Hypotheses.GetItem().m_mapDATHypo.GetItem().m_Budget = 0.0;
		}	
		
		for (PLNInsertions.MoveFirst_SFD(); PLNInsertions.IsValid_SFD(); PLNInsertions.MoveNext_SFD())
		{
			// on r�cup�re l'insertion courante
			CInsertion * pInsertion = PLNInsertions.GetItem_SFD();


			CalcBudgInsert(KEYPeriodeAction, KEYPeriodeTablier, pInsertion, m_Hypotheses.GetItem());

		} // End for Insertions
	}
}

void JITPTBLHypotheses::CalcBudgInsert(const JATPKEYPeriodeAction & KEYPeriodeAction, const JATPKEYPeriodeAction & KEYPeriodeTablier, const CInsertion * pInsertion , JITPDATHypoFormat & DATTemp)
{
	m_pDocCreuset->m_MapIdFormatsSel.MoveTo(pInsertion->m_IdFormat);
	if(! m_pDocCreuset->m_MapIdFormatsSel.IsValid())
		return;
	
	JITPDATHypothese& DATHypo = DATTemp.GetDATHypoByFormat(pInsertion->m_IdFormat);
	JITPDATHypothese& DATHypoTotal = DATTemp.GetDATHypoTotal();
	
	bool bFormatActif = false;
	if (m_pDocCreuset->m_MapIdFormatsSel.GetItem() != 0)
		bFormatActif = true;
	JITPDATHypothese& DATHypoFormatActif = DATTemp.GetDATHypoFormatActif();
		



	if (KEYPeriodeTablier.GetDateD() <= (pInsertion->m_DateD + (pInsertion->m_Duree - 1)) && KEYPeriodeTablier.GetDateF() >= pInsertion->m_DateD)
	{
		DATHypo.m_Budget += (JFlt64)pInsertion->m_Data.m_TarifP * pInsertion->m_NbInsertions;
		DATHypoTotal.m_Budget  += (JFlt64)pInsertion->m_Data.m_TarifP * pInsertion->m_NbInsertions;
		if(bFormatActif)
			DATHypoFormatActif.m_Budget  += (JFlt64)pInsertion->m_Data.m_TarifP * pInsertion->m_NbInsertions;

		if(pInsertion->m_Data.m_TarifP == 0)
		{
			DATHypo.m_budgetOK = false;
			DATHypoTotal.m_budgetOK = false;
			if(bFormatActif)
				DATHypoFormatActif.m_budgetOK  = false;
		}
	}

}