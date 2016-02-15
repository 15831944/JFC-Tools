// LiaisonChaineHertzienneThematique.h: interface for the CLiaisonChaineTeleFrance class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIAISONCHAINEHERTZIENNETHEMATIQUE_H__B7269720_DB2C_4EB7_880F_DCD6FB1BCC2D__INCLUDED_)
#define AFX_LIAISONCHAINEHERTZIENNETHEMATIQUE_H__B7269720_DB2C_4EB7_880F_DCD6FB1BCC2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//##ModelId=3D9AB7D50372
class CLiaisonChaineTeleFrance  
{
public:
	short LoadFile(LPCSTR NomFic);

	bool IsChaineHertzienne(short NrStationMediadata);
	bool IsChaineThematique(short NrStationMediadata);


	//##ModelId=3D9AB7D50386
	short GetNumeroMediadata(short NrStationMadiacabsat);

	short GetNumeroMediacabsat(short NrStationMediadata);

	//##ModelId=3D9AB7D50388
	CLiaisonChaineTeleFrance();
	//##ModelId=3D9AB7D50389
	virtual ~CLiaisonChaineTeleFrance();
private:
	CMap<short,short,short,short> m_MapStation_MediadataToMediacabsat;
	CMap<short,short,short,short> m_MapStation_MediacabsatToMediadata;

	bool m_fLoaded;
	CString m_Nomfich;

};

#endif // !defined(AFX_LIAISONCHAINEHERTZIENNETHEMATIQUE_H__B7269720_DB2C_4EB7_880F_DCD6FB1BCC2D__INCLUDED_)
