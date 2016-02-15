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

	// la fonction pour récupérer les informations de la bitmap
	const BITMAPINFO* GetBitmapInfos( void );

	// la fonction pour récupérer les données de la bitmap
	const VOID* GetBitmapBits( void );

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
