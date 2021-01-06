// Tarifs.cpp: implementation of the CTarifs class.
//
//////////////////////////////////////////////////////////////////////


#include "StdAfx.h"
#include ".\tarifs.h"
#include "JSRCPresse.h"
#include "AllTypeOfNode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

AFX_EXT_DATA CTarifs CTarifs::m_Instance;

CTarifs::CTarifs(void)
{
}

CTarifs::~CTarifs(void)
{
}

CTarifs* CTarifs::GetInstance()
{
	static JBool bLoaded = false;

	if (!bLoaded)
	{
		JStringEx gg;

		m_Instance.m_TarifsManuels.Load();

		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "TarifsPath", 0));

		gg.Reset();
		gg.Append("/Parutions");
		m_Instance.m_Parutions.Load(gg);

		gg.Reset();
		gg.Append("/Tarifs");
		m_Instance.m_TarifsAutos.Load(gg);
			
		gg.Reset();
		gg.Append("/Versions");
		m_Instance.m_VersionsTarifsAutos.Load(gg);

		SetCurrentDirectory(AfxGetApp()->GetProfileString("Data", "SourcesPath", 0));
		bLoaded = true;
	}
	
	return & m_Instance;
}

JBool CTarifs::SaveTarifsManuels()
{
	return  m_Instance.m_TarifsManuels.Save();
}


JBool CTarifs::GetTarifByVersion( JUnt32 idSupport, JUnt32 idFormat, JDate date, JUnt32 & tarif) const
{
	JUnt32 version;
	m_VersionsTarifsAutos.FindVersion( idSupport, date, version );
	return m_TarifsAutos.GetTarif( idSupport, idFormat, version, tarif );
}

JInt32 CTarifs::GetStatutTarif( JUnt32 idSupport, JUnt32 idFormat, JDate dateClick, JDate dateLimite) const
{
	JDate dateExact;
	JUnt32 duree;
	JUnt32 tarif;
	return GetTarifByDate( idSupport, idFormat, dateClick, dateLimite, dateExact, duree, tarif);
}

//ret<0 echec
//ret>0 reussite
JInt32 CTarifs::GetTarifByDate( JUnt32 idSupport, JUnt32 idFormat, JDate dateClick, JDate dateLimite, JDate& dateExact, JUnt32& duree, JUnt32 & tarif) const
{
	//Recherche de la parution
	JInt32 nRet = 0;
	JBool bRet;
	JBool bRetVer = true;
	JUnt32 version;

	tarif = 0;

	if (dateClick < dateLimite)
	{
		bRet = m_Parutions.FindDateExact(idSupport, dateClick, dateExact, duree, version, +1);
	}
	else
	{
		bRet = m_Parutions.FindDateExact(idSupport, dateClick, dateExact, duree, version, -1);
		if (bRet)
		{
			//nous sommes hors période
			if (dateExact < dateLimite)
				return -10000;

			//nous sommes trop loin de la dateclick
			if (dateExact+duree <= dateClick)
			{
				bRet = false;
				//return -10000;
			}
		}
	}

	//si on pas une parution auto
	if (!bRet)
	{
		//on creee une parution à la date clik
		dateExact = dateClick;
		duree = GetDureePeriod(idSupport);
		//ce n'est plus de l'automatisé
		CRegroupements* regroup = CRegroupements::GetInstance();
		IElem * pElem = regroup->GetIElemByID(idSupport);
		if(pElem && !pElem->IsCouplageFixe())
			nRet+=100;
		//on recherche une version
		bRetVer = m_VersionsTarifsAutos.FindVersion( idSupport, dateExact, version );

	}

	////Recherche du tarif/////
	if(!bRet)
	{
		JDate dt;
		dt = dateExact;
		do
		{
			if(m_TarifsManuels.GetTarifManuel(idSupport, idFormat, dt, tarif))
			{
				dateExact = dt;
				nRet+=20;
				return nRet;
			}
			dt+=1;
		}while (dt < dateLimite);
	}	

	//on privilegie le tarif manuel
	if ( m_TarifsManuels.GetTarifManuel(idSupport, idFormat, dateExact, tarif) )
	{
		//ce n'est plus de l'automatisé
		nRet+=20;
		return nRet;
	}

	//si pas de manuel alors on cherche un auto si on a eu une version
	if (bRetVer &&  m_TarifsAutos.GetTarif(idSupport, idFormat, version, tarif) )
	{
		nRet+=10;
		return nRet;
	}

	//echec

	//débrayage pour JF
	dateExact = dateClick;
	tarif = 0;
	return 10;

	return -1000;
}


