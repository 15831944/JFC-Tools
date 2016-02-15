// FicProba.h: interface for the CFicProba class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FICPROBA_H__2C4E1041_ABFF_11D3_989D_BB09FF00106A__INCLUDED_)
#define AFX_FICPROBA_H__2C4E1041_ABFF_11D3_989D_BB09FF00106A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef DllExport
#define DllExport
#endif

#include "probasupport.h"
#include "EcouteSupport.h"

//##ModelId=3D9AB7D503C2
class DllExport CFicProba  
{
public:
	//##ModelId=3D9AB7D503CC
	virtual	void CalculRatioConsomation(int NbRatio){};

	//##ModelId=3D9AB7D503D8
	int GetNbUser();
	//##ModelId=3D9AB7D503E0
	void SupressOneUser();
	//##ModelId=3D9AB7D503E1
	void AddOneUser();
	//##ModelId=3D9AB7D503E2
	CFicProba();
	//##ModelId=3D9AB7D60002
	virtual ~CFicProba();
	//##ModelId=3D9AB7D60004
	virtual int LoadFile(LPCSTR NomFic,int NbIndiv){return(0);};
	//##ModelId=3D9AB7D6000C
	virtual LPCSTR BindNomFichierUnique(LPCSTR NomFic){return(NULL);};
	//##ModelId=3D9AB7D6000F
	virtual LPCSTR GetNomFichierUnique(){return(NULL);};
	//##ModelId=3D9AB7D60017
	virtual void FreeFile(){};
	//##ModelId=3D9AB7D60019
	virtual CEverReach * GetEcouteEVR(int station){return(NULL);};
	//##ModelId=3D9AB7D60023
	virtual CProbaSupport * GetProbabilite(int station,int jour,int support){return(NULL);};
	//##ModelId=3D9AB7D60034
	virtual CProbaSupport * GetProbaNeutre(int jour,int support){return(NULL);};
	//##ModelId=3D9AB7D60038
	virtual int GetNbIndiv(){return(0);};
	//##ModelId=3D9AB7D6003F
	virtual int GetNbStation(){return(0);};
	//##ModelId=3D9AB7D60041
	virtual int GetNrStation(int idx){return(0);};
	//##ModelId=3D9AB7D6004A
	virtual long GetDateDebut(){return(0);};
	//##ModelId=3D9AB7D6004C
	virtual long GetNbJour(){return(0);};
	virtual CProbaSupport * GetDureeEcoute(int station,int jour,int support){return(NULL);};
	virtual unsigned char    GetRegionsIDV(int individu){return (NULL);};
	virtual CEcouteSupport * GetEcoutesWLY(int station){return(NULL);};
	virtual CEcouteSupport * GetEcoutesMLY(int station){return(NULL);};
	virtual CEcouteSupport* GetEcoutesQH(int station,int jour,int support){return(NULL);};
#ifdef OPTION_NOVA_RUSSE
	virtual unsigned char * GetEcoutesJRS(){return(NULL);};
#endif


private:
	//##ModelId=3D9AB7D60053
	int m_NbUser;
};
#endif // !defined(AFX_FICPROBA_H__2C4E1041_ABFF_11D3_989D_BB09FF00106A__INCLUDED_)
