//
// Fichier: JITPTBLEchelle.cpp
// Auteur:  Alain Chambard & Eddie Gent
// Date:    28/08/2003
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JATPDocCreuset.h"
#include "JITPTBLEchelle.h"

#include "MoteurPresse.h"

////////////////////
// les constructeurs

JITPTBLEchelle::JITPTBLEchelle(JATPDocCreuset *pDoc)
{
	m_pDocCreuset = pDoc;
}

JITPTBLEchelle::JITPTBLEchelle(const JITPTBLEchelle & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

/////////////////////////////////////////
// l'op�rateur pour recopier les �l�ments

JITPTBLEchelle & JITPTBLEchelle::operator =(const JITPTBLEchelle & Source)
{
	// on g�n�re une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////////
// la fonction pour r�cup�rer le nombre d'�l�ments

JInt32 JITPTBLEchelle::GetCount() const
{
	// on renvoie le nombre d'�l�ments
	return (m_Echelles.GetCount());
}

////////////////////////////////////////
// la fonction pour ajouter les �l�ments

JVoid JITPTBLEchelle::Add(const JUnt32 & IdSupport, const JATPKEYEtude & KEYEtude, const JATPKEYPeriodeAction & KEYPeriodeAction, JITPTBLHypotheses & ITRHypotheses, JBool Move, JBool RecalculApport)
{
	// on initialise le Fpu
	JFpu Fpu;

	// on v�rifie la validit� de la cl�
	if (!IdSupport)
		throw JInvalidCall::GetInstance();

	// on cr�e les r�sultats
	JUnt32			Tarif			= 0;
	JBool			TarifManuel		= false;
	JInt32			Periodicite		= 0;
	JFlt64			AudienceRef		= 0.0;
	JFlt64			AudienceProba	= 0.0;
	JFlt64			VolLecture		= 0.0;
	JFlt64			Effectif		= 0.0;
	JFlt64			IndAffinite		= -1;
	JFlt64			PrcAffinite		= -1;
	JFlt64			Apport			= -1;	
	JFCDistribution	Distribution;

	// R�cup�re tarif
	GetTarif(IdSupport,Tarif,TarifManuel,Periodicite,KEYEtude.GetDateD(),KEYEtude.GetDateF());

	// Calcul des apports si actif
	if (RecalculApport == true && m_pDocCreuset->m_ApportARecalculer == true)
	{
		// Active sablier si calcul des apports
		CWaitCursor Wait;

		// Index support dans plateau
		JInt32 IndexSupport = -1;

		// Init pourcentage
		m_pDocCreuset->m_PrctApport = 0.0;

		// Recherche index dans plateau
		for (JInt32 Idx = 0; Idx < m_pDocCreuset->m_EchelleSupports.GetCount(); Idx++) 
		{
			JUnt32 IdSupportEch = m_pDocCreuset->m_EchelleSupports.Item(Idx);
			if (IdSupportEch == IdSupport)
			{
				JUnt32 IdFormatCur = 0;
				for (m_pDocCreuset->m_MapIdFormatsSel.MoveFirst(); m_pDocCreuset->m_MapIdFormatsSel.IsValid(); m_pDocCreuset->m_MapIdFormatsSel.MoveNext())
				{
					JUnt32 EtatF = m_pDocCreuset->m_MapIdFormatsSel.GetItem();
				
					if(!(EtatF & 0x0001))
						continue;

					IdFormatCur = m_pDocCreuset->m_MapIdFormatsSel.GetKey();
				}

				// Cl� insertion
				CInsKeySFD KeySFD;
				KeySFD.m_IdSupport			= IdSupport;
				KeySFD.m_IdParent			= 0;
				KeySFD.m_IdFormat			= IdFormatCur;
				KeySFD.m_DateD				= m_pDocCreuset->m_KEYEtude.GetDateD();
				
				CInsertion Insertion;
				Insertion = KeySFD;


				// Pour les apports si id format = 0, on le force au premier format dispo
				if (Insertion.m_IdFormat == 0)
				{
					m_pDocCreuset->m_MapIdFormatsSel.MoveFirst();
					if (m_pDocCreuset->m_MapIdFormatsSel.IsValid())
						Insertion.m_IdFormat = m_pDocCreuset->m_MapIdFormatsSel.GetKey();
				}

				// Calcul apport avec nvlle insertion
				if (Insertion.m_IdFormat != 0)
					m_pDocCreuset->CalculApport(ITRHypotheses, Insertion, Apport);

				// Calcul pourcentage
				m_pDocCreuset->m_PrctApport = (JFlt64)(Idx+1) / (JFlt64) (m_pDocCreuset->m_EchelleSupports.GetCount()) * 100.0;

				// BUG BUG BUGm_pDocCreuset->UpdateAllViews(UPDATE_SEL_FORMAT);
				// m_pDocCreuset->UpdateAllViews(UPDATE_CALC_ECHELLE);
				m_pDocCreuset->UpdateAllViews(UPDATE_AFF_HYPO);
				

				// Arr�t, traitement support effectu�
				break;	
			}
		}
	}

	for( m_pDocCreuset->m_MapIdTerrainsAudienceSel.MoveFirst();
		 m_pDocCreuset->m_MapIdTerrainsAudienceSel.IsValid();
		 m_pDocCreuset->m_MapIdTerrainsAudienceSel.MoveNext())
	{

		CKeyMoteur keyMoteur;
		keyMoteur.Set(m_pDocCreuset->m_IdSrcAudienceSel, m_pDocCreuset->m_MapIdTerrainsAudienceSel.GetKey(), m_pDocCreuset->m_IdItemSel);
		
		Effectif = m_pDocCreuset->m_MoteurPresse.GetEffectif(keyMoteur);
		JInt32 PtEtal = 0;
		////////////////////////
		if( m_pDocCreuset->m_MoteurPresse.CalcAudience(keyMoteur, IdSupport, VolLecture, AudienceRef, AudienceProba, PtEtal, true) )
		{
			////////////////////////

			// on calcule la distribution � ce jour
			JFCMaille maille;
			
			m_pDocCreuset->m_MoteurPresse.SetMailleEchelle(keyMoteur, maille, IdSupport, 1);

			m_pDocCreuset->m_MoteurPresse.BuildDistributions(keyMoteur, maille);

			Distribution = CMoteurPresse::GetDistribution(m_pDocCreuset->m_MoteurPresse, VolLecture);


			/////////////////////////////////////////////////////////////
			//////////////////Calcul de l'affinit�


			JFlt64 EffectifAffinite = 0.0;
			JFlt64 VolAffinite = 0.0;
			JFlt64 AudienceAffiniteRef = 0.0;
			JFlt64 AudienceAffiniteProba = 0.0;
			JInt32 PtEtal = 0;

			keyMoteur.SetItem(m_pDocCreuset->m_Offre.m_IdItemAffinite);
			EffectifAffinite = m_pDocCreuset->m_MoteurPresse.GetEffectif(keyMoteur);

			m_pDocCreuset->m_MoteurPresse.CalcAudience(keyMoteur, IdSupport, VolAffinite, AudienceAffiniteRef, AudienceAffiniteProba, PtEtal, true);

			JFlt64 GRP = AudienceProba / Effectif;
			JFlt64 GRPAffinite = 0.0;
			if (EffectifAffinite >= 0.0)
				GRPAffinite = AudienceAffiniteProba / EffectifAffinite;
			if (GRPAffinite >= 0.0)
				IndAffinite = (GRP / GRPAffinite) * 100.0;
			if (AudienceAffiniteRef >= 0.0)
				PrcAffinite = (AudienceRef / AudienceAffiniteRef) * 100.0;
		}


	}

	
	// on r�cup�re la p�riode d'action de l'�tude
	JDate DateD = KEYPeriodeAction.GetDateD();
	JDate DateF = KEYPeriodeAction.GetDateF();

	// on v�rifie la validit� de la p�riode d'action
	if (!DateD.IsValid() || !DateF.IsValid())
	{
		// on r�cup�re la p�riode de l'�tude
		DateD = KEYEtude.GetDateD();
		DateF = KEYEtude.GetDateF();
	}

	// on ajoute le nouveau support dans �chelle
	JITPDATEchelle & DATEchelle = m_Echelles.Add(IdSupport, Move);

	// on initialise l'�chelle
	DATEchelle.m_Tarif			= Tarif;
	DATEchelle.m_TarifManuel	= TarifManuel;	
	DATEchelle.m_Periodicite	= Periodicite;
	DATEchelle.m_AudienceRef    = AudienceRef;
	DATEchelle.m_AudienceProba  = AudienceProba;
	DATEchelle.m_VolLecture		= VolLecture;
	DATEchelle.m_Effectif		= Effectif;
	DATEchelle.m_IndAffinite	= IndAffinite;
	DATEchelle.m_PrcAffinite    = PrcAffinite;
	DATEchelle.m_Distribution	= Distribution;
	DATEchelle.m_Apport			= Apport;

}

/////////////////////////////////////
// Function for the Tarif Supports
bool JITPTBLEchelle::GetTarif(JUnt32 IdSupport, JUnt32 & Tarif, JBool &TarifManuel, JInt32 & Periodicite,JDate DateDebut,JDate DateFin)
{
	Tarif = 0;
	CRegroupements * pReg = CRegroupements::GetInstance();
	CTarifs * pTarifs = CTarifs::GetInstance();

	// R�cup id support
	IElem *pElem = pReg->GetIElemByID(IdSupport);

	if(pElem && pTarifs)
	{
		// on recupere la periodicit�
		if (pElem->IsTitre())
			Periodicite = pElem->AsITitre()->GetPeriodicite();

		// Test si c'est un couplage
	/*	if(pElem->IsCouplage())
		{
			Periodicite = 0;
		
			JDate  DateExact;
			JUnt32 Duree;
			JUnt32 TarifFormat;
			JUnt32 IdFormat;
			for(m_pDocCreuset->m_MapIdFormatsSel.MoveFirst();
				m_pDocCreuset->m_MapIdFormatsSel.IsValid();
				m_pDocCreuset->m_MapIdFormatsSel.MoveNext())
			{
				if(m_pDocCreuset->m_MapIdFormatsSel.GetItem() & 0x0001)
				{
					IdFormat = m_pDocCreuset->m_MapIdFormatsSel.GetKey();
					if(pTarifs->GetTarifByDate(pElem->GetID(),IdFormat,DateDebut,DateFin,DateExact,Duree,TarifFormat) > 0)
					{
						Tarif+=TarifFormat;
						//Periodicite = 0;
					}
				}
			}
			if (Tarif!=0)
				return true;

			
			// Cr�e liste des supports couplage
			JList<IElem *> ListSupCoupl;

			pElem->AsCouplage()->GetFils(ListSupCoupl);

			for(ListSupCoupl.MoveFirst(); ListSupCoupl.IsValid(); ListSupCoupl.MoveNext())
			{
				JDate Date = DateDebut; 
				// R�cup le titre couplage fixe
				IElem * pIElem = pIElem = ListSupCoupl.GetItem();
				
				if(pIElem)
				{
					//if (pElem->IsTitre())
					//	Periodicite = pIElem->AsITitre()->GetPeriodicite();
					JUnt32 NbInsert = pIElem->AsTitreCouplage()->GetNbInsert();
					JFlt32 Abbattement = 0.0;

					if(m_pDocCreuset)
					{
						JDate	DateExact;
						JUnt32	TarifFormat;
						JUnt32	IdFormat;

						for(m_pDocCreuset->m_MapIdFormatsSel.MoveFirst();
							m_pDocCreuset->m_MapIdFormatsSel.IsValid();
							m_pDocCreuset->m_MapIdFormatsSel.MoveNext())
						{
							if(m_pDocCreuset->m_MapIdFormatsSel.GetItem() & 0x0001)
							{
								IdFormat = m_pDocCreuset->m_MapIdFormatsSel.GetKey();
								// Tarif recherch� avec Date la plus proche et format
								//if(pTarifs->GetTarifByVersion(pIElem->GetID(),IdFormat,DateDebut,TarifFormat))
								if(pTarifs->GetTarifByDate(pIElem->GetID(),IdFormat,DateDebut,DateFin,DateExact,Duree,TarifFormat) > 0)
									Tarif += ((JFlt32)TarifFormat*(1.0-(Abbattement/ 100.0)))*NbInsert; 
							}
						}
					}
				}
				else
					return false;
			}
			Periodicite = 0;
			return true;
		}
		else*/
		{
			// Supports Simple
			if(m_pDocCreuset)
			{
				JDate  DateExact;
				JUnt32 Duree;
				JUnt32 TarifFormat;
				JInt32 IdFormat;
				JUnt32 IdTitre;
				if(pElem->IsTitreApparie())
					IdTitre = pElem->AsTitreApparie()->GetIDTitre();
				else
					IdTitre = IdSupport;

				JInt32 NbTarifs = 0;
				for(m_pDocCreuset->m_MapIdFormatsSel.MoveFirst();
					m_pDocCreuset->m_MapIdFormatsSel.IsValid();
					m_pDocCreuset->m_MapIdFormatsSel.MoveNext())
				{
					if(m_pDocCreuset->m_MapIdFormatsSel.GetItem() >=2)
					{
						IdFormat = m_pDocCreuset->m_MapIdFormatsSel.GetKey();
						m_pDocCreuset->SubstitutFormat(IdTitre, IdFormat);

						// Attention d�sormais on v�rifie si il y a des tarifs manuels
						CKeyTarifManuel KeyTarifManuel;
						// attention prendre idsupport surtout pour les titres appari�s -- IdTitre;
						KeyTarifManuel.m_IdSupport  = IdSupport; 
						KeyTarifManuel.m_IdFormat   = IdFormat;
						m_pDocCreuset->m_MapTarifManuel.MoveTo(KeyTarifManuel);
						if (m_pDocCreuset->m_MapTarifManuel.IsValid())
						{
							// R�cup�re le tarif manuel
							JFlt32 TarifM = m_pDocCreuset->m_MapTarifManuel.GetItem();
							if(TarifM > 0)
								Tarif += (JUnt32)TarifM;

							// Positionne le flag TarifManuel
							TarifManuel = true;
						}
						// Tarif recherch� avec Date la plus proche et format
						// Recherche nouveau tarif par date, en testant les tarifs manuels s'ils existent
						else if(pTarifs->GetTarifByDate(IdTitre, IdFormat, m_pDocCreuset->m_MapTarifManuel, DateDebut, DateFin, DateExact, Duree, TarifFormat) > 0)
						{
							if(TarifFormat > 0)
								Tarif += TarifFormat;
						}
						NbTarifs += 1;
					}
				}
				if (NbTarifs != 1)
					Tarif = 0;
			}
		}
	}
	return true;
}

/////////////////////////////////////
// Function to set apports
void JITPTBLEchelle::SetApport(JUnt32 IdSupport, JFlt64 Apport) 
{
	// on d�place le curseur
	m_Echelles.MoveTo(IdSupport);
	if (m_Echelles.IsValid())
	{
		JITPDATEchelle & DATEchelle = m_Echelles.GetItem();
		DATEchelle.m_Apport = Apport;
	}
}	

JBool JITPTBLEchelle::IsValid() const
{
	// on renvoie l'�tat du curseur
	return (m_Echelles.IsValid());
}

////////////////////////////////////////////////////
// les fonctions pour r�cup�rer l'�l�ment du curseur

const JUnt32 & JITPTBLEchelle::GetKey() const
{
	// on renvoie l'�l�ment du curseur
	return (m_Echelles.GetKey());
}

const JITPDATEchelle & JITPTBLEchelle::GetItem() const
{
	// on renvoie l'�l�ment du curseur
	return (m_Echelles.GetItem());
}

/////////////////////////////////////////
// les fonctions pour d�placer le curseur

JVoid JITPTBLEchelle::MoveTo(const JUnt32 & IdSupport, JInt32 Move) const
{
	// on v�rifie la validit� de la cl�
	if (!IdSupport) throw JInvalidCall::GetInstance();

	// on d�place le curseur
	m_Echelles.MoveTo(IdSupport, Move);
}

JVoid JITPTBLEchelle::MoveFirst() const
{
	// on d�place le curseur
	m_Echelles.MoveFirst();
}

JVoid JITPTBLEchelle::MoveNext() const
{
	// on d�place le curseur
	m_Echelles.MoveNext();
}

JVoid JITPTBLEchelle::MovePrev() const
{
	// on d�place le curseur
	m_Echelles.MovePrev();
}

JVoid JITPTBLEchelle::MoveLast() const
{
	// on d�place le curseur
	m_Echelles.MoveLast();
}

///////////////////////////////////////////////
// la fonction pour supprimer l'�l�ment courant

JVoid JITPTBLEchelle::Remove(JInt32 Move)
{
	// on supprime l'�l�ment courant
	m_Echelles.Remove(Move);
}

/////////////////////////////////////////
// la fonction pour permuter les �l�ments

JVoid JITPTBLEchelle::Swap(JITPTBLEchelle & Source)
{
	// on permute les �l�ments
	m_Echelles.Swap(Source.m_Echelles);
}

////////////////////////////////////////
// la fonction pour lib�rer les �l�ments

JVoid JITPTBLEchelle::Reset()
{
	// on r�initialise les �l�ments
	m_Echelles.Reset();
}

/////////////////
// le destructeur

JITPTBLEchelle::~JITPTBLEchelle()
{
	// on ne fait rien
}


void JITPTBLEchelle::UpdateTarifs(const JATPKEYEtude & KEYEtude)
{
	for(m_Echelles.MoveFirst();m_Echelles.IsValid();m_Echelles.MoveNext())
	{
		JUnt32			Tarif		= 0;
		JBool			TarifManuel = false;
		JInt32			Periodicite = 0;

		// R�cup�re les infos tarifs
		GetTarif(m_Echelles.GetKey(), Tarif, TarifManuel, Periodicite, KEYEtude.GetDateD(), KEYEtude.GetDateF());
		m_Echelles.GetItem().m_Tarif = Tarif;
		m_Echelles.GetItem().m_TarifManuel = TarifManuel;
	}
}

