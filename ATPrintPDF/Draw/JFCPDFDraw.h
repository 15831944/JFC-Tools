//===========================
// fichier: JFCPDFDraw.h
//
// date: 28/08/2001
// auteur: JB
//===========================

// on teste si la macro qui identifie le fichier est d�j� d�finie
#ifndef _JFCPDFDRAW_H_

// on d�finit une macro qui identifie pour identifier le fichier
#define _JFCPDFDRAW_H_

// on inclut les fichiers n�cessaires
#include "../PdfLib/pdflib.h"

// on inclut les fichiers n�cessaires
#include "JFCDraw.h"
#include "JFCDecimilToUnit.h"

class JFCPDFDraw : public JFCDraw
{
public:
	// le constructeur:
	JFCPDFDraw(const char * filename, bool paysage = false, long zoom = 100, long offsetX = 0, long offsetY = 0, long orgX = 0, long orgY = 0);

	// la fonction pour d�placer le curseur
	void MoveTo(long posx, long posy);

	// la fonction pour tracer une ligne du curseur aux coordonn�es de destination
	void LineTo(long posx, long posy);

	// la fonction pour �crire du texte
	void Text(const char * texte, Align halign, Align valign);

	// les fonctions pour cr�er un chemin
	void BeginPath();
	void EndPath();

	// les fonctions pour initialiser les variables d'�tat
	void SetColor(long color);
	void SetFillColor(long color);
	//void SetFont(FontManager fonte);
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
	~JFCPDFDraw();

protected:
	// la fonction pour r�cup�rer les fonts
	static int CALLBACK EnumFoncProc(const LOGFONT *lpelfe, const TEXTMETRIC *lpntme, unsigned long FontType, LPARAM lParam);

	// la fonction pour rechercher les fontes
	bool SearchFontNames();

protected:
	// l'objet d'acc�s a la classe PDF
	PDF* m_pPDF;

	// le flag du Path
	bool m_Path;

	// la taille de la page
	long m_TailleX;
	long m_TailleY;

	// le param�tre de changement de coordonn�es
	DecimilToUnit m_Coord;

	// le nom des fontes syst�mes
	TCHAR m_FontNameRegu[LF_FACESIZE];
	TCHAR m_FontNameBold[LF_FACESIZE];

	// les variables d'�tat
	long m_Color;
	long m_FillColor;
	long m_hFont;
};

// fin du test sur la macro
#endif
