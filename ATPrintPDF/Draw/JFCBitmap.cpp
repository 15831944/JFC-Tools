// JFCBitmap.cpp: implementation of the JFCBitmap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JFCBitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma warning(disable:4101)   // unreferenced local variable

JFCBitmap::JFCBitmap()
{
	// on initialise l'entete du ficher
	m_pHeader = NULL;

	// on initialise les infos sur la bitmap
	m_pInfos = NULL;

	// on initialise les donnees de la bitmap
	m_pBits = NULL;

	// on initialise le nombre d'entree dans la lut
	m_SizeLut = 0;
}

DWORD JFCBitmap::_GetInfosSize ( void )
{
	DWORD size;

	// on calcule la taille des infos
	size = m_pHeader->bfOffBits - sizeof(BITMAPFILEHEADER);

	// on renvoie la taille
	return size;
}

DWORD JFCBitmap::_GetBitsSize ( void )
{
	DWORD size;

	// on recupere le nombre d'octets par ligne de balayage
	size = ( ( m_pInfos->bmiHeader.biWidth * m_pInfos->bmiHeader.biBitCount ) + 7 ) / 8;

	// on arrondit cette quantite sur un multiple de 4
	size = ( ( size + 3 ) / 4 ) * 4;

	// on multiplie par le nombre de ligne
	size *= m_pInfos->bmiHeader.biHeight;

	// on renvoie la taille
	return size;
}

BOOL JFCBitmap::_ReadHeader( CFile &file )
{
	DWORD size;

	// on alloue un bloc pour l'entete
	m_pHeader = (BITMAPFILEHEADER*)malloc( sizeof(BITMAPFILEHEADER) );
	if ( m_pHeader == NULL )
		return FALSE;

	// on teste les exceptions
	try
	{
		// on se place en debut du fichier
		file.Seek( 0, CFile::begin );

		// on lit l'en-tete du fichier
		size = file.Read( (VOID*)m_pHeader, sizeof(BITMAPFILEHEADER) );
	}
	catch ( CFileException* exception )
	{
		// on quitte
		return FALSE;
	}

	// on controle qu'on a lu la bonne quantite de donnees
	if ( size != sizeof(BITMAPFILEHEADER) )
		return FALSE;

	// on controle le type de l'entete
	if ( m_pHeader->bfType != 19778 )
		return FALSE;

	// note : le champs bfSize sera controle a la lecture des donnnees
	
	// on controle que les champs reserves sont bien nuls
	if ( m_pHeader->bfReserved1 != 0 || m_pHeader->bfReserved2 != 0 )
		return FALSE;

	// on controle l'index vers le debut des donnees
	if ( m_pHeader->bfOffBits < ( sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) ) )
		return FALSE;

	// on quitte
	return TRUE;
}

BOOL JFCBitmap::_ReadInfos( CFile &file )
{
	DWORD nbre;
	DWORD size;
	DWORD max;

	// on recupere la taille des infos
	nbre = _GetInfosSize();

	// on alloue un bloc pour les infos
	m_pInfos = (BITMAPINFO*)malloc( nbre );
	if ( m_pInfos == NULL )
		return FALSE;

	// on teste les exceptions
	try
	{
		// on se place en debut des infos
		file.Seek( sizeof(BITMAPFILEHEADER), CFile::begin );

		// on lit les infos
		size = file.Read( (VOID*)m_pInfos, nbre );
	}
	catch ( CFileException* exception )
	{
		// on quitte
		return FALSE;
	}

	// on controle qu'on a lu la bonne quantite de donnees
	if ( size != nbre )
		return FALSE;

	// on controle la taille des infos
	if ( m_pInfos->bmiHeader.biSize != sizeof(BITMAPINFOHEADER) )
		return FALSE;

	// on controle la taille de l'image
	if ( m_pInfos->bmiHeader.biWidth == 0 || m_pInfos->bmiHeader.biHeight == 0 )
		return FALSE;

	// on controle le nombre de plan memoire
	if ( m_pInfos->bmiHeader.biPlanes != 1 )
		return FALSE;

	// on calcule le nombre maximum d'entree dans la palette
	switch ( m_pInfos->bmiHeader.biBitCount )
	{
	case 1:
		max =   2;
		break;
	case 4:
		max =  16;
		break;
	case 8:
		max = 256;
		break;
	case 16:
		max =   0;
		break;
	case 24:
		max =   0;
		break;
	default:
		return FALSE;
	}

	// on verifie que l'image n'est pas compressee
	if ( m_pInfos->bmiHeader.biCompression != 0 )
		return FALSE;

	// note : le champs bfSizeImage sera controle a la lecture des donnnees

	// on controle le nombre de couleurs utilisees
	if ( m_pInfos->bmiHeader.biClrUsed > max )
		return FALSE;

	// on controle le nombre de couleurs importantes
	if ( m_pInfos->bmiHeader.biClrImportant > m_pInfos->bmiHeader.biClrUsed )
		return FALSE;

	// on modifie le nombre d'entrees dans la lut
	if ( m_pInfos->bmiHeader.biClrUsed == 0 )
		m_SizeLut = max;
	else
		m_SizeLut = m_pInfos->bmiHeader.biClrUsed;

	// on quitte
	return TRUE;
}

