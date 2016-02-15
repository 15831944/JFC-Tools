//==============================
// fichier: JFCPDFDraw.cpp
//
// date: 28/08/2001
// auteur: JB
//==============================

// on inclus les fichiers nécessaires
#include "stdafx.h"
#include "JFCPDFDraw.h"
#include "JFCBitmap.h"

//=================================================================================
// le constructeur:
//=================================================================================
JFCPDFDraw::JFCPDFDraw(const char * filename, bool paysage, long zoom, long offsetX, long offsetY, long orgX, long orgY) : m_TailleX((paysage) ? 2970 : 2100), m_TailleY((paysage) ? 2100 : 2970), m_Coord((paysage) ? 842 : 595, (paysage) ? 595 : 842, (paysage) ? 2970 : 2100, (paysage) ? 2100 : 2970, zoom, offsetX, offsetY), JFCDraw(orgX, orgY)
{
	// on recherche les fontes
	if (!SearchFontNames())
		throw long(0);

	// on alloue le PDF
	m_pPDF = PDF_new();

	// on détermine la compatibilité
	PDF_set_parameter(m_pPDF, "compatibility", "1.3");

	// on ouvre le fichier
	if (PDF_open_file(m_pPDF, filename) == -1)
	{
		PDF_delete(m_pPDF);
		throw long(0);
	}

	// on remplit ses paramètres
	PDF_set_info(m_pPDF, "Creator", "Atelier Radio");
    PDF_set_info(m_pPDF, "Author",  "JFC Informatique et Média");
    PDF_set_info(m_pPDF, "Title",   "Export PDF");

	// on construit les fontes
	char BufRegu[LF_FACESIZE + 32]; ::sprintf(BufRegu, "%s=COUR.TTF",   m_FontNameRegu);
	char BufBold[LF_FACESIZE + 32]; ::sprintf(BufBold, "%s=COURBD.TTF", m_FontNameBold);

	// on ajoute les fontes
	PDF_set_parameter(m_pPDF, "FontOutline", BufRegu);
	PDF_set_parameter(m_pPDF, "FontOutline", BufBold);

	// on définit les encodages
	PDF_set_parameter(m_pPDF, "Encoding", "cp1250=cp1250.cpg");
	PDF_set_parameter(m_pPDF, "Encoding", "cp1251=cp1251.cpg");

	// on initialise le flag du Path
	m_Path = false;
}

//=================================================================================
// la fonction pour récupérer les fonts
//=================================================================================
int CALLBACK JFCPDFDraw::EnumFoncProc(const LOGFONT *lpelfe, const TEXTMETRIC *lpntme, unsigned long FontType, LPARAM lParam)
{
	// on récupère l'instance
	JFCPDFDraw* pDraw = (JFCPDFDraw*)lParam;

	// on récupère les informations de la fonte
	ENUMLOGFONTEX* pInfos = (ENUMLOGFONTEX*)lpelfe;

	// on teste le type de la fonte
	if      (pInfos->elfLogFont.lfWeight == FW_REGULAR && pInfos->elfLogFont.lfItalic == FALSE) strcpy(pDraw->m_FontNameRegu, (const char*)pInfos->elfFullName);
	else if (pInfos->elfLogFont.lfWeight == FW_BOLD    && pInfos->elfLogFont.lfItalic == FALSE) strcpy(pDraw->m_FontNameBold, (const char*)pInfos->elfFullName);

	// on continue
	return (1);
}

//=================================================================================
// la fonction pour rechercher les fontes
//=================================================================================

bool JFCPDFDraw::SearchFontNames()
{
	LOGFONT LogFont;

	// on initialise les fontes
	m_FontNameRegu[0] = 0;
	m_FontNameBold[0] = 0;

	// on cherche la fonte Courier New
	//strcpy(LogFont.lfFaceName, "Courier New");
	strcpy(LogFont.lfFaceName, "Arial");

	// on définit les caractéristiques de la fonte
	LogFont.lfPitchAndFamily = DEFAULT_PITCH | FF_DONTCARE;
	if      (m_CodePage == 1250) LogFont.lfCharSet = EASTEUROPE_CHARSET;
	else if (m_CodePage == 1251) LogFont.lfCharSet = RUSSIAN_CHARSET;
	else                         LogFont.lfCharSet = ANSI_CHARSET;

	// on récupère le DC du bureau
	HDC hDC = ::GetWindowDC(::GetDesktopWindow());

	// on recherche les fontes
	::EnumFontFamiliesEx(hDC, &LogFont, JFCPDFDraw::EnumFoncProc, (long)this, 0);

	// on libère le DC du bureau
	::ReleaseDC(::GetDesktopWindow(), hDC);

	// on teste la validité des fontes
	return (m_FontNameRegu[0] != 0 && m_FontNameBold[0] != 0);
}

