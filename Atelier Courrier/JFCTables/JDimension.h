/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JDimension
//
//	Descriptif:
//	Element dimension de la table des dimensions
*/


#pragma once
#pragma warning( disable : 4251 )
#include "IElem.h"



class AFX_EXT_CLASS JDimension : public IElem  
{

public:
	JDimension(JUnt32 id, const JChar* lbl);
	virtual ~JDimension();
};