BOOL JFCBitmap::_ReadBits( CFile &file )
{
	DWORD nbre;
	DWORD size;

	// on recupere la taille des donnees
	nbre = _GetBitsSize();

	// on controle l'index vers le debut des donnees
	if ( m_pHeader->bfOffBits < ( sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + m_SizeLut * sizeof(RGBQUAD) ) )
		return FALSE;

	// on controle la taille du fichier
	if ( m_pHeader->bfSize != m_pHeader->bfOffBits + nbre )
		return FALSE;

	// on controle la taille de l'image
	if ( m_pInfos->bmiHeader.biSizeImage != 0 && m_pInfos->bmiHeader.biSizeImage != nbre )
		return FALSE;

	// on alloue un bloc pour les donnees
	m_pBits = malloc( nbre );
	if ( m_pBits == NULL )
		return FALSE;

	// on teste les exceptions
	try
	{
		// on se place en debut des donnees
		file.Seek( m_pHeader->bfOffBits, CFile::begin );

		// on lit les donnees
		size = file.Read( (VOID*)m_pBits, nbre );
	}
	catch ( CFileException* exception )
	{
		// on quitte
		return FALSE;
	}

	// on controle qu'on a lu la bonne quantite de donnees
	if ( size != nbre )
		return FALSE;

	// on quitte
	return TRUE;
}

HBITMAP JFCBitmap::_CreateBitmap( HDC hdc )
{
	HBITMAP bitmap;
	int     caps;

	// on teste le contexte de peripherique
	if ( hdc == NULL )
		return NULL;

	// on recupere les capacites raster du peripherique
	caps = ::GetDeviceCaps( hdc, RASTERCAPS );
	if ( caps == 0 )
		return NULL;

	// on cree la bitmap
	bitmap = ::CreateDIBitmap( hdc, &(m_pInfos->bmiHeader), CBM_INIT, m_pBits, m_pInfos, DIB_RGB_COLORS );

	// on renvoie la bitmap
	return bitmap;
}

HPALETTE JFCBitmap::_CreatePalette( void )
{
	LOGPALETTE* pal;
	HPALETTE    palette;
	DWORD       i;

	// on verifie que la bitmap contient une lut
	if ( m_SizeLut == 0 )
		return NULL;

	// on alloue un bloc pour la palette
	pal = (LOGPALETTE*)malloc( sizeof(LOGPALETTE) + ( m_SizeLut - 1 ) * sizeof(PALETTEENTRY) );
	if ( pal == NULL )
		return NULL;

	// on initialise la palette
	pal->palVersion    = 0x300;
	pal->palNumEntries = (WORD)m_SizeLut;
	for ( i=0; i<m_SizeLut; i+=1 )
	{
		pal->palPalEntry[i].peRed   = m_pInfos->bmiColors[i].rgbRed;
		pal->palPalEntry[i].peGreen = m_pInfos->bmiColors[i].rgbGreen;
		pal->palPalEntry[i].peBlue  = m_pInfos->bmiColors[i].rgbBlue;
		pal->palPalEntry[i].peFlags = 0;
	}

	// on cree la palette
	palette = ::CreatePalette( pal );

	// on libere le bloc de la palette
	free( (VOID*)pal );

	// on renvoie la palette
	return palette;
}

