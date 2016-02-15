// PanelLoader.cpp: implementation of the CPanelLoader class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PanelLoader.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//##ModelId=3D9AB7D50310
CPanelLoader::CPanelLoader()
{

}

//##ModelId=3D9AB7D50311
CPanelLoader::~CPanelLoader()
{
	ASSERT(m_TabFicProba.GetSize()==0);
}

#include "FicAQJ.h"
#include "FicAEJ.h"
//##ModelId=3D9AB7D502FB
CFicProba * CPanelLoader::LoadPanel(LPCSTR Chemin, LPCSTR Vague, int Type,int NbIndiv)
{
	CFicProba * NewPanel;
	switch(Type)
	{
		case 0:
		case 2:
			NewPanel=new CFicAQJ;
		break;
		case 1:
			NewPanel=new CFicAEJ;
		break;
		default: ASSERT(0);
	}

	CString NomFichier,NomVague;
	NomVague.Format("%s\\%s",Chemin,Vague);
	NomFichier=NewPanel->BindNomFichierUnique(NomVague);
	for(int p=0;p<m_TabFicProba.GetSize();p++)
	{
		if(m_TabFicProba[p]->GetNomFichierUnique()==NomFichier)
		{
			// Le panel que tomy cherche à charger l'est déjà.
			// On renseigne donc qu'un nouveau tomy utilise
			// ce panel et on le renvoie à tomy.
			m_TabFicProba[p]->AddOneUser();
			delete NewPanel;
			return(m_TabFicProba[p]);
		}
	}

	// Chargement de la vague.
	NewPanel->LoadFile(NomVague,NbIndiv);

	// On lui indique un client
	NewPanel->AddOneUser();

	// On la range dans le collector
	m_TabFicProba.Add(NewPanel);

	return(NewPanel);
}

// Retourne 1 si le panel est réellement déchargé
// retourne 0 si le panel en encore utilisé
//##ModelId=3D9AB7D502F0
bool CPanelLoader::UnloadPanel(CFicProba *pFicProba)
{
	// On recherche ce panel
	for(int p=0;p<m_TabFicProba.GetSize();p++)
	{
		if(m_TabFicProba[p]==pFicProba)
		{
			m_TabFicProba[p]->SupressOneUser();
			if(m_TabFicProba[p]->GetNbUser()==0)
			{
				m_TabFicProba.RemoveAt(p);
				delete pFicProba;
				return(1); // Le Panel est déchargé
			}
			return(0); // Ce panel est toujours utilisé
		}
	}

	ASSERT(0);
	// Ce n'est pas normal de demander le déchargement d'un panel qui n'est pas chargé.
	// C'est donc qu'il y a un problème quelque part docteur

	return(1); // Le Panel est déchargé ...
}

//##ModelId=3D9AB7D50305
CFicProba * CPanelLoader::LoadPanelSurEchntillonOffreElargie(LPCSTR NomPanelFusion)
{
	// Recherche dans le collector
	for(int p=0;p<m_TabFicProba.GetSize();p++)
	{
		if(m_TabFicProba[p]->GetNomFichierUnique()==NomPanelFusion)
		{
			// Le panel que tomy cherche à charger l'est déjà.
			// On renseigne donc qu'un nouveau tomy utilise
			// ce panel et on le renvoie à tomy.
			m_TabFicProba[p]->AddOneUser();
			return(m_TabFicProba[p]);
		}
	}

	// Ce n'est pas le panel loader qui construit un panel avec un sur-échantillon.
	// La fonction du panelk loader est faire partager les donnée.
	// Donc on renvoie ce message NULL.
	// Le demandeur construit le sur-échantillon et le rangera dans le panel loader
	return(NULL);
}

//##ModelId=3D9AB7D5030E
void CPanelLoader::StoreNewPanel(CFicProba * FicProba)
{
	// On lui indique un client
	FicProba->AddOneUser();

	// On la range dans le collector
	m_TabFicProba.Add(FicProba);
}
