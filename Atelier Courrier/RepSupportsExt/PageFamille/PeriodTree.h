/*
//	JFC Informatique et Média
//	2003
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CPeriodTree
//
//	Descriptif:
//	TreeCtrl spécilisé pour afficher les périodicités des support par un libellé de couleur différent
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
	virtual bool IsAttribue(int ID) const;/// redéfinition de la méthode
};