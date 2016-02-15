// ViewportWindow.cpp : implementation file
//

#include "stdafx.h"
#include "ViewportWindow.h"
#include "./Draw/JFCGDIDraw.h"
#include "./Composite/JFCDrawDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//================================================================================
// le constructeur:
//================================================================================
CViewportWindow::CViewportWindow(CPageInfo * pageinfo)
{
	// on fixe le pointeur sur les informations de la page
	m_PageInfo = pageinfo;
}


BEGIN_MESSAGE_MAP(CViewportWindow, CWnd)
	//{{AFX_MSG_MAP(CViewportWindow)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//================================================================================
// la fonction pour dessiner le contenu de la fen�tre:
//================================================================================
void CViewportWindow::OnPaint() 
{
	// device context for painting
	CPaintDC Dc(this);

	// la position de la bitmap
	long x = 0;
	long y = 0;

	// les dimensions de la bitmap
	long dx = 0;
	long dy = 0;	

	// position de la zone a redessiner
	long xZone = Dc.m_ps.rcPaint.left;
	long yZone = Dc.m_ps.rcPaint.top;

	// Dimension de la zone a redessiner
	long dxZone = Dc.m_ps.rcPaint.right - Dc.m_ps.rcPaint.left;
	long dyZone = Dc.m_ps.rcPaint.bottom - Dc.m_ps.rcPaint.top;

	// on initialise l'objet de conversion
	m_pCoord = new DecimilToUnit(GetDeviceCaps(Dc.m_hDC, HORZRES), GetDeviceCaps(Dc.m_hDC, VERTRES), GetDeviceCaps(Dc.m_hDC, HORZSIZE)*10, GetDeviceCaps(Dc.m_hDC, VERTSIZE)*10, m_PageInfo->GetZoom(), 0, 0);

	// on r�cup�re la taille logique du document
	long LogSizeX = m_PageInfo->GetSizeX_px();
	long LogSizeY = m_PageInfo->GetSizeY_px();

	// on r�cup�re la taille physique
	RECT ClientRect; this->GetClientRect(&ClientRect);
	long PhySizeX = ClientRect.right - ClientRect.left;
	long PhySizeY = ClientRect.bottom - ClientRect.top;

	// on compare les tailles logiques et physiques
	if (PhySizeX > LogSizeX)
	{ 
		dx = LogSizeX;
		x = (PhySizeX-LogSizeX)/2;
	}
	else
	{
		dx = PhySizeX;
	}

	if (PhySizeY > LogSizeY) 
	{
		dy = LogSizeY;
		y = (PhySizeY - LogSizeY) / 2;
	}
	else 
	{
		dy = PhySizeY;
	}

	// les variables pour g�rer la bitmap
	CDC DcComp;
	CBitmap BmpNew;	
	
	// on cr�e un contexte de p�riph�rique compatible
	if (FALSE != DcComp.CreateCompatibleDC(&Dc))
	{

		// on cr�e une bitmap compatible avec ce contexte de p�riph�rique
		if (FALSE != BmpNew.CreateCompatibleBitmap(&Dc, dxZone, dyZone))
		{
			CBitmap * pBmpOld = (CBitmap*)DcComp.SelectObject(&BmpNew);
			
			// on vide la bitmap
			CBrush fondgris(GetSysColor(COLOR_APPWORKSPACE));
			RECT zoneWindow; 
			zoneWindow.left = 0; zoneWindow.right  = dxZone;
			zoneWindow.top =  0; zoneWindow.bottom = dyZone;
			DcComp.FillRect(&zoneWindow, &fondgris);

			// On change l'origine pour centrer la page
			CPoint pointOrg = DcComp.SetWindowOrg((xZone-x),(yZone-y));

			// on r�cup�re les donn�es graphiques du document
			DrawDocument * pDrawDoc = m_PageInfo->GetDrawDocument();
			
			// Remplissage blanc de la page
			CBrush fondPage(RGB(255,255,255));
			RECT zonePage; 
			zonePage.left = 0; zonePage.right = dx;
			zonePage.top = 0; zonePage.bottom = dy;
			DcComp.FillRect(&zonePage,&fondPage);

			// on calcule le d�calage d'origine
			long offsetx = 0;
			long offsety = 0;
			
			if (pDrawDoc != 0)
			{
				offsetx = ((m_PageInfo->GetSizeX_mm()*100/m_PageInfo->GetZoom()) - pDrawDoc->GetPageWidth()) / 2;
				offsety = ((m_PageInfo->GetSizeY_mm()*100/m_PageInfo->GetZoom()) - pDrawDoc->GetPageHeight()) / 2;
			}

			try
			{
				// on cr�e un jfcGDIDraw
				JFCGDIDraw MyDraw(DcComp.m_hDC, offsetx, offsety, m_PageInfo->GetZoom(), m_PageInfo->GetOrgX_mm(), m_PageInfo->GetOrgY_mm());

				if (pDrawDoc)
				{
					// on dessine le document
					pDrawDoc->Draw(&MyDraw, m_PageInfo->GetCurrentPage(), m_PageInfo->GetCurrentPage());
				}
			}
			catch(...)
			{
				return;
			}
			
			// On repositionne l'origine
			DcComp.SetWindowOrg(pointOrg);
			// on copie la bitmap dans la zone � dessiner de la fen�tre
			Dc.BitBlt(xZone, yZone, dxZone, dyZone, &DcComp, 0, 0, SRCCOPY);

			// on restaure la bitmap initiale
			Dc.SelectObject(pBmpOld);
		}
		// on d�truit le contexte de p�riph�rique compatible
		DcComp.DeleteDC();
	}
	// on lib�re l'objet de conversion
	delete m_pCoord;
}

//================================================================================
// le destructeur:
//================================================================================
CViewportWindow::~CViewportWindow()
{
	
}
