/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JTitre
//
//	Descriptif:
//	Element titre de la table des titres
*/


#if !defined(AFX_JTITRE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
#define AFX_JTITRE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_

#pragma once
#pragma warning( disable : 4251 )
#include "IElem.h"
#include "ITitre.h"




class AFX_EXT_CLASS JTitre : public IElem , public ITitre
{

public:
	JTitre(JUnt32 id, const JChar* lbl, JUnt32 period, JUnt32 IdAudience, JBool Agregat);
	virtual ~JTitre();

	virtual JBool IsTitre()  const { return true; };
	virtual ITitre * AsITitre() { return this;};
	virtual CTitre * AsTitre() { return (CTitre*)this;};

	virtual JBool IsCouplage()  const { return m_cplFixe!=NULL?true:false; };
	virtual JBool IsCouplageFixe()  const { return m_cplFixe!=NULL?true:false; };

	virtual ICouplage * AsCouplage() { return m_cplFixe;};
	virtual CCouplageLibre * AsCouplageLibre() { return NULL;};
	virtual JCouplageFixe * AsCouplageFixe() { return m_cplFixe;};



};


#endif // !defined(AFX_JTITRE_H__25676C4C_BF74_480B_8CE7_BE01BF61C532__INCLUDED_)
