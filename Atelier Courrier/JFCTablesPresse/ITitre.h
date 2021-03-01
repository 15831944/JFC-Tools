/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	ITitre
//
//	Descriptif:
//	Classe de base d'un titre de presse à ne pas instancier directement
*/


#if !defined(AFX_ITITRE_H__INCLUDED_)
#define AFX_ITITRE_H__INCLUDED_

#pragma once

#include "JCouplageFixe.h"


class AFX_EXT_CLASS ITitre
{

public:
	virtual ~ITitre();
	ITitre(	JUnt32 Periodicite, JUnt32 IDAudience, JBool bAgregat = false);


	virtual JBool IsTitre();
	virtual ITitre * AsITitre() PURE;


	virtual JUnt32 GetPeriodicite() const;

	virtual JUnt32 GetIDAudience() const;

	virtual JBool IsAgregat() const;

protected:
	ITitre();

	JUnt32 m_Periodicite;
	JUnt32 m_IDAudience;

	JBool m_bAgregat;

public:
	JCouplageFixe * m_cplFixe;

};


#endif // !defined(AFX_ITITRE_H__INCLUDED_)
