//==========================
// fichier: ARMLLineField.h
//
// date: 08/09/2001
// auteur: JB
//==========================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _ARMLLINEFIELD_H_

// on définit une macro pour identifier le fichier
#define _ARMLLINEFIELD_H_

// on inclut les fichiers nécessaires

class LineField
{
public:
	// le constructeur:
	LineField(const CString & chaine);

	// le constructeur de recopie
	LineField(const LineField & source);

	// l'opérateur d'égalité
	LineField & operator = (const LineField & operande);

	// les fonction pour tester le type de champ
	bool IsText() const;
	bool IsInteger() const;
	bool IsTextPosition() const;
	bool IsCommand() const;

	// les fonction pour récupérer le champ
	bool GetCommand(CString & commande) const;
	bool GetText(CString & texte) const;
	bool GetTextPosition(char & justification, long & position) const;
	bool GetInteger(long & integer) const;

	// le destructeur
	~LineField();
protected:
	// la chaine de caractère
	CString m_String;
};
// fin du test sur la macro
#endif