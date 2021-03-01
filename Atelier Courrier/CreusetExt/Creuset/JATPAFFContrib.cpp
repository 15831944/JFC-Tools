//
// Fichier: JATPAFFContrib.cpp
// Auteur:  Jérémy Devilers
// Date:    19/12/2003
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "resource.h"
#include "JATPAFFContrib.h"
#include "JATPDocCreuset.h"

////////////////////
// les constructeurs

JATPAFFContrib::JATPAFFContrib()
{
	m_bVisible = false;
	Reset();
}

JATPAFFContrib::JATPAFFContrib(const JATPAFFContrib & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

void JATPAFFContrib::SetVisible(JBool bVisible)
{
	m_bVisible = bVisible;
}

JBool JATPAFFContrib::IsVisible()
{
	return m_bVisible;
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JATPAFFContrib & JATPAFFContrib::operator =(const JATPAFFContrib & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////
// la fonction pour réinitialiser les éléments

JVoid JATPAFFContrib::Reset()
{
	// on réinitialise l'élément
	m_ContribAdd = 0.0;
	m_XGRPAdd = 0.0;
	m_coutGRPAdd = 0.0;
	m_repriseAdd = 0.0;

	m_ContribSupp = 0.0;
	m_XGRPSupp = 0.0;
	m_coutGRPSupp = 0.0;
	m_repriseSupp = 0.0;

	m_PartBudget = 0.0;
	m_Prix = 0.0;
	m_PartXGRP = 0.0;
	m_XGRP = 0.0;

	m_idHypo = 0;
	m_idFormat = 0;
	m_idSupport = 0;

}

/////////////////
// le destructeur

JATPAFFContrib::~JATPAFFContrib()
{
	// on ne fait rien
}



///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
CKeyContrib::CKeyContrib()
{
	// on réinitialise l'élément
	m_Id1		= 0;
	m_Id2		= 0;
	m_Id3		= 0;
}

CKeyContrib::CKeyContrib(const CKeyContrib & Source)
{
	// on recopie l'élément
	m_Id1	= Source.m_Id1;
	m_Id2	= Source.m_Id2;
	m_Id3	= Source.m_Id3;
}

////////////////////////////////////
// la fonction pour tester l'élément

JBool CKeyContrib::IsValid() const
{
	// on renvoie la validité de l'élément
	return (m_Id1 && m_Id2 && m_Id3);
}

//////////////////////////////////////
// l'opérateur pour recopier l'élément

CKeyContrib & CKeyContrib::operator =(const CKeyContrib & Source)
{
	// on recopie l'élément
	m_Id1	= Source.m_Id1;
	m_Id2	= Source.m_Id2;
	m_Id3	= Source.m_Id3;

	// on renvoie notre référence
	return (*this);
}

////////////////////////////////////////////
// les opérateurs pour comparer les éléments

JBool CKeyContrib::operator <(const CKeyContrib & Reference) const
{
	if (m_Id1 < Reference.m_Id1)	return true;
	if (m_Id1 > Reference.m_Id1)	return false;
	if (m_Id2 < Reference.m_Id2)	return true;
	if (m_Id2 > Reference.m_Id2)	return false;
	if (m_Id3 < Reference.m_Id3)	return true;
	if (m_Id3 > Reference.m_Id3)	return false;
	return false;
}

JBool CKeyContrib::operator >(const CKeyContrib & Reference) const
{
	if (m_Id1 > Reference.m_Id1)	return true;
	if (m_Id1 < Reference.m_Id1)	return false;
	if (m_Id2 > Reference.m_Id2)	return true;
	if (m_Id2 < Reference.m_Id2)	return false;
	if (m_Id3 > Reference.m_Id3)	return true;
	if (m_Id3 < Reference.m_Id3)	return false;
	return false;
}

///////////////////////////////////////////
// la fonction pour réinitialiser l'élément

JVoid CKeyContrib::Reset()
{
	// on réinitialise l'élément
	m_Id1		= 0;
	m_Id2		= 0;
	m_Id3		= 0;
}

/////////////////
// le destructeur

CKeyContrib::~CKeyContrib()
{
	// on ne fait rien
}
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
JATPAFFContrib_New::JATPAFFContrib_New()
{
	m_bUpd = m_bUpdAll = false;
	m_bVisible = false;

	// Init grandeurs à afficher pour la contribution
	// m_bShowCouv = m_bShowGRP = m_bShowPGRP = m_bShowBudg = m_bShowPBudg = m_bShowCtCouv = false;

	// Par défaut on valide toutes les grandeurs
	m_bShowCouv = m_bShowGRP = m_bShowPGRP = m_bShowBudg = m_bShowPBudg = m_bShowCtCouv = true;

	// Init mode affichage (par defaut groupé par supports)
	m_ModeAffich = MODEAFFICH::AFFICH_BLOC_SUPPORTS;

	Reset();
}

JATPAFFContrib_New::JATPAFFContrib_New(const JATPAFFContrib_New & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

void JATPAFFContrib_New::SetVisible(JBool bVisible)
{
	m_bVisible = bVisible;
}

JBool JATPAFFContrib_New::IsVisible()
{
	return m_bVisible;
}

/////////////////////////////////////////
// l'opérateur pour recopier les éléments

JATPAFFContrib_New & JATPAFFContrib_New::operator =(const JATPAFFContrib_New & Source)
{
	// on génère une exception
	throw JInvalidCall::GetInstance();
}

//////////////////////////////////////////////
// la fonction pour réinitialiser les éléments

JVoid JATPAFFContrib_New::Reset()
{
	m_Contrib.Reset();
}
JATPAFFContrib_New::~JATPAFFContrib_New()
{
	// on ne fait rien
}

JVoid JATPAFFContrib_New::SetDocument(JATPDocCreuset* pATPDocCreuset)
{
	m_pATPDocCreuset = pATPDocCreuset;
}
JVoid JATPAFFContrib_New::Update(const JFCUpdate & Message)
{
	if(Message == UPDATE_INIPROGRESS
		|| Message == UPDATE_PROGRESS
		|| Message == UPDATE_ENDPROGRESS
		|| Message == UPDATE_AFF_HYPO
		|| Message == UPDATE_AFF_PLAN
		|| Message == UPDATE_AFF_CONTRIB
		|| Message == UPDATE_AFF_PANEL
		|| Message == UPDATE_AFF_PUPITRE
		|| Message == UPDATE_AFF_CREUSET
		|| Message == UPDATE_SUPP_HYPOTHESECUR
		|| Message == UPDATE_SEL_FORMAT
		|| Message == UPDATE_SEL_HYPOTHESE)
		return;

	if(Message == UPDATE_AFF_CONTRIBNEW && m_bVisible && m_bUpdAll == false && m_bUpd == false)
		return;

	if(  Message == UPDATE_CTXLECTURE
		|| Message == UPDATE_FILTREAUDIENCE
		|| Message == UPDATE_SEL_CIBLE
		//|| Message == UPDATE_ETUDE
		|| Message == UPDATE_KEY_PERIODE_ACTION
		|| Message == UPDATE_OPTIONSMOTEUR
		|| Message == UPDATE_OFFRE
		|| Message == UPDATE_SEL_TERRAIN
		|| Message == UPDATE_INITIAL
		|| Message == UPDATE_TARIFS         // si Mise à jour tarif manuel	
		)
	{
		Reset();
		m_bUpdAll = true;
		
		// Init grandeurs à afficher pour la contribution
		// m_bShowCouv = m_bShowGRP = m_bShowPGRP = m_bShowBudg = m_bShowPBudg = m_bShowCtCouv = false;

		// Par défaut on valide toutes les grandeurs
		m_bShowCouv = m_bShowGRP = m_bShowPGRP = m_bShowBudg = m_bShowPBudg = m_bShowCtCouv = true;

		return;
	}

	if(Message == UPDATE_CURRENT_HYPO || Message == UPDATE_CURRENT_HYPO_LIGHT)
	{
		CWaitCursor wait;

		int nCntSupp;
		// on récupère le nombre de supports
		// via plateau
		nCntSupp = m_pATPDocCreuset->m_PlateauSupports.GetCount();

		//boucle sur les formats // on est obligé de faire cette ruse à cause des itérateur internes
		JList<JUnt32> lstFormat;
		for( m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
		{
			//if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
				lstFormat.AddTail() = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
		}
		lstFormat.AddTail() = -1;//on ajout le format total

		
		//boucle sur les hypothèses
		int idHypo = m_pATPDocCreuset->m_IdxHypotheseCourant;

		m_mapHypo.MoveTo(idHypo);
		if(!m_mapHypo.IsValid())
			m_mapHypo.Add(idHypo) = idHypo;

		//boucle sur les supports
		for(int iSupport = 0; iSupport < nCntSupp; iSupport++)
		{
			//boucle sur les formats				
			for( lstFormat.MoveFirst();lstFormat.IsValid();lstFormat.MoveNext())
			{
				CKeyContrib key;
				key.m_Id1 = idHypo;
				key.m_Id2 = lstFormat.GetItem();
				key.m_Id3 = iSupport;

				Contrib contrib;

				contrib.Reset();

				contrib.SetNotDefined();

				m_Contrib.MoveTo(key);
				if(m_Contrib.IsValid())
					m_Contrib.Remove();

				m_Contrib.Add(key) = contrib;


			}
		}

		m_bUpd = true;
		
		return;
	}




	if( !m_bVisible || m_pATPDocCreuset->m_AFFCreuset.GetAffPlan() )
	{
		//on prépare la maj pour le prochain affichage
		if( Message == UPDATE_AFF_CONTRIBNEW
			|| Message == UPDATE_CTXLECTURE
			|| Message == UPDATE_FILTREAUDIENCE
			|| Message == UPDATE_SEL_CIBLE
			|| Message == UPDATE_ETUDE
			|| Message == UPDATE_KEY_PERIODE_ACTION
			|| Message == UPDATE_OPTIONSMOTEUR
			|| Message == UPDATE_OFFRE
			|| Message == UPDATE_SEL_TERRAIN
			|| Message == UPDATE_INITIAL
			|| Message == UPDATE_FORCECONTRIB
			|| Message == UPDATE_SEL_FORMAT
			|| Message == UPDATE_SEL_HYPOTHESE
			)
		{
			Reset();
			m_bUpdAll = true;
		}

		if(Message != UPDATE_FORCECONTRIB)
			return;
	}

	if(	Message == UPDATE_INITIAL
		|| Message == UPDATE_OFFRE
		|| Message == UPDATE_FORCECONTRIB
		|| Message == UPDATE_ETUDE
		|| m_bUpdAll)
	{
		m_bUpdAll = false;
		CWaitCursor wait;

		Reset();

		int nCntSupp;
		// on récupère le nombre de supports
		// via plateau
		nCntSupp = m_pATPDocCreuset->m_PlateauSupports.GetCount();

		//boucle sur les formats // on est obligé de faire cette ruse à cause des itérateur internes
		JList<JUnt32> lstFormat;
		for( m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
				lstFormat.AddTail() = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();

		lstFormat.AddTail() = -1;//on ajout le format total

		m_pATPDocCreuset->m_Progress.m_Range = (10 *  m_pATPDocCreuset->m_TblHypotheses.GetCount() * nCntSupp * lstFormat.GetCount() ) ;
		m_pATPDocCreuset->m_Progress.m_Step = 10;

		m_pATPDocCreuset->UpdateAllViews(UPDATE_INIPROGRESS);
		
		//boucle sur les hypothèses
		for(int idHypo = 0; idHypo < m_pATPDocCreuset->m_TblHypotheses.GetCount(); idHypo++)
		{
			//boucle sur les supports
			for(int iSupport = 0; iSupport < nCntSupp; iSupport++)
			{
				//boucle sur les formats				
				for( lstFormat.MoveFirst();lstFormat.IsValid();lstFormat.MoveNext())
				{
					CKeyContrib key;
					key.m_Id1 = idHypo;
					key.m_Id2 = lstFormat.GetItem();
					key.m_Id3 = iSupport;

					Contrib contrib;
					ComputeContrib(key.m_Id1, key.m_Id2, key.m_Id3, contrib);
					m_Contrib.MoveTo(key);
					if(m_Contrib.IsValid())
						m_Contrib.Remove();

					m_Contrib.Add(key) = contrib;

					m_pATPDocCreuset->UpdateAllViews(UPDATE_PROGRESS);

				}
			}
		}
		m_pATPDocCreuset->UpdateAllViews(UPDATE_ENDPROGRESS);
		return;
	}

	if(m_bUpd)
	{
		m_bUpd = false;
		CWaitCursor wait;


		int nCntSupp;
		// on récupère le nombre de supports
		// via plateau
		nCntSupp = m_pATPDocCreuset->m_PlateauSupports.GetCount();

		//boucle sur les formats // on est obligé de faire cette ruse à cause des itérateur internes
		JList<JUnt32> lstFormat;
		for( m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
				lstFormat.AddTail() = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();


		lstFormat.AddTail() = -1;//on ajout le format total

		m_pATPDocCreuset->m_Progress.m_Range = (10 *  m_mapHypo.GetCount() * nCntSupp * lstFormat.GetCount() ) ;
		m_pATPDocCreuset->m_Progress.m_Step = 10;

		m_pATPDocCreuset->UpdateAllViews(UPDATE_INIPROGRESS);
		
		//boucle sur les hypothèses
		for(m_mapHypo.MoveFirst();m_mapHypo.IsValid();m_mapHypo.MoveNext())
		{
			//boucle sur les supports
			for(int iSupport = 0; iSupport < nCntSupp; iSupport++)
			{
				//boucle sur les formats				
				for( lstFormat.MoveFirst();lstFormat.IsValid();lstFormat.MoveNext())
				{
					CKeyContrib key;
					key.m_Id1 = m_mapHypo.GetItem();
					key.m_Id2 = lstFormat.GetItem();
					key.m_Id3 = iSupport;

					Contrib contrib;
					ComputeContrib(key.m_Id1, key.m_Id2, key.m_Id3, contrib);
					m_Contrib.MoveTo(key);
					if(m_Contrib.IsValid())
						m_Contrib.Remove();

					m_Contrib.Add(key) = contrib;
	
					m_pATPDocCreuset->UpdateAllViews(UPDATE_PROGRESS);

				}
			}
		}
		m_pATPDocCreuset->UpdateAllViews(UPDATE_ENDPROGRESS);
		m_mapHypo.Reset();
		return;
	}
}

void JATPAFFContrib_New::SetType(TYPE type)
{
	m_type = type;
}

// Positionne le mode affichage des contributions (par supports, ou bien par grandeurs)
void JATPAFFContrib_New::SetModeAffich(MODEAFFICH ModeAffichage)
{
	m_ModeAffich = ModeAffichage;
}

// Récupère le mode affichage des contributions (par supports, ou bien par grandeurs)
int JATPAFFContrib_New::GetModeAffich()
{
	return m_ModeAffich;
}

CString JATPAFFContrib_New::GetValue(int idHypo, int idFormat, int idSupport) const
{
	return GetValue(idHypo, idFormat, idSupport, m_type);
}

// Renvoie le map des différentes valeurs
JMap<JUnt32, CString> & JATPAFFContrib_New::GetMapValue(int idHypo, int idFormat, int idSupport)
{
	CString outStr;

	// Init Map des résultats par grandeur
	m_MapTxtResult.Reset();

	// Resultat Couverture
	outStr = GetValue(idHypo, idFormat, idSupport, TYPE::COUV);
	m_MapTxtResult.MoveTo(TYPE::COUV);
	if (!m_MapTxtResult.IsValid())
		m_MapTxtResult.Add(TYPE::COUV) = outStr;

	// Resultat GRP
	outStr = GetValue(idHypo, idFormat, idSupport, TYPE::GRP);

	// m_MapTxtResult.Add(TYPE::GRP) = outStr;
	m_MapTxtResult.MoveTo(TYPE::GRP);
	if (!m_MapTxtResult.IsValid())
		m_MapTxtResult.Add(TYPE::GRP) = outStr;

	
	// Resultat Part de GRP
	outStr = GetValue(idHypo, idFormat, idSupport, TYPE::PARTGRP);
	m_MapTxtResult.MoveTo(TYPE::PARTGRP);
	if (!m_MapTxtResult.IsValid())
		m_MapTxtResult.Add(TYPE::PARTGRP) = outStr;


	// Resultat Budget
	outStr = GetValue(idHypo, idFormat, idSupport, TYPE::BUDGET);
	m_MapTxtResult.MoveTo(TYPE::BUDGET);
	if (!m_MapTxtResult.IsValid())
		m_MapTxtResult.Add(TYPE::BUDGET) = outStr;
	
	// Resultat Part de budget
	outStr = GetValue(idHypo, idFormat, idSupport, TYPE::PARTBUDGET);
	m_MapTxtResult.MoveTo(TYPE::PARTBUDGET);
	if (!m_MapTxtResult.IsValid())
		m_MapTxtResult.Add(TYPE::PARTBUDGET) = outStr;

	// Resultat Cout Couverture
	outStr = GetValue(idHypo, idFormat, idSupport, TYPE::CTCOUV);
	m_MapTxtResult.MoveTo(TYPE::CTCOUV);
	if (!m_MapTxtResult.IsValid())
		m_MapTxtResult.Add(TYPE::CTCOUV) = outStr;


	return m_MapTxtResult;
}

CString JATPAFFContrib_New::GetValue(int idHypo, int idFormat, int idSupport, TYPE type) const
{
	double val = -1.0;
	CString outStr;

	CKeyContrib key;
	key.m_Id1 = idHypo;
	key.m_Id2 =  idFormat;
	key.m_Id3 = idSupport;
	m_Contrib.MoveTo(key);
	if(m_Contrib.IsValid())
	{
		if(type == COUV)
			val = m_Contrib.GetItem().Couv();
		if(type == GRP)
			val = m_Contrib.GetItem().GRP();
		if(type == PARTGRP)
			val = m_Contrib.GetItem().PartGRP();
		if(type == BUDGET)
			val = m_Contrib.GetItem().Budget();
		if(type == PARTBUDGET)
			val = m_Contrib.GetItem().PartBudget();
		if(type == CTCOUV)
			val = m_Contrib.GetItem().CoutPtCouv();
	}
	else
	{
		// Calcul des contributions	pour la nouvelle hypothèse
		/*
		Contrib contrib;
		ComputeContrib(key.m_Id1, key.m_Id2, key.m_Id3, contrib);

		m_Contrib.MoveTo(key);
		if(!m_Contrib.IsValid())
		{
			m_Contrib.Add(key) = contrib;
		}

		// Recupere la valeur calculée sur la grandeur en cours
		if(type == COUV)
			val = m_Contrib.GetItem().Couv();
		if(type == GRP)
			val = m_Contrib.GetItem().GRP();
		if(type == PARTGRP)
			val = m_Contrib.GetItem().PartGRP();
		if(type == BUDGET)
			val = m_Contrib.GetItem().Budget();
		if(type == PARTBUDGET)
			val = m_Contrib.GetItem().PartBudget();
		if(type == CTCOUV)
			val = m_Contrib.GetItem().CoutPtCouv();
		*/
	}

	if(type == COUV
		|| type == PARTGRP
		|| type == PARTBUDGET)
		outStr.Format("%.1f %% ", val);
	if(type == GRP)
		outStr.Format("%.1f ", val);
	if(type == BUDGET
	|| type == CTCOUV)
	{
		outStr.Format("%.0f", val);
		CString Fmt = _T("### ### ### ### ### € ");
		CFormatStr::FormatNum(Fmt, outStr);
	}
    
	if (val <0.0)
	{
		outStr = "";
		if (val <-1.0)
			// Cas des elmts à recalculer
			outStr = "*";
	}	
	

	//if(!m_Contrib.IsValid())
	//	outStr.Format("i%d, ii%d, j%d ",idHypo, idFormat, idSupport);

	return outStr;
}


void JATPAFFContrib_New::ComputeContrib(LONG i, LONG ii, LONG j, Contrib &contrib)
{
	contrib.Reset();

	m_pATPDocCreuset->m_TblHypotheses.MoveTo(i);
	if (m_pATPDocCreuset->m_TblHypotheses.IsValid())
	{
		// on récupère l'identifiant du Support
		JUnt32 IdSupport;

		// en mode plateau
		const CKeySupport & KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(j);
		IdSupport = KeySupp.m_IdSupport; 


		// Id format courant
		JInt32 IdFormatCur = ii;

		JInt32 nbIns,nbInsCpl;
		bool bCpl;
		CPlanInsertions planSupp =  m_pATPDocCreuset->m_TblHypotheses.GetItem();
		planSupp.GetNbInsertSupport(KeySupp, IdFormatCur,m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
																		m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF(),nbIns,nbInsCpl,bCpl );
		if(IdFormatCur == -1)
		{
			nbIns = 0;
			JInt32 nbTmp = 0;
			for( m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
			{
				planSupp.GetNbInsertSupport(KeySupp, m_pATPDocCreuset->m_MapIdFormatsSel.GetKey(),m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
					m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF(),nbTmp,nbInsCpl,bCpl );
				nbIns += nbTmp;
			}
		}

		if(nbIns == 0)
			return;

		//contributuion

		//On recupere les données pour l'hypo
		m_pATPDocCreuset->m_ITRHypotheses.MoveTo(i);
		if(!m_pATPDocCreuset->m_ITRHypotheses.IsValid())
			return;

		

		const JITPDATHypoFormat & datFormatHypothese =  m_pATPDocCreuset->m_ITRHypotheses.GetItem();

		if(! datFormatHypothese.GetDATHypoByFormat(IdFormatCur).IsValid())
		{
			m_pATPDocCreuset->m_ITRHypotheses.CalcDatHypo(
						m_pATPDocCreuset->m_KEYPeriodeActionHyp,
						m_pATPDocCreuset->m_KEYPeriodeActionTablier,
						m_pATPDocCreuset->m_TblHypotheses.GetItem(), m_pATPDocCreuset->m_ITRHypotheses.GetItem(), false );
		}

		const JITPDATHypothese & datHypothese = ((IdFormatCur!=-1) ? datFormatHypothese.GetDATHypoByFormat(IdFormatCur) : datFormatHypothese.GetDATHypoTotal());


		//#####################################################################//

		// Supprime l'insertion
		if(IdFormatCur == -1)
		{
			for( m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
				planSupp.SuppAllInsertion(IdSupport, m_pATPDocCreuset->m_MapIdFormatsSel.GetKey(),m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
																		m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF() );
		}
		else
			planSupp.SuppAllInsertion(IdSupport, IdFormatCur,m_pATPDocCreuset->m_KEYPeriodeAction.GetDateD(),
																		m_pATPDocCreuset->m_KEYPeriodeAction.GetDateF() );


		JITPDATHypoFormat datFormatPlanSupp;
		m_pATPDocCreuset->m_ITRHypotheses.CalcDatHypo(
								m_pATPDocCreuset->m_KEYPeriodeActionHyp,
								m_pATPDocCreuset->m_KEYPeriodeActionTablier,
								planSupp, datFormatPlanSupp, true );

		const JITPDATHypothese & datPlanSupp = ((IdFormatCur!=-1) ? datFormatPlanSupp.GetDATHypoByFormat(IdFormatCur) : datFormatPlanSupp.GetDATHypoTotal());

		//#####################################################################//


		//Contrib
		contrib.effectif = datFormatHypothese.m_Effectif;

		JInt32 NbJours = datHypothese.m_DistributionJour.GetCount();
		contrib.nbInd = datHypothese.m_DistributionJour.Item(NbJours - 1).Item(1).AsJFlt64() - datPlanSupp.m_DistributionJour.Item(NbJours - 1).Item(1).AsJFlt64();
		contrib.nbIndTot = datHypothese.m_DistributionJour.Item(NbJours - 1).Item(1).AsJFlt64();



		//GRP
		contrib.cts = datHypothese.m_VolLecture - datPlanSupp.m_VolLecture;
		contrib.ctsTot = datHypothese.m_VolLecture;

	
		//budget
		contrib.budget = datHypothese.m_Budget - datPlanSupp.m_Budget;
		contrib.budgetTot = datHypothese.m_Budget; 

	}

}

CString JATPAFFContrib_New::GetTypeStringContrib() const
{
	CString str;
	str.LoadString(IDS_CR_CONTRIB);
	return str;
}

CString JATPAFFContrib_New::GetTypeString() const
{
	CString str;
	if(m_type == COUV)
		str.LoadString(IDS_CR_CONTRIBN_C);
	if(m_type == GRP)
		str.LoadString(IDS_CR_CONTRIBN_GRP);
	if(m_type == PARTGRP)
		str.LoadString(IDS_CR_CONTRIBN_PGRP);
	if(m_type == BUDGET)
		str.LoadString(IDS_CR_CONTRIBN_BUD);
	if(m_type == PARTBUDGET)
		str.LoadString(IDS_CR_CONTRIBN_PBUD);
	if(m_type == CTCOUV)
		str.LoadString(IDS_CR_CONTRIBN_CTCOUV);
	return str;
}

// Affichage de la couverture demandé
void JATPAFFContrib_New::ShowCouv(bool bShow)
{
	m_bShowCouv = bShow;
}

// Affichage du GRP demandé
void JATPAFFContrib_New::ShowGRP(bool bShow)
{
	m_bShowGRP = bShow;
}

// Affichage de la part de GRP demandé
void JATPAFFContrib_New::ShowPGRP(bool bShow)
{
	m_bShowPGRP = bShow;
}

// Affichage du Budget demandé
void JATPAFFContrib_New::ShowBudg(bool bShow)
{
	m_bShowBudg = bShow;
}

// Affichage de la part de Budget demandé
void JATPAFFContrib_New::ShowPBudg(bool bShow)
{
	m_bShowPBudg = bShow;
}

// Affichage du coût couverture demandé
void JATPAFFContrib_New::ShowCtCouv(bool bShow)
{
	m_bShowCtCouv = bShow;
}

// Veux t-on afficher la contribution couverture ?
bool JATPAFFContrib_New::CanCouv()
{
	return m_bShowCouv;
}

// Veux t-on afficher la contribution GRP ?
bool JATPAFFContrib_New::CanGRP()
{
	return m_bShowGRP;
}

// Veux t-on afficher la contribution de la part de GRP ?
bool JATPAFFContrib_New::CanPGRP()
{
	return m_bShowPGRP;
}

// Veux t-on afficher la contribution budget ?
bool JATPAFFContrib_New::CanBudg()
{
	return m_bShowBudg;
}

// Veux t-on afficher la contribution de la part de budget ?
bool JATPAFFContrib_New::CanPBudg()
{
	return m_bShowPBudg;
}

// Veux t-on afficher la contribution coût de la couverture ?
bool JATPAFFContrib_New::CanCtCouv()
{
	return m_bShowCtCouv;
}

// Nombre de grandeurs sélectionnées pour la contribution
int JATPAFFContrib_New::NbValue() const
{
	int nbVal = 0;
	if(m_bShowCouv)
		nbVal++;
	if(m_bShowGRP)
		nbVal++;
	if(m_bShowPGRP)
		nbVal++;
	if(m_bShowBudg)
		nbVal++;
	if(m_bShowPBudg)
		nbVal++;
	if(m_bShowCtCouv)
		nbVal++;
	return nbVal;
}


CString JATPAFFContrib_New::ExportContrib(int nHypo)
{
	CString exportStr;
	
	if(m_Contrib.GetCount()>1)
	{
		exportStr = "BLOC 7\n";
		exportStr += "Titre\tC%\tGRP\tPart (%) de GRP\tBudget\tPart (%) de budget\tCout / pt C%\n";

		int nCntSupp;
		// on récupère le nombre de supports
		nCntSupp = m_pATPDocCreuset->m_PlateauSupports.GetCount();

		//boucle sur les formats // on est obligé de faire cette ruse à cause des itérateur internes
		JList<JUnt32> lstFormat;
		for( m_pATPDocCreuset->m_MapIdFormatsSel.MoveFirst();
			m_pATPDocCreuset->m_MapIdFormatsSel.IsValid();
			m_pATPDocCreuset->m_MapIdFormatsSel.MoveNext())
		{
			//if(m_pATPDocCreuset->m_MapIdFormatsSel.GetItem()>=2)
				lstFormat.AddTail() = m_pATPDocCreuset->m_MapIdFormatsSel.GetKey();
		}
		// si plusieurs formats, on ajoute le format total
		if(lstFormat.GetCount() > 1)
			lstFormat.AddTail() = -1; // -1 = format TOTAL
		
		//boucle sur les supports
		for(int iSupport = 0; iSupport < nCntSupp; iSupport++)
		{
			CString Text;
			JUnt32 IdSupport;
			// on récupère l'identifiant du Support via le plateau
			const CKeySupport & KeySupp = m_pATPDocCreuset->m_PlateauSupports.Item(iSupport);
			IdSupport = KeySupp.m_IdSupport; 

			Text.Format(_T("Support %09d"), IdSupport);

			// on teste si on a trouvé le Support
			if (IdSupport)
			{
				// on récupère le Support
				CRegroupements * pReg = CRegroupements::GetInstance();
				IElem * pElem= pReg->GetIElemByID(IdSupport);
				if(pElem)
				{
					if(!KeySupp.m_IdParent)
					{
						if(pElem->IsCouplage())
							// Support Enfant decalé
							// Text.Format("+ %s", pElem->GetLabel().AsJCharPtr());
							Text.Format("%s", pElem->GetLabel().AsJCharPtr());
						else
							// on écrit le libellé du Support
							Text = pElem->GetLabel().AsJCharPtr();
					}
					else
						// Support Enfant decalé
						Text.Format("---> %s", pElem->GetLabel().AsJCharPtr());
				}
			}
			//boucle sur les formats				
			for( lstFormat.MoveFirst();lstFormat.IsValid();lstFormat.MoveNext())
			{
				const JSRCPresse  & srcpresse = JSRCPresse::GetInstance();
				srcpresse.m_TBLFormats.MoveTo(lstFormat.GetItem()); // Récup item correspondant à l'indice
				if(srcpresse.m_TBLFormats.IsValid())
				{
					JFormat *pElem = (JFormat *)srcpresse.m_TBLFormats.GetItem();
					if(pElem)
						exportStr.AppendFormat("%s %s", Text, pElem->GetLabelCourt().AsJCharPtr());  
				}
				else
					exportStr.AppendFormat("%s Total", Text);  
				
				CKeyContrib key;
				key.m_Id1 = nHypo;
				key.m_Id2 =  lstFormat.GetItem();
				key.m_Id3 = iSupport;
				m_Contrib.MoveTo(key);
				if(m_Contrib.IsValid())
				{
					if(m_Contrib.GetItem().Couv()>=0)
						exportStr.AppendFormat("\t%.2f" ,m_Contrib.GetItem().Couv());
					if(m_Contrib.GetItem().GRP()>=0)
						exportStr.AppendFormat("\t%.1f" ,m_Contrib.GetItem().GRP());
					if(m_Contrib.GetItem().PartGRP()>=0)
						exportStr.AppendFormat("\t%.2f" ,m_Contrib.GetItem().PartGRP());
					if(m_Contrib.GetItem().Budget()>=0)
						exportStr.AppendFormat("\t%.0f" ,m_Contrib.GetItem().Budget());
					if(m_Contrib.GetItem().PartBudget()>=0)
						exportStr.AppendFormat("\t%.2f" ,m_Contrib.GetItem().PartBudget());
					if(m_Contrib.GetItem().CoutPtCouv()>=0)
						exportStr.AppendFormat("\t%.2f" ,m_Contrib.GetItem().CoutPtCouv());
				}
				exportStr += "\n";
		
				
			}
		}

		exportStr += "\n";
	}

	return exportStr;
}
