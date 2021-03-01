#include "StdAfx.h"
#include ".\PrintAnyCWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPrintAnyCWnd::CPrintAnyCWnd(void)
{
}

CPrintAnyCWnd::~CPrintAnyCWnd(void)
{
}


bool CPrintAnyCWnd::CopyToClipBoard(CWnd* pWnd)
{	
	CDC MemDC;
	CBitmap ScreenBitmap;
	CBitmap* pOldBitmap = NULL;

//	try {
		// Travailler avec une fenêtre à jour !
		pWnd->Invalidate();
		pWnd->UpdateWindow();

		//
		// Récupération taille écran
		//
		CClientDC ScreenDC(pWnd);
		CRect rect; 

		pWnd->GetWindowRect(rect);
		pWnd->ScreenToClient(rect);

		int ScreenWidth  = rect.Width();
		int ScreenHeight = rect.Height();

		//
		// Création bitmap DDB
		//
		if ( !MemDC.CreateCompatibleDC(&ScreenDC) ) 
		{
			//throw CPrinterXcpt(__FILE__, "CopyToClipBoard" , __LINE__, CPrinterXcpt::PRINTER_DC_CREATE);	
			return false;
		}

		if ( !ScreenBitmap.CreateCompatibleBitmap(&ScreenDC, ScreenWidth, ScreenHeight) )
		{
			//throw CPrinterXcpt(__FILE__, "CopyToClipBoard" , __LINE__, CPrinterXcpt::PRINTER_DDB_CREATE);	
			return false;
		}

		pOldBitmap = MemDC.SelectObject( &ScreenBitmap );
		if ( !pOldBitmap )
		{
			//throw CPrinterXcpt(__FILE__, "CopyToClipBoard" , __LINE__, CPrinterXcpt::PRINTER_DC_SELECT);	
			return false;
		}

		if ( !MemDC.BitBlt(0, 0,ScreenWidth, ScreenHeight, &ScreenDC, rect.left, rect.top, SRCCOPY) )
		{
			//throw CPrinterXcpt(__FILE__, "CopyToClipBoard" , __LINE__, CPrinterXcpt::PRINTER_DDB_COPY);
			return false;
		}

		//
		// Copie vers presse-papier
		//

		if (!pWnd->OpenClipboard())
		{
			//throw CPrinterXcpt(__FILE__, "CopyToClipBoard" , __LINE__, CPrinterXcpt::PRINTER_CLIP_OPEN);
			return false;
		}

		if (!EmptyClipboard())
		{
			//throw CPrinterXcpt(__FILE__, "CopyToClipBoard" , __LINE__, CPrinterXcpt::PRINTER_CLIP_RAZ);
			return false;
		}
			
		if (!SetClipboardData(CF_BITMAP, ScreenBitmap.GetSafeHandle()))
		{
			//throw CPrinterXcpt(__FILE__, "CopyToClipBoard" , __LINE__, CPrinterXcpt::PRINTER_CLIP_COPY);
			return false;
		}
		
		if (!CloseClipboard())
		{
			//throw CPrinterXcpt(__FILE__, "CopyToClipBoard" , __LINE__, CPrinterXcpt::PRINTER_CLIP_CLOSE);
			return false;
		}
			
	//}
	//catch (CPrinterXcpt&)
	//{
	//	MemDC.SelectObject(pOldBitmap);
	//	ScreenBitmap.Detach();			
	//	throw;		
	//}
	return true;
}


void CPrintAnyCWnd::Print(CWnd* pWnd, CString title)
{
	
	CDC dc;
	CPrintDialog printDlg(FALSE,  PD_PAGENUMS | PD_NOSELECTION);
	
	if(printDlg.DoModal()==IDCANCEL)
		return;

	if(!CopyToClipBoard(pWnd))
		return;

	dc.Attach(printDlg.GetPrinterDC());

	// Create a text for the title
	//CString sTitleHeader=_T("CPrintAnyCWnd");
	
	dc.StartDoc(title);
	dc.StartPage();                         // Commencer la page


	// Get the page size and boundaries
	//CRect rectPage = pInfo->m_rectDraw;
	CRect rectPage;
	rectPage.SetRect(0,0, dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));


	TEXTMETRIC tm;
	CFont font;
	CSize textSize;
	int cyChar;

	// Create the font we will be using
	font.CreatePointFont(100, "Arial", &dc);
	CFont *pOldFont = dc.SelectObject(&font);

	//Set Margin
	rectPage.top+=rectPage.bottom/48;
	rectPage.bottom-=rectPage.bottom/48;
	rectPage.left+=200;
	rectPage.right-=200;

	// Get Text size in order to center
	dc.GetTextMetrics(&tm);
	textSize = dc.GetTextExtent(title);
	cyChar = tm.tmHeight;


	// Draw Text (centered)
	dc.TextOut(((rectPage.right+rectPage.left)/2)-(textSize.cx/2),
	                rectPage.top, title);
	rectPage.top += cyChar + cyChar / 4;

	// Draw header line divider
	dc.MoveTo(rectPage.left, rectPage.top);
	dc.LineTo(rectPage.right, rectPage.top);

	// Go to next line
	rectPage.top += cyChar / 4;

	pWnd->OpenClipboard();
	HBITMAP hbitmap = (HBITMAP)::GetClipboardData(CF_BITMAP);
	CloseClipboard();
	if(hbitmap)
	{
		BITMAP bm;
		::GetObject(hbitmap, sizeof(BITMAP), &bm);
		CSize chartSize(bm.bmWidth, bm.bmHeight);

		CDC dcMemory,dcScreen;
		dcScreen.Attach(::GetDC(NULL));

		// create "from" device context and select the
		// loaded bitmap into it
		dcMemory.CreateCompatibleDC(&dcScreen);
		dcMemory.SelectObject(hbitmap);

		// Print at 95% size within left/right margin 
		CSize printSize;
		printSize.cx=(int)(rectPage.right*.95);
		printSize.cy=printSize.cx/chartSize.cx*chartSize.cy;

		int posXCenter = ((rectPage.right+rectPage.left)/2) - (printSize.cx/2);
		int posYCenter = ((rectPage.top+rectPage.bottom)/2) - (printSize.cy/2);
		// Print chart centered
		dc.StretchBlt( posXCenter, 
				  posYCenter,/* rectPage.top,*/
				  printSize.cx,
				  printSize.cy,
				  &dcMemory, 
				  0, 0, chartSize.cx, chartSize.cy, SRCCOPY);
		dcMemory.DeleteDC();
	}

	// Revert and Destroy
	dc.SelectObject(pOldFont);
	font.DeleteObject();

	dc.EndPage();
	dc.EndDoc();
	dc.Detach();

}

