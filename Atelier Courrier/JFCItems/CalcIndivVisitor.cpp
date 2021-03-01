// CalcIndivVisitor.cpp: implementation of the CCalcIndivVisitor class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CalcIndivVisitor.h"
#include "ABuilder.h"
#include "ABQuestion.h"
#include "ABItem.h"
#include "ABNumValue.h"
#include "JSortVector.h"
#include "ItemsConstruitsBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCalcIndivVisitor::CCalcIndivVisitor()
{
	m_Valid				=	true;
	m_pParams			=	0;
	m_pItems			=	0;
	m_pQuestion			=	0;
	m_pBResults			=	0;
	m_pSResults			=	0;
	m_pQResults			=	0;
	m_pItemResults		=	0;
	m_pTResults			=	0;
}

CCalcIndivVisitor::~CCalcIndivVisitor()
{
	if(m_pBResults)
		delete m_pBResults;
}

JBool CCalcIndivVisitor::Calculate(CABuilder *pBuilder, CCalcItemParams * pParams, CItemsConstruitsBase *pItemConst)
{
	ASSERT(pBuilder);
	ASSERT(pParams);
	ASSERT(pItemConst);

	m_Valid				=	true;
	m_pParams			=	pParams;
	m_pItems			=	pItemConst;

	if(m_pBResults)
		delete m_pBResults;

	m_pBResults		= 0;
	m_pSResults		= 0;
	m_pQResults		= 0;
	m_pItemResults	= 0;
	m_pTResults		= 0;

	// Aout 2013
	// JBool CumulQuanti	= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 0) != 0?true:false;
	JBool CumulQuanti	= AfxGetApp()->GetProfileInt("Moteur", "CumulQuanti", 1) != 0?true:false;
	m_pParams->SetCumulQuanti(CumulQuanti);

	pBuilder->Accept(*this);

	return m_Valid ;
}

JBool CCalcIndivVisitor::CalculateEffectif(CABuilder *pBuilder, CCalcItemParams * pParams, CItemsConstruitsBase *pItemConst)
{
	if(Calculate(pBuilder,pParams,pItemConst))
	{
		pParams->DoEffectif();
	}
	return m_Valid;
}

void CCalcIndivVisitor::VisitStart(CABuilder & Builder)
{
	m_pBResults		=	new CResults(0,Builder.GetOperator());
}

void CCalcIndivVisitor::VisitEnd(CABuilder & Builder)
{
	if(m_pBResults && m_pBResults->Calculate())
	{
		// Populate the Params results
		const CBitArray * pResult = 0;
		if(m_pBResults->GetResult(pResult))
			m_pParams->SetResults(pResult);
		else
			m_pParams->SetResultsD(pResult);

		const JFLT32VECTOR * pProbas = 0;
		if(m_pBResults->GetProbas(pProbas))
			m_pParams->SetProbas(pProbas);
		else
			m_pParams->SetProbasD(pProbas);
		JMap<const JFLT32VECTOR*, JInt32> MapValues;
		m_pParams->SetNumericalValues(m_pBResults->GetAllValues(MapValues));


		// A VOIR  2014 AAAAA REMETTRE (4)
		//
		for(MapValues.MoveFirst(); MapValues.IsValid(); MapValues.MoveNext())
		{
			JFLT32VECTOR* pVal = (JFLT32VECTOR*)MapValues.GetKey();
			pVal->Reset();
		}
		//

	}
	else
		m_Valid = false;
}

void CCalcIndivVisitor::VisitStart(CABSegment & Segment)
{
	if(m_pBResults)
		m_pSResults		=	m_pBResults->AddResult(new CResults(0,Segment.GetOperator()));
}

void CCalcIndivVisitor::VisitEnd(CABSegment & Segment)
{
}

void CCalcIndivVisitor::VisitStart(CABQuestion & Question)
{
	if(m_pSResults)
	{
		m_pQResults = m_pSResults->AddResult(new CResults(0,Question.GetOperator()));
		m_pTResults = m_pQResults;
		m_pQuestion = 0;
	}
}

