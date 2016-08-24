/////////////////////////////////////////////////////////////////////////////
//	Skeleton Class for a Derived MyCug 97

#include "stdafx.h"
#include "resource.h"
#include "MyCug97.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(MyCug,CUGCtrl)
	//{{AFX_MSG_MAP(MyCug)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


#define PRINT_HEADER 0
#define PRINT_FOOTER 1
#define PRINT_ALIGN_LEFT 0
#define PRINT_ALIGN_CENTER 1
#define PRINT_ALIGN_RIGHT 2

/////////////////////////////////////////////////////////////////////////////
// MyCug construction/destruction
MyCug::MyCug()
{
	m_bPrintInProdress = FALSE;
}

MyCug::~MyCug()
{
}

/////////////////////////////////////////////////////////////////////////////
//	OnSetup
//		This function is called just after the grid window 
//		is created or attached to a dialog item.
//		It can be used to initially setup the grid
void MyCug::OnSetup()
{
	// Default font must be initialized in order for this example to work properly
	//m_defFont.CreateFont (14,0,0,0,300,0,0,0,0,0,0,0,0,"Times New Roman" );
	int rVal = m_defFont.CreatePointFont(100,"Arial");
	SetDefFont ( &m_defFont );

	SetDefColWidth ( 65 );

	int cols = 6,
		rows = 20;//60;

	SetTH_NumberRows ( 3 );
	SetTH_Height ( 60 );
	SetTH_RowHeight ( 0, 20 );
	SetTH_RowHeight ( 1, 20 );
	SetTH_RowHeight ( 2, 20 );

	SetNumberCols ( cols );
	SetNumberRows ( rows );

	JoinCells ( 0, -3, GetNumberCols()-1, -3 );
	JoinCells ( 0, -2, 1, -2 );
	JoinCells ( 2, -2, 3, -2 );
	JoinCells ( 4, -2, 5, -2 );
}

/////////////////////////////////////////////////////////////////////////////
//	OnGetCell
//		Populate the grid with basic data
void MyCug::OnGetCell(int col,long row,CUGCell *cell)
{
	if ( col == -1 && row >= 0 )
	{
		cell->SetNumberDecimals ( 0 );
		cell->SetNumber ( row );
	}
	else if ( col == 2 && row >= 0 )
	{
		cell->SetBackColor ( RGB( 255, 255, 255 ));
		cell->SetTextColor ( RGB( 0, 0, 0 ));
		if ( !m_bPrintInProdress )
		{	// drawing on the screen
			cell->SetCellType( UGCT_CHECKBOX );
		}
		else if ( cell->GetNumber() != 0 )
		{	// printing and cell content is set to TRUE
			cell->SetCellType( UGCT_NORMAL );
			cell->SetText( "TRUE" );
		}
		else
		{	// printing and cell content is set to FALSE
			cell->SetCellType( UGCT_NORMAL );
			cell->SetText( "FALSE" );
		}
	}
	else if ( col >= 0 )
	{
		if ( row == -3 )
			cell->SetText ( "Main Heading" );
		else if ( row == -2 )
			cell->SetText ( "Sub Heading" );
		else if ( row == -1 )
		{
			cell->SetNumberDecimals ( 0 );
			cell->SetNumber ( col );
		}
		else
		{
			if ( col == 3 )
			{
				cell->SetAlignment( UG_ALIGNRIGHT );
			}
			cell->SetBackColor ( RGB( 255, 255, 255 ));
			cell->SetTextColor ( RGB( 0, 0, 0 ));
			cell->SetNumberDecimals ( 0 );
			cell->SetNumber ( (row + 1) * (col + 1) );
		}
	}
}

void MyCug::OnDClicked(int col,long row,RECT *rect,POINT *point,BOOL processed)
{
	StartEdit();
}

COLORREF MyCug::OnGetDefBackColor(int section)
{
	return GetSysColor( COLOR_APPWORKSPACE );
}


