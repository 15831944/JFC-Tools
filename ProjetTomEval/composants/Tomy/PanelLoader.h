// PanelLoader.h: interface for the CPanelLoader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANELLOADER_H__518D525A_AE82_433A_A87E_2CEC12031CE5__INCLUDED_)
#define AFX_PANELLOADER_H__518D525A_AE82_433A_A87E_2CEC12031CE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef DllExport
#define DllExport
#endif

#include "ficproba.h"   
#include "afxtempl.h"
//##ModelId=3D9AB7D502D2
typedef CArray<CFicProba *,CFicProba *> CTabFicProba;

//##ModelId=3D9AB7D502E6
class  CPanelLoader  
{
public:
	//##ModelId=3D9AB7D502F0
	DllExport bool UnloadPanel(CFicProba * pFicProba);
	//##ModelId=3D9AB7D502FB
	DllExport CFicProba * LoadPanel(LPCSTR Chemin,LPCSTR Vague,int Type,int NbIndiv);
	//##ModelId=3D9AB7D50305
	DllExport CFicProba * LoadPanelSurEchntillonOffreElargie(LPCSTR NomPanelFusion);
	//##ModelId=3D9AB7D5030E
	DllExport void StoreNewPanel(CFicProba * FicProba);

	//##ModelId=3D9AB7D50310
	CPanelLoader();
	//##ModelId=3D9AB7D50311
	virtual ~CPanelLoader();

private:
	//##ModelId=3D9AB7D50319
	CTabFicProba m_TabFicProba;
};

DllExport CPanelLoader *  GetPtrPanelLoader();

#endif // !defined(AFX_PANELLOADER_H__518D525A_AE82_433A_A87E_2CEC12031CE5__INCLUDED_)
