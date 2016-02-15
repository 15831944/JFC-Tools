// PWFile.cpp: implementation of the CPWFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PWFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPWFile::CPWFile()
{
	// on n'a rien a faire
}

BOOL CPWFile::OpenFile( LPCSTR nom )
{
	BOOL succes;

	// on teste les exceptions
	try
	{
		// on ouvre le fichier
		succes = CStdioFile::Open( nom, CFile::shareDenyWrite | CFile::modeRead );
	}
	catch ( CFileException* )
	{
		// on modifie le code de retour
		succes = FALSE;
	}

	// on quitte
	return succes;
}

BOOL CPWFile::ReadString( CString *chaine )
{
	INT  taille;
	BOOL succes;

	// on initialise la taille
	taille = 0;

	// on boucle tant qu'on a pas une ligne non vide
	while ( taille == 0 )
	{
		// on teste les exceptions
		try
		{
			// on lit la ligne suivante
			succes = CStdioFile::ReadString( *chaine );
		}
		catch ( CFileException* )
		{
			// on vide la ligne
			chaine->Empty();	

			// on quitte
			return FALSE;
		}

		// on teste si on a atteint la fin du fichier
		if ( succes == FALSE )
		{
			// on vide la ligne
			chaine->Empty();	

			// on quitte
			return TRUE;
		}

		// on note la taille de la ligne
		taille = chaine->GetLength();
	}

	// on quitte
	return TRUE;
}

BOOL CPWFile::GetNbItems( CString *chaine, INT *nombre, TCHAR separateur )
{
	INT taille;
	INT courant;

	// on initialise le nombre d'elements
	*nombre = 0;

	// on enleve les eventuels blancs de debut et de fin
	chaine->TrimLeft();
	chaine->TrimRight();

	// on recupere la taille de la chaine
	taille = chaine->GetLength();
	if ( taille < 2 )
	{
		// on quitte
		return FALSE;
	}

	// on teste si le premier caractere est bien le separateur
	if ( (*chaine)[0] != separateur )
	{
		// on quitte
		return FALSE;
	}

	// on parcourt tous les caracteres significatifs de la chaine
	for ( courant = 1; courant < taille; courant += 1 )
	{
		// on teste si le caractere est un chiffre
		if ( ((*chaine)[courant] < '0') || ((*chaine)[courant] > '9') )
		{
			// on quitte
			return FALSE;
		}

		// on modifie le nombre d'elements
		*nombre = ((*nombre) * 10) + ((*chaine)[courant] - '0');
	}

	// on quitte
	return TRUE;
}

BOOL CPWFile::GetStringItems( CString *chaine, CStringArray *elements, TCHAR separateur )
{
	CString element;
	INT     taille;
	INT     debut;
	INT     fin;

	// on vide la liste des elements
	elements->RemoveAll();

	// on recupere la taille de la chaine
	taille = chaine->GetLength();
	if ( taille == 0 )
	{
		return TRUE;
	}

	// on teste les exceptions
	try
	{
		// on initialise la position du premier caractere
		debut = 0;

		// on parcourt tous les caracteres de la ligne
		for ( fin = 0; fin < taille; fin += 1 )
		{
			// on teste si c'est un caractere de separation
			if ( (*chaine)[fin] == separateur )
			{
				// on recupere l'element
				element = chaine->Mid( debut, fin - debut );	

				// on enleve les eventuels blancs de debut et de fin
				element.TrimLeft();
				element.TrimRight();

				// on ajoute l'element
				elements->Add( element );

				// on positionne le prochain caractere de debut
				debut = fin + 1;
			}
		}

		// on recupere le dernier element
		if ( debut < taille )
		{
			element = chaine->Mid( debut, fin - debut );
		}
		else
		{
			element.Empty();
		}

		// on enleve les eventuels blancs de debut et de fin
		element.TrimLeft();
		element.TrimRight();

		// on ajoute l'element
		elements->Add( element );
	}
	catch ( CMemoryException* )
	{
		// on vide la liste des elements
		elements->RemoveAll();

		// on quitte
		return FALSE;
	}

	// on quitte
	return TRUE;
}

VOID CPWFile::CloseFile()
{
	// on teste les exceptions
	try
	{
		// on ferme le fichier
		CStdioFile::Close();
	}
	catch ( CFileException* )
	{
		// on ne peut rien faire
	}

	// on quitte
	return;
}

CPWFile::~CPWFile()
{
	// on n'a rien a faire
}
