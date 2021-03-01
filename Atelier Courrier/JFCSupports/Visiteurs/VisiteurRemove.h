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
//	Visiteur permettant de retier un noeud en vue d'un deplacement
//	DesignPattern Visiteur.
*/


#if !defined(AFX_VISITEURREMOVE_H__D29E5A3F_E079_49CE_896C_D6A222A8309E__INCLUDED_)
#define AFX_VISITEURREMOVE_H__D29E5A3F_E079_49CE_896C_D6A222A8309E__INCLUDED_

#pragma once

#include "Visiteur.h"
#include "..\AllTypeOfNode.h"


class AFX_EXT_CLASS CVisiteurRemove : public IVisiteur  
{

public:
	CVisiteurRemove();
	virtual ~CVisiteurRemove();

	//Visiteurs de composites
	virtual JVoid VisiteFamille( CFamille* nd );
	virtual JVoid VisiteCentre( CCentre* nd );

	virtual JVoid VisiteCouplageLibre( CCouplageLibre* nd );
	virtual JVoid VisiteLot( CLot* nd );

	//Pas de Visiteurs de leaf
	virtual JVoid VisiteTitre( CTitre* nd );
	virtual JVoid VisiteTitreApparente( CTitreApparente* nd );
	virtual JVoid VisiteTitreLot( CTitreLot* nd );
	virtual JVoid VisiteTitreCouplageLibre( CTitreCouplageLibre* nd );

private:
	JVoid Remove(INode* nd);

};

#endif // !defined(AFX_VISITEURREMOVE_H__D29E5A3F_E079_49CE_896C_D6A222A8309E__INCLUDED_)
