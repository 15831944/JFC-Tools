//============================
// fichier: PoliceManager.h
//
// date: 11/09/2001
// auteur: JB
//============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _POLICEMANAGER_H_

// on définit une macro pour identifier le fichier
#define _POLICEMANAGER_H_

class FontManager
{
public:
	// le constructeur de recopie
	FontManager(const FontManager & source);

	// l'opérateur d'affectation
	FontManager & operator=(const FontManager & operande);

	// la fonction pour récupérer le nom de la fonte
	const char * GetFontName() const;

	// la fonction pour récupérer la taille de la fonte en pts
	long GetFontSize() const;

	// la fonction qui permet de recuperer l'indicateur de gras
	bool IsBold() const;

	// la fonction qui permet de recuperer l'indicateur de gras
	bool IsItalic() const;

	// fonction d'ajout de fonte au lancement de l'application
	static const FontManager & GetFont(long indice);
	
	// le destructeur
	~FontManager();

public:
	// les fonctions pour manipuler le mode d'incorporation
	static void SetEmbedding(int mode);
	static int  GetEmbedding();

public:
	// nombre de fontes
	enum {m_Nb_Font=4};

public:
	// les fontes possibles
	static const FontManager COURRIER_JFC_10;
	static const FontManager COURRIER_JFC_10_BOLD;
	static const FontManager COURRIER_JFC_11;
	static const FontManager COURRIER_JFC_11_BOLD;

private:
	// le mode d'incorporation des fontes
	static int m_Embedding;

private:
	// le constructeur:
	FontManager(const char * fontname,long taille, bool bold, bool italic);

private:	
	const char* m_FontName; // le nom de la fonte	
	long        m_Size;     // la taille de la fonte en points
	bool        m_Bold;     // l'indicateur de gras
	bool        m_Italic;   // l'indicateur d'italique
};
// fin du test sur la macro
#endif