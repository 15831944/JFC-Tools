//===========================
// fichier: JFCGDIDraw.h   //
//                         //
// date: 25/07/2001        //
// auteur: JB              //
//===========================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _JFCGDIDRAW_H_

// on d�finit une macro pour identifier le fichier
#define _JFCGDIDRAW_H_

// on inclus les fichiers n�cessaires
#include "JFCDraw.h"
#include "JFCBitmap.h"
#include "JFCDecimilToUnit.h"
#include <windows.h>

class JFCGDIDraw : public JFCDraw
{
public:
	// le constructeur
	JFCGDIDraw(HDC hdc, long orgX, long orgY, long zoom, long offsetX = 0, long offsetY = 0);

	// la fonction pour d�placer le curseur
	void MoveTo(long posx, long posy);

	// la fonction pour tracer une ligne du curseur aux coordonn�es de destination
	void LineTo(long posx, long posy);

	// la fonction pour �crire du texte
	void Text(const char * texte, Align HAlign = LEFT, Align VAlign = BOTTOM);

	// les fonctions pour cr�er un chemin
	void BeginPath();
	void EndPath();

	// les fonctions pour initialiser les variables d'�tat
	void SetColor(long color);
	void SetFillColor(long color);
	void SetFont(long font);
	void SetLineWidth(long linewidth);

	// les fonctions de balisage de la page
	void StartPage();
	void EndPage();

	// la fonction pour r�cup�rer le num�ro de page
	long GetNoPage();

	// la fonction pour dessiner un BMP
	void SetPicture(const char * filename, long Xmax, long Ymax);

	// le destructeur
	~JFCGDIDraw();

protected:
	// la fonction pour cr�er une structure fonte d'apr�s un identifiant
	void CreateLogfont(LOGFONT & lf, FontManager fonte);

protected:

	//HDC				m_hDC;								// le handle du p�riph�rique
	long			m_idDC;								// l'identifiant du hDC � restaurer
	HPEN			m_hPen;								// le stylo
	HBRUSH			m_hBrush;							// le pinceau
	//long			m_Oldfont;							// l'indice de l'ancienne font
	//HFONT			m_Tab_Font[FontManager::m_Nb_Font];	// le handle de la font
	//long			m_IndiceFont;						// indice du tableau de fonts
	DecimilToUnit	m_Coord;							// l'objet de transformation des donn�es

};

// fin du test sur la macro
#endif
