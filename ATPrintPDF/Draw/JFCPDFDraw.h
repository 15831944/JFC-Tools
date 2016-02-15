//===========================
// fichier: JFCPDFDraw.h
//
// date: 28/08/2001
// auteur: JB
//===========================

// on teste si la macro qui identifie le fichier est déjà définie
#ifndef _JFCPDFDRAW_H_

// on définit une macro qui identifie pour identifier le fichier
#define _JFCPDFDRAW_H_

// on inclut les fichiers nécessaires
#include "../PdfLib/pdflib.h"

// on inclut les fichiers nécessaires
#include "JFCDraw.h"
#include "JFCDecimilToUnit.h"

class JFCPDFDraw : public JFCDraw
{
public:
	// le constructeur:
	JFCPDFDraw(const char * filename, bool paysage = false, long zoom = 100, long offsetX = 0, long offsetY = 0, long orgX = 0, long orgY = 0);

	// la fonction pour déplacer le curseur
	void MoveTo(long posx, long posy);

	// la fonction pour tracer une ligne du curseur aux coordonnées de destination
	void LineTo(long posx, long posy);

	// la fonction pour écrire du texte
	void Text(const char * texte, Align halign, Align valign);

	// les fonctions pour créer un chemin
	void BeginPath();
	void EndPath();

	// les fonctions pour initialiser les variables d'état
	void SetColor(long color);
	void SetFillColor(long color);
	//void SetFont(FontManager fonte);
	void SetFont(long font);
	void SetLineWidth(long linewidth);

	// les fonctions de balisage de la page
	void StartPage();
	void EndPage();

	// la fonction pour récupérer le numéro de page
	long GetNoPage();

	// la fonction pour dessiner un BMP
	void SetPicture(const char * filename, long Xmax, long Ymax);

	// le destructeur
	~JFCPDFDraw();

protected:
	// la fonction pour récupérer les fonts
	static int CALLBACK EnumFoncProc(const LOGFONT *lpelfe, const TEXTMETRIC *lpntme, unsigned long FontType, LPARAM lParam);

	// la fonction pour rechercher les fontes
	bool SearchFontNames();

protected:
	// l'objet d'accés a la classe PDF
	PDF* m_pPDF;

	// le flag du Path
	bool m_Path;

	// la taille de la page
	long m_TailleX;
	long m_TailleY;

	// le paramètre de changement de coordonnées
	DecimilToUnit m_Coord;

	// le nom des fontes systèmes
	TCHAR m_FontNameRegu[LF_FACESIZE];
	TCHAR m_FontNameBold[LF_FACESIZE];

	// les variables d'état
	long m_Color;
	long m_FillColor;
	long m_hFont;
};

// fin du test sur la macro
#endif