void CCalcIndivVisitor::VisitEnd(CABQuestion & Question)
{
	m_pQResults = 0;
	m_pTResults = 0;
}

void CCalcIndivVisitor::VisitStart(CABTerme & Terme)
{
	// Create new terme results
	if(m_pTResults)
	{
		m_pTResults = m_pTResults->AddResult(new CResults(m_pTResults,Terme.GetOperator()));

		if(!Terme.HasOperator())
		{
			const CBitArray* pResult = m_pParams->GetTerrain()->GetIndividusByRepereAbsolut(Terme.GetID(),m_pParams->GetSegmentsIDs());
			m_pTResults->SetResult(pResult); if (pResult == NULL) m_Valid = false;

			if(!m_pQuestion)
				m_pQuestion = m_pParams->GetTerrain()->GetQuestionByRepAbs(Terme.GetID());

			if(m_pQuestion)
			{
				if(m_pQuestion->HasValues())
				{
					if(m_pParams->GetTerrain()->VarHasNumValues(Terme.GetID()))
						m_pTResults->SetValues(m_pParams->GetTerrain()->GetVarNumValuesByRepAbs(Terme.GetID()));
					else if(m_pParams->GetTerrain()->VarHasBinValues(Terme.GetID()))
						m_pTResults->SetBinValues(m_pParams->GetTerrain()->GetVarBinValuesByRepAbs(Terme.GetID()));
				}
				else if(m_pQuestion->IsProbability())
				{
					if(m_pParams->GetTerrain()->VarHasNumValues(Terme.GetID()))
						m_pTResults->SetProbas(m_pParams->GetTerrain()->GetVarNumValuesByRepAbs(Terme.GetID()));
					else if(m_pParams->GetTerrain()->VarHasBinValues(Terme.GetID()))
						m_pTResults->SetBinValues(m_pParams->GetTerrain()->GetVarBinValuesByRepAbs(Terme.GetID()));
				}
			}
		}
	}
}

void CCalcIndivVisitor::VisitEnd(CABTerme & Terme)
{
	// Step back to Previous Result object
	if(m_pTResults && m_pTResults->GetPrev())
		m_pTResults = m_pTResults->GetPrev();
}

void CCalcIndivVisitor::Visit(CABPoint & Point)
{
	if(m_pQResults)
		m_pQResults->SetPointsData(Point.GetValue(),Point.GetOperator());
	if(m_pItemResults)
		m_pItemResults->SetPointsData(Point.GetValue(),Point.GetOperator());
}

void CCalcIndivVisitor::Visit(CABClass & Class)
{
	// Get the Operators
	JList<IBloc::OPERATOR> Operators;
	Class.GetOperators(Operators);

	// Get the Values
	JList<JFlt32> Values;
	Class.GetValues(Values);

	// Get the numeric variable values
	// Generate vector of Variable Numeric Values
	JMap< const JFLT32VECTOR *, JInt32> ValuesMap;
	CArray<long, long &>	VectCumulQuanti;
	CBitArray Indiv;
	bool FlagFirst = true;
	bool CumulQuanti;
	
	if(m_pQResults)
		m_pQResults->GetValuesMap(ValuesMap);

	// Recup option Cumul Quanti
	CumulQuanti = m_pParams->GetCumulQuanti();

	if (CumulQuanti)
	{
		// 1er passage = remplissage du vecteur des quantitatifs cumules
		for(ValuesMap.MoveFirst(); ValuesMap.IsValid(); ValuesMap.MoveNext())
		{
			const JFLT32VECTOR *pVal = ValuesMap.GetKey();
			if(pVal)
			{
				if (FlagFirst == true)
				{
					VectCumulQuanti.SetSize(pVal->GetCount());
				}
				FlagFirst = false;

				// Cumule les valeurs quanti
				CumulValeurQuanti(*pVal, VectCumulQuanti);
			}
		}
		// On remet les elmts à 0 à -1
		long Val = -1;
		for(int Ind=0; Ind < VectCumulQuanti.GetCount(); Ind++)
		{
			if (VectCumulQuanti.GetAt(Ind) == 0)
			{
				VectCumulQuanti.SetAt(Ind, Val);
			}
		}
	}

	FlagFirst = true;
	for(ValuesMap.MoveFirst(); ValuesMap.IsValid(); ValuesMap.MoveNext())
	{
		const JFLT32VECTOR *pVal = ValuesMap.GetKey();
		if(pVal)
		{
			// Calculate the individu array
			if (FlagFirst == true)
			{
				Indiv.SetSize(pVal->GetCount());
				Indiv.SetAll(true);
			}
			FlagFirst = false;
			
			for(Operators.MoveFirst(), Values.MoveFirst(); Operators.IsValid(); Operators.MoveNext(), Values.MoveNext())
			{
				if (!CumulQuanti)
					// Calcul sans cumul des quantis
					DoUnsortedValueOperation(Operators.GetItem(), Values.GetItem(), *pVal, Indiv);
				else
					// Calcul avec cumul des quantis
					DoUnsortedValueOperation_AvecCumul(Operators.GetItem(), Values.GetItem(), *pVal, Indiv, VectCumulQuanti);
			}
			if(m_pQResults && Indiv.GetSize())
				m_pQResults->SetValuesFiltre(ValuesMap.GetKey(),&Indiv);


			// Essai 2014 / Libération au moment de passer dans Tris Croises
			/*
			JFLT32VECTOR *pVal = (JFLT32VECTOR *)ValuesMap.GetKey();
			pVal->Reset();
			*/
		}
	}
}


