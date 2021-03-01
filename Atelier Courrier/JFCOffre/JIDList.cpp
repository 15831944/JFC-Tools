// JIDList.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "JIDList.h"


JIDList::JIDList()
{
}

JIDList::~JIDList()
{
}

bool JIDList::IDExist( JUnt32 id )
{
	for (MoveFirst(); IsValid(); MoveNext() )
		if (GetItem() == id )
			return true; 
	return false;
}

void JIDList::RemoveID( JUnt32 id )
{
	for (MoveFirst(); IsValid(); MoveNext() )
		if (GetItem() == id )
			Remove(); 
}

void JIDList::Recopie( JList<JUnt32>& src)
{
	Reset();
	for (src.MoveFirst(); src.IsValid(); src.MoveNext() )
		AddTail() = src.GetItem();
}