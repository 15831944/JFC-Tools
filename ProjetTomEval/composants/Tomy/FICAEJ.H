// FicAEJ.h: interface for the CFicAEJ class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FICAEJ_H__2C4E1040_ABFF_11D3_989D_BB09FF00106A__INCLUDED_)
#define AFX_FICAEJ_H__2C4E1040_ABFF_11D3_989D_BB09FF00106A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "probasupport.h"
#include "ficproba.h"

//##ModelId=3D9AB7D60201
class CFicAEJ  : public CFicProba
{
public:
	CDWordArray m_Consomation;
	//##ModelId=3D9AB7D60216
	CByteArray m_NrRatioConso; // [indiv]
	//##ModelId=3D9AB7D6021F
	void CalculRatioConsomation(int NbRatio);
	//##ModelId=3D9AB7D60221
	void SetNomFichierUnique(LPCSTR Nomfic){m_NomFic=Nomfic;};
	//##ModelId=3D9AB7D6022A
	CFicAEJ();
	//##ModelId=3D9AB7D6022B
	virtual ~CFicAEJ();
	//##ModelId=3D9AB7D60233
	void FreeFile();
	//##ModelId=3D9AB7D60234
	int LoadFile(LPCSTR NomFic);

	//##ModelId=3D9AB7D6023D
	LPCSTR BindNomFichierUnique(LPCSTR NomFic);
	//##ModelId=3D9AB7D6023F
	LPCSTR GetNomFichierUnique(){return(m_NomFic);};

	//##ModelId=3D9AB7D60247
	long m_DateDebut;
	//##ModelId=3D9AB7D60248
	short m_NbJour;
	//##ModelId=3D9AB7D60251
	int m_NbIndiv;
	//##ModelId=3D9AB7D6025B
	short m_NbStation;
	//##ModelId=3D9AB7D60265
	short * m_NrStation;
	//##ModelId=3D9AB7D60266
	short ** m_NbEcran; // [station][jour]
	//##ModelId=3D9AB7D6026F
	short *** m_NrEcran; // [station][jour][Ecran]
	//##ModelId=3D9AB7D60279
	short ** m_NbEcranP; // [station][jour]
	//##ModelId=3D9AB7D6027A
	short *** m_NrEcranP; // [station][jour][EcranP]
	//##ModelId=3D9AB7D60284
	CProbaSupport *** m_DureeQH; // Durée d'ecoute [Station][Jour DATE][Ecran]
	//##ModelId=3D9AB7D6028E
	CProbaSupport *** m_ProbaQH; // Probabilités [Station][Jour NOMME][EcranP]

#ifdef	OPTION_DUREEECOUTE
	//##ModelId=3D9AB7D60297
	CProbaSupport * GetDureeEcoute(int station,int jour,int support);
#endif
	//##ModelId=3D9AB7D602A3
	CProbaSupport * GetProbabilite(int station,int jour,int support);
	//##ModelId=3D9AB7D602B5
	int GetNbIndiv(){return(m_NbIndiv);};
	//##ModelId=3D9AB7D602BF
	int GetNbStation(){return(m_NbStation);};
	//##ModelId=3D9AB7D602C0
	int GetNrStation(int idx){return(m_NrStation[idx]);};
	//##ModelId=3D9AB7D602C9
	long GetDateDebut(){return(m_DateDebut);};
	//##ModelId=3D9AB7D602CA
	long GetNbJour(){return(m_NbJour);};

private:
	//##ModelId=3D9AB7D602CB
	CString m_NomFic;
protected:
	//##ModelId=3D9AB7D602D3
	bool m_FileLoaded;
};

#endif // !defined(AFX_FICAEJ_H__2C4E1040_ABFF_11D3_989D_BB09FF00106A__INCLUDED_)



