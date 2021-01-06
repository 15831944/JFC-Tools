#include "StdAfx.h"
#include ".\excelprinter.h"
#include "afxmt.h"

CExcelPrinter::CExcelPrinter(CATPColors::APPCOLOR app, CWnd* pParent)
{
	m_pParent = pParent;
	SetColors(app);
	m_printThread = new CXLPrintThread();

	m_pdlgProgress = NULL;
}

CExcelPrinter::~CExcelPrinter(void)
{
	delete m_printThread;
}

void CExcelPrinter::PrintPreview(CString macroPath, CString methodeName)
{
	m_printThread->m_params.macroPath = macroPath;
	m_printThread->m_params.methodeName = methodeName;
	m_printThread->m_params.typeAction = XLParams::PREVIEW;

	RunExcel();
}

void CExcelPrinter::DirectPrint(CString macroPath, CString methodeName)
{
	m_printThread->m_params.macroPath = macroPath;
	m_printThread->m_params.methodeName = methodeName;
	m_printThread->m_params.typeAction = XLParams::PRINT;

	RunExcel();
}

void CExcelPrinter::SaveAsFile(CString filePath, CString macroPath, CString methodeName)
{
	m_printThread->m_params.filePath = filePath;
	m_printThread->m_params.macroPath = macroPath;
	m_printThread->m_params.methodeName = methodeName;
	m_printThread->m_params.typeAction = XLParams::SAVE;

	RunExcel();
}

void CExcelPrinter::Open(CString macroPath, CString methodeName)
{
	m_printThread->m_params.macroPath = macroPath;
	m_printThread->m_params.methodeName = methodeName;
	m_printThread->m_params.typeAction = XLParams::OPEN;

	RunExcel();
}

void Open(CString macroPath, CString methodeName);


void CExcelPrinter::RunExcel()
{
	CDlgXLProgress dlgProgress(*m_printThread, m_appColor, m_pParent);
	if (dlgProgress.DoModal() != IDOK)
	{
		AfxMessageBox("Problème d'export (encore et encore...)",MB_OK);
	}
}

void CExcelPrinter::SetColors(CATPColors::APPCOLOR app)
{
	m_appColor = app;
}
