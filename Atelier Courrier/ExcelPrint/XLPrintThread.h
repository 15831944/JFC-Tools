/*
//	JFC Informatique et Média
//	2005
//
//	Auteur:
//	Jérémy DEVILERS
//
//
//	Classe:
//	CXLPrintThread
//
//	Descriptif:
//	Thread executant Excel pour ne pas faire de freeze de l'appli pendant que la macro tourne
*/

#pragma once

#include "WrapperExcel\CApplication.h"
#include "WrapperExcel\CWorkbook.h"
#include "WrapperExcel\CWorkbooks.h"
#include "WrapperExcel\CWorksheet.h"
#include "WrapperExcel\CWorksheets.h"
#include "Dlgprogress.h"

struct XLParams
{
	CString macroPath;
	CString methodeName;
	CString filePath;
	enum TYPEACTION {SAVE = 0,PRINT,PREVIEW,OPEN} typeAction;
};

class CXLPrintThread
{
public:
	CXLPrintThread(void);
	~CXLPrintThread(void);

	bool ExecuteMacro();

	XLParams m_params;

private:
	bool InitExcel(BOOL bVisible = FALSE);
	bool RunMacro(CString macroPath, CString methodeName);
	bool CloseWorkbook();
	bool ExitExcel();

	bool Save();
	bool PrintPreview();
	bool DirectPrint();
	bool Open();

private:

	bool m_bExcelInit;
	CApplication	m_app;
	CWorkbooks		m_WorkBooks;
	CWorkbook		m_bookMacro;
	CWorkbook		m_book;
	COleVariant		m_covOptional;//paramètre optionnel

public:
	CDlgXLProgress * m_pdlgProgress;
};
