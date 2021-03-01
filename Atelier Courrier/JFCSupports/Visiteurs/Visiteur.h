/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	IVisiteur
//
//	Descriptif:
//	Classe de base pour les visiteurs
*/


#if !defined(AFX_VISITEUR_H__F72FA4C6_39EB_4816_9FA1_69160172AC7D__INCLUDED_)
#define AFX_VISITEUR_H__F72FA4C6_39EB_4816_9FA1_69160172AC7D__INCLUDED_

#pragma once

#include "..\ClassesComposites\Noeud.h"

class CRegroupements;
class CFamille;  
class CCentre;  

class CCouplageLibre;
class CLot;

class CTitre;  
class CTitreApparente;  

class CTitreLot;  
class CTitreCouplageLibre;  


class AFX_EXT_CLASS IVisiteur  
{

public:
	IVisiteur() {};
	virtual ~IVisiteur() {};

	//Visiteurs de composites
	virtual JVoid VisiteFamille( CFamille* nd ) {};
	virtual JVoid VisiteCentre( CCentre* nd ) {};

	virtual JVoid VisiteCouplageLibre( CCouplageLibre* nd ) {};
	virtual JVoid VisiteLot( CLot* nd ) {};

	//Visiteurs de leaf
	virtual JVoid VisiteTitre( CTitre* nd ) {};
	virtual JVoid VisiteTitreApparente( CTitreApparente* nd ) {};
	virtual JVoid VisiteTitreLot( CTitreLot* nd ) {};
	virtual JVoid VisiteTitreCouplageLibre( CTitreCouplageLibre* nd ) {};

protected:
	CRegroupements* m_regroup;

};

#endif // !defined(AFX_VISITEUR_H__F72FA4C6_39EB_4816_9FA1_69160172AC7D__INCLUDED_)
