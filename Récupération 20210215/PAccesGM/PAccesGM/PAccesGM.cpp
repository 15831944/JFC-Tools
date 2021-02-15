//
// PAccesGM.cpp
//

// on inclut les définitions nécessaires
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

// on définit la version de Windows
#define _WIN32_WINNT 0x0500

// on inclut les définitions nécessaires
#include "windows.h"

/////////////////////////////////////////
// la fonction pour relancer le programme

static long Restart(WCHAR* pCommandLine)
{
	STARTUPINFOW        Info;
	PROCESS_INFORMATION Process;
	DWORD               Error;

	// on crée les paramètres
	CONST WCHAR* pUsername = L"jfc_edi";
	CONST WCHAR* pDomain   = L"PAR-APP03";
	CONST WCHAR* pPassword = L"Passw0rd";

	// on initialise les informations
	Info.cb              = sizeof(Info);
	Info.lpReserved      = NULL;
	Info.lpDesktop       = NULL;
	Info.lpTitle         = NULL;
	Info.dwX             = 0;
	Info.dwY             = 0;
	Info.dwXSize         = 0;
	Info.dwYSize         = 0;
	Info.dwXCountChars   = 0;
	Info.dwYCountChars   = 0;
	Info.dwFillAttribute = 0;
	Info.dwFlags         = STARTF_USESHOWWINDOW;
	Info.wShowWindow     = SW_HIDE;
	Info.cbReserved2     = 0;
	Info.lpReserved2     = NULL;
	Info.hStdInput       = NULL;
	Info.hStdOutput      = NULL;
	Info.hStdError       = NULL;

	// on crée le processus
	if (FALSE == ::CreateProcessWithLogonW(pUsername,
	                                       pDomain,
	                                       pPassword,
	                                       LOGON_NETCREDENTIALS_ONLY,
	                                       NULL,
	                                       pCommandLine,
	                                       CREATE_DEFAULT_ERROR_MODE,
	                                       NULL,
	                                       NULL,
	                                       &Info,
	                                       &Process)) return (-1);

	// on attend la fin du processus
	if (WAIT_OBJECT_0 != ::WaitForSingleObject(Process.hProcess, 60000))
		return (-1);

	// on récupère le code d'erreur
	if (FALSE == ::GetExitCodeProcess(Process.hProcess, &Error))
		Error = 0xFFFFFFFF;

	// on libère les handles
	::CloseHandle(Process.hProcess);
	::CloseHandle(Process.hThread);

	// on teste l'erreur
	if (Error != 0) return (-1);

	// on quitte
	return (0);
}

/////////////////////////////////
// le point d'entrée du programme

int main(int argc, char* argv[])
{
	// on initialise le code
	int code = 0;

	// on teste le nombre d'arguments
	if (argc < 3)
		return (-1);

	// on récupère le code
	if (1 != ::sscanf_s(argv[1], "%d", &code))
		return (-1);

	// on teste le code
	if (code == 0)
	{
		// on vérifie le nombre de paramètres
		if (argc != 3) return (-1);

		// on calcule la longueur de la ligne de commande
		size_t length = ::strlen(argv[0]) + ::strlen(argv[2]) + 32;

		// on alloue le buffer
		WCHAR* pCommandLine = (WCHAR*)::malloc(sizeof(WCHAR) * length); if (pCommandLine == 0) return (-1);

		// on formate les paramètres
		::swprintf_s(pCommandLine, length, L"\"%S\" 100 \"%S\"", argv[0], argv[2]);

		// on lance le programme
		if (0 != ::Restart(pCommandLine))
		{
			// on libère le buffer
			::free(pCommandLine);

			// on quitte
			return (-1);
		}

		// on libère le buffer
		::free(pCommandLine);
	}
	else if (code == 1)
	{
		// on vérifie le nombre de paramètres
		if (argc != 4) return (-1);

		// on calcule les longueurs des paramètres
		size_t length = ::strlen(argv[0]) + ::strlen(argv[2]) + ::strlen(argv[3]) + 32;

		// on alloue les buffers
		WCHAR* pCommandLine = (WCHAR*)::malloc(sizeof(WCHAR) * length); if (pCommandLine == 0) return (-1);

		// on formate les paramètres
		::swprintf_s(pCommandLine, length, L"\"%S\" 101 \"%S\" \"%S\"", argv[0], argv[2], argv[3]);

		// on lance le programme
		if (0 != ::Restart(pCommandLine))
		{
			// on libère le buffer
			::free(pCommandLine);

			// on quitte
			return (-1);
		}

		// on libère le buffer
		::free(pCommandLine);
	}
	else if (code == 100)
	{
		// on vérifie le nombre de paramètres
		if (argc != 3) return (-1);

		// on récupère les attributs du fichier
		DWORD AttrF = ::GetFileAttributes(argv[2]);

		// on vérifie qu'il s'agit bien d'un fichier
		if (AttrF == INVALID_FILE_ATTRIBUTES || (AttrF & FILE_ATTRIBUTE_DIRECTORY) != 0)
			return (-1);

		// on teste si le fichier est en lecture seule
		if ((AttrF & FILE_ATTRIBUTE_READONLY) != 0)
		{
			// on modifie les attributs du fichier
			if (FALSE == ::SetFileAttributes(argv[2], AttrF & (~FILE_ATTRIBUTE_READONLY)))
				return (-1);
		}

		// on suprime le fichier
		if (FALSE == ::DeleteFile(argv[2]))
			return (-1);
	}
	else if (code == 101)
	{
		// on vérifie le nombre de paramètres
		if (argc != 4) return (-1);

		// on récupère les attributs des fichiers
		DWORD AttrF = ::GetFileAttributes(argv[2]);
		DWORD AttrD = ::GetFileAttributes(argv[3]);

		// on vérifie qu'il s'agit bien d'un fichier
		if (AttrF == INVALID_FILE_ATTRIBUTES || (AttrF & FILE_ATTRIBUTE_DIRECTORY) != 0)
			return (-1);

		// on teste l'existence du fichier de destination
		if (AttrD != INVALID_FILE_ATTRIBUTES)
		{
			// on vérifie qu'il s'agit bien d'un fichier
			if ((AttrD & FILE_ATTRIBUTE_DIRECTORY) != 0)
				return (-1);

			// on teste si le fichier est en lecture seule
			if ((AttrD & FILE_ATTRIBUTE_READONLY) != 0)
			{
				// on modifie les attributs du fichier
				if (FALSE == ::SetFileAttributes(argv[3], AttrD & (~FILE_ATTRIBUTE_READONLY)))
					return (-1);
			}

			// on suprime le fichier
			if (FALSE == ::DeleteFile(argv[3]))
				return (-1);
		}

		// on teste si le fichier est en lecture seule
		if ((AttrF & FILE_ATTRIBUTE_READONLY) != 0)
		{
			// on modifie les attributs du fichier
			if (FALSE == ::SetFileAttributes(argv[2], AttrF & (~FILE_ATTRIBUTE_READONLY)))
				return (-1);
		}

		// on suprime le fichier
		if (FALSE == ::MoveFile(argv[2], argv[3]))
			return (-1);
	}

	// on quitte
	return (0);
}
