//===========================
// fichier: JFCGDIDraw.h
//
// date: 25/07/2001
// auteur: JB
//===========================

// on inclus les fichiers n�cessaires
#include "stdafx.h"
#include "JFCGDIDraw.h"

//=========================================================================
// le constructeur:
//=========================================================================
JFCGDIDraw::JFCGDIDraw(HDC hdc, long orgX, long orgY, long zoom, long offsetX, long offsetY) : JFCDraw(orgX, orgY),
			m_Coord(GetDeviceCaps(hdc, HORZRES), GetDeviceCaps(hdc, VERTRES), GetDeviceCaps(hdc, HORZSIZE)*10, GetDeviceCaps(hdc, VERTSIZE)*10, zoom, offsetX, offsetY)
{
	m_hDC = hdc;

	// on sauvegarde le DC pr�c�dent
	m_idDC = SaveDC(m_hDC);

	// on passe en unite pixel
	SetMapMode(m_hDC, MM_TEXT);

	// on passe en mode transparence
	(::SetBkMode(m_hDC, TRANSPARENT));

	// on cr�e le stylo par d�faut
	m_hPen = (::CreatePen(PS_SOLID, 1, 0x000000));

	// on cr�e le pinceau par d�faut
	m_hBrush = (::CreateSolidBrush(0xFFFFFF));

	// on initialise les parametres du tableau de fonte
	for (int i=0; i<FontManager::m_Nb_Font; i++)
	{
		JFCGDIDraw::m_Tab_Font[i] = NULL;
	}
}

//=========================================================================
// la fonction pour d�placer le curseur:
//=========================================================================
void JFCGDIDraw::MoveTo(long posx, long posy)
{
	// la position pr�c�dente
	POINT oldpos; oldpos.x = m_CurseurX; oldpos.y = m_CurseurY;

	posx += m_OrgX; posy += m_OrgY;

	// on d�place le curseur
	if (MoveToEx(m_hDC, m_Coord.GetUnitPosX(posx), m_Coord.GetUnitPosY(posy), &oldpos))
	{
		// on met � jour le curseur
		m_CurseurX = posx; m_CurseurY = posy;
	}
	else
	{
		// on renvoie une exception
		throw long(0);
	}
}

//=========================================================================
// la fonction pour tracer une ligne � partir du curseur:
//=========================================================================
void JFCGDIDraw::LineTo(long posx, long posy)
{
	posx += m_OrgX; posy += m_OrgY;

	// on teste la ligne
	if (::LineTo(m_hDC, m_Coord.GetUnitPosX(posx), m_Coord.GetUnitPosY(posy)))
	{
		// on met � jour le curseur
		m_CurseurX = posx + m_OrgX; m_CurseurY = posy + m_OrgY;
	}
	else
	{
		// on l�ve une exception
		throw long(0);
	}
}

//=========================================================================
// la fonction pour �crire du texte:
//=========================================================================
void JFCGDIDraw::Text(const char * texte, Align HAlign, Align VAlign)
{
	// la taille du texte
	SIZE TextSize;

	// la position du texte
	long TextPosX = m_Coord.GetUnitPosX(m_CurseurX);
	long TextPosY = m_Coord.GetUnitPosY(m_CurseurY);

	// on r�cup�re la longeur du texte
	long textlength = 0; const char * txt = texte; while(*txt++) textlength+=1;

	// on calcule les dimensions du texte
	(::GetTextExtentPoint32(m_hDC, texte, textlength, &TextSize));

	// on teste l'alignement horizontal
	switch(HAlign)
	{
	case CENTER:
		// on positionne la coordonn�e X au centre du texte
		TextPosX -= TextSize.cx / 2;
		break;
	case RIGHT:
		// on positionne la coordonn�e X � la droite du texte
		TextPosX -= TextSize.cx;
		break;
	case LEFT:
		break;
		// on laisse la coordonn�e comme elle est
	}
	// on teste l'alignement vertical
	switch (VAlign)
	{
	case CENTER:
		// on positionne la coordonn�e Y au centre du texte
		TextPosY -= TextSize.cy / 2;
		break;
	case TOP:
		// on positionne la coordonn�e en haut du texte
		TextPosY -= TextSize.cx;
		break;
	case BOTTOM:
		// on laisse la coordonn�e telle qu'elle est
		break;
	}

	// on �crit le texte
	if (!(::TextOut(m_hDC, TextPosX, TextPosY, texte,  textlength)))
	{
		throw long(0);
	}
}
//=========================================================================
// les fonctions pour cr�er un chemin:
//=========================================================================
void JFCGDIDraw::BeginPath()
{
	// on commence l'enregistrement du chemin
	(::BeginPath(m_hDC));
}

