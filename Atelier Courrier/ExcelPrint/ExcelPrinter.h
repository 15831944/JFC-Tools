/*
//	JFC Informatique et M�dia
//	2005
//
//	Auteur:
//	J�r�my DEVILERS
//
//
//	Classe:
//	CExcelPrinter
//
//	Descriptif:
//	Point d'entr�e de la DLL de mise en forme Excel
//	Principe: colle le contenu du presse parier dans un document excel et execute la macro pass�e en param�tre
*/

#pragma once

#include "XLPrintThread.h"
#include "DlgProgress.h"

#include "ATPColors.h"


class AFX_EXT_CLASS CExcelPrinter
{
public:
	CExcelPrinter(CATPColors::APPCOLOR app, CWnd* pParent);
	virtual ~CExcelPrinter(void);

public:

	void SaveAsFile(CString filePath, CString macroPath, CString methodeName);
	void PrintPreview(CString macroPath, CString methodeName);
	void DirectPrint(CString macroPath, CString methodeName);
	void Open(CString macroPath, CString methodeName);


private:
	void RunExcel();
	void SetColors(CATPColors::APPCOLOR app);


	CWnd * m_pParent;
	CATPColors::APPCOLOR m_appColor;

private:
	CXLPrintThread* m_printThread;
	CDlgXLProgress * m_pdlgProgress;
};


