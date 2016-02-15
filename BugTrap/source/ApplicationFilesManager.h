#pragma once

#include <list>
#include <string>

// UNICODE support
namespace std
{
#ifdef UNICODE
	typedef wstring			tstring;
	typedef wostringstream	tostringstream;
	typedef wofstream		tofstream;
	typedef wifstream		tifstream;
	typedef wfstream		tfstream;
	typedef wistringstream	tistringstream;


#else
	typedef string			tstring;
	typedef ostringstream	tostringstream;
	typedef ofstream		tofstream;
	typedef ifstream		tifstream;
	typedef fstream			tfstream;
	typedef istringstream	tistringstream;
#endif
}

class ApplicationFilesManager
{
public:

	std::list<std::tstring>& getFileNames(); 

	void WriteFilesInDirectory(PCTSTR _directory);

	void AddFile(TCHAR* _file);

	void AddFilesContainingStr(TCHAR* _path, TCHAR* _string);

private:

	std::list<std::tstring> m_List; 


};