BOOL JFCBitmap::_WriteHeader( CFile &file )
{
	// on teste les exceptions
	try
	{
		// on change la taille du fichier
		file.SetLength( m_pHeader->bfSize );

		// on se place en debut du fichier
		file.Seek( 0, CFile::begin );

		// on ecrit l'en-tete du fichier
		file.Write( (VOID*)m_pHeader, sizeof(BITMAPFILEHEADER) );
	}
	catch ( CFileException* exception )
	{
		// on quitte
		return FALSE;
	}

	// on quitte
	return TRUE;
}

BOOL JFCBitmap::_WriteInfos( CFile &file )
{
	DWORD nbre;

	// on recupere la taille des infos
	nbre = _GetInfosSize();

	// on teste les exceptions
	try
	{
		// on se place en debut des infos
		file.Seek( sizeof(BITMAPFILEHEADER), CFile::begin );

		// on ecrit les infos
		file.Write( (VOID*)m_pInfos, nbre );
	}
	catch ( CFileException* exception )
	{
		// on quitte
		return FALSE;
	}

	// on quitte
	return TRUE;
}

BOOL JFCBitmap::_WriteBits( CFile &file )
{
	DWORD nbre;

	// on recupere la taille des donnees
	nbre = _GetBitsSize();

	// on teste les exceptions
	try
	{
		// on se place en debut des donnees
		file.Seek( m_pHeader->bfOffBits, CFile::begin );

		// on ecrit les donnees
		file.Write( (VOID*)m_pBits, nbre );
	}
	catch ( CFileException* exception )
	{
		// on quitte
		return FALSE;
	}

	// on quitte
	return TRUE;
}

void JFCBitmap::_Delete( BITMAPFILEHEADER* &header, BITMAPINFO* &infos, VOID* &bits, DWORD &sizelut )
{
	// on teste si on doit liberer le bloc de l'entete
	if ( header != NULL )
	{
		// on libere le bloc
		free ( (VOID*)header );

		// on reinitialise le pointeur
		header = NULL;
	}

	// on teste si on doit liberer le bloc des infos
	if ( infos != NULL )
	{
		// on libere le bloc
		free ( (VOID*)infos );

		// on reinitialise le pointeur
		infos = NULL;
	}

	// on teste si on doit liberer le bloc des donnees
	if ( bits != NULL )
	{
		// on libere le bloc
		free ( (VOID*)bits );

		// on reinitialise le pointeur
		bits = NULL;
	}

	// on reinitialise la taille de la lut
	sizelut = 0;

	// on quitte
	return;
}

BOOL JFCBitmap::IsValid( void )
{
	// on teste si une bitmap est actuellement en memoire
	if ( m_pHeader == NULL )
		return FALSE;
	else
		return TRUE;
}

BOOL JFCBitmap::Load( LPCTSTR nom )
{
	BITMAPFILEHEADER* header;
	BITMAPINFO*       infos;
	VOID*             bits;
	DWORD             sizelut;
	CFile             file;
	BOOL              suc;

	// on ouvre le fichier
	suc = file.Open( nom, CFile::modeRead );
	if ( suc == FALSE )
		return FALSE;

	// on sauvegarde l'etat actuel
	header  = m_pHeader;
	infos   = m_pInfos;
	bits    = m_pBits;
	sizelut = m_SizeLut;

	// on reinitialise les pointeurs
	m_pHeader = NULL;
	m_pInfos  = NULL;
	m_pBits   = NULL;
	m_SizeLut = 0;

	// on lit l'entete du fichier
	suc = _ReadHeader( file );
	if ( suc == TRUE )
	{
		// on lit les infos de la bitmap
		suc = _ReadInfos( file );
		if ( suc == TRUE )
		{
			// on lit les donnees de la bitmap
			suc = _ReadBits( file );
		}
	}

	// on teste les exceptions
	try
	{
		// on ferme le fichier
		file.Close();
	}
	catch ( CFileException* exception )
	{
		// modifie l'indicateur de reussite
		suc = FALSE;
	}

	// on teste si le chargement s'est bien passe
	if ( suc == FALSE )
	{
		// on libere tous les blocs
		_Delete( m_pHeader, m_pInfos, m_pBits, m_SizeLut );

		// on restaure l'etat
		m_pHeader  = header;
		m_pInfos   = infos;
		m_pBits    = bits;
		m_SizeLut  = sizelut;

		// on quitte
		return FALSE;
	}

	// on libere les anciens blocs
	_Delete( header, infos, bits, sizelut );

	// on quitte
	return TRUE;
}

