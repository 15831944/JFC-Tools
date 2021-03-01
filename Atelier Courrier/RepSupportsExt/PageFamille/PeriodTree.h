/*
//	JFC Informatique et M�dia
//	2003
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CPeriodTree
//
//	Descriptif:
//	TreeCtrl sp�cilis� pour afficher les p�riodicit�s des support par un libell� de couleur diff�rent
*/

#pragma once

//JFCSupport
#include "SupportTree.h"

class CDlgFamille;

class CPeriodTree : public CSupportTree
{
public:
	CPeriodTree(void);
	virtual ~CPeriodTree(void);
	CDlgFamille * m_pDlgParent;

private:
	virtual bool IsAttribue(int ID) const;/// red�finition de la m�thode
};