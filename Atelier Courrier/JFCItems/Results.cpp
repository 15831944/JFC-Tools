#include "stdafx.h"
#include ".\results.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CResults::CResults(CResults *pPrev, IBloc::OPERATOR ResultOperator)
:m_pPrev(pPrev), m_ResultOp(ResultOperator)
{
	// Individual vector
	m_pResult		= 0; // Constant
	m_pResultD		= 0; // Deletable
	// Probabilities vector
	m_pProbas		= 0; // Constant
	m_pProbasD		= 0; // Deletable
	// values vector
	m_pValues		= 0; // Constant
	m_pValuesD		= 0; // Deletable

	m_bPoints			= false;
	m_PointsOperator	= IBloc::Nop; 

	m_bCalculated	=	false;
	m_bCalcRet		=	false;
}

CResults::~CResults(void)
{
	if(m_pResultD)
		delete m_pResultD;

	if(m_pProbasD)
		delete m_pProbasD;

	if(m_pValuesD)
		delete m_pValuesD;

	for(m_Results.MoveFirst(); m_Results.IsValid(); m_Results.MoveNext())
		delete m_Results.GetItem();
}

void CResults::Reset()
{
	if(m_pResultD)
		delete m_pResultD;

	if(m_pProbasD)
		delete m_pProbasD;

	if(m_pValuesD)
		delete m_pValuesD;

	for(m_Results.MoveFirst(); m_Results.IsValid(); m_Results.MoveNext())
		delete m_Results.GetItem();

	// Individual vector
	m_pResult		= 0; // Constant
	m_pResultD		= 0; // Deletable
	// Probabilities vector
	m_pProbas		= 0; // Constant
	m_pProbasD		= 0; // Deletable
	// Values vector
	m_pValues		= 0; // Constant
	m_pValuesD		= 0; // Deletable
	m_bPoints			= false;
	m_PointsOperator	= IBloc::Nop; 
	/// Results Operator
	m_ResultOp			= IBloc::Nop;
	/// List of Sub Result objects
	m_Results.Reset();
	/// Previous Result object (Owner?)
	m_pPrev				= 0;
	m_PointsValue		= 0;
	m_vPoints.Reset();

	m_bCalculated	=	false;
	m_bCalcRet		=	false;
}

bool	CResults::GetResult(const CBitArray	* & pResult) const
{ 
	pResult = m_pResult;

	if(m_pResultD)
	{
		pResult = m_pResultD;
		return  false;
	}

	return true;
}

bool	CResults::GetProbas(const JFLT32VECTOR * & pProba) const
{
	pProba = m_pProbas;

	if(m_pProbasD)
	{
		pProba = m_pProbasD;
		return false;
	}
	return true; 
}

CResults *	CResults::AddResult(CResults *pResult)
{
	m_Results.AddTail() = pResult;
	return pResult;
}

// Récupére les valeurs vecteurs individus, les transforme en vecteur binaire si nécessaire
void	CResults::SetValues(const JFLT32VECTOR * pValues, bool TransBinary)
{
	m_pValues = pValues;
}



const JMap<const JFLT32VECTOR*, JInt32> & CResults::GetAllValues(JMap<const JFLT32VECTOR*, JInt32> & ValuesMap)
{
	JMap<const JFLT32VECTOR*, JInt32> TmpValues;
 	GetValuesMap(TmpValues); ValuesMap.Swap(TmpValues);
	return ValuesMap;
}

void	CResults::GetValuesMap(JMap< const JFLT32VECTOR *,JInt32> & ValuesMap)
{
	if(m_Results.GetCount())
	{
		for(m_Results.MoveFirst(); m_Results.IsValid(); m_Results.MoveNext())
			m_Results.GetItem()->GetValuesMap(ValuesMap);
	}

	if(m_pValues != 0)
	{
		ValuesMap.MoveTo(m_pValues);
		if(!ValuesMap.IsValid())
			ValuesMap.Add(m_pValues) = 1;
		else
			ValuesMap.GetItem() += 1;
	}
	if(m_pValuesD != 0)
	{
		ValuesMap.MoveTo(m_pValuesD);
		if(!ValuesMap.IsValid())
			ValuesMap.Add(m_pValuesD) = 1;
		else
			ValuesMap.GetItem() += 1;
	}
}

