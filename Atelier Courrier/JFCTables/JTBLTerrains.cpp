//
// Fichier: JTBLTerrains.cpp
// Auteur:  Sylvain SAMMURI
// Date:    11/03/2003
//

// on inclut les définitions nécessaires
#include "stdafx.h"
#include "JTBLTerrains.h"

////////////////////
// les constructeurs

JTBLTerrains::JTBLTerrains()
{
	// on ne fait rien
}

/// la fonction pour rechercher un élément
const JTerrain * JTBLTerrains::GetItem(JInt32 IdSource, JInt32 IdTerrain) const
{
	//MoveTo(IdTerrain);
	for(MoveFirst(); IsValid(); MoveNext())
	{
		if (GetItem().m_IdSource == IdSource
			&& GetItem().m_IdTerrain == IdTerrain)
			return &GetItem();
	}

	return 0;
}

/// la fonction pour récupérer un élément
const JTerrain & JTBLTerrains::GetItem() const
{
	return JList<JTerrain>::GetItem();
}

////////////////////////////////
//  fonction d'import
JVoid JTBLTerrains::Import(CStdioFile& file)
{
	CString str;
	while (file.ReadString(str))
	{
		//ligne vide ou commentaire
		str.Trim();
		if(str.Find("//") == 0 || str.IsEmpty())
			continue;

		JTerrain terrain;
		CString resToken;
		int curPos= 0;

		// l'identifiant terrain
		resToken= str.Tokenize(",",curPos);
		terrain.m_IdTerrain = atoi(resToken);
		
		// l'identifiant source
		resToken= str.Tokenize(",",curPos);
		terrain.m_IdSource = atoi(resToken);

		// le chemin
		resToken= str.Tokenize(",",curPos);
		resToken.Remove('"');
		terrain.m_Chemin = resToken;

		if (!PathAvailable(terrain.m_Chemin.AsJCharPtr()))
		{
			// Here some invali character in path terrain
			// AfxMessageBox(_T("Path non valide dans Terrains.Tbl >> ") + terrain.m_Chemin.AsJCharPtr(), MB_ICONERROR | MB_OK);
					// AfxMessageBox(_T("Path non valide dans Terrains.Tbl >> ") + terrain.m_IdTerrain, MB_ICONERROR | MB_OK);

			CString TxtMess;
			TxtMess.Format("%s%s", "Path non valide dans Terrains.Tbl >> ", terrain.m_Chemin.AsJCharPtr());
			AfxMessageBox(TxtMess, MB_ICONERROR | MB_OK);
		}

		// le libellé
		resToken= str.Tokenize(",",curPos);
		resToken.Remove('"');
		terrain.m_Libelle = resToken;

		// Par défaut si absent, on initialise seuil rédhibitoire à 0
		terrain.m_SeuilRedhibitoire = 0;

		// Par défaut si absent, on initialise seuil rédhibitoire à 0
		terrain.m_SeuilAvertissement = 0; 

		// Lecture Seuil rédhibitoire
		resToken= str.Tokenize(",",curPos);
		if (resToken !="")
		{
			// le seuil rédhibitoire
			terrain.m_SeuilRedhibitoire = atoi(resToken);

			// Seuil avertissement
			resToken= str.Tokenize(",",curPos);
			if (resToken !="")
			{
				// le seuil avertissement
				terrain.m_SeuilAvertissement = atoi(resToken);
			}
			else
			{
				// Par défaut si absent, on initialise seuil avertissement à 0
				terrain.m_SeuilAvertissement = 0; 
			}
		}
		
		// on ajoute l'élément
		if (terrain.IsValid())
			AddTail() = terrain;
	}
}

JBool JTBLTerrains::PathAvailable(CString PathLabel)
{
	int Occur = PathLabel.FindOneOf("+,;/");

	if (PathLabel.FindOneOf("+,;") == -1)
	{
		return true;
	}		
	return false;
}	

/////////////////
// le destructeur
JTBLTerrains::~JTBLTerrains()
{
	// on ne fait rien
}
JVoid JTBLTerrains::MoveLast() const
{
	JList<JTerrain>::MoveLast();
}
JVoid JTBLTerrains::MoveFirst() const
{
	JList<JTerrain>::MoveFirst();
}
JBool JTBLTerrains::IsValid() const
{
	return JList<JTerrain>::IsValid();
}
JVoid JTBLTerrains::MovePrev() const
{
	JList<JTerrain>::MovePrev();
}
JVoid JTBLTerrains::MoveNext() const
{
	JList<JTerrain>::MoveNext();
}

////////////////////////////////////////////////////////////
// Nombre d'élément
JInt32 JTBLTerrains::GetCount() const
{
	return (JList<JTerrain>::GetCount()); 
}