void JFCGDIDraw::EndPath()
{
	// on termine le chemin
	(::EndPath(m_hDC));

	// on dessine le path
	(::FillPath(m_hDC));
}
//=========================================================================
// les fonction pour initialiser les variables d'�tat:
//=========================================================================

// la couleur courante
void JFCGDIDraw::SetColor(long color)
{
	// recuperation de la couleur du nuancier
	color = GestionCouleurs::GetColor(color);
	// on change la couleur du texte
	SetTextColor(m_hDC, color);

	// on prend une structure LOGPEN
	LOGPEN logpen;

	// on r�cup�re les param�tres du crayon courant
	if ((::GetObject(m_hPen, sizeof(LOGPEN), &logpen)) != (sizeof(LOGPEN)))
	{
		throw long(0);
	}

	// on modifie la structure
	logpen.lopnColor = color;

	// on cr�e le nouveau crayon
	HPEN hPen = (::CreatePenIndirect(&logpen));

	// on le s�lectionne
	SelectObject(m_hDC, hPen);

	// on lib�re l'ancien
	if (!DeleteObject(m_hPen))
	{
		throw long(0);
	}

	// on met � jour le handle sur le stylo courant
	m_hPen = hPen;
}

// la couleur de remplissage courante
void JFCGDIDraw::SetFillColor(long color)
{
	// recuperation de la couleur du nuancier
	long composante = (long)GestionCouleurs::GetColor(color);

	// on prend une structure LOGBRUSH
	LOGBRUSH logbrush;

	// on r�cup�re les param�tre du pinceau courant
	if ((::GetObject(m_hBrush, sizeof(LOGBRUSH), &logbrush)) != (sizeof(LOGBRUSH)))
	{
		throw long(0);
	}

	// on modifie la structure
	logbrush.lbColor = composante;

	// on cr�e un pinceau
	HBRUSH hBrush = (::CreateBrushIndirect(&logbrush));

	// on le s�lectionne
	SelectObject(m_hDC, hBrush);

	// on lib�re l'ancien
	if (!DeleteObject(m_hBrush))
	{
		throw long(0);
	}

	// on met � jour le handle sur le pinceau courant
	m_hBrush = hBrush;
}
// ===========================================================
// la fonte en cours
// ===========================================================

void JFCGDIDraw::SetFont(long font)
{

	// on verifie que l'indice du tableau est bien present
	if (font < 0 || font >= FontManager::m_Nb_Font)
	{
		throw long(0);
	}

	// on verifie que la fonte est presente dans le tableau de fontes
	if (m_Tab_Font[font] == NULL)
	{
		// On cr�e la fonte :
		// on recupere la fonte a cr�er
		FontManager newFont =FontManager::GetFont(font);
		// On recupere les parametres du DC
		long ppi = (GetDeviceCaps(m_hDC,VERTRES) * 254) / (GetDeviceCaps(m_hDC,VERTSIZE) * 10);

		// On selectionne le type de caratere
		long charSet;
		if      (m_CodePage == 1252) charSet = ANSI_CHARSET;      // version Francaise
		else if (m_CodePage == 1250) charSet = EASTEUROPE_CHARSET;// version Polonaise
		else if (m_CodePage == 1251) charSet = RUSSIAN_CHARSET;   // version Russe
		else charSet = DEFAULT_CHARSET;

		// on cree la fonte et on la place dans le tableau de fonts
		m_Tab_Font[font] = ::CreateFont(
			/*Height*/-(((MulDiv(newFont.GetFontSize(), ppi, 72) * m_Coord.m_Scale) + 50) / 100),
			/*Widhth*/0,
			/*Escapement*/0,
			/*Orientation*/0,
			/*Weight*/(newFont.IsBold()) ? FW_BOLD : FW_REGULAR,
			/*Italic*/(newFont.IsItalic()) ? TRUE : FALSE,
			/*Underline*/0,
			/*StrikeOut*/0,
			/*Charset*/charSet,
			/*OutPutPrecision*/OUT_TT_PRECIS,
			/*ClipPrecision*/CLIP_DEFAULT_PRECIS,
			/*Quality*/DRAFT_QUALITY,
			/*Pitch&Family*/FF_SWISS|VARIABLE_PITCH,
			/*Face*/newFont.GetFontName());

		if (m_Tab_Font[font] == NULL) throw long(0);
	}

	// On selectionne la font
	if (NULL == ::SelectObject(m_hDC,m_Tab_Font[font])) throw long(0);
}

// la largeur de ligne en cours
void JFCGDIDraw::SetLineWidth(long linewidth)
{
	// on prend une structure LOGPEN
	LOGPEN logpen;

	// on r�cup�re les param�tres du crayon courant
	if ((::GetObject(m_hPen, sizeof(LOGPEN), &logpen)) != (sizeof(LOGPEN)))
	{
		throw long(0);
	}

	// on fixe la largeur
	logpen.lopnWidth.x = m_Coord.GetDimX(linewidth);

	// on cr�e le nouveau crayon
	HPEN hPen = (::CreatePenIndirect(&logpen));

	// on le s�lectionne
	SelectObject(m_hDC, hPen);

	// on lib�re l'ancien
	if (!DeleteObject(m_hPen))
	{
		throw long(0);
	}

	// on met � jour le handle sur le stylo courant
	m_hPen = hPen;
}