void	CResults::SetValuesFiltre(const JFLT32VECTOR * pValues, const CBitArray *pFiltre)
{
	if(m_Results.GetCount())
	{
		for(m_Results.MoveFirst(); m_Results.IsValid(); m_Results.MoveNext())
			m_Results.GetItem()->SetValuesFiltre(pValues,pFiltre);
	}
	else
	{
		if(pValues == m_pValues)
		{
			if(m_pResultD)
				*m_pResultD &= *pFiltre;
			else
			{
				m_pResultD = new CBitArray((*m_pResult&*pFiltre));
			}
		}
	}
}

JBool	CResults::Calculate()
{
	if(m_bCalculated)
		return m_bCalcRet;

	m_bCalcRet = m_bCalculated = true;

	// Nothing to do  if no sub termes
	if(m_Results.GetCount() == 0)
		return m_bCalcRet;

	bool bProba = false;

	JInt32 x=0;
	for(x =0,m_Results.MoveFirst(); m_Results.IsValid(); ++x,m_Results.MoveNext())
	{
		CResults * pResult = m_Results.GetItem();

		// Make sure that the subterme was calulated and has a result
		if(!pResult->Calculate() || !pResult->HasResult())
		{
			// Not calculated or has no result
			// so we cannot proceed any further
			m_bCalcRet = false;
			m_bCalculated = true;
			return m_bCalcRet;
		}

		// All is well so store the results
		if(pResult->HasProbas())
			bProba = true;
	}

	// Calcul sans proba
	m_bCalcRet = CalculateWithoutProba();

	if(m_bCalcRet & bProba)
	{
		// Calcul avec proba
		m_bCalcRet = CalculateWithProba();
	}

	// Check for question induite Point
	if(m_bCalcRet && m_bPoints)
	{
		if(bProba)
		{
			if(!CalculatePointsProba(m_vPoints))
				return false;
			if(!DoPointsOperationProba())
				return false;
		}
		else
		{
			if(!CalculatePoints(m_vPoints))
				return false;
			if(!DoPointsOperation())
				return false;
		}
	}
	return m_bCalcRet;
}

void	CResults::SetBinValues(const CBitArray *pValues)
{
	if(pValues)
	{
		if(m_pResultD==0)
		{
			if(m_pResult)
			{
				m_pResultD = new CBitArray((*m_pResult&*pValues));
				m_pResult = 0;
			}
			else
				m_pResult = pValues;
		}
		else
			*m_pResultD	&=	*pValues;
	}
}

