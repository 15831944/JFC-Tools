#pragma once

#include "ATPIVisitor.h"

class CATPExportEchelle  : public CATPIVisitor
{
public:
	CATPExportEchelle(void);
	~CATPExportEchelle(void);

	// Visite de l'objet entete pour export echelle
	virtual void Visit		(JATPDocExport * pObjet){};
	virtual void VisitBegin (JATPDocExport * pObjet);
	virtual void VisitEnd   (JATPDocExport * pObjet);

	// Export hypothèse / non utilisé ici
	virtual void VisitBegin(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport){};
	virtual void Visit(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport){};
	virtual void VisitEnd(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport){};

	// Visite Export échelle
	virtual void VisitBegin(CInfoEchSupport * pObjet);
	virtual void Visit(CInfoEchSupport		* pObjet);
	virtual void VisitEnd(CInfoEchSupport	* pObjet);


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

	// Export contxtes
	virtual VOID OnExportCtxLecture(JATPDocExport * pObjet);

	//  Export entete supports échelle
	virtual VOID OnExportEnteteSupports(JATPDocExport * pObjet);

	// Export des supports échelles
	virtual VOID OnExportSupportEchelle(CInfoEchSupport * pObjet);

	// Export Saut de lignes	
	virtual VOID OnExportSTLigne(JATPDocExport		* pObjet,const JUnt32 NbLigne);
	virtual VOID OnExportSTLigne(CInfoEchSupport	* pObjet,const JUnt32 NbLigne);

};
