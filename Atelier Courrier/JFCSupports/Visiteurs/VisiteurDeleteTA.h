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
//	Visiteur permettant de supprimer un titre apparenté
//	DesignPattern Visiteur.
*/

#if !defined(AFX_VISITEURDELETETA_H__C963D946_8002_4016_A803_190EF83FADC4__INCLUDED_)
#define AFX_VISITEURDELETETA_H__C963D946_8002_4016_A803_190EF83FADC4__INCLUDED_

#pragma once

#include "Visiteur.h"
#include "VisiteurDelete.h"
#include "..\AllTypeOfNode.h"

class AFX_EXT_CLASS CVisiteurDeleteTA : public CVisiteurDelete
{
public:
	CVisiteurDeleteTA();
	virtual ~CVisiteurDeleteTA();

	virtual JVoid VisiteTitreApparente(CTitreApparente* nd);
};

#endif // !defined(AFX_VISITEURDELETETA_H__C963D946_8002_4016_A803_190EF83FADC4__INCLUDED_)