JBool	CResults::CalculateWithoutProba()
{
	// Get new individuals vector, because JLib mes deux collections
	// are a pain in the arse!
	JInt32 Count = m_Results.GetCount();
	const CBitArray ** pIndiv = new const CBitArray * [Count];
	bool * bResultType = new bool [Count];
	int x = 0;
	int y = 0;
	int z = 0;

	// Calculate all sub terms first
	for(x = 0, m_Results.MoveFirst(); m_Results.IsValid(); ++x,m_Results.MoveNext())
	{
		// All is well so store the results
		bResultType[x] = m_Results.GetItem()->GetResult(pIndiv[x]);
	}

	switch(m_ResultOp)
	{
		case IBloc::And:
		{
			if(Count == 1 && bResultType[0] && !m_pResultD)
			{
				// Only set this if the results are permanent
				m_pResult = pIndiv[0];
				break;
			}

			for(x = 0; x < Count; ++x)
			{
				if(m_pResultD)
					*m_pResultD	&=	*pIndiv[x];
				else
					m_pResultD = new CBitArray(*pIndiv[x]);
			}
		}
		break;

		case IBloc::Or:
		{
			if(Count == 1 && bResultType[0] && !m_pResultD)
			{
				// Only set this if the results are permanent
				m_pResult = pIndiv[0];
				break;
			}

			for(x = 0; x < Count; ++x)
			{
				if(m_pResultD)
					*m_pResultD	|=	*pIndiv[x];
				else
					m_pResultD = new CBitArray(*pIndiv[x]);
			}
		}
		break;

		case IBloc::Not:
		{
			for(x = 0; x < Count; ++x)
			{
				if(m_pResultD)
					*m_pResultD &= ~(*pIndiv[x]);
				else
					m_pResultD = new CBitArray(~(*pIndiv[x]));
			}
		}
		break;

		case IBloc::Xor:
		{
			CBitArray * pB = new CBitArray [Count];
			for(x = 0; x < Count; ++x)
			{
				pB[x] = *pIndiv[x];
				for(y = 0; y < Count; ++y)
				{
					if(y != x)
						pB[x] &= ~(*pIndiv[y]);
				}
			}

			for(x = 0; x < Count; ++x)
			{
				if(m_pResultD)
					*m_pResultD |= pB[x];
				else
					m_pResultD = new CBitArray(pB[x]);
			}
			delete [] pB;
		}
		break;

		default: break;
	}

	if(pIndiv)
		delete pIndiv;
	if(bResultType)
		delete bResultType;

	return true;
}

