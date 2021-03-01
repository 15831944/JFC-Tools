#pragma once
#include "atpexpcsvtestplan.h"

class CATPExpCsvQuickPlan :
	public CATPExpCsvTestPlan
{
public:
	CATPExpCsvQuickPlan(void);
	~CATPExpCsvQuickPlan(void);


	void VisitBegin(JATPDocExport * pObjet);

	void VisitBegin(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport);

	void Visit(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport);

	VOID OnExportEntete(JATPDocExport * pObjet);
	VOID OnExportPeriode(JATPDocExport * pObjet);

	VOID OnExportResultHypothese(CInfoHypo *pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport);


};
