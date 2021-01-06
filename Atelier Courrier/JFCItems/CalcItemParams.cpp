#include "stdafx.h"
#include ".\calcitemparams.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCalcItemParams::CCalcItemParams(void)
{
	m_Effectif			= 0.0;
	m_NbCas				= 0;
	m_EffectifTot		= 0.0;
	m_NbCasTot			= 0;

	m_Effectif0Exclu	= 0.0;
	m_NbCas0Exclu		= 0;
	m_EffectifTot0Exclu	= 0.0;
	m_NbCasTot0Exclu	= 0;

	m_pResults			= 0;
	m_pResultsD			= 0;
	m_pProbas			= 0;
	m_pProbasD			= 0;
	m_pTerrain			= 0;
	m_EchantillonID		= 1;
	m_UniversID			= 1;
	m_TypePoidsID		= 1;
	m_PopSegment.RemoveAll();
	m_pValuesD			= 0;
	m_pValuesD_Binaire	= 0;
	m_pValueBinaire		= 0;
	m_Values			= 0.0;
	m_MinBorne			= 0.0;
	m_MaxBorne			= 0.0;
	m_IdSegments.Reset();
	m_ItemAutorise		= true;
	m_CumulQuanti		= false;

	// Aout 2013
	m_DivSpeNiche		= 1.0;
}

void CCalcItemParams::SetSegmentsIDs(const JVector<JInt32> & IdSegments)
{
	m_IdSegments.Reset();
	m_IdSegments.SetCount(IdSegments.GetCount());
	m_IdSegments.MoveFirst();
	for (IdSegments.MoveFirst(); IdSegments.IsValid(); IdSegments.MoveNext()) {
		m_IdSegments.GetItem() = IdSegments.GetItem();
		m_IdSegments.MoveNext();
	}
}
void CCalcItemParams::SetSegmentsIDs(const JList<JInt32> & IdSegments)
{
	m_IdSegments.Reset();
	m_IdSegments.SetCount(IdSegments.GetCount());
	m_IdSegments.MoveFirst();
	for (IdSegments.MoveFirst(); IdSegments.IsValid(); IdSegments.MoveNext()) {
		m_IdSegments.GetItem() = IdSegments.GetItem();
		m_IdSegments.MoveNext();
	}
}
void CCalcItemParams::SetSegmentsIDs(const JList<JUnt32> & IdSegments)
{
	m_IdSegments.Reset();
	m_IdSegments.SetCount(IdSegments.GetCount());
	m_IdSegments.MoveFirst();
	for (IdSegments.MoveFirst(); IdSegments.IsValid(); IdSegments.MoveNext())
	{
		m_IdSegments.GetItem() = static_cast<JUnt32>(IdSegments.GetItem());
		m_IdSegments.MoveNext();
	}
}
void CCalcItemParams::SetSegmentsIDs(const std::list<JUnt32> & IdSegments)
{
	m_IdSegments.Reset();
	m_IdSegments.SetCount(IdSegments.size());
	m_IdSegments.MoveFirst();
	std::list<JUnt32>::const_iterator seg_it = IdSegments.begin();
	while(seg_it != IdSegments.end())
	{
		m_IdSegments.GetItem() = static_cast<JUnt32>(*seg_it);
		m_IdSegments.MoveNext();
		++seg_it;
	}
}
CCalcItemParams::~CCalcItemParams(void)
{
	if(m_pProbasD)
		delete m_pProbasD;

	if(m_pResultsD)
		delete m_pResultsD;

	if(m_pValuesD)
		delete m_pValuesD;

	if(m_pValuesD_Binaire)
		delete m_pValuesD_Binaire;

	if(m_pValueBinaire)
		delete m_pValueBinaire;
}

