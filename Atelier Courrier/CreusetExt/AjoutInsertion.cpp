#include "StdAfx.h"
#include ".\ajoutinsertion.h"
#include "JATPDocCreuset.h"



bool CAjoutInsertion::AjoutInsertionSuccessif(LONG IdSupport,LONG IdFormat,LONG IdHypothese, JATPDocCreuset * pATPDocCreuset)
{
	// Voir si hypothese valide
	if(IdHypothese <= 0 || IdHypothese >= pATPDocCreuset->m_TblHypotheses.GetCount())
		return false;

	// Si nouvelle hypothèse active, on l'active
	if (pATPDocCreuset->m_IdxHypotheseActif != IdHypothese)
	{
		pATPDocCreuset->m_IdxHypotheseActif	= IdHypothese;
		pATPDocCreuset->m_MapAffHypothese.ResetSet(IdHypothese);
	}

	// on récupère les insertions de l'hypothèse courante
	pATPDocCreuset->m_TblHypotheses.MoveTo(IdHypothese);
	CPlanInsertions & PLNInsertions = pATPDocCreuset->m_TblHypotheses.GetItem();

	//pATPDocCreuset->SubstitutFormat(IdSupport, IdFormat);

	return CAjoutInsertion::AjoutInsertionSuccessif(IdSupport, IdFormat, PLNInsertions, pATPDocCreuset->m_KEYPeriodeAction.GetDateD(), pATPDocCreuset->m_KEYPeriodeAction.GetDateF(),1,pATPDocCreuset);
}

