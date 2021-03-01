/*
//	JFC Informatique et Média
//	2004
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	JIDList
//
//	Descriptif:
//	Extension de la JList
//	Attention aux itérateur, il est interne à la liste risque de problèmes
*/

#pragma once
#ifndef JIDLIST_H
#define JIDLIST_H


class AFX_EXT_CLASS JIDList : public JList<JUnt32>
{

public:
	JIDList();
	virtual ~JIDList();

//	permet le copie sans toucher à la source(pas un swap)
	void Recopie( JList<JUnt32>& src);
//teste la présence d'un élément
	bool IDExist( JUnt32 id );
//retire un élément
	void RemoveID( JUnt32 id );
};

#endif
