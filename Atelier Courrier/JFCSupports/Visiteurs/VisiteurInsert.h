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
//	Visiteur permettant d'inserer un noued
//	DesignPattern Visiteur.
*/


#if !defined(AFX_VISITEURINSERT_H__D29E5A3F_E079_49CE_896C_D6A222A8309E__INCLUDED_)
#define AFX_VISITEURINSERT_H__D29E5A3F_E079_49CE_896C_D6A222A8309E__INCLUDED_

#pragma once

#include "Visiteur.h"
#include "..\AllTypeOfNode.h"



class AFX_EXT_CLASS CVisiteurInsert : public IVisiteur  
{

public:
	CVisiteurInsert();
	virtual ~CVisiteurInsert();


	//Visiteurs de composites
	virtual JVoid VisiteFamille( CFamille* nd );
	virtual JVoid VisiteCentre( CCentre* nd );
	virtual JVoid VisiteLot( CLot* nd );
	virtual JVoid VisiteCouplageLibre( CCouplageLibre* nd );

	//Visiteurs de leaf
	virtual JVoid VisiteTitre( CTitre* nd );
	virtual JVoid VisiteTitreApparente( CTitreApparente* nd );
	virtual JVoid VisiteTitreLot( CTitreLot* nd );
	virtual JVoid VisiteTitreCouplageLibre( CTitreCouplageLibre* nd );


private:
	JVoid Insert( INode* nd );
	JVoid InsertInNode(INode* nd);

};

#endif // !defined(AFX_VISITEURINSERT_H__D29E5A3F_E079_49CE_896C_D6A222A8309E__INCLUDED_)
