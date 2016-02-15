//============================
// fichier: JFCDrawFrame.h
//
// date: 03/09/2001
// auteur: JB
//============================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAWFRAME_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAWFRAME_H_

// on inclut les fichiers nécessaires
#include "Afxtempl.h "
#include "JFCDrawComposite.h"

class DrawFrame : public DrawComposite
{
public:

	class Champ
	{
	public:
		// les constructeurs
		Champ();
		Champ(char justification, long position, CString texte, bool bold);
		// le constructeur de recopie
		Champ(const Champ & source);
		// l'opérateur d'égalité
		Champ & operator=(const Champ & operande);
		// le destructeur
		~Champ();

		// le caractère de justification
		char m_Justification;
		// la position en x
		long m_Position;
		// le texte
		CString m_Texte;
		// les infos sur la police
		bool m_Bold;
	};

	class Ligne
	{
	public:
		// le constructeur
		Ligne();
		// le constructeur de recopie
		Ligne(const Ligne & source);
		// l'opérateur d'égalité
		Ligne & operator=(const Ligne & operande);
		// le destructeur
		~Ligne();
		// le tableau de champ
		CArray<Champ, Champ&> m_Donnees;
	};

public:
	// le constructeur
	DrawFrame();

	// le constructeur de recopie
	DrawFrame(const DrawFrame & source);

	// la fonction pour dessiner la frame
	virtual void Draw(JFCDraw * pDraw, long firstpage, long lastpage);

	// la fonction de clonage
	virtual DrawComponent * Clone() const;

	// le destructeur
	virtual ~DrawFrame();

	// les fonctions pour remplir les 
	void SetFrameParams(long lignedepart, long nbligne, long position, long largeur);
	void SetStrokeStyles(long widthTG, long widthTH, long widthTD, long widthTB);
	void SetBkColor(long bkcolor);
	void SetNbLine(long nbline);
	void AddField(long idligne, char justification, long position, CString texte, bool bold);
	
	// la fonction pour créer les composants
	void CreateFrame();
protected:
	// la fonction pour récupérer la position absolue
	long GetPosX(long posrelative);

protected:
	// les paramètres de position du cadre
	long m_WidthTG;
	long m_WidthTH;
	long m_WidthTD;
	long m_WidthTB;

	// les paramètres de positionnement dans la page
	long m_StartLigne;
	long m_NbLigne;
	long m_StartCol;
	long m_NbCol;

	// la couleur utilisée
	long m_couleur;

	// le paramètre de couleur de fond du cadre
	long m_BackgroundColor;

	// le container de données
	CArray<Ligne, Ligne&> m_FrameData;
};
// fin du test sur la macro
#endif