void CCalcIndivVisitor::Visit(CABNumValue & NumValue)
{
	// 2013
	bool					FlagFirst = true;
	CBitArray				Indiv;
	CArray<long, long &>	VectCumulQuanti;
	int						NbElmt;
	JBool					CumulQuanti;

	// Recup option Cumul Quanti
	CumulQuanti = m_pParams->GetCumulQuanti();

	// double TAgreg = m_pParams->GetAgregatedQuantity();

	// Generate vector of Variable Numeric Values
	JMap< const JFLT32VECTOR *, JInt32> ValuesMap;
	if(m_pQResults)
		m_pQResults->GetValuesMap(ValuesMap);
	

	if (CumulQuanti)
	{
		// 1er passage = remplissage du vecteur des quantitatifs cumules
		for(ValuesMap.MoveFirst(); ValuesMap.IsValid(); ValuesMap.MoveNext())
		{
			const JFLT32VECTOR *pVal = ValuesMap.GetKey();
			if(pVal)
			{
				if (FlagFirst == true)
				{
					VectCumulQuanti.SetSize(pVal->GetCount());
				}
				FlagFirst = false;

				// Mise à jour du vecteur cumul quanti avec le vecteur de la nouvelle modalié
				CumulValeurQuanti(*pVal, VectCumulQuanti);
			}
		}

		// On remet les elmts à 0 à -1
		long Val = -1;
		for(int Ind=0; Ind < VectCumulQuanti.GetCount(); Ind++)
		{
			if (VectCumulQuanti.GetAt(Ind) == 0)
			{
				VectCumulQuanti.SetAt(Ind, Val);
			}
		}
	}

	FlagFirst = true;
	for(ValuesMap.MoveFirst(); ValuesMap.IsValid(); ValuesMap.MoveNext())
	{
		const JFLT32VECTOR *pVal = ValuesMap.GetKey();
		if(pVal)
		{
			if (FlagFirst == true)
			{
				Indiv.SetSize(pVal->GetCount());
				Indiv.SetAll(false);
			}
			FlagFirst = false;

			// Get the Operators
			if(NumValue.HasOperator())
			{
				DoUnsortedValueOperation(NumValue.GetOperator(), 0, *pVal, Indiv);
			}
			else
			{
				JFLT32VECTOR Values;
				NumValue.GetValues(Values);
				for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
				{
					if (!CumulQuanti)
						// Calcul sans cumul des quantis
						DoUnsortedValueOperation(IBloc::EQ, Values.GetItem(), *pVal, Indiv);
					else
					{
						// Calcul avec cumul des quantis
						DoUnsortedValueOperation_AvecCumul(IBloc::EQ, Values.GetItem(), *pVal, Indiv, VectCumulQuanti);
					}
				}
			}
			
			if(m_pQResults && Indiv.GetSize())
				m_pQResults->SetValuesFiltre(ValuesMap.GetKey(),&Indiv);

			// A VOIR 2014 AAAAA REMETTRE (2)
			JFLT32VECTOR *pVal2 = (JFLT32VECTOR *)pVal;
			pVal2->Reset();
		}

		// A VOIR 2014 AAAAA REMETTRE (1)
		// FreeExtra 2014  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@à
		if (CumulQuanti)
		{
			VectCumulQuanti.FreeExtra();   // voir RemoveAll
		}
		//
	}
}

