//=======================
// fichier : JFCDraw.h
//
// Date: 25/07/2001
// auteur: JB
//=======================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCDRAW_H_

// on définit une macro pour identifier le fichier
#define _JFCDRAW_H_

// on inclut les fichiers nécessaires
#include "PoliceManager.h"

// on inclut les couleurs necessaires
#include "GestionCouleurs.h"

// les types énumérés pour les positions
enum Align{CENTER, RIGHT, LEFT, TOP, BOTTOM};

class JFCDraw
{
public:
	// le constructeur
	JFCDraw(long orgX, long orgY);

	// la fonction pour déplacer le curseur
	virtual void MoveTo(long posx, long posy) = 0;

	// la fonction pour tracer une ligne du curseur aux coordonnées de destination
	virtual void LineTo(long posx, long posy) = 0;

	// la fonction pour écrire du texte
	virtual void Text(const char * texte, Align HAlign = LEFT, Align VAlign = BOTTOM) = 0;

	// les fonctions pour créer un chemin
	virtual void BeginPath() = 0;
	virtual void EndPath() = 0;

	// les fonctions pour initialiser les variables d'état
	virtual void SetColor(long color) = 0;
	virtual void SetFillColor(long color) = 0;
	virtual void SetFont(long font) = 0;
	virtual void SetLineWidth(long linewidth) = 0;

	// les fonctions de balisage de la page
	virtual void StartPage() = 0;
	virtual void EndPage() = 0;

	// la fonction pour dessiner un BMP
	virtual void SetPicture(const char * filename, long Xmax, long Ymax) = 0;

	// le destructeur
	virtual ~JFCDraw();

	// la fonction pour initialiser le code page
	static void SetCodePage(long codePage);

protected:
	// le curseur
	long m_CurseurX;
	long m_CurseurY;

	// les variables de contrôle
	bool	m_PageStarted;
	long	m_NoPage;
	long	m_PosXMin;
	long	m_PosXMax;
	long	m_PosYMin;
	long	m_PosYMax;
	long    m_FontID;

	HDC		m_hDC;								// le handle du périphérique
	HFONT	m_Tab_Font[FontManager::m_Nb_Font];	// le handle de la font
	long	m_IndiceFont;						// indice du tableau de fonts
	long	m_Oldfont;							// l'indice de l'ancienne font

	// l'origine à partir de laquelle on dessine
	long m_OrgX;
	long m_OrgY;

	static long m_CodePage;		// la page de code du langage
};

// fin de l'inclusion conditionnelle
#endif



