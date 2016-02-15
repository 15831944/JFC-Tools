#include "StdAfx.h"

#include "ApplicationFilesManager.h"
#include <iostream>
#include <fstream>


void ApplicationFilesManager::AddFile(TCHAR* _file)
{
	m_List.push_back(_file);
}
std::list<std::tstring>& ApplicationFilesManager::getFileNames()
{
	return m_List;
}
void ApplicationFilesManager::AddFilesContainingStr(TCHAR* _path, TCHAR* _string)
{
	WIN32_FIND_DATA FindData;
	TCHAR szFindFileTemplate[MAX_PATH];
	std::tofstream out;
	std::tstring l_fileName;

	PathCombine(szFindFileTemplate, _path, _T("*"));
	HANDLE hFindFile = FindFirstFile(szFindFileTemplate, &FindData);


	if (hFindFile != INVALID_HANDLE_VALUE)
	{
		
		BOOL bMore = TRUE;
		while (bMore)
		{
			if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				

				l_fileName = FindData.cFileName;
			

				if (l_fileName.find(_string) != std::tstring::npos)
				{
					
					PathCombine(szFindFileTemplate, _path, l_fileName.c_str());

					m_List.push_back(szFindFileTemplate);
				}
				
			}

			bMore = FindNextFile(hFindFile, &FindData);
		}
		FindClose(hFindFile);
	}



}


void ApplicationFilesManager::WriteFilesInDirectory(PCTSTR _directory)
{
	std::list<std::tstring>::const_iterator it;
	std::ifstream in;
	std::ofstream out;
	std::tofstream outb;
	TCHAR szFileName[MAX_PATH];

	std::tstring filename;

	for(it = m_List.begin();it != m_List.end();++it)
	{
		filename =(*it);

	

		const size_t last_slash_idx = filename.find_last_of(_T("\\/"));
		if (std::tstring::npos != last_slash_idx)
		{
			filename.erase(0, last_slash_idx + 1);
		}


		PathCombine(szFileName, _directory,filename.c_str());

		CopyFile((*it).c_str(),szFileName,FALSE);
		


	}
}