JUnt32 CTarifs::GetDureePeriod(JUnt32 idSupport) const
{
	CRegroupements* regroup = CRegroupements::GetInstance();

	IElem * pElem = regroup->GetIElemByID(idSupport);
	JUnt32 idperiod = 0;
	if(pElem && pElem->IsTitre() && !pElem->IsCouplage())
			idperiod =  pElem->AsITitre()->GetPeriodicite();

	JUnt32 period = 1;


	switch (idperiod)
	{
		case 1://quotidien
			period = 1;
			break;
		case 2://bihebdo
			period = 3;
			break;
		case 3://hebdo
			period = 7;
			break;
		case 4://bimensuel
			period = 15;
			break;
		case 5://mensuel
			period = 30;
			break;
		case 6://bimestriel
			period = 60;
			break;
		case 7://trimestriel
			period = 90;
			break;
		case 8://hs
			period = 1;
			break;
	}
	
	return period;

}

JBool CTarifs::GetNumParution( JUnt32 idSupport, JDate date, JUnt32 & numero) const
{
	return m_Parutions.FindNumero(idSupport, date, numero);
}

RETTRF CTarifs::GetTarifByDateNew( JUnt32 idSupport, JUnt32 idFormat, JDate dateClick, JDate dateLimiteMin, JDate dateLimiteMax, JDate& dateExact, JUnt32& duree, JUnt32 & tarif) const
{

//enum RETTRF {TRFOK, NOTRF, TRFMAN, TRFPAR, TRFOUT};
	JUnt32 version;
	JBool bRet, bRetVer;

	bool bparManuel = false;

	bRetVer = bRet = m_Parutions.FindDateExact(idSupport, dateClick, dateExact, duree, version, -1);
	
	if(bRet)
	{
		
		//si l'insertion ne contient pas la date du click
		if (dateExact+duree <= dateClick)
		{
			//bRet = false;
			//on construit la parution à partir de la date trouvée(valide) et en ajoutant des durées standard
			
			dateExact = dateExact + duree;//on applique la durée de la deniere insertion
			duree = GetDureePeriod(idSupport);
			if(duree == 1)
				return NOTRF;
			while(dateExact+duree <= dateClick)
				dateExact = dateExact + duree;
			
			
			//duree = GetDureePeriod(idSupport);
			//if(duree == 1)
			//	return NOTRF;
			//do
			//{
			//	dateExact = dateExact + duree /*+ 1*/;
			//}while(dateExact+duree <= dateClick);
			bparManuel = true;
			bRetVer = m_VersionsTarifsAutos.FindVersion( idSupport, dateExact, version );

		}

		//Si la date trouvée est avant la date limite ou si l'insertion sort de la période
		if ((dateExact < dateLimiteMin)||(dateExact+duree-1 > dateLimiteMax))
			return TRFOUT;
	}
	else
	{
		//pas de parution trouvée on cale la parution sur le click
		dateExact = dateClick;
		duree = GetDureePeriod(idSupport);
		//on recherche une version
		bRetVer = m_VersionsTarifsAutos.FindVersion( idSupport, dateExact, version );
			bparManuel = true;

		//Si la date trouvée est avant la date limite ou si l'insertion sort de la période
		if ((dateExact < dateLimiteMin)||(dateExact+duree > dateLimiteMax))
			return TRFOUT;
	}

	//on privilegie le tarif manuel
	if ( m_TarifsManuels.GetTarifManuel(idSupport, idFormat, dateExact, tarif) )
	{
		//ce n'est plus de l'automatisé
		return TRFMAN;
	}
	
	if(bRetVer && m_TarifsAutos.GetTarif(idSupport, idFormat, version, tarif))
	{
		if(bparManuel)
			return TRFPAR;

		return TRFOK;
	}
	tarif = 0;

	return TRFMAN;
}