void CCalcIndivVisitor::Visit(CABQuant & Quant)
{
	JList<JInt32> Values;
	Quant.GetValues(Values);

	// Get the Type and Quantile 
	Values.MoveFirst();
	JInt32 Quantile		=	Values.GetItem();
	Values.MoveNext();
	JInt32 QuantileType =	Values.GetItem();

	// Get and sort the numeric variable values
	JVector<VALUEINDIV> Val;
	JInt32 NbIndivs = GetQuantiVarValues(Val);
	JSortVector<VALUEINDIV>::Sort(Val, -1);

	VALUEINDIV* pBase = 0;
	for (Val.MoveFirst(); Val.IsValid(); Val.MoveNext())
	{
		VALUEINDIV* pItem = &Val.GetItem();
		if (pBase != 0)
		{
			if (pItem->m_Value < pBase->m_Value)
				throw JInternalError::GetInstance();
			if (pItem->m_Value == pBase->m_Value && pItem->m_Indiv < pBase->m_Indiv)
				throw JInternalError::GetInstance();
		}
		pBase = pItem;
	}

	// Create the individu array
	CBitArray Indiv;
	Indiv.SetSize(NbIndivs);
	Indiv.SetAll(false);

	// First and Last indexes
	JInt32 First = 0;
	JInt32 Last  = 0;

	// Skip Non répondants
	Val.MoveTo(First);
	while(Val.IsValid())
	{
		if(Val.GetItem().m_Value >= 0.0)
			break;
		Val.MoveNext();
		First += 1;
		Last  += 1;
	}

	// Calculate Range of Indiv
	if(Quantile == 0)
	{
		First = 0;
	}
	else if (Quantile > 0 && Quantile <= QuantileType)
	{
		JInt32 Div = (Val.GetCount() - First) / QuantileType;
		JInt32 Rem = (Val.GetCount() - First) % QuantileType;
		for (JInt32 Idx = 0; Idx < Quantile; Idx += 1)
		{
			First = Last;
			Last += Div; if (Idx < Rem) Last += 1;
		}
	}

	// Set Bornes
	if(First < Last)
	{
		Val.MoveTo(First);    if (Val.IsValid()) { m_pParams->SetMinBorne(Val.GetItem().m_Value); m_pParams->SetMaxBorne(Val.GetItem().m_Value); }
		Val.MoveTo(Last - 1); if (Val.IsValid()) { m_pParams->SetMaxBorne(Val.GetItem().m_Value); }
	}

	// Select individuals
	Val.MoveTo(First);
	while(Val.IsValid() && Val.GetIndex() < Last)
	{
		Indiv.SetAt(Val.GetItem().m_Indiv, true);
		Val.MoveNext();
	}

	if(m_pQResults)
		m_pQResults->SetBinValues(&Indiv);
}

