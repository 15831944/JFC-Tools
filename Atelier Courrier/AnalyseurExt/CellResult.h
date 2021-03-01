#pragma once

#include "ICellResult.h"
#include "CalcItemParams.h"
#include "Results.h"
#include "AnalyseurEnums.h"

class CCellResult : public ICellResult
{
public:
	CCellResult(void);
	virtual ~CCellResult(void);

	virtual const CCellResult *		AsCCellResult()	const	{ return this; } ;
	virtual CCellResult *			AsCCellResult()	{ return this; } ;
	virtual bool					IsCCellResult() { return true; }

	// Gets the result (attention si on est en mode moyenne 0 exclu, le préciser et passer le vecteur quantitatif
	// virtual double				GetResult(RESULTTYPE Type);
	virtual double					GetResult(RESULTTYPE Type, const JFLT32VECTOR * pQuanti = 0, bool ModeMoyenne0Exclu = false, ITEMTYPE ElmtType = ITYPE_ITEM, bool ModeExclusif = false);

	virtual void					CopyParams(const CCalcItemParams &Param);
	virtual CResults *				AddResult(IBloc::OPERATOR Op);
	virtual CResults *				AddTarget(IBloc::OPERATOR Op);

protected:
	// Calculate the cell
	virtual bool Calculate(RESULTTYPE Type, const JFLT32VECTOR * pQuanti, ITEMTYPE ElmtType = ITYPE_ITEM, bool ModeExclusif = false);

	CCalcItemParams*	m_TParams;
	CCalcItemParams*	m_UParams;
	CResults *			m_UResults;
	CResults *			m_TResults;

	DWORD				m_Calculated;
	
	bool				m_bCalculated;

	double				m_TEffectif;
	double				m_TnbCas;
	double				m_TAgregatedQuantity;
	double				m_UEffectif;

	// Les effectifs et agrégats sans les 0
	double				m_TEffectif_0Exclu;
	double				m_TAgregatedQuantity_0Exclu;

};
