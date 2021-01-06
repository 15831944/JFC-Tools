/*
//	JFC Informatique et M�dia
//	2004
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	JIDList
//
//	Descriptif:
//	Extension de la JList
//	Attention aux it�rateur, il est interne � la liste risque de probl�mes
*/

#pragma once
#ifndef JIDLIST_H
#define JIDLIST_H


class AFX_EXT_CLASS JIDList : public JList<JUnt32>
{

public:
	JIDList();
	virtual ~JIDList();

//	permet le copie sans toucher � la source(pas un swap)
	void Recopie( JList<JUnt32>& src);
//teste la pr�sence d'un �l�ment
	bool IDExist( JUnt32 id );
//retire un �l�ment
	void RemoveID( JUnt32 id );
};

#endif