bool CAjoutInsertion::AjoutInsertionSuccessif(LONG IdSupport,LONG IdFormat, CPlanInsertions & PLNInsertions, JDate &DebPeriode, JDate &FinPeriode, int nbInsert, JATPDocCreuset * pATPDocCreuset)
{
	// Définition insertion
	CInsKeySDF	KeySDF;
	CInsertion  Insertion;
	
	// Recherche insertion
	KeySDF.m_DateD		= FinPeriode;
	KeySDF.m_IdFormat   = IdFormat;
	KeySDF.m_IdParent   = 0;
	KeySDF.m_IdSupport  = IdSupport;
	Insertion = KeySDF;

	// 1ere date possible insertion
	JDate		DatePossible;

	// Récup durée insertion
	JUnt32 Tarif		= 0;
	JUnt32 Duree		= 0;
	JDate DateReal		= Insertion.m_DateD;
	CTarifs * pTarifs	= CTarifs::GetInstance();
	if (pTarifs == NULL)
		return false;

	//////On recherche la dernière insertion sur la période
	PLNInsertions.MoveTo_SDF(KeySDF, -1);
	int nbIns = -1;
	
	if(PLNInsertions.IsValid_SDF()
		&& PLNInsertions.GetItem_SDF()->m_IdSupport == KeySDF.m_IdSupport
//		&& PLNInsertions.GetItem_SDF()->m_IdFormat == KeySDF.m_IdFormat
		&& PLNInsertions.GetItem_SDF()->m_IdParent == KeySDF.m_IdParent)
	{
		//on l'a trouvé
		DatePossible = PLNInsertions.GetItem_SDF()->m_DateD;
		DatePossible += PLNInsertions.GetItem_SDF()->m_Duree;
		nbIns = PLNInsertions.GetItem_SDF()->m_NbInsertions;
	}
	else
		//pas d'insertion on prend la date de début de période
		DatePossible = DebPeriode;

	////on cherche la parution suivante
	JInt32 IdFormatBis = IdFormat;
	pATPDocCreuset->SubstitutFormat(IdSupport, IdFormatBis);

	// Recherche nouveau tarif par date, en testant les tarifs manuels s'ils existent
	RETTRF ret = pTarifs->GetNextTarifByDateNew(IdSupport, IdFormatBis, pATPDocCreuset->m_MapTarifManuel,
												DatePossible,
												DebPeriode, FinPeriode,
												DateReal, Duree, Tarif);
	

	//si on sort de la période on reprend du début
	if(ret == TRFOUT)
	{
		DatePossible = DebPeriode;
		JDate dateTmp;
		JUnt32 DureeTmp;
		JUnt32 TarifTmp = 0;

		// Recherche nouveau tarif par date, en testant les tarifs manuels s'ils existent
		ret = pTarifs->GetNextTarifByDateNew(IdSupport, IdFormatBis, pATPDocCreuset->m_MapTarifManuel, 
											 DatePossible,
											 DebPeriode, FinPeriode,
											 dateTmp, DureeTmp, TarifTmp);
		
		//on va boucler sur toutes les parutions pour trouver celle avec le moins d'insertion
		nbIns = 32000;
		do
		{
			
			KeySDF.m_DateD = dateTmp;
			PLNInsertions.MoveTo_SDF(KeySDF,-1);
			if(PLNInsertions.IsValid_SDF()
				&& PLNInsertions.GetItem_SDF()->m_IdSupport == KeySDF.m_IdSupport
				//&& PLNInsertions.GetItem_SDF()->m_IdFormat == KeySDF.m_IdFormat
				&& PLNInsertions.GetItem_SDF()->m_IdParent == KeySDF.m_IdParent)
			{
  				if( PLNInsertions.GetItem_SDF()->m_NbInsertions < nbIns)
				{
					DateReal = dateTmp;
					Duree = DureeTmp;
					Tarif = TarifTmp;
					nbIns = PLNInsertions.GetItem_SDF()->m_NbInsertions;
				}
			}
			else//on a trouvé un trou, on le complète
			{
				DateReal = dateTmp;
				Duree = DureeTmp;
				Tarif = TarifTmp;
				break;
			}

			DatePossible = DatePossible+DureeTmp;
			/*
			ret = pTarifs->GetNextTarifByDateNew(IdSupport, IdFormatBis, DatePossible,
				DebPeriode, FinPeriode,
				dateTmp, DureeTmp, TarifTmp);
			*/

			// Recherche nouveau tarif par date, en testant les tarifs manuels s'ils existent
			ret = pTarifs->GetNextTarifByDateNew(IdSupport, IdFormatBis,
												 pATPDocCreuset->m_MapTarifManuel,
												 DatePossible,
												 DebPeriode, FinPeriode,
												 dateTmp, DureeTmp, TarifTmp);

		}while (ret != TRFOUT);

	}

	if(ret != NOTRF)
	{
		KeySDF.m_DateD		= DateReal;
		KeySDF.m_IdFormat   = IdFormat;
		KeySDF.m_IdParent   = 0;
		KeySDF.m_IdSupport  = IdSupport;
		Insertion = KeySDF;
		
		Insertion.m_NbInsertions += nbInsert;

		Insertion.m_Duree = Duree;
		Insertion.m_Data.m_Duree = Duree;
		Insertion.m_Data.m_TarifN = Tarif;
		Insertion.m_Data.m_TarifP = Tarif;


		if(IsCouplage(Insertion.m_IdSupport))
		{
			if (!AddInsertionCouplage(Insertion,DebPeriode,FinPeriode, pATPDocCreuset, true))
				return false;
		}
		
		if(PLNInsertions.AddInsertion(Insertion,DebPeriode,FinPeriode) != NULL)
			return true;//la seule sortie true.
	}



	return false;
}

bool CAjoutInsertion::IsCouplage(JUnt32 IdSupport)
{
	// Add all children
	CRegroupements * pReg = CRegroupements::GetInstance();

	// Récup id support
	IElem *pElem = pReg->GetIElemByID(IdSupport);

	if(!pElem)
		return false;

	// Test si c'est un couplage
	return pElem->IsCouplage();
}

