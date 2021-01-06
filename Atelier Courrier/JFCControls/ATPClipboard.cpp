//
// Fichier: ATPClipboard.cpp
// Date:    18/09/2001
//

// on inclut les d�finitions n�cessaires
#include "stdafx.h"
#include "ATPClipboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
/////////////////////////////////////////////////////////////////////////////
// la fonction pour charger un fichier en m�moire

static HGLOBAL LoadFile(const TCHAR* pFilename)
{
	DWORD   Size;
	DWORD   Verif;
	HANDLE  hFile;
	HGLOBAL hBuffer;
	VOID*   pBuffer;
	BOOL    Succes;

	// on ouvre le fichier
	hFile = CreateFile(pFilename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL); if (hFile == INVALID_HANDLE_VALUE) return (NULL);

	// on r�cup�re la taille du fichier
	Size = GetFileSize(hFile, NULL); if (Size == 0xFFFFFFFF) { CloseHandle(hFile); return (NULL); }

	// on alloue un bloc de m�moire
	hBuffer = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, Size + 1); if (hBuffer == NULL) { CloseHandle(hFile); return (NULL); }

	// on verrouille le bloc de m�moire
	pBuffer = GlobalLock(hBuffer); if (pBuffer == NULL) { GlobalFree(hBuffer); CloseHandle(hFile); return (NULL); };

	// on lit le fichier dans le bloc
	Succes = ReadFile(hFile, pBuffer, Size, &Verif, NULL); if (Succes == FALSE || Verif != Size) { GlobalUnlock(hBuffer); GlobalFree(hBuffer); CloseHandle(hFile); return (NULL); };

	// on positionne le caract�re nul final
	((BYTE*)pBuffer)[Size] = 0;

	// on d�verrouille la m�moire
	GlobalUnlock(hBuffer);

	// on ferme le fichier
	CloseHandle(hFile);

	// on renvoie le buffer
	return (hBuffer);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour remplir le presse papier

static BOOL SetClipData(HWND hWnd, HGLOBAL hBuffer)
{
	// on ouvre le presse papier
	if (FALSE == OpenClipboard(hWnd)) return (FALSE);

	// on vide le presse papier
	if (FALSE == EmptyClipboard()) { CloseClipboard(); return (FALSE); }

	// on place les donn�es dans le presse papier
	if (NULL == SetClipboardData(CF_TEXT, (HANDLE)hBuffer)) { CloseClipboard(); return (FALSE); }

	// on ferme le presse papier
	CloseClipboard();

	// on quitte
	return (TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// la fonction pour copier un fichier dans le presse papier

BOOL CopyFileToClipBrd(HWND hWnd, const TCHAR* pFilename)
{
	BOOL    Succes;
	HGLOBAL hBuffer;

	// on v�rifie la validit� du fichier
	if (pFilename == NULL) return (FALSE);

	// on v�rifie la validit� de la fen�tre
	if (FALSE == IsWindow(hWnd)) return (FALSE);

	// on charge le fichier en m�moire
	hBuffer = LoadFile(pFilename); if (hBuffer == NULL) return (FALSE);

	// on place la m�moire dans le presse papier
	Succes = SetClipData(hWnd, hBuffer); if (Succes == FALSE) { GlobalFree(hBuffer); return (FALSE); }

	// on quitte
	return (TRUE);
}