JInt32 CCalcIndivVisitor::GetQuantiVarValues(JVector<VALUEINDIV> & Values)
{
	// Get dimensions
	JInt32 NbValues = 0;
	JInt32 NbIndivs = 0;

	// Vector of Individus
	CBitArray* pResult = 0;

	// Generate vector of Variable Numeric Values
	JMap<const JFLT32VECTOR *, JInt32> ValuesMap;
	if(m_pQResults)
	{
		m_pQResults->GetValuesMap(ValuesMap);
		if(m_pQResults->Calculate())
		{
			m_pQResults->GetResult((const CBitArray*&)(pResult));
			if (pResult != 0)
				NbIndivs = pResult->GetSize();
		}
	}

	// Calculate Values indexes
	JArray<JInt32> ValuesIdx;
	ValuesIdx.SetCount(NbIndivs);
	for (JInt32 Idx = 0; Idx < NbIndivs; Idx += 1) {
		if (pResult->GetAt(Idx)) {
			ValuesIdx.Item(Idx) = NbValues;
			NbValues += 1;
		}
		else
			ValuesIdx.Item(Idx) = -1;
	}

	// Allocate Values
	Values.SetCount(NbValues);
	for (JInt32 Jdx = 0; Jdx < NbIndivs; Jdx += 1) {
		Values.MoveTo(ValuesIdx.Item(Jdx));
		if (Values.IsValid()) {
			Values.GetItem().m_Indiv = Jdx;
			Values.GetItem().m_Value = -1.0;
		}
	}

	for(ValuesMap.MoveFirst(); ValuesMap.IsValid(); ValuesMap.MoveNext())
	{
		const JFLT32VECTOR *pVal = ValuesMap.GetKey();
		if(pVal)
		{
			// Add the values to this vector
			for(pVal->MoveFirst(); pVal->IsValid(); pVal->MoveNext())
			{
				Values.MoveTo(ValuesIdx.Item(pVal->GetIndex()));
				if (Values.IsValid())
				{
					if (pVal->GetItem() >= 0.0)
					{
						if (Values.GetItem().m_Value >= 0.0)
							Values.GetItem().m_Value += pVal->GetItem();
						else
							Values.GetItem().m_Value = pVal->GetItem();
					}
				}
			}
		}
	}

	return NbIndivs;
}

void CCalcIndivVisitor::DoUnsortedValueOperation(IBloc::OPERATOR Op, JFlt32 Value, const JFLT32VECTOR &Values, CBitArray &Indiv)
{
	CBitArray Src;
	Src.SetSize(Values.GetCount());
	Src.SetAll(false);
	JInt32 Ind = 0;

	switch(Op)
	{
		// All values less than value
		case IBloc::LT:
		{
			for(Ind=0, Values.MoveFirst(); Values.IsValid(); Values.MoveNext(),++Ind)
			{
				if(Values.GetItem() < Value)
					Src.SetAt(Ind, true);
			}
			Indiv &= Src;
		}
		break;

		// All values less than or equal to value
		case IBloc::LTE:
		{
			for(Ind=0, Values.MoveFirst(); Values.IsValid(); Values.MoveNext(),++Ind)
			{
				if(Values.GetItem() <= Value)
					Src.SetAt(Ind, true);
			}
			Indiv &= Src;
		}
		break;

		// All values greater than value
		case IBloc::GT:
		{
			for(Ind=0, Values.MoveFirst(); Values.IsValid(); Values.MoveNext(),++Ind)
			{
				if(Values.GetItem() > Value)
					Src.SetAt(Ind, true);
			}
			Indiv &= Src;
		}
		break;

		// All values greater than or equal to value
		case IBloc::GTE:
		{
			for(Ind=0, Values.MoveFirst(); Values.IsValid(); Values.MoveNext(),++Ind)
			{
				if(Values.GetItem() >= Value)
					Src.SetAt(Ind, true);
			}
			Indiv &= Src;
		}
		break;

		// All values equal to value
		case IBloc::EQ:
		{
			for(Ind=0, Values.MoveFirst(); Values.IsValid(); Values.MoveNext(),++Ind)
			{
				if(Values.GetItem() == Value)
				Src.SetAt(Ind, true);
			}
			Indiv |= Src;
		}
		break;

		// All of the values are valid
		case IBloc::ALL:
		{
			Src.SetAll(true);
			Indiv |= Src;
		}
		break;
		
		default:
			m_Valid =false;
			break;
	}
}