// Copies the parameters from rhs
void CCalcItemParams::CopyParams(const CCalcItemParams &rhs)
{
	m_pTerrain		= rhs.m_pTerrain ;
	m_EchantillonID = rhs.m_EchantillonID ;
	m_UniversID		= rhs.m_UniversID ;
	m_TypePoidsID	= rhs.m_TypePoidsID ;
	m_PopSegment	= rhs.m_PopSegment ;

	m_IdSegments.Reset();
	m_IdSegments.SetCount(rhs.m_IdSegments.GetCount());
	m_IdSegments.MoveFirst();
	for (rhs.m_IdSegments.MoveFirst(); rhs.m_IdSegments.IsValid(); rhs.m_IdSegments.MoveNext()) {
		m_IdSegments.GetItem() = rhs.m_IdSegments.GetItem();
		m_IdSegments.MoveNext();
	}

	delete m_pValuesD;
	delete m_pValuesD_Binaire;

	if (rhs.m_pValuesD)
	{
		m_pValuesD = new JFLT32VECTOR;
		m_pValuesD->SetCount(rhs.m_pValuesD->GetCount());
		for (m_pValuesD->MoveFirst(), rhs.m_pValuesD->MoveFirst(); m_pValuesD->IsValid(); m_pValuesD->MoveNext(), rhs.m_pValuesD->MoveNext())
			m_pValuesD->GetItem() = rhs.m_pValuesD->GetItem();
	}
	else m_pValuesD = 0;

	if (rhs.m_pValuesD_Binaire)
	{
		m_pValuesD_Binaire = new JFLT32VECTOR;
		m_pValuesD_Binaire->SetCount(rhs.m_pValuesD_Binaire->GetCount());
		for (m_pValuesD_Binaire->MoveFirst(), rhs.m_pValuesD_Binaire->MoveFirst(); m_pValuesD_Binaire->IsValid(); m_pValuesD_Binaire->MoveNext(), rhs.m_pValuesD_Binaire->MoveNext())
		{
			if (rhs.m_pValuesD_Binaire->GetItem() >= 1)
				m_pValuesD_Binaire->GetItem() = 1;
			else
				m_pValuesD_Binaire->GetItem() = 1;
		}
	}
	else m_pValuesD_Binaire = 0;

	m_ItemAutorise = rhs.m_ItemAutorise;

	m_CumulQuanti  = rhs.m_CumulQuanti;
}

// Apparemment cette fonction récupère les valeurs numériques pour calculer les moyennes
void CCalcItemParams::SetNumericalValues(const JFLT32VECTOR* pValues)
{
	delete m_pValuesD;
	delete m_pValuesD_Binaire;

	if (pValues)
	{
		m_pValuesD			= new JFLT32VECTOR;
		m_pValuesD_Binaire	= new JFLT32VECTOR;

		m_pValuesD->SetCount(pValues->GetCount());
		m_pValuesD_Binaire->SetCount(pValues->GetCount());

		const CBitArray * quali = GetResults();
		
		int idx = 0;
		for (m_pValuesD->MoveFirst(), pValues->MoveFirst(); m_pValuesD->IsValid(); m_pValuesD->MoveNext(), pValues->MoveNext())
		{
			bool qualiItem = quali->GetAt(idx++);

			m_pValuesD->GetItem() = (qualiItem)? pValues->GetItem() : 0;

			if (pValues->GetItem() >= 1 && qualiItem)
				m_pValuesD_Binaire->GetItem() = 1;
			else
				m_pValuesD_Binaire->GetItem() = 0;

		}

	}
	else m_pValuesD = 0;


}

