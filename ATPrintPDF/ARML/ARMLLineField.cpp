//=============================
// fichier: ARMLLineField.cpp
//
// date: 08/09/2001
// auteur: JB
//=============================

// on inclut les fichiers nécessaires
#include "stdafx.h"
#include "ARMLLineField.h"

//========================================================
// le constructeur:
//========================================================
LineField::LineField(const CString & chaine)
{
	// on initialise la chaine
	m_String = chaine;
}

//========================================================
// le constructeur de recopie:
//========================================================
LineField::LineField(const LineField & source)
{
	// on recopie le paramètre
	m_String = source.m_String;
}

//========================================================
// les fonctions pour connaître le type du champ:
//========================================================
bool LineField::IsCommand() const
{
	// on teste le premier caractère du champ
	return (m_String.GetAt(0) == '@');
}

bool LineField::IsInteger() const
{
	// on teste les caractères de la chaine
	for(long i = 0, max = m_String.GetLength(); i < max; i++)
	{
		// on teste le caractère
		if (m_String.GetAt(i) < '0' || m_String.GetAt(i) > '9') return (false);
	}
	// on est arrivé ici, tous les caractères sont valides on renvoie le code de succés
	return (true);
}

bool LineField::IsText() const
{
	// on teste si les bornes sont des guillemets
	return (m_String.GetAt(0) == '\"' && m_String.GetAt(m_String.GetLength() -1) == '\"');
}

bool LineField::IsTextPosition() const
{
	// on teste le premier caractère
	if (m_String.GetAt(0) != 'G' && m_String.GetAt(0) != 'D' && m_String.GetAt(0) != 'C') return (false);

	// on teste les caractère suivants
	for (long i = 1, max = m_String.GetLength(); i < max; i++)
	{
		// on teste si le caractère est un chiffre
		if (m_String.GetAt(i) < '0' || m_String.GetAt(i) > '9') return (false);
	}
	// on est arrivé ici, on renvoie le code de succés
	return (true);
}

//===========================================================
// les fonctions pour récupérer les informations du champ:
//===========================================================
bool LineField::GetCommand(CString & Commande) const
{
	// on teste si c'est une commande
	if (!this->IsCommand()) return (false);

	// on vide le paramètre
	Commande = "";

	// on récupère la commande
	for (long i = 1, max = m_String.GetLength(); i < max; i++)
	{
		// on copie le caractère
		Commande += m_String.GetAt(i);
	}
	// on renvoie le code de succés
	return (true);
}

bool LineField::GetInteger(long & integer) const
{
	// on teste si c'est un entier
	if (!this->IsInteger()) return (false);

	// on récupère la valeur
	integer = atoi(m_String);

	// on renvoie le code de succés
	return (true);
}

bool LineField::GetText(CString & texte) const
{
	// on teste si c'est un champ de texte
	if (!this->IsText()) return (false);

	// on initialise le paramètre
	texte = "";

	// on récupère le texte
	for (long i = 1, max = m_String.GetLength(); i < (max-1); i++)
	{
		// on recopie le caractère
		texte += m_String.GetAt(i);
	}
	// on renvoie le code de succés
	return (true);
}

bool LineField::GetTextPosition(char & justification, long & position) const
{
	// on teste si c'est un champ position de texte
	if (!this->IsTextPosition()) return (false);

	// on récupère le paramètre de justification
	justification = m_String.GetAt(0);

	// on recopie les chiffres dans une chaine temporaire
	CString strtmp = "";
	for (long i = 1, max = m_String.GetLength(); i < max; i++)
	{
		strtmp += m_String.GetAt(i);
	}
	// on récupère la valeur de la position
	position = atoi(strtmp);

	// on renvoie le code de succés
	return (true);
}

//=================================================================
// le destructeur:
//=================================================================
LineField::~LineField()
{
	// on ne fait rien
}