JBool	CResults::CalculateWithProba()
{
	// Get new individuals vector, because JLib mes deux collections
	// are a pain in the arse!
	JInt32 Count = m_Results.GetCount();
	const CBitArray ** pIndiv = new const CBitArray * [Count];
	bool * bProbaType = new bool [Count];
	const JFLT32VECTOR ** pProbas = new const JFLT32VECTOR * [Count];
	int x = 0;
	int y = 0;
	int z = 0;

	// Calculate all sub terms first
	for(x = 0, m_Results.MoveFirst(); m_Results.IsValid(); ++x, m_Results.MoveNext())
	{
		CResults * pResult = m_Results.GetItem();
		// All is well so store the results
		pResult->GetResult(pIndiv[x]);
		bProbaType[x] = pResult->GetProbas(pProbas[x]);
	}

	switch(m_ResultOp)
	{
		case IBloc::And:
		{
			if(Count == 1 && pProbas[0] && bProbaType[0] && !m_pProbasD)
			{
				m_pProbas = pProbas[0];
				break;
			}

			if(!m_pProbasD)
				m_pProbasD = new JFLT32VECTOR;

			for(x = 0; x < Count; ++x)
			{
				if(pProbas[x])
				{
					if(m_pProbasD->GetCount())
					{
						for(m_pProbasD->MoveFirst(),pProbas[x]->MoveFirst(); pProbas[x]->IsValid(); m_pProbasD->MoveNext(), pProbas[x]->MoveNext())
							m_pProbasD->GetItem() *= pProbas[x]->GetItem();
					}
					else
					{
						m_pProbasD->SetCount(pProbas[x]->GetCount());
						for(m_pProbasD->MoveFirst(),pProbas[x]->MoveFirst(); pProbas[x]->IsValid(); m_pProbasD->MoveNext(), pProbas[x]->MoveNext())
							m_pProbasD->GetItem() = pProbas[x]->GetItem();
					}
				}
				else
				{
					if(m_pProbasD->GetCount())
					{
						for(y = 0, m_pProbasD->MoveFirst(); m_pProbasD->IsValid(); ++y, m_pProbasD->MoveNext())
							m_pProbasD->GetItem() *= pIndiv[x]->GetAt(y)?1.0f:0.0f;
					}
					else
					{
						m_pProbasD->SetCount(pIndiv[x]->GetSize());
						for(y = 0, m_pProbasD->MoveFirst(); m_pProbasD->IsValid(); ++y, m_pProbasD->MoveNext())
							m_pProbasD->GetItem() = pIndiv[x]->GetAt(y)?1.0f:0.0f;
					}
				}
			}
		}
		break;

		case IBloc::Or:
		{
			if(Count == 1 && pProbas[0] && bProbaType[0] && !m_pProbasD)
			{
				m_pProbas = pProbas[0];
				break;
			}

			if(!m_pProbasD)
				m_pProbasD = new JFLT32VECTOR;

			for(x = 0; x < Count; ++x)
			{
				if(pProbas[x])
				{
					if(m_pProbasD->GetCount())
					{
						for(m_pProbasD->MoveFirst(),pProbas[x]->MoveFirst(); pProbas[x]->IsValid(); m_pProbasD->MoveNext(), pProbas[x]->MoveNext())
							m_pProbasD->GetItem() *= (1.0f-pProbas[x]->GetItem());
					}
					else
					{
						m_pProbasD->SetCount(pProbas[x]->GetCount());
						for(m_pProbasD->MoveFirst(),pProbas[x]->MoveFirst(); pProbas[x]->IsValid(); m_pProbasD->MoveNext(), pProbas[x]->MoveNext())
							m_pProbasD->GetItem() = 1.0f-pProbas[x]->GetItem();
					}
				}
				else
				{
					if(m_pProbasD->GetCount())
					{
						for(y = 0, m_pProbasD->MoveFirst(); m_pProbasD->IsValid(); ++y, m_pProbasD->MoveNext())
							m_pProbasD->GetItem() *= pIndiv[x]->GetAt(y)?0.0f:1.0f;
					}
					else
					{
						m_pProbasD->SetCount(pIndiv[x]->GetSize());
						for(y = 0, m_pProbasD->MoveFirst(); m_pProbasD->IsValid(); y++, m_pProbasD->MoveNext())
							m_pProbasD->GetItem() = pIndiv[x]->GetAt(y)?0.0f:1.0f;
					}
				}
			}
			for(m_pProbasD->MoveFirst(); m_pProbasD->IsValid(); m_pProbasD->MoveNext())
				m_pProbasD->GetItem() = 1.0f-m_pProbasD->GetItem();
		}
		break;

		case IBloc::Not:
		{
			if(!m_pProbasD)
				m_pProbasD = new JFLT32VECTOR;

			for(x = 0; x < Count; x++)
			{
				if(pProbas[x])
				{
					if(m_pProbasD->GetCount())
					{
						for(m_pProbasD->MoveFirst(),pProbas[x]->MoveFirst(); pProbas[x]->IsValid(); m_pProbasD->MoveNext(), pProbas[x]->MoveNext())
							m_pProbasD->GetItem() *= (1.0f-pProbas[x]->GetItem());
					}
					else
					{
						m_pProbasD->SetCount(pProbas[x]->GetCount());
						for(m_pProbasD->MoveFirst(),pProbas[x]->MoveFirst(); pProbas[x]->IsValid(); m_pProbasD->MoveNext(), pProbas[x]->MoveNext())
							m_pProbasD->GetItem() = 1.0f-pProbas[x]->GetItem();
					}
				}
				else
				{
					if(m_pProbasD->GetCount())
					{
						for(y = 0, m_pProbasD->MoveFirst(); m_pProbasD->IsValid(); ++y, m_pProbasD->MoveNext())
							m_pProbasD->GetItem() *= pIndiv[x]->GetAt(y)?0.0f:1.0f;
					}
					else
					{
						m_pProbasD->SetCount(pIndiv[x]->GetSize());
						for(y = 0, m_pProbasD->MoveFirst(); m_pProbasD->IsValid(); ++y, m_pProbasD->MoveNext())
							m_pProbasD->GetItem() = pIndiv[x]->GetAt(y)?0.0f:1.0f;
					}
				}
			}
		}
		break;

		case IBloc::Xor:
		{
			if(!m_pProbasD)
				m_pProbasD = new JFLT32VECTOR;

			JFLT32VECTOR * pV = new JFLT32VECTOR[Count];

			for(x = 0; x < Count; ++x)
			{
				pV[x].SetCount(pIndiv[x]->GetSize());

				if(pProbas[x])
				{
					for(pV[x].MoveFirst(),pProbas[x]->MoveFirst(); pProbas[x]->IsValid(); pV[x].MoveNext(), pProbas[x]->MoveNext())
						pV[x].GetItem() = pProbas[x]->GetItem();
				}
				else
				{
					for(y = 0, pV[x].MoveFirst(); pV[x].IsValid(); ++y, pV[x].MoveNext())
						pV[x].GetItem() = pIndiv[x]->GetAt(y)?1.0f:0.0f;
				}

				for(y = 0; y < Count; ++y)
				{
					if(y != x)
					{
						if(pProbas[y])
						{
							for(pV[x].MoveFirst(),pProbas[y]->MoveFirst(); pProbas[y]->IsValid(); pV[x].MoveNext(), pProbas[y]->MoveNext())
								pV[x].GetItem() *= 1.0f-pProbas[y]->GetItem();
						}
						else
						{
							for(z = 0, pV[x].MoveFirst(); pV[x].IsValid(); ++z, pV[x].MoveNext())
								pV[x].GetItem() *= pIndiv[y]->GetAt(z)?0.0f:1.0f;
						}
					}
				}
			}

			for(x = 0; x < Count; ++x)
			{
				if(m_pProbasD->GetCount())
				{
					for(pV[x].MoveFirst(),m_pProbasD->MoveFirst(); pV[x].IsValid(); pV[x].MoveNext(), m_pProbasD->MoveNext())
						m_pProbasD->GetItem() *= 1.0f-pV[x].GetItem();
				}
				else
				{
					m_pProbasD->SetCount(m_pResultD->GetSize());
					for(pV[x].MoveFirst(),m_pProbasD->MoveFirst(); pV[x].IsValid(); pV[x].MoveNext(), m_pProbasD->MoveNext())
						m_pProbasD->GetItem() = 1.0f-pV[x].GetItem();
				}
			}

			delete [] pV;

			for(m_pProbasD->MoveFirst(); m_pProbasD->IsValid(); m_pProbasD->MoveNext())
				m_pProbasD->GetItem() = 1.0f-m_pProbasD->GetItem();
		}
		break;

		default: break;
	}

	if(pIndiv)
		delete pIndiv;
	if(pProbas)
		delete pProbas;
	if(bProbaType)
		delete bProbaType;

	return true;
}

