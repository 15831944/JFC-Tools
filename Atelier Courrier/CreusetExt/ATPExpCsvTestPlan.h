#pragma once

#include "ATPIVisitor.h"

class CATPExpCsvTestPlan : public CATPIVisitor
{
public:
	CATPExpCsvTestPlan(void);
	~CATPExpCsvTestPlan(void);

	// Visite de l'objet export test plan
	virtual void Visit		(JATPDocExport * pObjet){};
	virtual void VisitBegin (JATPDocExport * pObjet);
	virtual void VisitEnd   (JATPDocExport * pObjet);

	// Export début hypothèse pour test plan
	virtual void VisitBegin(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport);

	// Export hypothèse pour test plan
	virtual void Visit(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport);

	// Export hypothèse pour test plan
	virtual void VisitEnd(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport);

	// Visite les échelles
	virtual void Visit		(CInfoEchSupport	* pObjet){};
	virtual void VisitBegin	(CInfoEchSupport	* pObjet){};
	virtual void VisitEnd	(CInfoEchSupport	* pObjet){};

protected:

	// Export du n° de version
	virtual VOID OnExportNoVersion(JATPDocExport * pObjet);

	// Export de l'entete Atelier Creuset
	virtual VOID OnExportEntete(JATPDocExport * pObjet);

	// Export du nom étude
	virtual VOID OnExportEtude(JATPDocExport * pObjet);

	// Export du produit associé
	virtual VOID OnExportProduit(JATPDocExport * pObjet);

	// Export de la période
	virtual VOID OnExportPeriode(JATPDocExport * pObjet);

	// Export de la cible
	virtual VOID OnExportCible(JATPDocExport * pObjet);

	// Export Réference affinité
	virtual VOID OnExportRefAffinite(JATPDocExport * pObjet);

	// Export du filtre d'audience	
	virtual VOID OnExportFiltreAudience(JATPDocExport * pObjet);

	// Export Source
	virtual VOID OnExportSource(JATPDocExport * pObjet);

	// Export Terrains
	virtual VOID OnExportTerrains(JATPDocExport * pObjet);

	// Export Segment
	virtual VOID OnExportSegment(JATPDocExport * pObjet);

	// Export Type de poids
	virtual VOID OnExportTypePoid(JATPDocExport * pObjet);
	
	// Export Type de moteur calcul
	virtual VOID OnExportTypeCalcul(JATPDocExport * pObjet);

	// Export Formats utilisés
	virtual VOID OnExportFormats(JATPDocExport * pObjet);

	// Export Hypothèses
	virtual VOID OnExportHypothese(CInfoHypo *pObjet, JUnt32 IndexHypo);

	// Export Résultat Hypothèse
	virtual VOID OnExportResultHypothese(CInfoHypo *pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport);

	// Export Distribution Cumulée hypothèse
	virtual VOID OnExportDistriCumHypothese(CInfoHypo *pObjet, JUnt32 IndexHypo);

	// Export Distribution Nette hypothèse
	virtual VOID OnExportDistriNetteHypothese(CInfoHypo *pObjet, JUnt32 IndexHypo);

	// Export Saut de lignes	
	virtual VOID OnExportSTLigne(JATPDocExport * pObjet,const JUnt32 NbLigne);
	virtual VOID OnExportSTLigne(CInfoHypo * pObjet,const JUnt32 NbLigne);

	virtual VOID OnExportCtxLecture(JATPDocExport * pObjet);

	virtual void OnExportCourbeReponse(JATPDocExport * pObjet);

};
