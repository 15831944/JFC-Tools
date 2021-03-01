/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CTitreLot
//
//	Descriptif:
//	Titre dans un lot
//	DesignPattern Classe composite Classe acceptant un visiteur.
*/


#if !defined(AFX_TITRELOT_H__BB8F7FF8_9606_4255_ADEA_96B8E21E3ACB__INCLUDED_)
#define AFX_TITRELOT_H__BB8F7FF8_9606_4255_ADEA_96B8E21E3ACB__INCLUDED_

#pragma once

#include "Leaf.h"


class AFX_EXT_CLASS CTitreLot : public ILeaf  
{
public:
	CTitreLot();
	CTitreLot( JUnt32 ID, JUnt32 idcentre, JUnt32 parentID, INode* hpar);
	virtual ~CTitreLot();

	virtual JBool IsTitre() { return true; };
	virtual JBool IsTitreLot() { return true; };
	virtual CTitreLot * AsTitreLot() { return this;};

	virtual JVoid Accept (IVisiteur* v);

	virtual JUnt32 GetPeriodicite() const;

	virtual JUnt32 GetIDCentre() const;

	virtual JArchive& Send(JArchive & lhs, JInt32 Version) const;
	virtual JArchive& Recv(JArchive & lhs, JInt32 Version);

private:
	JUnt32 m_IDCentre;

};

#endif // !defined(AFX_TITRELOT_H__BB8F7FF8_9606_4255_ADEA_96B8E21E3ACB__INCLUDED_)