void CCalcItemParams::SetNumericalValues(const JMap<const JFLT32VECTOR*, JInt32> & ValuesMap, bool SpeItemNiche, bool NicheQuantifiable)
{
	// OPTIM 2.7
	delete m_pValuesD;

	ValuesMap.MoveFirst();
	if (ValuesMap.IsValid())
	{
		// Init Vector Deletable
		m_pValuesD = new JFLT32VECTOR;
		JInt32 NbIndivs = ValuesMap.GetKey()->GetCount();
		m_pValuesD->SetCount(NbIndivs);
		
		for (m_pValuesD->MoveFirst(); m_pValuesD->IsValid(); m_pValuesD->MoveNext())
		{ 
			m_pValuesD->GetItem() = -1.0; 
		}

		// A OPTIMISER 2.7
		if (ValuesMap.GetCount() == 1)
		{
			const JFLT32VECTOR* pValues = ValuesMap.GetKey();

			// Construit le vecteur binaire pour filtrage quanti (MARS 2014)
			const CBitArray * quali = GetResults();
			int idx = 0;

			for (m_pValuesD->MoveFirst(), pValues->MoveFirst(); m_pValuesD->IsValid(); m_pValuesD->MoveNext(), pValues->MoveNext())
			{
				// Récup qualitatif value  (0 si à ne pas prendre en compte)
				bool qualiItem = quali->GetAt(idx++);

				double val_value = pValues->GetItem();

				// Il faut en + tester via le filtrage
				if (qualiItem && pValues->GetItem() >= 0.0)
				{
					m_pValuesD->GetItem() = pValues->GetItem();
				}
			}

			// A VOIR 2014 AAAAA REMETTRE (3)
			/*
			JFLT32VECTOR* pVal = (JFLT32VECTOR*)pValues;
			pVal->Reset();
			*/
		}
		else
		{
			double val = 0;

			while (ValuesMap.IsValid())
			{
				const JFLT32VECTOR* pValues = ValuesMap.GetKey();

				// Construit le vecteur binaire pour filtrage quanti (MARS 2014)
				const CBitArray * quali = GetResults();
				int idx = 0;

				for (m_pValuesD->MoveFirst(), pValues->MoveFirst(); m_pValuesD->IsValid(); m_pValuesD->MoveNext(), pValues->MoveNext())
				{
					if (pValues && pValues->GetCount() > 0)
					{
						// Récup qualitatif value  (0 si à ne pas prendre en compte)
						double val_value = pValues->GetItem();

						bool qualiItem = quali->GetAt(idx++);

						// MARS 2014 / Il faut en + tester via le filtrage cible
						if (qualiItem && pValues->GetItem() >= 0.0)
						{
							val = m_pValuesD->GetItem();

							if (m_pValuesD->GetItem() >= 0.0) 
								m_pValuesD->GetItem() += pValues->GetItem();
							else
								m_pValuesD->GetItem() = pValues->GetItem();

							val = m_pValuesD->GetItem();
						}
					}
				}
				ValuesMap.MoveNext();
			}
		}
	}
	else 
		m_pValuesD = 0;

	// A VIRER
	//// MARS 2014
	//// TEST  AOUT 2013  (pour la niche il faut diviser par le nombre de lignes la composant)
	//if (m_pValuesD && SpeItemNiche == true && !NicheQuantifiable)
	//{
	//	double div = ValuesMap.GetCount();
	//	for (m_pValuesD->MoveFirst(); m_pValuesD->IsValid(); m_pValuesD->MoveNext())
	//	{
	//		m_pValuesD->GetItem() = m_pValuesD->GetItem() / div;
	//	}
	//}
	
}

const JDBLVECTOR * CCalcItemParams::GetStatisticValues()
{
	if(m_pTerrain)
		return m_pTerrain->GetStatisticValues(m_EchantillonID,m_UniversID,m_TypePoidsID);
	else
		return 0;
}

void CCalcItemParams::SetResults(const CBitArray * pResults)
{
	m_pResults = pResults;
}

void CCalcItemParams::SetResultsD(const CBitArray * pResults)
{ 
	if(m_pResultsD)
		delete m_pResultsD;

	m_pResultsD = 0;

	if(pResults)
		m_pResultsD = new CBitArray(*pResults);
}

void CCalcItemParams::SetProbas(const JFLT32VECTOR * pProba)
{
	m_pProbas = pProba;
}

void CCalcItemParams::SetProbasD(const JFLT32VECTOR * pProba)
{
	if(m_pProbasD)
		delete m_pProbasD;
	m_pProbasD = 0;

	if(pProba)
	{
		m_pProbasD = new JFLT32VECTOR;
		m_pProbasD->SetCount(pProba->GetCount());
		for(pProba->MoveFirst(), m_pProbasD->MoveFirst(); pProba->IsValid(); pProba->MoveNext(), m_pProbasD->MoveNext())
			m_pProbasD->GetItem() = pProba->GetItem();
	}
}

