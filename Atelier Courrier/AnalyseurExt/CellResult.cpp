#include "StdAfx.h"
#include ".\cellresult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCellResult::CCellResult(void)
//:m_TResults(0,IBloc::And), m_UResults(0,IBloc::And)
{
	m_Calculated				= 0;

	m_bCalculated				= false;

	m_TEffectif					= 0.0;
	m_TnbCas					= 0.0;
	 m_TAgregatedQuantity		= 0.0;
	m_UEffectif					= 0.0;

	// Init des effectifs et quantity sans les 0
	m_TEffectif_0Exclu			= 0.0;
	m_TAgregatedQuantity_0Exclu = 0.0;

	m_TParams = NULL;
	m_UParams = NULL;
	m_UResults = NULL;
	m_TResults = NULL;
}

CCellResult::~CCellResult(void)
{
	if(m_TParams)
		delete m_TParams;
	if(m_UParams)
		delete m_UParams;
//	if(m_UResults)
//		delete m_UResults;
	if(m_TResults)
		delete m_TResults;
}

void	CCellResult::CopyParams(const CCalcItemParams &Param)
{
	if(!m_TParams)
		m_TParams = new CCalcItemParams();
	m_TParams->CopyParams(Param);

	if(!m_UParams)
		m_UParams = new CCalcItemParams();
	m_UParams->CopyParams(Param);
}

CResults *	CCellResult::AddResult(IBloc::OPERATOR Op)
{ 
	if(!m_TResults)
	{
		m_TResults  = new CResults(0,IBloc::And);
		m_UResults  = m_TResults->AddResult(new CResults(m_TResults,IBloc::And));
	}
	return m_UResults->AddResult(new CResults(m_UResults,Op));
}

CResults *	CCellResult::AddTarget(IBloc::OPERATOR Op) 
{
	if(!m_TResults)
		m_TResults  = new CResults(0,IBloc::And);
	return m_TResults->AddResult(new CResults(m_TResults,Op)); 
}

// Gets the result
double	CCellResult::GetResult(RESULTTYPE Type, const JFLT32VECTOR * pQuanti, bool ModeMoyenne0Exclu, ITEMTYPE ElmtType, bool ModeExclusif)
{

	if(!Calculate(Type, pQuanti, ElmtType, ModeExclusif))
		return 0.0;

	switch(Type)
	{
		case RTYPE_EFFECTIVE_000:
		{
			return m_TEffectif/1000.0;
		}
		break;

		case RTYPE_NBCAS:
		{
			return m_TnbCas;
		}
		break;

		case RTYPE_QUANTITY_000:
		{
			// Attention au mode de calcul quantity, avec ou sans les 0
			if (ModeMoyenne0Exclu == false)
				return m_TAgregatedQuantity/1000.0;
			else
				return m_TAgregatedQuantity_0Exclu/1000.0;
		}
		break;

		case RTYPE_AVERAGEQUANTUM:
		case RTYPE_AVERAGEQUANTUM_P00:
		{
			// Attention au mode de calcul moyenne, avec ou sans les 0
			if (ModeMoyenne0Exclu == false)
			{
				if(m_TEffectif != 0.0)
					return m_TAgregatedQuantity/m_TEffectif; 
			}
			else
			{
				if(m_TEffectif_0Exclu != 0.0)
					return m_TAgregatedQuantity_0Exclu/m_TEffectif_0Exclu; 
			}
		}
		break;

		case RTYPE_EFFECTIVE_NONCIBLE:
		{
			return m_UEffectif;
		}
		break;

		case RTYPE_EFFECTIVE_CIBLE:
		{
			if (ModeMoyenne0Exclu == false)
			{
				return m_TEffectif;
			}
			else
			{
				return m_TEffectif_0Exclu;
			}
		}
		break;
	}
	return 0.0;
}

bool CCellResult::Calculate(RESULTTYPE Type, const JFLT32VECTOR * pQuanti, ITEMTYPE ElmtType, bool ModeExclusif)
{
	if(m_bCalculated)
		return true;

	const CBitArray * pResult = 0;
	const JFLT32VECTOR * pProbas = 0;

	if(m_TResults != 0)
	{
		if(!m_TResults->HasResult())
		{
			m_TResults->Calculate();

			m_TResults->GetResult(pResult);
			m_TParams->SetResults(pResult);
			m_TResults->GetProbas(pProbas);
			m_TParams->SetProbas(pProbas);

			// OPTIM A VOIR ****
			JMap<const JFLT32VECTOR*, JInt32> MapValues;
			if ((ElmtType == ITEMTYPE::ITYPE_NICHE || ElmtType == ITEMTYPE::ITYPE_CONTRENICHE) && ModeExclusif)
				m_TParams->SetNumericalValues(m_TResults->GetAllValues(MapValues), true); 
			else
				m_TParams->SetNumericalValues(m_TResults->GetAllValues(MapValues), false); 

			//
		}


		// OPTIM **
		if(m_TParams->DoEffectif(pQuanti))
			m_Calculated |= (RTYPE_EFFECTIVE_000|RTYPE_NBCAS|RTYPE_EFFECTIVE_CIBLE|RTYPE_AVERAGEQUANTUM|RTYPE_AVERAGEQUANTUM_P00);
		//

		// OPTIM ****
		// if(m_TParams->DoNumericalValues())
		if(m_TParams->DoNumericalValues(pQuanti))
			m_Calculated |= RTYPE_QUANTITY_000;

		m_TEffectif					= m_TParams->GetEffectif();
		m_TnbCas					= m_TParams->GetNbCas();
		m_TAgregatedQuantity		= m_TParams->GetAgregatedQuantity();

		// Récupère les effectis et quantity sans les 0 du vecteur valeurs numériques
		if (m_TParams->DoEffectif0Exclu(pQuanti))
		{
			m_TEffectif_0Exclu			= m_TParams->GetEffectif0Exclu();
			m_TAgregatedQuantity_0Exclu	= m_TParams->GetAgregatedQuantity();
		}
	}

	if(m_UResults != 0)
	{
		if(!m_UResults->HasResult())
		{
			m_UResults->Calculate();
		}

		m_UResults->GetResult(pResult);
		m_UParams->SetResults(pResult);
		m_UResults->GetProbas(pProbas);
		m_UParams->SetProbas(pProbas);

		// OPTIM **** LONG
		JMap<const JFLT32VECTOR*, JInt32> MapValues;
		if ((ElmtType == ITEMTYPE::ITYPE_NICHE || ElmtType == ITEMTYPE::ITYPE_CONTRENICHE)  && ModeExclusif)
			m_UParams->SetNumericalValues(m_UResults->GetAllValues(MapValues), true);
		else
			m_UParams->SetNumericalValues(m_UResults->GetAllValues(MapValues), false);
		//

		// OPTIM 2.7 ****
		if(m_UParams != 0 && m_UParams->DoEffectif(pQuanti))
			m_Calculated |= RTYPE_EFFECTIVE_NONCIBLE;

		m_UEffectif	= m_UParams->GetEffectif();
	}

	if(m_TParams != 0)
		delete m_TParams;
	if(m_UParams != 0)
		delete m_UParams;
	if(m_TResults != 0)
		delete m_TResults;

	m_TParams = NULL;
	m_UParams = NULL;
	m_UResults = NULL;
	m_TResults = NULL;
	
	m_bCalculated = true;

	return (Type&m_Calculated);
}
