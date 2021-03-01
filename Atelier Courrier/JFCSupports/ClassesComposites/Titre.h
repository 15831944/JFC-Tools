/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CTitre
//
//	Descriptif:
//	Feuille titre de l'arborecence famille centres
//	DesignPattern Classe composite Classe acceptant un visiteur.
*/


#if !defined(AFX_TITRE_H__BB8F7FF8_9606_4255_ADEA_96B8E21E3ACB__INCLUDED_)
#define AFX_TITRE_H__BB8F7FF8_9606_4255_ADEA_96B8E21E3ACB__INCLUDED_

#pragma once

#include "Leaf.h"
#include "ITitre.h"


class AFX_EXT_CLASS CTitre : public ILeaf, public ITitre  
{
public:
	CTitre();
	CTitre(JUnt32 ID, const JChar* lbl, JUnt32 parentID);
	virtual ~CTitre();

	virtual JBool IsTitre() const  { return true; };

	virtual ITitre * AsITitre() { return this;};
	virtual const ITitre * AsITitre() const { return this;};

	virtual CTitre * AsTitre() { return this;};


	virtual JVoid Accept (IVisiteur* v);

	virtual JArchive& Send(JArchive & lhs, JInt32 Version) const;
	virtual JArchive& Recv(JArchive & lhs, JInt32 Version);

};



#endif // !defined(AFX_TITRE_H__BB8F7FF8_9606_4255_ADEA_96B8E21E3ACB__INCLUDED_)