JBool	CResults::CalculatePoints(JVector<long> &vPoints)
{
	int x=0;
	int Count = m_Results.GetCount();
	if(Count)
	{
		// Calculate all sub terms first
		for(x = 0, m_Results.MoveFirst(); x < Count; ++x,m_Results.MoveNext())
		{
			CResults * pResult = m_Results.GetItem();
			if(pResult)
				pResult->CalculatePoints(vPoints);
			else
				return false;
		}
	}
	else
	{
		const CBitArray * pIndiv = 0;
		GetResult(pIndiv);
		if(!pIndiv)
			return false;

		if(vPoints.GetCount()==0)
		{
			Count = pIndiv->GetSize();
			vPoints.SetCount(Count);
			for(x = 0, vPoints.MoveFirst(); x < Count; vPoints.MoveNext(),++x)
				vPoints.GetItem() = (pIndiv->GetAt(x)?1:0);
		}
		else
		{
			if(vPoints.GetCount() != pIndiv->GetSize())
				return false;

			Count = pIndiv->GetSize();
			for(x = 0; x < Count; ++x)
			{
				if(pIndiv->GetAt(x))
				{
					vPoints.MoveTo(x);
					++ vPoints.GetItem();
				}
			}
		}
	}
	return true;
}

JBool	CResults::CalculatePointsProba(JVector<long> &vPoints)
{
	int x=0;
	int Count = m_Results.GetCount();
	if(Count)
	{
		// Calculate all sub terms first
		for(x = 0, m_Results.MoveFirst(); x < Count; ++x,m_Results.MoveNext())
		{
			CResults * pResult = m_Results.GetItem();
			if(pResult)
				pResult->CalculatePointsProba(vPoints);
			else
				return false;
		}
	}
	else
	{
		const JFLT32VECTOR * pProba;
		GetProbas(pProba);
		if(!pProba)
			return false;

		if(vPoints.GetCount()==0)
		{
			Count = pProba->GetCount();
			vPoints.SetCount(Count);
			for(pProba->MoveFirst(), vPoints.MoveFirst(); pProba->IsValid(); pProba->MoveNext(), vPoints.MoveNext())
				vPoints.GetItem() = (pProba->GetItem()?1:0);
		}
		else
		{
			if(vPoints.GetCount() != pProba->GetCount())
				return false;

			for(pProba->MoveFirst(), vPoints.MoveFirst(); pProba->IsValid(); pProba->MoveNext(), vPoints.MoveNext())
				if(pProba->GetItem() != 0.0) 
					++ vPoints.GetItem();
		}
	}
	return true;
}

