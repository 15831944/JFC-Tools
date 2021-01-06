/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CVisiteurDelete
//
//	Descriptif:
//	Visiteur permettant de supprimer un noeud
//	DesignPattern Visiteur.
*/


#if !defined(AFX_VISITEURDELETE_H__C963D946_8002_4016_A803_190EF83FADC4__INCLUDED_)
#define AFX_VISITEURDELETE_H__C963D946_8002_4016_A803_190EF83FADC4__INCLUDED_

#pragma once

#include "Visiteur.h"
#include "..\AllTypeOfNode.h"


class AFX_EXT_CLASS CVisiteurDelete : public IVisiteur  
{
public:
	CVisiteurDelete();
	virtual ~CVisiteurDelete();

	//Visiteurs de composites
	virtual JVoid VisiteFamille( CFamille* nd );
	virtual JVoid VisiteCentre( CCentre* nd );

	virtual JVoid VisiteCouplageLibre( CCouplageLibre* nd );
	virtual JVoid VisiteLot( CLot* nd );

	virtual JVoid VisiteTitre(CTitre* nd);
	virtual JVoid VisiteTitreApparente(CTitreApparente* nd);
	virtual JVoid VisiteTitreLot(CTitreLot* nd);
	virtual JVoid VisiteTitreCouplageLibre(CTitreCouplageLibre* nd);


protected:
	JVoid DeleteInParent(INode* nd);
	JVoid DeleteFils(IComposite* nd);

};

#endif // !defined(AFX_VISITEURDELETE_H__C963D946_8002_4016_A803_190EF83FADC4__INCLUDED_)
