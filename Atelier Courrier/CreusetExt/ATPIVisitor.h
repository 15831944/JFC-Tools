//
// Fichier: ATPIVisitor.h
// Date:    02/03/2004
//

#pragma once

// on inclut les définitions nécessaires
class JATPDocExport;
class CInfoHypo;
class CInfoEchSupport;

class CATPIVisitor
{
public:
	CATPIVisitor(void);
	~CATPIVisitor(void);

	// Visite le Document Export
	virtual void Visit		(JATPDocExport		* pObjet) = 0;
	virtual void VisitBegin	(JATPDocExport		* pObjet) = 0;
	virtual void VisitEnd	(JATPDocExport		* pObjet) = 0;

	// Visite les hypothèses
	virtual void Visit		(CInfoHypo			* pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport) = 0;
	virtual void VisitBegin	(CInfoHypo			* pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport) = 0;
	virtual void VisitEnd	(CInfoHypo			* pObjet, JUnt32 IndexHypo, JATPDocExport * pDocExport) = 0;

	// Visite les échelles
	virtual void Visit		(CInfoEchSupport	* pObjet) = 0;
	virtual void VisitBegin	(CInfoEchSupport	* pObjet) = 0;
	virtual void VisitEnd	(CInfoEchSupport	* pObjet) = 0;

};