void CCalcItemParams::Raz_pValuesD()
{
	/*
	if (m_pValuesD)
	{
		JFLT32VECTOR *pVal = (JFLT32VECTOR *)m_pValuesD;
		pVal->Reset();
	}
	*/
}

//////////////////////////////////////////////////////////////////////////////////
// On calcule l'effectif
bool CCalcItemParams::DoEffectif(const JFLT32VECTOR * pQuanti)
{
	m_Effectif		=	0.0;
	m_EffectifTot	=	0.0;
	m_NbCas			=	0;
	m_NbCasTot		=	0;

	JFlt32 Proba	=	1.0;
	JInt32 x		=	0;
	const JFLT32VECTOR * pProba		= GetProbas();
	const CBitArray * pIndiv		= GetResults();
	CBitArray Filter				= m_PopSegment;
	const CBitArray * pEchantillon	= GetEchantillonFilter();
	if(pEchantillon)
		Filter &= (*pEchantillon);
	double Factor					= GetFactor(); 
	const JDBLVECTOR *	Poids		= GetStatisticValues();

	JInt32 NbIndivs = Filter.GetSize();

	int Nb = 0;

	if (m_ItemAutorise)
	{
		if(pProba && Poids)
		{
			/* OPTIM 2.7
			for(x = 0; x < NbIndivs ; ++x)
			{
				if(Filter.GetAt(x))
				{
					Poids->MoveTo(x);
					if(Poids->GetItem() == 0.0)
						continue;

					// Pas de cumul si la valeur numérique est < zéro  (ce sont les #)
					if (pQuanti != 0)
					{
						pQuanti->MoveTo(x);
						if(pQuanti->GetItem() < 0.0)
							continue;
					}

					double val = (Poids->GetItem()*Factor);

					pProba->MoveTo(x);
					Proba = pProba->GetItem();

					if(Proba)
					{
						m_Effectif += (val*Proba);
						++m_NbCas;
					}
					m_EffectifTot += val;
					++m_NbCasTot;

				}
			}
			*/

			// OPTIM
			if (pQuanti != 0)
			{
				for(x = 0; x < NbIndivs ; ++x)
				{
					if(Filter.GetAt(x))
					{
						Poids->MoveTo(x);
						if(Poids->GetItem() == 0.0)
							continue;

						// Pas de cumul si la valeur numérique est < zéro  (ce sont les #)
						pQuanti->MoveTo(x);
						if(pQuanti->GetItem() < 0.0)
							continue;

						double val = (Poids->GetItem()*Factor);

						pProba->MoveTo(x);
						Proba = pProba->GetItem();

						if(Proba)
						{
							m_Effectif += (val*Proba);
							++m_NbCas;
						}
						m_EffectifTot += val;
						++m_NbCasTot;

					}
				}
			}
			else
			{
				for(x = 0; x < NbIndivs ; ++x)
				{
					if(Filter.GetAt(x))
					{
						Poids->MoveTo(x);
						if(Poids->GetItem() == 0.0)
							continue;

						double val = (Poids->GetItem()*Factor);

						pProba->MoveTo(x);
						Proba = pProba->GetItem();

						if(Proba)
						{
							m_Effectif += (val*Proba);
							++m_NbCas;
						}
						m_EffectifTot += val;
						++m_NbCasTot;

					}
				}
			}
		}
		else if(pIndiv && Poids)
		{
			/* OPTIM 2.7
			for(x = 0; x < NbIndivs; ++x)
			{
				if(Filter.GetAt(x))
				{
					Poids->MoveTo(x);
					if(Poids->GetItem() == 0.0)
						continue;

					// Pas de cumul si la valeur numérique est < zéro  (ce sont les #)
					if (pQuanti != 0)
					{
						pQuanti->MoveTo(x);
						if(pQuanti->GetItem() < 0.0)
							continue;
					}

					double val = (Poids->GetItem()*Factor);

					if(pIndiv->GetAt(x))
					{
						m_Effectif += val;
						++m_NbCas;
					}
					m_EffectifTot += val;
					++m_NbCasTot;

				}
			}
			*/

			// OPTIM
			if (pQuanti != 0)
			{
				for(x = 0; x < NbIndivs; ++x)
				{
					if(Filter.GetAt(x))
					{
						Poids->MoveTo(x);
						if(Poids->GetItem() == 0.0)
							continue;

						// Voir 2014
						// Pas de cumul si la valeur numérique est < zéro  (ce sont les #)
						pQuanti->MoveTo(x);
						if (pQuanti->IsValid())
						{
							if(pQuanti->GetItem() < 0.0)
								continue;
						}
						else
							continue;
					

						double val = (Poids->GetItem()*Factor);

						if(pIndiv->GetAt(x))
						{
							m_Effectif += val;
							++m_NbCas;
						}
						m_EffectifTot += val;
						++m_NbCasTot;

					}
				}
			}
			else
			{
				for(x = 0; x < NbIndivs; ++x)
				{
					if(Filter.GetAt(x))
					{
						Poids->MoveTo(x);
						if(Poids->GetItem() == 0.0)
							continue;

						double val = (Poids->GetItem()*Factor);

						if(pIndiv->GetAt(x))
						{
							m_Effectif += val;
							++m_NbCas;
						}
						m_EffectifTot += val;
						++m_NbCasTot;

					}
				}
			}
		}
		else
			return false;
	}

	return true;
}