void CCalcIndivVisitor::CumulValeurQuanti(const JFLT32VECTOR &Values, CArray<long, long &>	&VectCumulQuanti)
{
	JInt32 Ind = 0;
	long Val;

	for(Ind=0, Values.MoveFirst(); Values.IsValid(); Values.MoveNext(),++Ind)
	{
		// Valeur indiv sur la moda
		long ValIndiv = (long)Values.GetItem();

		if (ValIndiv > 0)
		{
			VectCumulQuanti.GetAt(Ind) += ValIndiv;
		}
	}
}

void CCalcIndivVisitor::DoUnsortedValueOperation_AvecCumul(IBloc::OPERATOR Op, JFlt32 Value, const JFLT32VECTOR &Values, CBitArray &Indiv, CArray<long, long &>	&VectCumulQuanti)
{
	CBitArray Src;
	Src.SetSize(Values.GetCount());
	Src.SetAll(false);
	JInt32 Ind = 0;

	switch(Op)
	{
		// All values less than value
		case IBloc::LT:
		{
			// Vecteur final à ajouter en tenant compte du vecteur cumul
			for(Ind=0, Values.MoveFirst(); Values.IsValid(); Values.MoveNext(),++Ind)
			{
				long Val = VectCumulQuanti.GetAt(Ind);
				if(Val < Value)
				{
					Src.SetAt(Ind, true);
				}
			}
			Indiv &= Src;
		}
		break;

		// All values less than or equal to value
		case IBloc::LTE:
		{
			// Vecteur final à ajouter en tenant compte du vecteur cumul
			for(Ind=0, Values.MoveFirst(); Values.IsValid(); Values.MoveNext(),++Ind)
			{
				long Val = VectCumulQuanti.GetAt(Ind);
				if(Val <= Value)
				{
					Src.SetAt(Ind, true);
				}
			}
			Indiv &= Src;
		}
		break;

		// All values greater than value
		case IBloc::GT:
		{
			// Vecteur final à ajouter en tenant compte du vecteur cumul
			for(Ind=0, Values.MoveFirst(); Values.IsValid(); Values.MoveNext(),++Ind)
			{
				long Val = VectCumulQuanti.GetAt(Ind);
				if(Val > Value)
				{
					Src.SetAt(Ind, true);
				}
			}
			Indiv &= Src;
		}
		break;

		// All values greater than or equal to value
		case IBloc::GTE:
		{
			// Vecteur final à ajouter en tenant compte du vecteur cumul
			for(Ind=0, Values.MoveFirst(); Values.IsValid(); Values.MoveNext(),++Ind)
			{
				long Val = VectCumulQuanti.GetAt(Ind);
				if(Val >= Value)
				{
					Src.SetAt(Ind, true);
				}
			}

			Indiv &= Src;
		}
		break;

		// All values equal to value
		case IBloc::EQ:
		{
			// Vecteur final à ajouter en tenant compte du vecteur cumul
			for(Ind=0, Values.MoveFirst(); Values.IsValid(); Values.MoveNext(),++Ind)
			{
				long Val = VectCumulQuanti.GetAt(Ind);
				if(Val == Value)
				{
					Src.SetAt(Ind, true);
				}

			}
			Indiv |= Src;
		}
		break;

		// All of the values are valid
		case IBloc::ALL:
		{
			Src.SetAll(true);
			Indiv |= Src;
		}
		break;
		
		default:
			m_Valid =false;
			break;
	}
}

void CCalcIndivVisitor::VisitStart(CABItem & Item)
{
	JList<JInt32> Values;
	Item.GetValues(Values);

	if(Values.GetCount())
	{
		m_pItemResults = new CResults(0,Item.GetOperator());
		m_pSResults->AddResult(m_pItemResults);

		for(Values.MoveFirst(); Values.IsValid(); Values.MoveNext())
		{
			const CItemConstruit * pItem = m_pItems->GetItemConstruit(Values.GetItem());
			CCalcItemParams Params;
			Params.CopyParams(*m_pParams);
			if(pItem && pItem->Calculate(&Params))
			{
				CResults * pR = m_pItemResults->AddResult(new CResults(0,IBloc::Nop));
				*pR = Params;
			}
			else
				m_Valid = false;
		}
	}
}

void CCalcIndivVisitor::VisitEnd(CABItem & Item)
{
	m_pItemResults = 0;
}
