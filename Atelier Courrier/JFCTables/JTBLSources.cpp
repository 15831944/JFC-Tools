//
// Fichier: JTBLSources.cpp
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "JTBLSources.h"

////////////////////
// les constructeurs

JTBLSources::JTBLSources()
{
}


/// la fonction pour r�cup�rer un �l�ment
const JSource * JTBLSources::GetItem(JInt32 IdSource) const
{
	//MoveTo(IdSource);
	//if (IsValid())
	//	return &GetItem();
	//else
	//	return 0;
	for(MoveFirst(); IsValid(); MoveNext())
	{
		if(GetItem().m_IdSource == IdSource)
			return &GetItem();
	}
	return 0;
}

/// la fonction pour r�cup�rer un �l�ment
const JSource & JTBLSources::GetItem() const
{
	return JList<JSource>::GetItem();
}

////////////////////////////////
//  fonction d'import
JVoid JTBLSources::Import(CStdioFile& file)
{
	CString str;
	while (file.ReadString(str))
	{
		//ligne vide ou commentaire
		str.Trim();
		if(str.Find("//") == 0 || str.IsEmpty())
			continue;

		JSource source;
		CString resToken;
		int curPos= 0;

		// l'identifiant
		resToken= str.Tokenize(",",curPos);
		source.m_IdSource = atoi(resToken);
		
		// l'identifiant m�dia
		resToken= str.Tokenize(",",curPos);
		source.m_IdMedia = atoi(resToken);

		// le libell�
		resToken= str.Tokenize(",",curPos);
		resToken.Remove('"');
		source.m_Libelle = resToken;

		// on ajoute l'�l�ment
		if (source.IsValid())
			AddTail() = source;//Add(source.m_IdSource) = source;
	}
}

/////////////////
// le destructeur
JTBLSources::~JTBLSources()
{
}

JVoid JTBLSources::MoveLast() const
{
	JList<JSource>::MoveLast();
}
JVoid JTBLSources::MoveFirst() const
{
	JList<JSource>::MoveFirst();
}
JBool JTBLSources::IsValid() const
{
	return JList<JSource>::IsValid();
}
JVoid JTBLSources::MovePrev() const
{
	JList<JSource>::MovePrev();
}
JVoid JTBLSources::MoveNext() const
{
	JList<JSource>::MoveNext();
}

