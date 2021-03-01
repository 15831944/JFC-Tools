/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JPoids
//
//	Descriptif:
//	Element poids de la table des poids
*/


#pragma once
#pragma warning( disable : 4251 )
#include "IElem.h"



class AFX_EXT_CLASS JPoids : public IElem  
{

public:
	JPoids(JUnt32 id, const JChar* lbl);
	virtual ~JPoids();
};