//=========================================================================
// les fonctions de balisage de la page:
//=========================================================================
void JFCGDIDraw::StartPage()
{
	// on ne fait rien??
}

void JFCGDIDraw::EndPage()
{
	// on ne fait rien ??
}

//=========================================================================
// la fonction pour dessiner un bmp:
//=========================================================================
void JFCGDIDraw::SetPicture(const char * filename, long WidthMax, long HeightMax)
{
	// on convertit les param�tres de taille en pixel
	long widthmax = m_Coord.GetDimX(WidthMax);
	long heightmax = m_Coord.GetDimY(HeightMax);

	// on cr�e un objet bitmap
	JFCBitmap Bitmap;
	if (!Bitmap.Load(filename)) return;
	if (!Bitmap.IsValid()) return;

	// on r�cup�re les dimensions du bitmap
	DWORD width, height; Bitmap.GetSize(&width, &height);

	// on calcule le rapport � appliquer pour rentrer dans le rectangle
	long Num, Den;
	if ((widthmax*height) > (heightmax*width)) { Num = heightmax; Den = height;}
	else {Num = widthmax; Den = width;}

	// on initialise les anciens objets et les nouveaux aussi
	HBITMAP oldbmp = NULL;
	HBITMAP newbmp = NULL;
	HPALETTE oldpal = NULL;
	HPALETTE newpal = NULL;

	// on cr�e un nouveau bitmap compatible avec le DC d'affichage
	newbmp = Bitmap.CreateBitmap(m_hDC);

	// on teste le nouveau bitmap
	if (newbmp != NULL)
	{
		// on cr�e un DC m�moire
		HDC hdcmem = (::CreateCompatibleDC(m_hDC));

		// on teste le nouveau DC m�moire
		if (hdcmem != NULL)
		{
			// on s�lectionne le bitmap dans le nouveau DC memoire
			oldbmp = (HBITMAP) (::SelectObject(hdcmem, newbmp));

			// on teste si le p�riph�rique a une palette
			long caps = (::GetDeviceCaps(m_hDC, RASTERCAPS));
			if ( (caps & RC_PALETTE) != 0)
			{
				// on cr�e la palette pour afficher la bitmap
				newpal = Bitmap.CreatePalette();

				// on teste la nouvelle palette
				if (newpal != 0)
				{
					// on s�lectionne la palette dans le DC
					oldpal = (HPALETTE) (::SelectPalette(m_hDC, newpal, FALSE));

					// on r�alise la palette
					(::RealizePalette(m_hDC));
				}
			}
			// on calcule le rectangle d'affichage du Bitmap
			RECT rectbmp;
			rectbmp.left = m_Coord.GetUnitPosX(m_CurseurX) + (widthmax - (width * Num / Den))/2;
			rectbmp.right = rectbmp.left + (width * Num / Den);
			rectbmp.top = m_Coord.GetUnitPosY(m_CurseurY) + (heightmax - (height * Num / Den))/2;
			rectbmp.bottom = rectbmp.top + (height * Num / Den);

			// on effectue la copie
			(::StretchBlt(m_hDC, rectbmp.left, rectbmp.top, (rectbmp.right - rectbmp.left), (rectbmp.bottom - rectbmp.top), hdcmem, 0, 0, width, height, SRCCOPY));

			// on teste si l'on doit res�lectionner l'ancienne palette
			if (oldpal != NULL)
			{
				// on s�lectionne l'ancienne palette
				(::SelectPalette(m_hDC, oldpal, FALSE));

				// on r�alise la palette
				(::RealizePalette(m_hDC));

				// on d�truit la palette
				(::DeleteObject(newpal));
			}
			// on res�lectionne l'ancienne bitmap
			(::SelectObject( hdcmem, oldbmp ));

			// on d�truit la bitmap
			(::DeleteObject( newbmp ));

			// on d�truit le DC m�moire
			(::DeleteDC(hdcmem));
		}
	}
}

//=========================================================================
// le destructeur:
//=========================================================================
JFCGDIDraw::~JFCGDIDraw()
{
	// on restore l'ancien dc
	if (!RestoreDC(m_hDC, m_idDC))
	{
		throw long(0);
	}

	// on lib�re les objets
	if (m_hPen != 0 && !DeleteObject(m_hPen))
	{
		throw long (0);
	}
	if (m_hBrush != 0 && !DeleteObject(m_hBrush))
	{
		throw long (0);
	}
}
