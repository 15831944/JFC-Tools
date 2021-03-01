/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JTitreCouplageFixe
//
//	Descriptif:
//	Titre dans un couplage fixe
*/


#if !defined(AFX_JTITRECOUPLAGEFIXE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
#define AFX_JTITRECOUPLAGEFIXE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_

#pragma once
#pragma warning( disable : 4251 )
#include "IElem.h"
#include "JTitreCouplage.h"




class AFX_EXT_CLASS JTitreCouplageFixe : public IElem , public JTitreCouplage
{

public:
	JTitreCouplageFixe(JUnt32 id, const JChar* lbl, JUnt32 Periodicite, JUnt32 IDAudience, JUnt32 nbInsert);
	virtual ~JTitreCouplageFixe();

	virtual JBool IsTitre() const { return true; };
	virtual JBool IsTitreCouplageFixe()  const { return true; };
	virtual JTitreCouplageFixe * AsTitreCouplageFixe() { return this;};
	virtual JTitreCouplage * AsTitreCouplage() { return this;};
	virtual ITitre * AsITitre() { return this;};

};


#endif // !defined(AFX_JTITRECOUPLAGEFIXE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
