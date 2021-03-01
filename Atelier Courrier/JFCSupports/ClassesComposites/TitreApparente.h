/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CTitreApparente
//
//	Descriptif:
//	Titre apparenté (titre à  créer) possède un médiaweight et un support tuteur dont il prend l'ID d'audiance
//	DesignPattern Classe composite Classe acceptant un visiteur.
*/


#if !defined(AFX_TITREAPPARENTE_H__24DB7A6F_BFB7_483F_B829_5944C6BB4D0A__INCLUDED_)
#define AFX_TITREAPPARENTE_H__24DB7A6F_BFB7_483F_B829_5944C6BB4D0A__INCLUDED_

#pragma once

#include "Leaf.h"
#include "ITitre.h"

class AFX_EXT_CLASS CTitreApparente : public ILeaf , public ITitre 
{

public:
	CTitreApparente();
	CTitreApparente(JUnt32 ID, const JChar* lbl, JUnt32 idsupptuteur, JUnt32 mw, JUnt32 parentID);
	virtual ~CTitreApparente();

	JVoid SetLabel(const JChar* lbl);
	CString GetLabelForEdit () const;

	virtual JBool IsTitre()  const { return true; };
	virtual JBool IsTitreApparente()  const { return true; };
	virtual ITitre * AsITitre() { return this;};
	virtual CTitre * AsTitre() { return  (CTitre*)this;};
	virtual CTitreApparente * AsTitreApparente() { return this;};

	virtual JVoid Accept (IVisiteur* v);

	virtual JUnt32 GetSupportTuteur() const;

	virtual JUnt32 GetMediaWeight() const;
	virtual JVoid SetMediaWeight(JUnt32 mw);
	
	virtual JArchive& Send(JArchive & lhs, JInt32 Version) const;
	virtual JArchive& Recv(JArchive & lhs, JInt32 Version);

private:
	JUnt32 m_IDsupportTuteur;
	JUnt32 m_MediaWeight;

};


#endif // !defined(AFX_TITREAPPARENTE_H__24DB7A6F_BFB7_483F_B829_5944C6BB4D0A__INCLUDED_)