////////////////////////////////////////////////////////////////////////////////////
// On calcule l'effectif sans les zéros du vecteur quantitatif
bool CCalcItemParams::DoEffectif0Exclu(const JFLT32VECTOR * pQuanti)
{
	m_Effectif0Exclu	=	0.0;
	m_EffectifTot0Exclu	=	0.0;
	m_NbCas0Exclu		=	0;
	m_NbCasTot0Exclu	=	0;

	if (pQuanti == 0)
	{

		// Par defaut les exclu 0 sont initialisé aux mêmes valeurs (cas des items non numériques)
		m_Effectif0Exclu	= m_Effectif; 
		m_NbCas0Exclu		= m_NbCas;
		m_EffectifTot0Exclu	= m_Effectif; 
		m_NbCasTot0Exclu	= m_NbCas;
		return true;
	}

	JFlt32 Proba		=	1.0;
	JInt32 x			=	0;
	const JFLT32VECTOR * pProba		= GetProbas();

	const CBitArray * pIndiv		= GetResults();
	CBitArray Filter				= m_PopSegment;
	const CBitArray * pEchantillon	= GetEchantillonFilter();
	if(pEchantillon)
		Filter &= (*pEchantillon);
	double Factor					= GetFactor(); 
	const JDBLVECTOR *	Poids		= GetStatisticValues();

	// Tout d'abord on vérifie que le vecteur valeurs numériques a le même nombre d'individus
	JInt32 NbI1 = Poids->GetCount();
	JInt32 NbI2 = pQuanti->GetCount();

	if (pQuanti->GetCount() != Poids->GetCount()) return false;

	JInt32 NbIndivs = Filter.GetSize();

	if (m_ItemAutorise)
	{
		if(pProba && Poids)
		{

			/* OPTIM 2.7
			for(x = 0; x < NbIndivs ; ++x)
			{
				if(Filter.GetAt(x))
				{
					Poids->MoveTo(x);
					if(Poids->GetItem() == 0.0)
						continue;

					// Pas de cumul si la valeur numérique est à zéro
					pQuanti->MoveTo(x);
					if(pQuanti->GetItem() <= 0.0)
						continue;

					double val = (Poids->GetItem()*Factor);

					pProba->MoveTo(x);
					Proba = pProba->GetItem();

					if(Proba)
					{
						m_Effectif0Exclu += (val*Proba);
						++m_NbCas0Exclu;
					}
					m_EffectifTot0Exclu += val;
					++m_NbCasTot0Exclu;
				}
				else
					JBool Ok = false;
			}
			*/

			// OPTIM
			for(Poids->MoveFirst(), pQuanti->MoveFirst(), pProba->MoveFirst(), x=0; Poids->IsValid(), x<NbIndivs; ++x, Poids->MoveNext(), pQuanti->MoveNext(), pProba->MoveNext())
			{
				if(Filter.GetAt(x))
				{
					if(Poids->GetItem() == 0.0)
						continue;

					// Pas de cumul si la valeur numérique est à zéro
					if(pQuanti->GetItem() <= 0.0)
						continue;

					double val = (Poids->GetItem()*Factor);
					if(pProba->GetItem())
					{
						m_Effectif0Exclu += (val * pProba->GetItem());
						++m_NbCas0Exclu;
					}
					m_EffectifTot0Exclu += val;
					++m_NbCasTot0Exclu;
				}
				else
					JBool Ok = false;
			}
		}
		else if(pIndiv && Poids)
		{
			/* OPTIM 2.7
			for(x = 0; x < NbIndivs; ++x)
			{
				if(Filter.GetAt(x))
				{
					Poids->MoveTo(x);
					if(Poids->GetItem() == 0.0)
						continue;

					// Pas de cumul si la valeur numérique est à zéro
					pQuanti->MoveTo(x);
					if(pQuanti->GetItem() <= 0.0)
						continue;

					double val = (Poids->GetItem()*Factor);

					if(pIndiv->GetAt(x))
					{
						m_Effectif0Exclu += val;
						++m_NbCas0Exclu;
					}
					m_EffectifTot0Exclu += val;
					++m_NbCasTot0Exclu;
				}
				else
					JBool Ok = false;
			}
			*/

			// OPTIM
			for(Poids->MoveFirst(), pQuanti->MoveFirst(), x=0; Poids->IsValid(), x<NbIndivs; ++x, Poids->MoveNext(), pQuanti->MoveNext())
			{
				if(Filter.GetAt(x))
				{
					if(Poids->GetItem() == 0.0)
						continue;

					// Pas de cumul si la valeur numérique est à zéro
					if(pQuanti->GetItem() <= 0.0)
						continue;

					double val = (Poids->GetItem()*Factor);

					if(pIndiv->GetAt(x))
					{
						m_Effectif0Exclu += val;
						++m_NbCas0Exclu;
					}
					m_EffectifTot0Exclu += val;
					++m_NbCasTot0Exclu;
				}
				else
					JBool Ok = false;
			}
		}
		else
			return false;
	}

	return true;
}