bool CAjoutInsertion::AddInsertionCouplage(CInsertion & Insertion, JDate DateMin, JDate DateMax, JATPDocCreuset *pATPDocCreuset, bool bNext/*=false*/)
{
	// Add all children
	CRegroupements * pReg = CRegroupements::GetInstance();
	CTarifs * pTarifs	= CTarifs::GetInstance();

	if (pReg == NULL || pTarifs == NULL)
		return false;

	// Récup id support
	IElem *pElem = pReg->GetIElemByID(Insertion.m_IdSupport);

	JDate MinGlobal = DateMax;
	JDate MaxGlobal = Insertion.m_DateD;

	if(!pElem && !pElem->IsCouplage())
		return false;

	// Crée liste des supports couplage
	JList<IElem *> ListSupCoupl;

	pElem->AsCouplage()->GetFils(ListSupCoupl);

	CPlanInsertions &PlnCouplage = Insertion.GetCouplageInsertions(); 
	CPlanInsertions PlanTmp;
	// Find DateDebut Couplage & DateFin Couplage
	for(ListSupCoupl.MoveFirst(); ListSupCoupl.IsValid(); ListSupCoupl.MoveNext())
	{
		// Récup le titre couplage
		IElem * pIElem = ListSupCoupl.GetItem();
		if(pIElem)
		{
			JUnt32 Tarif = 0;
			JUnt32 Duree = 0;
			JDate DateReal = Insertion.m_DateD;
			JDate Max = Insertion.m_DateD;
			for (int i = 0; i< pIElem->AsTitreCouplage()->GetNbInsert(); i++)
			{

				// Récup date départ insertion et durée
				CInsertion InsFils;
				InsFils.m_IdSupport =	pIElem->GetID();
				InsFils.m_IdFormat	=	Insertion.m_IdFormat;
				InsFils.m_IdParent	=	Insertion.m_IdSupport;
				InsFils.m_DateD		=	Max;
				InsFils.m_NbInsertions = 1;
				InsFils.m_Data.m_TarifN = 0;

				////on cherche la parution suivante pour ce support
				RETTRF ret;
				JInt32 IdFormatBis = Insertion.m_IdFormat;
				pATPDocCreuset->SubstitutFormat(pIElem->GetID(), IdFormatBis);

				if(i==0 && bNext==false)
				{
					// Recherche nouveau tarif par date, en testant les tarifs manuels s'ils existent
					ret = pTarifs->GetTarifByDateNew(pIElem->GetID(), IdFormatBis, pATPDocCreuset->m_MapTarifManuel,
													 InsFils.m_DateD,
													 DateMin, DateMax,
													 DateReal, Duree, Tarif);
				}
				else
				{
					// Recherche nouveau tarif par date, en testant les tarifs manuels s'ils existent
					ret = pTarifs->GetNextTarifByDateNew(pIElem->GetID(), IdFormatBis, pATPDocCreuset->m_MapTarifManuel,
														 InsFils.m_DateD,
														 DateMin, DateMax,
														 DateReal, Duree, Tarif);
				}

				if(ret!= NOTRF && ret != TRFOUT)
				{
					InsFils.m_Duree = Duree;
					InsFils.m_Data.m_TarifP = Tarif;
					InsFils.m_Data.m_TarifN = Tarif;
					InsFils.m_DateD = DateReal;
					PlanTmp.AddInsertion(InsFils, DateMin, DateMax);
					Max= DateReal + Duree;
					if(DateReal<MinGlobal)
						MinGlobal = DateReal;
					if(DateReal+Duree>MaxGlobal)
						MaxGlobal = DateReal+Duree;
				}
				else
					return false;

			}//for
		}//if
		else
			return false;


	}//for

	Insertion.m_DateD = MinGlobal;
	Insertion.m_Duree = MaxGlobal-MinGlobal;
	Insertion.m_Data.m_Duree = Insertion.m_Duree;
	Insertion.m_NbInsertions = 1;
	
	for (PlanTmp.MoveFirst_SDF();PlanTmp.IsValid_SDF(); PlanTmp.MoveNext_SDF())
		PlnCouplage.AddInsertion( *PlanTmp.GetItem_SDF(), DateMin, DateMax);

	return true;
}


bool CAjoutInsertion::SupprLastInsertion(LONG IdSupport, LONG IdFormat, LONG IdHypothese, JATPDocCreuset * pATPDocCreuset)
{
	if (pATPDocCreuset)
	{
		// Si nouvelle hypothèse active, on l'active
		if (pATPDocCreuset->m_IdxHypotheseActif != IdHypothese)
		{
			pATPDocCreuset->m_IdxHypotheseActif	= IdHypothese;
			pATPDocCreuset->m_MapAffHypothese.ResetSet(IdHypothese);
			// Update affichage insertions
			pATPDocCreuset->UpdateAllViews(UPDATE_AFF_CREUSET);
		}

		// on récupère les insertions de l'hypothèse courante
		pATPDocCreuset->m_TblHypotheses.MoveTo(IdHypothese);
		CPlanInsertions & PLNInsertions = pATPDocCreuset->m_TblHypotheses.GetItem();

		return PLNInsertions.SuppLastInsertion(IdSupport, IdFormat,pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
															pATPDocCreuset->m_KEYPeriodeAction.GetDateF() );
	}
	return false;
}


