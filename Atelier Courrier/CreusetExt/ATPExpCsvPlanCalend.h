#pragma once

#include "ATPIVisitor.h"

class CATPExpCsvPlanCalend : public CATPIVisitor
{
public:
	CATPExpCsvPlanCalend(void);
	~CATPExpCsvPlanCalend(void);

	// Visite de l'objet export plan calendaire
	void Visit		(JATPDocExport * pObjet){};
	void VisitBegin (JATPDocExport * pObjet);
	void VisitEnd   (JATPDocExport * pObjet);

	// Export d�but hypoth�se pour plan calendaire
	void VisitBegin(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport);

	// Export hypoth�se pour plan calendaire
	void Visit(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport);

	// Export fin hypoth�se pour plan calendaire
	void VisitEnd(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport);

	// Visite les �chelles
	void Visit		(CInfoEchSupport	* pObjet){};
	void VisitBegin	(CInfoEchSupport	* pObjet){};
	void VisitEnd	(CInfoEchSupport	* pObjet){};

private:
	
	// Export du n� de version
	VOID OnExportNoVersion(JATPDocExport * pObjet);

	// Export de l'entete Atelier Creuset
	VOID OnExportEntete(JATPDocExport * pObjet);

	// Export du nom �tude
	VOID OnExportEtude(JATPDocExport * pObjet);

	// Export du produit associ�
	VOID OnExportProduit(JATPDocExport * pObjet);

	// Export de la p�riode
	VOID OnExportPeriode(JATPDocExport * pObjet);

	// Export de la cible
	VOID OnExportCible(JATPDocExport * pObjet);

	// Export R�ference affinit�
	VOID OnExportRefAffinite(JATPDocExport * pObjet);

	// Export du filtre d'audience	
	VOID OnExportFiltreAudience(JATPDocExport * pObjet);

	// Export Source
	VOID OnExportSource(JATPDocExport * pObjet);

	// Export Terrains
	VOID OnExportTerrains(JATPDocExport * pObjet);

	// Export Segment
	VOID OnExportSegment(JATPDocExport * pObjet);

	// Export Type de poids
	VOID OnExportTypePoid(JATPDocExport * pObjet);
	
	// Export Type de moteur calcul
	VOID OnExportTypeCalcul(JATPDocExport * pObjet);

	// Export Formats utilis�s
	VOID OnExportFormats(JATPDocExport * pObjet);

	// Export Hypoth�ses
	VOID OnExportHypothese(CInfoHypo *pObjet, JUnt32 IndexHypo);

	// Export Graphe Couverture
	VOID OnExportGrapheCouverture(CInfoHypo *pObjet, JUnt32 IndexHypo);

	// Export Graphe GRP
	VOID OnExportGrapheGRP(CInfoHypo *pObjet, JUnt32 IndexHypo);

	// Export Saut de lignes	
	VOID OnExportSTLigne(JATPDocExport * pObjet,const JUnt32 NbLigne);
	VOID OnExportSTLigne(CInfoHypo * pObjet,const JUnt32 NbLigne);

	void OnExportCourbeReponse(JATPDocExport * pObjet);

	VOID OnExportCtxLecture(JATPDocExport * pObjet);

};
