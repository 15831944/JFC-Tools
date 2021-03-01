/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CTitreCouplageLibre
//
//	Descriptif:
//	Titre dans un couplage libre
//	DesignPattern Classe composite Classe acceptant un visiteur.
*/

#if !defined(AFX_TitreCouplageLibre_H__40FBC72F_36A7_46B6_9F85_127BB15B39C1__INCLUDED_)
#define AFX_TitreCouplageLibre_H__40FBC72F_36A7_46B6_9F85_127BB15B39C1__INCLUDED_

#pragma once

#include "Leaf.h"
#include "JTitreCouplage.h"


class AFX_EXT_CLASS CTitreCouplageLibre : public ILeaf, public JTitreCouplage 
{

public:
	CTitreCouplageLibre();
	CTitreCouplageLibre( JUnt32 ID, JUnt32 nbinsert, JUnt32 idcentre, JUnt32 parentID, INode* hpar);
	virtual ~CTitreCouplageLibre();

	virtual JBool IsTitre()  const { return true; };
	virtual JBool IsTitreCouplage()  const { return true; };
	virtual JBool IsTitreCouplageLibre()  const { return true; };
	virtual CTitreCouplageLibre * AsTitreCouplageLibre() { return this;};
	virtual JTitreCouplage * AsTitreCouplage() { return this;};
	virtual ITitre * AsITitre() { return this;};

	virtual JVoid Accept (IVisiteur* v);
	
	friend class CVisiteurInsert;


	virtual JUnt32 GetIDCentre() const;
	JUnt32 GetMediaWeight() const  { return m_MediaWeight; } ;

	virtual JArchive& Send(JArchive & lhs, JInt32 Version) const;
	virtual JArchive& Recv(JArchive & lhs, JInt32 Version);


private:
	JUnt32 m_IDCentre;
	JUnt32 m_MediaWeight;
};

#endif // !defined(AFX_TitreCouplageLibre_H__40FBC72F_36A7_46B6_9F85_127BB15B39C1__INCLUDED_)