RETTRF CTarifs::GetNextTarifByDateNew( JUnt32 idSupport, JUnt32 idFormat, JDate dateFinInsert, JDate dateLimiteMin, JDate dateLimiteMax, JDate& dateExact, JUnt32& duree, JUnt32 & tarif) const
{

//enum RETTRF {TRFOK, NOTRF, TRFMAN, TRFPAR, TRFOUT};
	JUnt32 version;
	JBool bRet, bRetVer;

	bool bparManuel = false;

	if(dateFinInsert == dateLimiteMin)
	{
		//dateFinInsert = dateFinInsert + GetDureePeriod(idSupport);
		bRetVer = bRet = m_Parutions.FindDateExact(idSupport, dateFinInsert, dateExact, duree, version, -1);
		JDate dateExactprec = dateExact;
		while((dateExact<dateLimiteMin && bRet) && (dateFinInsert<dateLimiteMax))
		{
			if(GetDureePeriod(idSupport)==1)
				dateFinInsert = dateFinInsert + GetDureePeriod(idSupport);
			else
				dateFinInsert = dateFinInsert + GetDureePeriod(idSupport)/2;
			bRetVer = bRet = m_Parutions.FindDateExact(idSupport, dateFinInsert, dateExact, duree, version, -1);
			if(dateExact == dateExactprec)//rustine si aucune parution dans l'année
			{
				bRetVer = bRet = m_Parutions.FindDateExact(idSupport, dateFinInsert, dateExact, duree, version, +1);
				break;
			}
		}

	}
	else
		bRetVer = bRet = m_Parutions.FindDateExact(idSupport, dateFinInsert, dateExact, duree, version, +1);
	
	if(bRet)
	{
		
		//si l'insertion ne contient pas la date du click
		if (dateExact+duree < dateFinInsert+1)
		{
			//bRet = false;
			//on construit la parution à partir de la date trouvée(valide) et en ajoutant des durées standard
			duree = GetDureePeriod(idSupport);
			do
			{
				dateExact = dateExact + duree /*+ 1*/;
			}while(dateExact+duree <= dateFinInsert/*+1*/);
			bparManuel = true;
			bRetVer = m_VersionsTarifsAutos.FindVersion( idSupport, dateExact, version );

		}

		if(duree > dateLimiteMax-dateLimiteMin)
			return NOTRF;

		//Si la date trouvée est avant la date limite ou si l'insertion sort de la période
		if ((dateExact < dateLimiteMin)||(dateExact+duree > dateLimiteMax))
			return TRFOUT;
	}
	else
	{
		//pas de parution trouvée on cale la parution sur le click
		dateExact = dateFinInsert;
		duree = GetDureePeriod(idSupport);
		
		if(duree > dateLimiteMax-dateLimiteMin)
			return NOTRF;
		
		//on recherche une version
		bRetVer = m_VersionsTarifsAutos.FindVersion( idSupport, dateExact, version );
			bparManuel = true;

		//Si la date trouvée est avant la date limite ou si l'insertion sort de la période
		if ((dateExact < dateLimiteMin)||(dateExact+duree > dateLimiteMax))
			return TRFOUT;
	}


	//on privilegie le tarif manuel
	if ( m_TarifsManuels.GetTarifManuel(idSupport, idFormat, dateExact, tarif) )
	{
		//ce n'est plus de l'automatisé
		return TRFMAN;
	}
	
	if(bRetVer && m_TarifsAutos.GetTarif(idSupport, idFormat, version, tarif))
	{
		if(bparManuel)
			return TRFPAR;

		return TRFOK;
	}
	tarif = 0;

	return TRFMAN;
}