//=================================================================================
// la fonction pour déplacer le curseur:
//=================================================================================
void JFCPDFDraw::MoveTo(long posx, long posy)
{
	// on met à jour le curseur
	m_CurseurX = posx + m_OrgX; m_CurseurY = m_TailleY - (posy + m_OrgY);

	if (m_Path)
	{
		// on lance la commande pdf dans le cas d'un path
		PDF_moveto(m_pPDF, (float)m_CurseurX, (float)m_CurseurY);
	}
}

//=========================================================================
// la fonction pour tracer une ligne à partir du curseur:
//=========================================================================
void JFCPDFDraw::LineTo(long posx, long posy)
{
	// on met à jour les positions par rapport à l'origine
	posx += m_OrgX; posy += m_OrgY;

	if (!m_Path)
	{
		// on calcule les couleurs
		float red   = (float)(m_Color & 0x0000FF) / (float)0x0000FF;
		float green = (float)(m_Color & 0x00FF00) / (float)0x00FF00;
		float blue  = (float)(m_Color & 0xFF0000) / (float)0xFF0000;

		// on fixe la couleur
		PDF_setrgbcolor_stroke(m_pPDF, red, green, blue);

		// on se place sur le curseur
		PDF_moveto(m_pPDF, (float)m_CurseurX, (float)m_CurseurY);
	}

	// on trace la ligne depuis le curseur jusqu'à la position
	PDF_lineto(m_pPDF, (float)posx, (float)(m_TailleY-posy));

	// on met à jour le curseur
	m_CurseurX = posx; m_CurseurY = m_TailleY - posy;

	if (!m_Path)
	{
		PDF_stroke(m_pPDF);
	}
}

//=========================================================================
// la fonction pour écrire du texte:
//=========================================================================
void JFCPDFDraw::Text(const char * texte, Align halign, Align valign)
{
	// on récupère la longeur du texte
	long length = 0; const char * txt = texte; while(*txt++) length+=1;

	long fontsize = (long)PDF_get_value(m_pPDF, "fontsize", 0);
	long width    = length*fontsize*600/1000;

	// on teste l'alignement horizontal
	switch(halign)
	{
	case RIGHT:
		PDF_set_text_pos(m_pPDF, (float)(m_CurseurX-width), (float)(m_CurseurY-(fontsize/2)));
		break;
	case CENTER:
		PDF_set_text_pos(m_pPDF, (float)(m_CurseurX-(width/2)), (float)(m_CurseurY-(fontsize/2)));
		break;
	case LEFT:
		PDF_set_text_pos(m_pPDF, (float)m_CurseurX, (float)(m_CurseurY-(fontsize/2)));
		break;
	}

	// on calcule les couleurs
	float red   = (float)(m_Color & 0x0000FF) / (float)0x0000FF;
	float green = (float)(m_Color & 0x00FF00) / (float)0x00FF00;
	float blue  = (float)(m_Color & 0xFF0000) / (float)0xFF0000;

	// on sélectionne la couleur courante
	PDF_setrgbcolor_fill(m_pPDF, red, green, blue);

	// on enregistre le texte
	PDF_show(m_pPDF, texte);
}

//=========================================================================
// les fonctions de création d'un chemin:
//=========================================================================
void JFCPDFDraw::BeginPath()
{
	// on calcule les couleurs
	float red   = (float)(m_Color & 0x0000FF) / (float)0x0000FF;
	float green = (float)(m_Color & 0x00FF00) / (float)0x00FF00;
	float blue  = (float)(m_Color & 0xFF0000) / (float)0xFF0000;

	// on modifie la couleur si on est dans un path
	PDF_setrgbcolor_stroke(m_pPDF, red, green, blue);

	// on calcule les couleurs
	red   = (float)(m_FillColor & 0x0000FF) / (float)0x0000FF;
	green = (float)(m_FillColor & 0x00FF00) / (float)0x00FF00;
	blue  = (float)(m_FillColor & 0xFF0000) / (float)0xFF0000;

	// on modifie la couleur de remplissage
	PDF_setrgbcolor_fill(m_pPDF, red, green, blue);

	// on signale le commencement d'un chemin
	m_Path = true;
}