JBool CResults::DoPointsOperationProba()
{
	if(!m_bPoints)
		return true;

	if(!m_pProbasD)
	{
		if(!m_pProbas)
			return false;
		else
		{
			m_pProbasD = new JFLT32VECTOR();
			m_pProbasD->SetCount(m_pProbas->GetCount());
			for(m_pProbasD->MoveFirst(), m_pProbas->MoveFirst(); m_pProbas->IsValid(); m_pProbasD->MoveNext(), m_pProbas->MoveNext())
				m_pProbasD->GetItem() = m_pProbas->GetItem();
		}
	}

	JInt32 Ind = 0;

	switch(m_PointsOperator)
	{
		// All values less than value
		case IBloc::LT:
		{
			for(Ind=0, m_vPoints.MoveFirst(); m_vPoints.IsValid(); m_vPoints.MoveNext(),++Ind)
				if(!(m_vPoints.GetItem() < m_PointsValue))
				{
					m_pProbasD->MoveTo(Ind);
					m_pProbasD->GetItem() = 0.0;
				}
		}
		break;

		// All values less than or equal to value
		case IBloc::LTE:
		{
			for(Ind=0, m_vPoints.MoveFirst(); m_vPoints.IsValid(); m_vPoints.MoveNext(),++Ind)
				if(!(m_vPoints.GetItem() <= m_PointsValue))
				{
					m_pProbasD->MoveTo(Ind);
					m_pProbasD->GetItem() = 0.0;
				}
		}
		break;

		// All values greater than value
		case IBloc::GT:
		{
			for(Ind=0, m_vPoints.MoveFirst(); m_vPoints.IsValid(); m_vPoints.MoveNext(),++Ind)
				if(!(m_vPoints.GetItem() > m_PointsValue))
				{
					m_pProbasD->MoveTo(Ind);
					m_pProbasD->GetItem() = 0.0;
				}
		}
		break;

		// All values greater than or equal to value
		case IBloc::GTE:
		{
			for(Ind=0, m_vPoints.MoveFirst(); m_vPoints.IsValid(); m_vPoints.MoveNext(),++Ind)
				if(!(m_vPoints.GetItem() >= m_PointsValue))
				{
					m_pProbasD->MoveTo(Ind);
					m_pProbasD->GetItem() = 0.0;
				}
		}
		break;

		// All values equal to value
		case IBloc::EQ:
		{
			for(Ind=0, m_vPoints.MoveFirst(); m_vPoints.IsValid(); m_vPoints.MoveNext(),++Ind)
				if(!(m_vPoints.GetItem() == m_PointsValue))
				{
					m_pProbasD->MoveTo(Ind);
					m_pProbasD->GetItem() = 0.0;
				}
		}
		break;

		// All of the values are valid
		case IBloc::ALL:
		{
		}
		break;
		
		default:
			return false;
		break;
	}
	return true;
}