RETTRF CTarifs::GetStatutTarifNew( JUnt32 idSupport, JUnt32 idFormat, JDate dateToTest, JDate dateLimiteMin, JDate dateLimiteMax) const
{
//enum RETTRF {TRFOK, NOTRF, TRFMAN, TRFPAR, TRFOUT};
	CRegroupements* regroup = CRegroupements::GetInstance();
	IElem * pElem = regroup->GetIElemByID(idSupport);
	if(pElem && pElem->IsCouplage())
	{
		JList<JUnt32> lstFils;
		pElem->AsCouplage()->GetFils(lstFils);
		JDate dateExact;
		JUnt32 duree = 0;
		JUnt32 tarif = 0;
		RETTRF ret = GetTarifByDateNew(idSupport, idFormat, dateToTest, dateLimiteMin, dateLimiteMax, dateExact, duree, tarif);
		//le méthode GetTarifByDateNew retourne TRFPAR qd on a un couplage
		if(ret == TRFPAR)
			ret = TRFOK;
		for(lstFils.MoveFirst(); lstFils.IsValid(); lstFils.MoveNext())
		{
			RETTRF retFils = GetTarifByDateNew(lstFils.GetItem(), idFormat, dateToTest, dateLimiteMin, dateLimiteMax, dateExact, duree, tarif);
			if(retFils == NOTRF && ret!=TRFOUT)
				ret = NOTRF;
			if(retFils == TRFOUT)
				ret = TRFOUT;
		}
		return ret;
	}
	else
	{
		JDate dateExact;
		JUnt32 duree = 0;
		JUnt32 tarif;
		return GetTarifByDateNew(idSupport, idFormat, dateToTest, dateLimiteMin, dateLimiteMax, dateExact, duree, tarif);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Même méthode de récupération tarif mais avec passage des tarifs manuels utilisateurs
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////

JInt32 CTarifs::GetTarifByDate		(JUnt32 idSupport, JUnt32 idFormat, const JMap <CKeyTarifManuel,JFlt32> &MapTarifManuel, JDate dateClick, JDate dateLimite, JDate& dateExact, JUnt32& duree, JUnt32 & tarif) const
{
	//Recherche de la parution
	JInt32 nRet = 0;
	JBool bRet;
	JBool bRetVer = true;
	JUnt32 version;

	tarif = 0;

	if (dateClick < dateLimite)
	{
		bRet = m_Parutions.FindDateExact(idSupport, dateClick, dateExact, duree, version, +1);
	}
	else
	{
		bRet = m_Parutions.FindDateExact(idSupport, dateClick, dateExact, duree, version, -1);
		if (bRet)
		{
			//nous sommes hors période
			if (dateExact < dateLimite)
				return -10000;

			//nous sommes trop loin de la dateclick
			if (dateExact+duree <= dateClick)
			{
				bRet = false;
				//return -10000;
			}
		}
	}

	//si on pas une parution auto
	if (!bRet)
	{
		//on creee une parution à la date clik
		dateExact = dateClick;
		duree = GetDureePeriod(idSupport);
		//ce n'est plus de l'automatisé
		CRegroupements* regroup = CRegroupements::GetInstance();
		IElem * pElem = regroup->GetIElemByID(idSupport);
		if(pElem && !pElem->IsCouplageFixe())
			nRet+=100;
		//on recherche une version
		bRetVer = m_VersionsTarifsAutos.FindVersion( idSupport, dateExact, version );

	}

	////Recherche du tarif/////
	if(!bRet)
	{
		JDate dt;
		dt = dateExact;
		do
		{
			if(m_TarifsManuels.GetTarifManuel(idSupport, idFormat, dt, tarif))
			{
				dateExact = dt;
				nRet+=20;
				return nRet;
			}
			dt+=1;
		}while (dt < dateLimite);
	}	

	// Ajout ALAIN Janvier 2007
	// Vérifie si c'est un tarif manuel temporaire (saisie dans l'appli)
	CKeyTarifManuel KeyTarifManuel;
	KeyTarifManuel.m_IdSupport  = idSupport;
	KeyTarifManuel.m_IdFormat   = idFormat;
	MapTarifManuel.MoveTo(KeyTarifManuel);
	if (MapTarifManuel.IsValid())
	{
		//ce n'est plus de l'automatisé
		tarif = (JUnt32)MapTarifManuel.GetItem();
		nRet+=20;
		return nRet;
	}

	//on privilegie le tarif manuel
	if ( m_TarifsManuels.GetTarifManuel(idSupport, idFormat, dateExact, tarif) )
	{
		//ce n'est plus de l'automatisé
		nRet+=20;
		return nRet;
	}

	//si pas de manuel alors on cherche un auto si on a eu une version
	if (bRetVer &&  m_TarifsAutos.GetTarif(idSupport, idFormat, version, tarif) )
	{
		nRet+=10;
		return nRet;
	}

	//echec

	//débrayage pour JF
	dateExact = dateClick;
	tarif = 0;
	return 10;

	return -1000;
}

RETTRF CTarifs::GetTarifByDateNew	(JUnt32 idSupport, JUnt32 idFormat, const JMap <CKeyTarifManuel,JFlt32> &MapTarifManuel, JDate dateClick, JDate dateLimiteMin, JDate dateLimiteMax, JDate& dateExact, JUnt32& duree, JUnt32 & tarif) const
{
	//enum RETTRF {TRFOK, NOTRF, TRFMAN, TRFPAR, TRFOUT};
	JUnt32 version;
	JBool bRet, bRetVer;

	bool bparManuel = false;

	bRetVer = bRet = m_Parutions.FindDateExact(idSupport, dateClick, dateExact, duree, version, -1);

	if(bRet)
	{
		//si l'insertion ne contient pas la date du click
		if (dateExact+duree <= dateClick)
		{
			//bRet = false;
			//on construit la parution à partir de la date trouvée(valide) et en ajoutant des durées standard

			dateExact = dateExact + duree;//on applique la durée de la deniere insertion
			duree = GetDureePeriod(idSupport);
			if(duree == 1)
				return NOTRF;
			while(dateExact+duree <= dateClick)
				dateExact = dateExact + duree;

			bparManuel = true;
			bRetVer = m_VersionsTarifsAutos.FindVersion( idSupport, dateExact, version );
		}

		//Si la date trouvée est avant la date limite ou si l'insertion sort de la période
		if ((dateExact < dateLimiteMin)||(dateExact+duree-1 > dateLimiteMax))
			return TRFOUT;
	}
	else
	{
		//pas de parution trouvée on cale la parution sur le click
		dateExact = dateClick;
		duree = GetDureePeriod(idSupport);

		//on recherche une version
		bRetVer = m_VersionsTarifsAutos.FindVersion( idSupport, dateExact, version );
		bparManuel = true;

		//Si la date trouvée est avant la date limite ou si l'insertion sort de la période
		if ((dateExact < dateLimiteMin)||(dateExact+duree > dateLimiteMax))
			return TRFOUT;
	}

	// Ajout ALAIN Janvier 2007
	// Vérifie si c'est un tarif manuel temporaire (saisie dans l'appli)
	CKeyTarifManuel KeyTarifManuel;
	KeyTarifManuel.m_IdSupport  = idSupport;
	KeyTarifManuel.m_IdFormat   = idFormat;
	MapTarifManuel.MoveTo(KeyTarifManuel);
	if (MapTarifManuel.IsValid())
	{
		//ce n'est plus de l'automatisé
		tarif = (JUnt32)MapTarifManuel.GetItem(); 
		return TRFMAN;
	}

	//on privilegie le tarif manuel
	if ( m_TarifsManuels.GetTarifManuel(idSupport, idFormat, dateExact, tarif) )
	{
		//ce n'est plus de l'automatisé
		return TRFMAN;
	}

	if(bRetVer && m_TarifsAutos.GetTarif(idSupport, idFormat, version, tarif))
	{
		if(bparManuel)
			return TRFPAR;

		return TRFOK;
	}
	tarif = 0;

	return TRFMAN;
}

/*
// Ne pas effacer......... en c*ours pour le problème des titres apparentés
//
RETTRF CTarifs::GetTarifByDateNew	(JUnt32 idSupport, JUnt32 idFormat, const JMap <CKeyTarifManuel,JFlt32> &MapTarifManuel, JDate dateClick, JDate dateLimiteMin, JDate dateLimiteMax, JDate& dateExact, JUnt32& duree, JUnt32 & tarif) const
{
	//enum RETTRF {TRFOK, NOTRF, TRFMAN, TRFPAR, TRFOUT};
	JUnt32 version;
	JBool bRet, bRetVer;

	bool bparManuel = false;

	bRetVer = bRet = m_Parutions.FindDateExact(idSupport, dateClick, dateExact, duree, version, -1);

	// Attention aux titres apparentés sans parution
	CRegroupements * pReg = CRegroupements::GetInstance();
	IElem * pElem = pReg->GetIElemByID(idSupport);

	if (!pElem->IsTitreApparente())
	{
		if(bRet)
		{
			//si l'insertion ne contient pas la date du click
			if (dateExact+duree <= dateClick)
			{
				//bRet = false;
				//on construit la parution à partir de la date trouvée(valide) et en ajoutant des durées standard
				
				dateExact = dateExact + duree;//on applique la durée de la deniere insertion
				duree = GetDureePeriod(idSupport);
				if(duree == 1)
					return NOTRF;
				while(dateExact+duree <= dateClick)
					dateExact = dateExact + duree;
				
				bparManuel = true;
				bRetVer = m_VersionsTarifsAutos.FindVersion( idSupport, dateExact, version );
			}

			//Si la date trouvée est avant la date limite ou si l'insertion sort de la période
			if ((dateExact < dateLimiteMin)||(dateExact+duree-1 > dateLimiteMax))
				return TRFOUT;
		}
		else
		{
			//pas de parution trouvée on cale la parution sur le click
			dateExact = dateClick;
			duree = GetDureePeriod(idSupport);

			//on recherche une version
			bRetVer = m_VersionsTarifsAutos.FindVersion( idSupport, dateExact, version );
				bparManuel = true;

			//Si la date trouvée est avant la date limite ou si l'insertion sort de la période
			if ((dateExact < dateLimiteMin)||(dateExact+duree > dateLimiteMax))
				return TRFOUT;
		}
	}

	// Ajout ALAIN Janvier 2007
	// Vérifie si c'est un tarif manuel temporaire (saisie dans l'appli)
	CKeyTarifManuel KeyTarifManuel;
	KeyTarifManuel.m_IdSupport  = idSupport;
	KeyTarifManuel.m_IdFormat   = idFormat;
	MapTarifManuel.MoveTo(KeyTarifManuel);
	if (MapTarifManuel.IsValid())
	{
		//ce n'est plus de l'automatisé
		tarif = (JUnt32)MapTarifManuel.GetItem(); 
		return TRFMAN;
	}

	//on privilegie le tarif manuel
	if ( m_TarifsManuels.GetTarifManuel(idSupport, idFormat, dateExact, tarif) )
	{
		//ce n'est plus de l'automatisé
		return TRFMAN;
	}

	// Si c'est un titre apparenté, et qu'on a pas de tarif manuel dessus, on prend le tarif selon les dates
	if (pElem->IsTitreApparente())
	{
		if(GetTarifByDate(idSupport, idFormat, dateClick, dateLimiteMax, dateExact, duree, tarif))
		{
			return TRFOK;
		}
		else
		{			
			tarif = 0;
			return TRFMAN;
		}	
	}

	if(bRetVer && m_TarifsAutos.GetTarif(idSupport, idFormat, version, tarif))
	{
		if(bparManuel)
			return TRFPAR;

		return TRFOK;
	}
	tarif = 0;

	return TRFMAN;
}
*/

RETTRF CTarifs::GetNextTarifByDateNew(JUnt32 idSupport, JUnt32 idFormat, const JMap <CKeyTarifManuel,JFlt32> &MapTarifManuel, JDate dateFinInsert, JDate dateLimiteMin, JDate dateLimiteMax, JDate& dateExact, JUnt32& duree, JUnt32 & tarif) const
{
	//enum RETTRF {TRFOK, NOTRF, TRFMAN, TRFPAR, TRFOUT};
	JUnt32 version;
	JBool bRet, bRetVer;

	bool bparManuel = false;

	if(dateFinInsert == dateLimiteMin)
	{
		//dateFinInsert = dateFinInsert + GetDureePeriod(idSupport);
		bRetVer = bRet = m_Parutions.FindDateExact(idSupport, dateFinInsert, dateExact, duree, version, -1);
		JDate dateExactprec = dateExact;
		while((dateExact<dateLimiteMin && bRet) && (dateFinInsert<dateLimiteMax))
		{
			if(GetDureePeriod(idSupport)==1)
				dateFinInsert = dateFinInsert + GetDureePeriod(idSupport);
			else
				dateFinInsert = dateFinInsert + GetDureePeriod(idSupport)/2;
			bRetVer = bRet = m_Parutions.FindDateExact(idSupport, dateFinInsert, dateExact, duree, version, -1);
			if(dateExact == dateExactprec)//rustine si aucune parution dans l'année
			{
				bRetVer = bRet = m_Parutions.FindDateExact(idSupport, dateFinInsert, dateExact, duree, version, +1);
				break;
			}
		}

	}
	else
		bRetVer = bRet = m_Parutions.FindDateExact(idSupport, dateFinInsert, dateExact, duree, version, +1);
	
	if(bRet)
	{
		
		//si l'insertion ne contient pas la date du click
		if (dateExact+duree < dateFinInsert+1)
		{
			//bRet = false;
			//on construit la parution à partir de la date trouvée(valide) et en ajoutant des durées standard
			duree = GetDureePeriod(idSupport);
			do
			{
				dateExact = dateExact + duree /*+ 1*/;
			}while(dateExact+duree <= dateFinInsert/*+1*/);
			bparManuel = true;
			bRetVer = m_VersionsTarifsAutos.FindVersion( idSupport, dateExact, version );

		}

		if(duree > dateLimiteMax-dateLimiteMin)
			return NOTRF;

		//Si la date trouvée est avant la date limite ou si l'insertion sort de la période
		if ((dateExact < dateLimiteMin)||(dateExact+duree > dateLimiteMax))
			return TRFOUT;
	}
	else
	{
		//pas de parution trouvée on cale la parution sur le click
		dateExact = dateFinInsert;
		duree = GetDureePeriod(idSupport);
		
		if(duree > dateLimiteMax-dateLimiteMin)
			return NOTRF;
		
		//on recherche une version
		bRetVer = m_VersionsTarifsAutos.FindVersion( idSupport, dateExact, version );
			bparManuel = true;

		//Si la date trouvée est avant la date limite ou si l'insertion sort de la période
		if ((dateExact < dateLimiteMin)||(dateExact+duree > dateLimiteMax))
			return TRFOUT;
	}

	// Ajout ALAIN Janvier 2007
	// Vérifie si c'est un tarif manuel temporaire (saisie dans l'appli)
	CKeyTarifManuel KeyTarifManuel;
	KeyTarifManuel.m_IdSupport  = idSupport;
	KeyTarifManuel.m_IdFormat   = idFormat;
	MapTarifManuel.MoveTo(KeyTarifManuel);
	if (MapTarifManuel.IsValid())
	{
		//ce n'est plus de l'automatisé
		tarif = (JUnt32) MapTarifManuel.GetItem(); 
		return TRFMAN;
	}

	//on privilegie le tarif manuel
	if ( m_TarifsManuels.GetTarifManuel(idSupport, idFormat, dateExact, tarif) )
	{
		//ce n'est plus de l'automatisé
		return TRFMAN;
	}
	
	if(bRetVer && m_TarifsAutos.GetTarif(idSupport, idFormat, version, tarif))
	{
		if(bparManuel)
			return TRFPAR;

		return TRFOK;
	}
	tarif = 0;

	return TRFMAN;
}

JBool  CTarifs::GetTarifByVersion	(JUnt32 idSupport, JUnt32 idFormat, const JMap <CKeyTarifManuel,JFlt32> &MapTarifManuel, JDate date, JUnt32 & tarif) const
{
	// Ajout ALAIN Janvier 2007
	// Vérifie si c'est un tarif manuel temporaire (saisie dans l'appli)
	CKeyTarifManuel KeyTarifManuel;
	KeyTarifManuel.m_IdSupport  = idSupport;
	KeyTarifManuel.m_IdFormat   = idFormat;
	MapTarifManuel.MoveTo(KeyTarifManuel);
	if (MapTarifManuel.IsValid())
	{
		// renvoi le tarif manuel
		tarif = (JUnt32) MapTarifManuel.GetItem(); 
		return true;
	}
	else
	{
		JUnt32 version;
		m_VersionsTarifsAutos.FindVersion( idSupport, date, version );
		return m_TarifsAutos.GetTarif( idSupport, idFormat, version, tarif );
	}
}
