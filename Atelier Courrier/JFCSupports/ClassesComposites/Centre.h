/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CCentre
//
//	Descriptif:
//	Centre d'interêt dans l'arborescence des familles
//	DesignPattern Classe composite Classe acceptant un visiteur.
*/


#if !defined(AFX_CENTRE_H__3703B0A4_6E6F_4A16_AF2A_D93527C4D376__INCLUDED_)
#define AFX_CENTRE_H__3703B0A4_6E6F_4A16_AF2A_D93527C4D376__INCLUDED_

#pragma once

#include "Ilot.h"


class AFX_EXT_CLASS CCentre : public ILot  
{

public:
	CCentre();
	CCentre(JUnt32 ID, const JChar* lbl, JUnt32 parentID);
	virtual ~CCentre();

	virtual JVoid Accept (IVisiteur* v);
	
	virtual JArchive& Send(JArchive & lhs, JInt32 Version) const;
	virtual JArchive& Recv(JArchive & lhs, JInt32 Version);

};

#endif // !defined(AFX_CENTRE_H__3703B0A4_6E6F_4A16_AF2A_D93527C4D376__INCLUDED_)