void JFCPDFDraw::EndPath()
{
	// on ferme le chemin et on remplit
	PDF_fill_stroke(m_pPDF);

	// on signale la fin du path
	m_Path = false;
}

//=========================================================================
// les fonction pour initialiser les variables d'état:
//=========================================================================
// la couleur courante
void JFCPDFDraw::SetColor(long color)
{
	// recuperation de la couleur du nuancier
	color = GestionCouleurs::GetColor(color);

	// on modifie la couleur de dessin
	m_Color = color;
}

// la couleur de remplissage courante
void JFCPDFDraw::SetFillColor(long color)
{
	// recuperation de la couleur du nuancier
	color = GestionCouleurs::GetColor(color);

	// on modifie la couleur de remplissage
	m_FillColor = color;
}

// la fonte en cours
void JFCPDFDraw::SetFont(long font)
{
	FontManager newfont = FontManager::GetFont(font);

	// on récupère le code de page
	CString codepage;
	if      (m_CodePage == 1250) codepage.Format("cp%d", m_CodePage);
	else if (m_CodePage == 1251) codepage.Format("cp%d", m_CodePage);
	else                         codepage = "winansi";

	// on cherche la fonte demandée en fonction du gras
	if (newfont.IsBold() == FALSE)
	{
		// normal
		m_hFont = PDF_findfont(m_pPDF, m_FontNameRegu, (const char*)codepage, FontManager::GetEmbedding());

		// on modifie la fonte
		PDF_setfont(m_pPDF, m_hFont, (float)(newfont.GetFontSize()/0.283465));
	}
	else
	{
		//gras
		m_hFont = PDF_findfont(m_pPDF, m_FontNameBold, (const char*)codepage, FontManager::GetEmbedding());

		// on modifie la fonte
		PDF_setfont(m_pPDF, m_hFont, (float)(newfont.GetFontSize()/0.283465));
	}
}

// la largeur de trait courante
void JFCPDFDraw::SetLineWidth(long linewidth)
{
	// on modifie la largeur de la ligne
	PDF_setlinewidth(m_pPDF, (float)linewidth);
}

//=========================================================================
// les fonctions de balisage de la page:
//=========================================================================
void JFCPDFDraw::StartPage()
{
	if (m_TailleX < m_TailleY)
	{
		// on commence une page
		PDF_begin_page(m_pPDF, a4_width, a4_height);

		// on applique l'échelle
		PDF_scale(m_pPDF, (float)((double)a4_width / m_TailleX), (float)((double)a4_height / m_TailleY));
		PDF_setlinecap(m_pPDF, 2);
	}
	else
	{
		// on commence une page
		PDF_begin_page(m_pPDF, a4_height, a4_width);

		// on applique l'échelle
		PDF_scale(m_pPDF, (float)((double)a4_height / m_TailleX), (float)((double)a4_width / m_TailleY));
		PDF_setlinecap(m_pPDF, 2);
	}
}

void JFCPDFDraw::EndPage()
{
	// on termine une page
	PDF_end_page(m_pPDF);
}

//==========================================================================
// la fonction pour dessiner un bmp:
//==========================================================================
void JFCPDFDraw::SetPicture(const char * filename, long Xmax, long Ymax)
{
	// on récupère le nom du logo
	CString filejpg = "./" + CString(filename);
	filejpg.Replace(".bmp", ".jpg");

	// on ouvre l'image et on récupère son handle
	long hImage = PDF_open_image_file(m_pPDF, "jpeg", (const char*)filejpg, "", 0);
	if (hImage == -1) return;

	// on récupère les dimensions
	float width  = PDF_get_value(m_pPDF, "imagewidth",  (float)hImage);
	float height = PDF_get_value(m_pPDF, "imageheight", (float)hImage);

	// on calcule l'echelle
	float scale = (((float)Ymax/height) < ((float)Xmax/width)) ? ((float)Ymax/height) : ((float)Xmax/width);

	// on calcule le déplacement à ajouter au curseur
	m_CurseurX += (long) ((Xmax - width * scale) / 2.0);
	m_CurseurY += (long) (((Ymax - height * scale) / 2.0) - Ymax);

	// on place l'image
	if (hImage!=-1) PDF_place_image(m_pPDF, hImage, (float) m_CurseurX, (float) m_CurseurY, scale);
}

//==========================================================================
// le destructeur:
//==========================================================================
JFCPDFDraw::~JFCPDFDraw()
{
	// on ferme le fichier PDF
	PDF_close (m_pPDF);
	PDF_delete(m_pPDF);
}