/// Sets up and calculate the Numerical Values (If any)
// bool	CCalcItemParams::DoNumericalValues(bool Moyenne0Exclu)
bool	CCalcItemParams::DoNumericalValues(bool Moyenne0Exclu, const JFLT32VECTOR * pQuanti)
{
	m_Values			=	0.0;

	JFlt32 Proba		=	1.0;
	JInt32 x			=	0;

	const JFLT32VECTOR * pProba		= GetProbas();
	const CBitArray * pIndiv		= GetResults();
	CBitArray Filter				= m_PopSegment;
	const CBitArray * pEchantillon	= GetEchantillonFilter();
	if(pEchantillon)
		Filter &= (*pEchantillon);
	double Factor					= GetFactor(); 
	const JDBLVECTOR *	Poids		= GetStatisticValues();
	const JFLT32VECTOR * Values		= GetNumericalValues();

	// OPTIM
	double TestEffectif = 0.0;

	JInt32 NbIndivs = Filter.GetSize();

	if(Values && pProba && Poids)
	{
		/* OPTIM 2.7
		for(x = 0, Values->MoveFirst(); Values->IsValid(); ++x, Values->MoveNext())
		{
			double Val = Values->GetItem();
			if(Val > -1.0 && Filter.GetAt(x))
			{
				pProba->MoveTo(x);
				Proba = pProba->GetItem();
				if(Proba)
				{
					Poids->MoveTo(x);
					if(Poids->GetItem() == 0.0)
						continue;

					// On ne comptabilise pas les valeurs numériques 0
					if (Val == 0.0 && Moyenne0Exclu)
						continue;

					double Pds = (Poids->GetItem()*Factor);
					m_Values += (Val * (Pds*Proba));
				}
			}
		}
		*/

		// OPTIM
		for(x = 0, Values->MoveFirst(), pProba->MoveFirst(), Poids->MoveFirst(); Values->IsValid(); ++x, Values->MoveNext(), pProba->MoveNext(), Poids->MoveNext())
		{
			double Val = Values->GetItem();
			if(Values->GetItem() > -1.0 && Filter.GetAt(x))
			{
				if(pProba->GetItem())
				{
					if(Poids->GetItem() == 0.0)
						continue;

					// On ne comptabilise pas les valeurs numériques 0
					if (Values->GetItem() == 0.0 && Moyenne0Exclu)
						continue;

					double Pds = (Poids->GetItem()*Factor);
					m_Values += (Values->GetItem() * (Pds * pProba->GetItem()));
				}
			}
		}
	}
	else if(Values && pIndiv && Poids)
	{
		/* OPTIM 2.7
		for(x = 0, Values->MoveFirst(); Values->IsValid(); ++x, Values->MoveNext())
		{
			double Val = Values->GetItem();
			if(Val > -1.0 && Filter.GetAt(x) && pIndiv->GetAt(x))
			{
				Poids->MoveTo(x);
				if(Poids->GetItem() == 0.0)
					continue;

				// On ne comptabilise pas les valeurs numériques 0
				if (Val == 0.0 && Moyenne0Exclu)
					continue;

				m_Values += Val * (Poids->GetItem()*Factor);
			}
		}
		*/
		
		bool okTest =true;

		for(x = 0, Values->MoveFirst(), Poids->MoveFirst(); Values->IsValid(); ++x, Values->MoveNext(), Poids->MoveNext())
		{
			double Val_Values = Values->GetItem();

			if(pIndiv->GetAt(x))
			{
				if(Filter.GetAt(x))
				{
					double Val = Values->GetItem();
					if(Values->GetItem() > -1.0 && Filter.GetAt(x) && pIndiv->GetAt(x))
					{
						if(Poids->GetItem() == 0.0)
							continue;

						// On ne comptabilise pas les valeurs numériques 0
						if (Values->GetItem() == 0.0 && Moyenne0Exclu)
							continue;

						double ValPoid = Poids->GetItem();
						m_Values += Values->GetItem() * (Poids->GetItem()*Factor);

						TestEffectif += (Poids->GetItem()*Factor);
					}
				}
			}
		}
	}
	else
		return false;

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// Récupére les valeurs numériques, avec ou sans tenir compte des zéros
const JFLT32VECTOR *	CCalcItemParams::GetNumericalValues(bool TransfertBinaire, bool Value0_Ok) const
{ 
	if (!TransfertBinaire)
	{
		// Pas de transformation en vecteur binaire demandé

		// A VIRER
		/*
		double tot = 0;
		if (m_pValuesD)
		{
			int x = 0;
			for (m_pValuesD->MoveFirst(); m_pValuesD->IsValid(); m_pValuesD->MoveNext())
			{
				JFlt32 &Value = m_pValuesD->GetItem();
				if (Value > 0)
					tot += Value;

				if (x == 93)
					bool Stop = true;
				x++;
			}
		}
		*/
		return m_pValuesD;
	}
	else
	{
		/*
		if (m_pValuesD)
		{
			for (m_pValuesD->MoveFirst(); m_pValuesD->IsValid(); m_pValuesD->MoveNext())
			{
				JFlt32 &Value = m_pValuesD->GetItem();
				if (Value > 0)
					Value = 1;
			}
		}
		*/
		return m_pValueBinaire;
	}
} 