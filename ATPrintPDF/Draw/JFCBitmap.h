// JFCBitmap.h: interface for the JFCBitmap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JFCBITMAP_H__20346634_7288_11D3_BDC1_0020AF139373__INCLUDED_)
#define AFX_JFCBITMAP_H__20346634_7288_11D3_BDC1_0020AF139373__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class JFCBitmap : public CObject  
{
public:
	// le constructeur
	JFCBitmap();

	// la fonction pour savoir si une bitmap est actuellement en memoire
	BOOL IsValid( void );

	// la fonction pour charger une bitmap en memoire a partir d'un fichier
	BOOL Load( LPCTSTR nom );

	// la fonction pour obtenir la taille de la bitmap
	void GetSize( DWORD* larg, DWORD* haut );

	// la fonction pour obtenir des infos sur la bitmap
	void GetInfos( DWORD* format, DWORD* resX, DWORD* resY );
	
	// la fonction pour obtenir les données de la bitmap
	void* GetBits( void );
	
	// la fonction pour obtenir la taille en octets des données de la bitmap
	long GetBitsSize();

	// la fonction pour creer une bitmap compatible avec un DC
	HBITMAP CreateBitmap( HDC hdc );

	// la fonction pour creer une palette permettant de dessiner la bitmap
	HPALETTE CreatePalette( void );

	// la fonction pour enregistrer la bitmap dans un fichier
	BOOL Save( LPCTSTR nom );

	// la fonction pour effacer la bitmap de la memoire
	void Delete( void );

	// le destructeur
	virtual ~JFCBitmap();

protected:
	// l'entete du ficher
	BITMAPFILEHEADER* m_pHeader;

	// les infos sur la bitmap
	BITMAPINFO* m_pInfos;

	// les donnees de la bitmap
	VOID* m_pBits;

	// le nombre d'entree dans la lut
	DWORD m_SizeLut;

	// la fonction pour calculer la taille des infos de l'image
	DWORD _GetInfosSize ( void );

	// la fonction pour calculer la taille des donnees de l'image
	DWORD _GetBitsSize ( void );

	// la fonction pour lire l'entete du fichier
	BOOL _ReadHeader( CFile &file );

	// la fonction pour lire les infos sur la bitmap
	BOOL _ReadInfos( CFile &file );

	// la fonction pour lire les donnees de la bitmap
	BOOL _ReadBits( CFile &file );

	// la fonction pour creer une bitmap compatible avec un DC
	HBITMAP _CreateBitmap( HDC hdc );

	// la fonction pour creer une palette permettant de dessiner la bitmap
	HPALETTE _CreatePalette( void );

	// la fonction pour ecrire l'entete du fichier
	BOOL _WriteHeader( CFile &file );

	// la fonction pour ecrire les infos sur la bitmap
	BOOL _WriteInfos( CFile &file );

	// la fonction pour ecrire les donnees de la bitmap
	BOOL _WriteBits( CFile &file );

	// la fonction pour effacer la bitmap de la memoire
	void _Delete( BITMAPFILEHEADER* &header, BITMAPINFO* &infos, VOID* &bits, DWORD &sizelut );
};

#endif // !defined(AFX_JFCBITMAP_H__20346634_7288_11D3_BDC1_0020AF139373__INCLUDED_)
