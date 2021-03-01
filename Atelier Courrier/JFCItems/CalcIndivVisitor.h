
#pragma once

#include "IBloc.h"
#include "IAbsolutVisitor.h"
#include "Terrain.h"
#include "BitArray.h"
#include "CalcItemParams.h"
#include "Results.h"

class CItemsConstruitsBase;

/**
 * \ingroup JFCITEMS
 * This Class calculates an Item Construit
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-10-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class CCalcIndivVisitor : public IAbsolutVisitor  
{
public:
	CCalcIndivVisitor();
	virtual ~CCalcIndivVisitor();

	/**
	 * Calculates an Item Construit
	 * \sa CCalcItemParams
	 * \param *pBuilder pointer to a CABuilder object
	 * \param pParams pointer to a CCalcItemParams object
	 * \param *pItemConst pointer to the CItemsConstruits
	 * \return True is calculated
	 */
	virtual JBool	Calculate(CABuilder *pBuilder, CCalcItemParams * pParams, CItemsConstruitsBase * pItemConst);
	/**
	 * Calculates the Effectif of an Item construit
	 * \sa CCalcItemParams
	 * \param *pBuilder pointer to a CABuilder object
	 * \param pParams pointer to a CCalcItemParams object
	 * \param *pItemConst pointer to the CItemsConstruits
	 * \return True is calculated
	 */
	virtual JBool	CalculateEffectif(CABuilder *pBuilder, CCalcItemParams * pParams, CItemsConstruitsBase *pItemConst);

public:
	/// IAbsolutVisitor overloads
	virtual void VisitStart(CABuilder & Builder);
	virtual void VisitEnd(CABuilder & Builder);
	virtual void VisitStart(CABSegment & Segment);
	virtual void VisitEnd(CABSegment & Segment);
	virtual void VisitStart(CABQuestion & Question);
	virtual void VisitEnd(CABQuestion & Question);
	virtual void VisitStart(CABTerme & Terme);
	virtual void VisitEnd(CABTerme & Terme);
	virtual void Visit(CABClass & Class);
	virtual void Visit(CABPoint & Point);
	virtual void Visit(CABNumValue & NumValue);
	virtual void Visit(CABQuant & Quant);
	virtual void VisitStart(CABItem & Item);
	virtual void VisitEnd(CABItem & Item);
	/// TODO: Complete the Score calculation
	virtual void Visit(CABScore & Score) {} ;

private:
	struct	VALUEINDIV
	{
		JFlt32	m_Value;
		JInt32	m_Indiv;

		VALUEINDIV & operator =(const VALUEINDIV & Source)
		{
			if (this != &Source)
			{
				m_Value = Source.m_Value;
				m_Indiv = Source.m_Indiv;
			}

			return (*this);
		}

		JBool operator <(const VALUEINDIV & Source) const
		{
			if (m_Value < Source.m_Value) return (true);
			if (m_Value > Source.m_Value) return (false);

			if (m_Indiv < Source.m_Indiv) return (true);
			if (m_Indiv > Source.m_Indiv) return (false);

			return (false);
		}
	};

protected:
	virtual	void	DoUnsortedValueOperation(IBloc::OPERATOR Op, JFlt32 Value, const JFLT32VECTOR &Values, CBitArray &Indiv);
	virtual	void	DoUnsortedValueOperation_AvecCumul(IBloc::OPERATOR Op, JFlt32 Value, const JFLT32VECTOR &Values, CBitArray &Indiv, CArray<long, long &>	&VectCumulQuanti);
	virtual void    CumulValeurQuanti(const JFLT32VECTOR &Values, CArray<long, long &>	&VectCumulQuanti);
	virtual	JInt32	GetQuantiVarValues(JVector<VALUEINDIV> & Values);

protected:
	CResults *				m_pBResults;
	CResults *				m_pSResults;
	CResults *				m_pQResults;
	CResults *				m_pItemResults;
	CResults *				m_pTResults;

	JBool					m_Valid;
	CItemsConstruitsBase *	m_pItems;
	CCalcItemParams		*	m_pParams;
	const CQuestion *		m_pQuestion;
};
