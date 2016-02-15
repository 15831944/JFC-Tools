// PageInfo.h: interface for the CPageInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PAGEINFO_H__B1655DA2_0409_4A93_A0E6_9082F2D2A150__INCLUDED_)
#define AFX_PAGEINFO_H__B1655DA2_0409_4A93_A0E6_9082F2D2A150__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Draw/JFCDecimilToUnit.h"
#include "./Composite/JFCDrawDocument.h"

class CPageInfo  
{
public:
	// le constructeur
	CPageInfo(long HorzRes, long VertRes, long HorzSize, long VertSize);

	// les fonctions pour fixer la taille de la page
	void SetPageWidth(long pagewidth);
	void SetPageHeight(long pageheight);

	// les fonctions pour g�rer la page en cours
	void SetCurrentPage(long numpage);
	long GetCurrentPage();
	long GetNbPage();

	// la fonction pour g�rer le zoom
	void SetZoom(long zoom);
	long GetZoom();

	// les fonctions pour r�cup�rer les tailles logiques
	long GetSizeX_mm();
	long GetSizeX_px();
	long GetSizeY_mm();
	long GetSizeY_px();

	// les fonctions pour g�rer l'origine de la zone � dessiner
	long GetOrgX_mm();
	long GetOrgX_px();
	long GetOrgY_mm();
	long GetOrgY_px();
	void SetOrgX_mm(long orgX);
	void SetOrgX_px(long orgX);
	void SetOrgY_mm(long orgY);
	void SetOrgY_px(long orgY);

	// les fonctions pour r�cup�rer les marges
	void GetMarges_mm(long & margeTop, long & margeBottom, long & margeLeft, long & margeRight);
	void GetMarges_px(long & margeTop, long & margeBottom, long & margeLeft, long & margeRight);
	
	// la fonction pour r�cup�rer le document
	DrawDocument * GetDrawDocument();
	void SetDrawDocument(DrawDocument ** pDrawDoc);

	// le destructeur
	virtual ~CPageInfo();

protected:
	
	long m_PageWidth;			// la largeur de la fen�tre
	long m_PageHeight;			// la hauteur de la fen�tre
	long m_NumPage;				// la page en cours
	long m_OrgX;				// l'origine de la fen�tre d'impression
	long m_OrgY;
	long m_Zoom;				// le zoom
	
	DrawDocument ** m_pDrawDoc;	// le pointeur sur le document

private:
	// la r�solution horizontale
	long m_HorzRes;
	long m_VertRes;
	long m_HorzSize;
	long m_VertSize;
};

#endif // !defined(AFX_PAGEINFO_H__B1655DA2_0409_4A93_A0E6_9082F2D2A150__INCLUDED_)
