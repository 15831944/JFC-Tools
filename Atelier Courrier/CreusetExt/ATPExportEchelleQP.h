#pragma once
#include "atpexportechelle.h"

class CATPExportEchelleQP :
	public CATPExportEchelle
{
public:
	CATPExportEchelleQP(void);
	~CATPExportEchelleQP(void);

	VOID OnExportEntete(JATPDocExport * pObjet);
	VOID OnExportPeriode(JATPDocExport * pObjet);

	VOID OnExportEnteteSupports(JATPDocExport * pObjet);

	VOID OnExportSupportEchelle(CInfoEchSupport * pObjet);
};
