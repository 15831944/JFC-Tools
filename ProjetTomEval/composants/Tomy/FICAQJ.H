// FicAQJ.h: interface for the CFixAQS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FICAQJ_H__5E587942_726B_11D3_989D_F2A7B13AF30A__INCLUDED_)
#define AFX_FICAQJ_H__5E587942_726B_11D3_989D_F2A7B13AF30A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "probasupport.h"
#include "EcouteSupport.h"
#include "ficproba.h"

//##ModelId=3D9AB7D60084
class CFicAQJ  : public CFicProba
{
public:
	//##ModelId=3D9AB7D6008F
	void FreeFile();
	//##ModelId=3D9AB7D60090
	int LoadFile(LPCSTR NomFic,int NbIndiv);
	//##ModelId=3D9AB7D60099
	LPCSTR BindNomFichierUnique(LPCSTR NomFic);
	//##ModelId=3D9AB7D600A2
	LPCSTR GetNomFichierUnique(){return(m_NomFic);};

	//##ModelId=3D9AB7D600A3
	CFicAQJ();
	//##ModelId=3D9AB7D600A4
	virtual ~CFicAQJ();

	//##ModelId=3D9AB7D600AC
	long m_DateDebut;
	//##ModelId=3D9AB7D600AD
	short m_NbJour;
	//##ModelId=3D9AB7D600B6
	int m_NbIndiv;
	//##ModelId=3D9AB7D600B7
	short m_NbStation;
	//##ModelId=3D9AB7D600C0
	short * m_NrStation;
	//##ModelId=3D9AB7D600CC
	CProbaSupport *** m_DureeQH; // Durée d'ecoute [Station][Jour DATE][QHeure]
	//##ModelId=3D9AB7D600E0
	CProbaSupport *** m_ProbaQH; // Probabilités [Station][Jour NOMME][QHeure]
	//##ModelId=3D9AB7D600EA
	CProbaSupport ** m_ProbaQHNeutre; // Probabilités [Jour NOMME][QHeure]

#ifdef OPTION_RUSSIE
	unsigned char  *   m_RegionsIDV;  // Regions [Individu]
	CEcouteSupport *   m_EcoutesWLY;  // Ecoutes [Station]
	CEcouteSupport *   m_EcoutesMLY;  // Ecoutes [Station]
	CEcouteSupport *** m_EcoutesQH;   // Ecoutes [Station][JrNomme][QHeure]
	unsigned char    GetRegionsIDV(int individu){return(m_RegionsIDV[individu]);}
	CEcouteSupport * GetEcoutesWLY(int station){return(&m_EcoutesWLY[station]);};
	CEcouteSupport * GetEcoutesMLY(int station){return(&m_EcoutesMLY[station]);};
	CEcouteSupport * GetEcoutesQH (int station,int jour,int support){return(&m_EcoutesQH[station][jour][support]);};
#endif
#ifdef OPTION_NOVA_RUSSE
	unsigned char  *   m_EcoutesJRS;  // [Individu]
	unsigned char * GetEcoutesJRS(){return(m_EcoutesJRS);};
#endif

#ifdef OPTION_DUREEECOUTE
	//##ModelId=3D9AB7D600F3
	CProbaSupport * GetDureeEcoute(int station,int jour,int support){return(&m_DureeQH[station][jour][support]);};
#endif
	//##ModelId=3D9AB7D600FF
	CProbaSupport * GetProbabilite(int station,int jour,int support){return(&m_ProbaQH[station][jour][support]);};
	//##ModelId=3D9AB7D60111
	CProbaSupport * GetProbaNeutre(int jour,int support){return(&m_ProbaQHNeutre[jour][support]);};

	//##ModelId=3D9AB7D6011C
	int GetNbIndiv(){return(m_NbIndiv);};
	//##ModelId=3D9AB7D6011D
	int GetNbStation(){return(m_NbStation);};
	//##ModelId=3D9AB7D60125
	int GetNrStation(int idx){return(m_NrStation[idx]);};
	//##ModelId=3D9AB7D6012F
	long GetDateDebut(){return(m_DateDebut);};
	//##ModelId=3D9AB7D60130
	long GetNbJour(){return(m_NbJour);};

#ifdef OPTION_REACH_BELGE
	CEverReach *m_Ecoute;		// m_NbStation
	CEverReach * GetEcouteEVR(int station){return(&m_Ecoute[station]);};
#endif

private:
	//##ModelId=3D9AB7D60131
	CString m_NomFic;
	//##ModelId=3D9AB7D60139
	bool m_FileLoaded;
};

#endif // !defined(AFX_FICAQJ_H__5E587942_726B_11D3_989D_F2A7B13AF30A__INCLUDED_)