/////////////////////////////////////////////////////////////////////////////
//	PrintRangeEx -	this function allowes to print the grid out of a dialog
//					based app.
int MyCug::PrintRangeEx ( int stCol, long stRow, int edCol, long edRow )
{
	CPrintDialog printDlg( FALSE, PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS );
	DOCINFO docInfo;
	CDC printDC;
	int totalNumPages, pageNum;

	// check if the print range is valid
	if ( stCol == edCol || stRow == edRow )
		return FALSE;

	m_bPrintInProdress = TRUE;

	if ( printDlg.DoModal() == IDOK )
	{// continue with printing process
		//initialize docinfo structure
		docInfo.cbSize = sizeof ( DOCINFO );
		docInfo.lpszDocName = "PtrWnd";
		docInfo.lpszDatatype = NULL;
		docInfo.lpszOutput = NULL;

		// verify if the print range is valid in the grid
		if ( stCol < 0 || stCol >= GetNumberCols () ) stCol = 0;
		if ( stRow < 0 || stRow >= GetNumberRows () ) stRow = 0;
		if ( edCol < 0 || edCol >= GetNumberCols () ) edCol = GetNumberCols()-1;
		if ( edRow < 0 || edRow >= GetNumberRows () ) edRow = GetNumberRows()-1;

		// lets makesure that the start values are smaller than the end values
		long temp;
		if ( stCol > edCol )
		{
			temp = stCol;
			stCol = edCol;
			edCol = temp;
		}
		if ( stRow > edRow )
		{
			temp = stRow;
			stRow = edRow;
			edRow = temp;
		}

		// begin printing process, everything is ok
		printDC.Attach ( printDlg.GetPrinterDC() );
		printDC.m_bPrinting = TRUE;

		if ( printDC.StartDoc ( &docInfo ) != -1 )
		{// print job successfully started
			// calculate number pages in the print job
			totalNumPages = PrintInit ( &printDC, &printDlg, stCol, stRow, edCol, edRow );

			for ( pageNum = 1; pageNum <= totalNumPages; pageNum ++ )
			{
				// Pring page by page
				printDC.StartPage ();
				PrintPage ( &printDC, pageNum );

				// This part is optional since it will make function calls
				//  to print page headers ...
				PrintHeader ( &printDC, PRINT_HEADER, PRINT_ALIGN_LEFT );
				PrintHeader ( &printDC, PRINT_HEADER, PRINT_ALIGN_CENTER, 0, "Header" );
				PrintHeader ( &printDC, PRINT_HEADER, PRINT_ALIGN_RIGHT );
				// and page footers ...
				PrintHeader ( &printDC, PRINT_FOOTER, PRINT_ALIGN_LEFT, pageNum );
				PrintHeader ( &printDC, PRINT_FOOTER, PRINT_ALIGN_CENTER, pageNum, "Footer" );
				PrintHeader ( &printDC, PRINT_FOOTER, PRINT_ALIGN_RIGHT, pageNum );

				// priting extra text below ...
				// m_CUGPrint->m_printVScale
				long vStart = 0, nTemp, nRow = 0;
				PrintGetOption( UG_PRINT_TOPMARGIN, &nTemp );
				vStart = nTemp;
				vStart += GetTH_Height();
				while ( nRow < GetNumberRows())
				{
					vStart += GetRowHeight( nRow );
					nRow ++;
				}

//				vStart = ( vStart + 100 ) * m_CUGPrint->m_printVScale;
//				printDC.TextOut( 200, vStart, "Printing extra text !!" );

				printDC.EndPage ();
			}

			// Finish print job
			printDC.EndDoc ();
		}
	}

	m_bPrintInProdress = FALSE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
//	PrintHeader -	This function prints headers on same page as Ultimate Grid
//		pDC			- pointer to printer device context
//		style		- distinguish between header (0) and footer (1)
//		alignment	- horizontal alingnment of text; left(0), center(1), right(2)
//		page		- currently page
//		string		- string to be displayed
//	#define PRINT_HEADER 0
//	#define PRINT_FOOTER 1
//	#define PRINT_ALIGN_LEFT 0
//	#define PRINT_ALIGN_CENTER 1
//	#define PRINT_ALIGN_RIGHT 2

int MyCug::PrintHeader ( CDC *pDC, int style, int alignment, int page, CString string )
{
	// set default string values
	if ( string == "" )
	{
		if ( style == PRINT_FOOTER )	// printing footer
			string.Format ( "Page: %d", page );
		else	// printing header
			GetParent()->GetWindowText( string );
	}

	// retrieve page size
	int horzPx = pDC->GetDeviceCaps ( HORZRES ),
		horzSz = pDC->GetDeviceCaps ( HORZSIZE ),
		vertPx = pDC->GetDeviceCaps ( VERTRES ),
		vertSz = pDC->GetDeviceCaps ( VERTSIZE );

	// calculate real margin size
	long tMargin, bMargin, lMargin, rMargin;
	PrintGetOption ( UG_PRINT_LEFTMARGIN, &lMargin );
	PrintGetOption ( UG_PRINT_RIGHTMARGIN, &rMargin );
	lMargin = ( horzPx / horzSz ) * lMargin;
	rMargin = ( horzPx / horzSz ) * rMargin;
	PrintGetOption ( UG_PRINT_TOPMARGIN, &tMargin );
	PrintGetOption ( UG_PRINT_BOTTOMMARGIN, &bMargin );
	tMargin = ( vertPx / vertSz ) * tMargin;
	bMargin = ( vertPx / vertSz ) * bMargin;

	// create and set font of the string
	CFont headerFont, *oldFont;
	headerFont.CreateFont ( -MulDiv( 10/*font size*/, pDC->GetDeviceCaps(LOGPIXELSY)/*pixels per inch*/, 72),
							0, 0, 0, 300, 0, 0, 0, 0, 0, 0, 0, 0, "Times New Roman" );
	oldFont = pDC->SelectObject ( &headerFont );

	// find out what is the size of the string
	CSize textSize = pDC->GetTextExtent ( string );

	// calculate print string horizontal location
	int locX, locY;
	switch ( alignment )
	{
	case PRINT_ALIGN_LEFT:	// align left
			locX = 0;
			break;
	case PRINT_ALIGN_CENTER:	// align center
			locX = (int)(( horzPx / 2 ) - ( textSize.cx / 2 ));
			break;
	case PRINT_ALIGN_RIGHT:	// align right
			locX = horzPx - 20 - textSize.cx;
			break;
	default :	// align center
			locX = (int)(( horzPx / 2 ) - ( textSize.cx / 2 ));
			break;
	}

	// calculate print string vertical location
	switch ( style )
	{
	case PRINT_HEADER:	// print header
			locY = ( tMargin / 2 ) - ( textSize.cy / 2 );
			break;
	case PRINT_FOOTER:	// print footer
			locY = vertPx - ( bMargin / 2 ) - ( textSize.cy / 2 );
			break;
	default :	// print header
			locY = (int)(( tMargin / 2 ) - ( textSize.cy / 2 ));
			break;
	}

	// print text
	pDC->TextOut ( locX, locY, string );

	// select old font back to the DC
	pDC->SelectObject ( oldFont );

	////////////////////////////////////////////////////////////////////////////////////
	// debuging purposes only !!!!!!!!!!!!!!!!!
/*	CString tempStr;
	for (int counter = 0; counter < horzPx; counter += 100 )
		pDC->TextOut ( counter, locY + 50, "." );
	tempStr.Format ( "Horizontal resolution: %dpx,  Size: %dmm", horzPx, horzSz );
	pDC->TextOut ( 200, 2000, tempStr );
	tempStr.Format ( "Vertical resolution:   %dpx,  Size: %dmm", vertPx, vertSz );
	pDC->TextOut ( 200, 2040, tempStr );
	tempStr.Format ( "Top margin:  %dpx;  Bottom margin: %dpx", tMargin, bMargin );
	pDC->TextOut ( 200, 2100, tempStr );
	tempStr.Format ( "Left margin: %dpx;  Right margin:  %dpx", lMargin, rMargin );
	pDC->TextOut ( 200, 2140, tempStr );
//	tempStr.Format ( "String size cx: %d, cy: %d", textSize.cx, textSize.cy );
//	pDC->TextOut ( 200, 2200, tempStr );
*/	////////////////////////////////////////////////////////////////////////////////////

	return FALSE;
}
