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

	// Export hypoth�se / non utilis� ici
	virtual void VisitBegin(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport){};
	virtual void Visit(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport){};
	virtual void VisitEnd(CInfoHypo * pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport){};

	// Visite Export �chelle
	virtual void VisitBegin(CInfoEchSupport * pObjet);
	virtual void Visit(CInfoEchSupport		* pObjet);
	virtual void VisitEnd(CInfoEchSupport	* pObjet);


protected:

	// Export du n� de version
	virtual VOID OnExportNoVersion(JATPDocExport * pObjet);

	// Export de l'entete Atelier Creuset
	virtual VOID OnExportEntete(JATPDocExport * pObjet);

	// Export du nom �tude
	virtual VOID OnExportEtude(JATPDocExport * pObjet);

	// Export du produit associ�
	virtual VOID OnExportProduit(JATPDocExport * pObjet);

	// Export de la p�riode
	virtual VOID OnExportPeriode(JATPDocExport * pObjet);

	// Export de la cible
	virtual VOID OnExportCible(JATPDocExport * pObjet);

	// Export R�ference affinit�
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

	// Export Formats utilis�s
	virtual VOID OnExportFormats(JATPDocExport * pObjet);

	// Export contxtes
	virtual VOID OnExportCtxLecture(JATPDocExport * pObjet);

	//  Export entete supports �chelle
	virtual VOID OnExportEnteteSupports(JATPDocExport * pObjet);

	// Export des supports �chelles
	virtual VOID OnExportSupportEchelle(CInfoEchSupport * pObjet);

	// Export Saut de lignes	
	virtual VOID OnExportSTLigne(JATPDocExport		* pObjet,const JUnt32 NbLigne);
	virtual VOID OnExportSTLigne(CInfoEchSupport	* pObjet,const JUnt32 NbLigne);

};
