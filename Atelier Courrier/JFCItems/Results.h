#pragma once

#include "IBloc.h"
#include "BitArray.h"
#include "CalcItemParams.h"

typedef JVector<JFlt32> JFLT32VECTOR;

/// This objects calculates Item results (Individuals & Probabilities)
/**
 * \ingroup JFCITEMS
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-28-2003
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
class AFX_EXT_CLASS CResults
{
public:

	/**
	 * Constructor
	 * \param *pPrev Previous result Object or (Owner)
	 * \param ResultOperator The Result operator to be applied to sub results
	 */
								CResults(CResults *pPrev, IBloc::OPERATOR ResultOperator);
	/**
	 * Destructor
	 * \param void 
	 * \return 
	 */
	virtual						~CResults(void);


	void						Reset();
	/**
	 * Calculate the results
	 * \return false if calculation failed
	 */
	JBool						Calculate();
	/**
	 * Assignement operator for setting the results
	 *
	 */
	CResults &					operator=(const CCalcItemParams & Params);
	/**
	 * Checks if this object has an operator
	 * \return True is an operator is present
	 */
	JBool						HasOperator()								{ return m_ResultOp!=IBloc::Nop; } ;
	/**
	 * Adds a Sub result to this result object
	 * \param *pResult Sub result object
	 * \return A pointer to the same sub object
	 */
	CResults *					AddResult(CResults *pResult);
	/**
	 * Get the Previous Result object
	 * \return Pointer to previous result object
	 */
	CResults *					GetPrev()									{ return m_pPrev; } ;
	/**
	 * Sets a Result CBitArray (Constant) thus will not be deleted
	 * \param *pResult (Constant) Result Bit Array
	 */
	void						SetResult(const CBitArray *pResult)			{ m_pResult = pResult; } ;
	/**
	 * Adds a binary value CBitArray (Constant) to the ResultArray
	 * \param *pValues (Constant) Values Bit Array
	 */
	void						SetBinValues(const CBitArray *pValues);
	/**
	 * Sets a Result CBitArray that will be deleted in the destructor
	 * \param *pResult Result Bit Array pointer
	 */
	void						SetResultD(CBitArray *pResult)				{ m_pResultD = pResult; } ;
	/**
	 * \return true if a result exists
	 */
	bool						HasResult() const { if(m_pResult || m_pResultD) return true;
												else return false; } ;
	/**
	 * Get the result Bit Array
	 * \return a constant pointer to the result bit array
	 */
	bool						GetResult(const CBitArray * & pResult) const;
	/**
	 * Sets the Probability vector (constant)
	 * \param pProbas constant Pointer to the probability vector
	 */
	void						SetProbas(const JFLT32VECTOR * pProbas)	{ m_pProbas = pProbas; } ;
	/**
	 * Sets the Probability vector that will be deleted in the destructor
	 * \param pProbas Pointer to the probability vector
	 */
	void						SetProbasD(JFLT32VECTOR * pProbas)	{ m_pProbasD = pProbas; } ;
	/**
	 * \return true if a probas exists
	 */
	bool						HasProbas() const 
	{ 
		if(m_pProbas || m_pProbasD) 
			return true;
		else 
			return false; 
	} ;

	/**
	 * Gets the probability vector
	 * \return pointer to the probability vector
	 */
	bool						GetProbas(const JFLT32VECTOR * & pProba) const;
	/**
	 * Sets a Numerical Variable Index to this result object
	 * \param VarIndex Numeric Variable Index
	 */
	void						SetValues(const JFLT32VECTOR * pValues, bool TransBinary = false);
	/**
	 * Gets a Map of all Variable values
	 * \param Values Map 
	 */
	const JMap<const JFLT32VECTOR*, JInt32> & GetAllValues(JMap<const JFLT32VECTOR*, JInt32> & ValuesMap);
	/**
	 * Gets a Map of Variable values
	 * \param Values Map 
	 */
	void						GetValuesMap(JMap< const JFLT32VECTOR *,JInt32> & ValuesMap);
	/**
	 * Sets a Values Variable Bit Array filter to apply to the results
	 * \param QuantiNum Quanti Variable Number
	 * \param *pFiltre pointer to Bit Array filter
	 */
	void						SetValuesFiltre(const JFLT32VECTOR * pValues, const CBitArray *pFiltre);
	/**
	 * Set the Question induite Points flag
	 */
	void						SetPointsData(long Value, IBloc::OPERATOR Operator)
									{	m_PointsValue = Value; m_PointsOperator = Operator; m_bPoints = true;};

private:
	/**
	 * Calculate the results with probabilities
	 * \return false if calculation failed
	 */
	JBool						CalculateWithProba();
	/**
	 * Calculate the results without probabilities
	 * \return false if calculation failed
	 */
	JBool						CalculateWithoutProba();
	/**
	 * Calculate the results with hit Points
	 * \return false if calculation failed
	 */
	JBool						CalculatePoints(JVector<long> &vPoints);
	JBool						CalculatePointsProba(JVector<long> &vPoints);
	JBool						DoPointsOperationProba();
	JBool						DoPointsOperation();

	/// Results Operator
	IBloc::OPERATOR				m_ResultOp;
	/// List of Sub Result objects
	JList< CResults *>			m_Results;
	/// Previous Result object (Owner?)
	CResults *					m_pPrev;
	/// Individual vector (Constant)
	const CBitArray *			m_pResult;
	/// Probabilities vector (Constant)
	const JFLT32VECTOR	*		m_pProbas;
	/// Individual vector (Deletable)
	CBitArray *					m_pResultD;
	/// Probabilities vector (Deletable)
	JFLT32VECTOR	*			m_pProbasD;
	/// Numeric variable Index
	const JFLT32VECTOR *		m_pValues;
	/// Numeric variable Index (Deletable)
	const JFLT32VECTOR *		m_pValuesD;
	/// Variables pour comptage
	bool						m_bPoints;
	long						m_PointsValue;
	IBloc::OPERATOR				m_PointsOperator;
	JVector<long> 				m_vPoints;
	bool						m_bCalculated;
	bool						m_bCalcRet;

};
