/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JTitreCouplage
//
//	Descriptif:
//	Titre dans un couplage fixe ou libre
*/


#if !defined(AFX_JTITRECOUPLAGE_H__INCLUDED_)
#define AFX_JTITRECOUPLAGE_H__INCLUDED_

#pragma once
#pragma warning( disable : 4251 )
#include "ITitre.h"

class AFX_EXT_CLASS JTitreCouplage : public ITitre
{

public:
	JTitreCouplage(JUnt32 Periodicite, JUnt32 IDAudience, JUnt32 nbInsert);
	virtual ~JTitreCouplage();

	virtual JBool IsTitreCouplage() { return true; };

	virtual JUnt32 GetNbInsert() const;

protected:
	JTitreCouplage();
	JUnt32 m_nbInsert;
};


#endif // !defined(AFX_JTITRECOUPLAGE_H__INCLUDED_)
