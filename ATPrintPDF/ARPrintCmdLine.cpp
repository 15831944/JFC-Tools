// ARPrintCmdLine.cpp: implementation of the CARPrintCmdLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ATPrintPDF.h"
#include "ARPrintCmdLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//====================================================================
// Le constructeur:
//====================================================================
CARPrintCmdLine::CARPrintCmdLine()
{
	// on initialise les éléments
	m_Orientation = 1;
	m_HLanceur    = 0;
	m_NoParam     = 1;
}

//====================================================================
// la fonction pour parser la ligne de commande:
//====================================================================
void CARPrintCmdLine::ParseParam(LPCTSTR lpszParam, BOOL bFlag, BOOL bLast)
{
	// on teste le type du paramètre
	if (bFlag != FALSE)
	{
		// on teste le paramètre
		if (0 == ::strcmp(lpszParam, "P100") || 0 == ::strcmp(lpszParam, "P80"))
		{
			// on modifie l'orientation
			m_Orientation = 1;
		}
		else if (0 == ::strcmp(lpszParam, "P160") || 0 == ::strcmp(lpszParam, "P240"))
		{
			// on modifie l'orientation
			m_Orientation = 2;
		}
	}
	else
	{
		// on récupère le paramètre
		const char* param = (const char*)lpszParam;
		// on teste le paramètre
		switch (m_NoParam)
		{
			case 1:
			{
				long i       = 0;
				long Flag    = 0;
				char tmpchar = 0;
				while((tmpchar = *(param + i)) != 0)
				{
					// on teste que le caractère est un chiffre
					if (tmpchar < '0' || tmpchar > '9') 
					{
						Flag = -1;
						break;
					}
					// on ajoute la valeur
					Flag = (Flag * 10) + (tmpchar - '0');
					// on incrémente le compteur
					i+=1;
				}
				if (Flag == 0)
				{
					m_nShellCommand = CCommandLineInfo::FilePrint;
				}
				else if (Flag == 1 || Flag == 2) 
				{
					m_nShellCommand = CCommandLineInfo::FileOpen;
				}
				else 
				{
					m_nShellCommand = CCommandLineInfo::FileNothing;
				}
				m_NoParam += 1;
			}
			break;
			case 2:
			{
				this->m_strFileName = lpszParam;
				m_NoParam          += 1;
			}
			break;
			case 3:
			{
				long     i   = 0;
				char tmpchar = 0;
				m_HLanceur   = 0;
				while((tmpchar = *(param+i)) != 0)
				{
					// on teste que le caractère est un chiffre
					if (tmpchar < '0' || tmpchar > '9') 
					{
						m_HLanceur = 0;
						m_NoParam += 1;
						return;
					}
					// on ajoute la valeur
					m_HLanceur = (m_HLanceur * 10) + (tmpchar - '0');
					// on incrémente le compteur
					i += 1;
				}
				m_NoParam += 1;
			}
			break;
		default:
			m_NoParam += 1;
			break;
		}
	}
}

//====================================================================
// Le destructeur:
//====================================================================
CARPrintCmdLine::~CARPrintCmdLine()
{

}