void JFCBitmap::GetSize( DWORD* larg, DWORD* haut )
{
	// on teste si une bitmap est bien en memoire
	if ( m_pHeader != NULL )
	{
		// on renseigne les dimensions de la bitmap
		*larg = m_pInfos->bmiHeader.biWidth;
		*haut = m_pInfos->bmiHeader.biHeight;
	}
	else
	{
		// on met les dimensions a zero
		*larg = 0;
		*haut = 0;
	}

	// on quitte
	return;
}

void JFCBitmap::GetInfos( DWORD* format, DWORD* resX, DWORD* resY )
{
	// on teste si une bitmap est bien en memoire
	if ( m_pHeader != NULL )
	{
		// on renseigne le format
		*format = m_pInfos->bmiHeader.biBitCount;

		// on renseigne les resolutions
		*resX = m_pInfos->bmiHeader.biXPelsPerMeter;
		*resY = m_pInfos->bmiHeader.biYPelsPerMeter;
	}
	else
	{
		// on met le format a zero
		*format = 0;

		// on met les resolutions a zero
		*resX = 0;
		*resY = 0;
	}

	// on quitte
	return;
}

void* JFCBitmap::GetBits( void )
{
	// on teste si une bitmap est bien en memoire
	if ( m_pHeader != NULL )
	{
		// on renvoie les données
		return (m_pBits);
	}
	else
	{
		// on renvoie un pointeur nul
		return (NULL);
	}	
}

long JFCBitmap::GetBitsSize()
{
	// on teste si une bitmap est bien en memoire
	if (m_pHeader != NULL)
	{
		// on renvoie la taille des données
		return ((long) _GetBitsSize());
	}
	else
	{
		// on renvoie une taille nulle
		return (0);
	}
}

HBITMAP JFCBitmap::CreateBitmap( HDC hdc )
{
	HBITMAP bitmap;

	// on teste si une bitmap est bien en memoire
	if ( m_pHeader == NULL )
		return FALSE;

	// on cree la bitmap
	bitmap = _CreateBitmap ( hdc );

	// on renvoie la bitmap
	return bitmap;
}

HPALETTE JFCBitmap::CreatePalette( void )
{
	HPALETTE palette;

	// on teste si une bitmap est bien en memoire
	if ( m_pHeader == NULL )
		return FALSE;

	// on cree la palette
	palette = _CreatePalette();

	// on renvoie la palette
	return palette;
}

BOOL JFCBitmap::Save( LPCTSTR nom )
{
	CFile file;
	BOOL  suc;

	// on teste si une bitmap est bien en memoire
	if ( m_pHeader == NULL )
		return FALSE;

	// on cree le fichier
	suc = file.Open( nom, CFile::modeCreate | CFile::modeWrite );
	if ( suc == FALSE )
		return FALSE;

	// on ecrit l'entete du fichier
	suc = _WriteHeader( file );
	if ( suc == TRUE )
	{
		// on ecrit les infos de la bitmap
		suc = _WriteInfos( file );
		if ( suc == TRUE )
		{
			// on ecrit les donnees de la bitmap
			suc = _WriteBits( file );
		}
	}

	// on teste les exceptions
	try
	{
		// on ferme le fichier
		file.Close();
	}
	catch ( CFileException* exception )
	{
		// modifie l'indicateur de reussite
		suc = FALSE;
	}

	// on teste si l'operation s'est bien passee
	if ( suc == FALSE )
	{
		// on detruit le fichier
		file.Abort();

		// on quitte
		return FALSE;
	}

	// on quitte
	return TRUE;
}

void JFCBitmap::Delete( void )
{
	// on efface la bitmap
	_Delete( m_pHeader, m_pInfos, m_pBits, m_SizeLut );

	// on quitte
	return;
}

JFCBitmap::~JFCBitmap()
{
	// on efface la bitmap
	_Delete( m_pHeader, m_pInfos, m_pBits, m_SizeLut );
}

#pragma warning(default:4101)   // unreferenced local variable