JBool CResults::DoPointsOperation()
{
	if(!m_pResultD)
	{
		if(!m_pResult)
			return false;
		else
			m_pResultD = new CBitArray(*m_pResult);
	}

	CBitArray & Indiv = *m_pResultD; 
	CBitArray Src;
	Src.SetSize(Indiv.GetSize());
	Src.SetAll(false);
	JInt32 Ind = 0;

	switch(m_PointsOperator)
	{
		// All values less than value
		case IBloc::LT:
		{
			for(Ind=0, m_vPoints.MoveFirst(); m_vPoints.IsValid(); m_vPoints.MoveNext(),++Ind)
				if(m_vPoints.GetItem() < m_PointsValue)
					Src.SetAt(Ind, true);
			Indiv &= Src;
		}
		break;

		// All values less than or equal to value
		case IBloc::LTE:
		{
			for(Ind=0, m_vPoints.MoveFirst(); m_vPoints.IsValid(); m_vPoints.MoveNext(),++Ind)
				if(m_vPoints.GetItem() <= m_PointsValue)
					Src.SetAt(Ind, true);
			Indiv &= Src;
		}
		break;

		// All values greater than value
		case IBloc::GT:
		{
			for(Ind=0, m_vPoints.MoveFirst(); m_vPoints.IsValid(); m_vPoints.MoveNext(),++Ind)
				if(m_vPoints.GetItem() > m_PointsValue)
					Src.SetAt(Ind, true);
			Indiv &= Src;
		}
		break;

		// All values greater than or equal to value
		case IBloc::GTE:
		{
			for(Ind=0, m_vPoints.MoveFirst(); m_vPoints.IsValid(); m_vPoints.MoveNext(),++Ind)
				if(m_vPoints.GetItem() >= m_PointsValue)
					Src.SetAt(Ind, true);
			Indiv &= Src;
		}
		break;

		// All values equal to value
		case IBloc::EQ:
		{
			for(Ind=0, m_vPoints.MoveFirst(); m_vPoints.IsValid(); m_vPoints.MoveNext(),++Ind)
				if(m_vPoints.GetItem() == m_PointsValue)
					Src.SetAt(Ind, true);
			if(m_PointsValue)
				Indiv &= Src;
			else
				Indiv = ((~Indiv)&Src);
		}
		break;

		// All of the values are valid
		case IBloc::ALL:
		{
			Src.SetAll(true);
			Indiv &= Src;
		}
		break;
		
		default:
			return false;
		break;
	}
	return true;
}

CResults &	CResults::operator=(const CCalcItemParams & Params)
{
	const JFLT32VECTOR * rhsP	= Params.GetProbas();
	if(rhsP)
	{
		JFLT32VECTOR * P = new JFLT32VECTOR;
		P->SetCount(rhsP->GetCount());
		for(P->MoveFirst(),rhsP->MoveFirst(); rhsP->IsValid(); P->MoveNext(), rhsP->MoveNext())
			P->GetItem() = rhsP->GetItem();
		SetProbasD(P);
	}
	delete m_pValuesD;
	const JFLT32VECTOR* rhsV = Params.GetNumericalValues();
	if(rhsV)
	{
		JFLT32VECTOR * V = new JFLT32VECTOR;
		V->SetCount(rhsV->GetCount());
		for(V->MoveFirst(),rhsV->MoveFirst(); rhsV->IsValid(); V->MoveNext(), rhsV->MoveNext())
			V->GetItem() = rhsV->GetItem();
		m_pValuesD = V;
	}
	else m_pValuesD = 0;
	if(Params.GetResults())
		SetResultD(new CBitArray(*Params.GetResults()));

	return *this;
}
