#include "stdafx.h"

#include <iostream>
#include <fstream>

using namespace std;

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>

#include "JFCIniFile.h"


JFCIniFile::JFCIniFile( string const iniPath)
{
	Path( iniPath);
	caseInsensitive = true;
}

bool JFCIniFile::ReadFile()
{
	fstream f;
	string   line;
	string   keyname, valuename, value;
	string::size_type pLeft, pRight;

	f.open( path.c_str(), ios::in);
	if ( f.fail())
		return false;

	while( getline( f, line))
	{
		if ( (line.length() > 0) && (line[line.length() - 1] == '\r'))

			line = line.substr( 0, line.length() - 1);

		if ( line.length())
		{
			if ( !isprint( line[0]))
			{
				f.close();
				return false;
			}

			if (( pLeft = line.find_first_of(";#[=")) != string::npos)
			{
				switch ( line[pLeft])
				{
				case '[':
					if ((pRight = line.find_last_of("]")) != string::npos && pRight > pLeft)
					{
						keyname = line.substr( pLeft + 1, pRight - pLeft - 1);
						AddKeyName( keyname);
					}
					break;

				case '=':
					valuename = line.substr( 0, pLeft);
					value = line.substr( pLeft + 1);
					SetValue( keyname, valuename, value);
					break;

				case ';':
				case '#':
					if ( !names.size())
						HeaderComment( line.substr( pLeft + 1));
					else
						KeyComment( keyname, line.substr( pLeft + 1));
					break;
				}
			}
		}
	}

	f.close();
	if ( names.size())	return true;
	else				return false;
}

bool JFCIniFile::WriteFile()
{
	unsigned commentID, keyID, valueID;
	fstream f;

	f.open( path.c_str(), ios::out);
	if ( f.fail()) return false;

	for ( commentID = 0; commentID < comments.size(); ++commentID)
		f << ';' << comments[commentID] << endl;

	if ( comments.size()) f << endl;

	for ( keyID = 0; keyID < keys.size(); ++keyID)
	{
		f << '[' << names[keyID] << ']' << endl;

		// commentaires
		for ( commentID = 0; commentID < keys[keyID].comments.size(); ++commentID)
			f << ';' << keys[keyID].comments[commentID] << endl;

		// valeurs
		for ( valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
			f << keys[keyID].names[valueID] << '=' << keys[keyID].values[valueID] << endl;
		f << endl;
	}
	f.close();

	return true;
}

long JFCIniFile::FindKey( string const keyname) const
{
	for ( unsigned keyID = 0; keyID < names.size(); ++keyID)
		if ( CheckCase( names[keyID]) == CheckCase( keyname))
			return long(keyID);

	return noID;
}

long JFCIniFile::FindValue( unsigned const keyID, string const valuename) const
{
	if ( !keys.size() || keyID >= keys.size())
		return noID;

	for ( unsigned valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
		if ( CheckCase( keys[keyID].names[valueID]) == CheckCase( valuename))
			return long(valueID);

	return noID;
}

unsigned int JFCIniFile::AddKeyName( string const keyname)
{
	names.resize( names.size() + 1, keyname);
	keys.resize( keys.size() + 1);

	return (unsigned int) names.size() - 1;
}

string JFCIniFile::KeyName( unsigned const keyID) const
{
	if ( keyID < names.size())
		return names[keyID];
	else
		return "";
}

unsigned int JFCIniFile::NumValues( unsigned const keyID)
{
	if ( keyID < keys.size())
		return (unsigned int)keys[keyID].names.size();

	return 0;
}

unsigned int JFCIniFile::NumValues( string const keyname)
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return 0;

	return (unsigned int)keys[keyID].names.size();
}

string JFCIniFile::ValueName( unsigned const keyID, unsigned const valueID) const
{
	if ( keyID < keys.size() && valueID < keys[keyID].names.size())
		return keys[keyID].names[valueID];

	return "";
}

string JFCIniFile::ValueName( string const keyname, unsigned const valueID) const
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return "";

	return ValueName( keyID, valueID);
}

bool JFCIniFile::SetValue( unsigned const keyID, unsigned const valueID, string const value)
{
	if ( keyID < keys.size() && valueID < keys[keyID].names.size())
		keys[keyID].values[valueID] = value;

	return false;
}

bool JFCIniFile::SetValue( unsigned const keyID, string const valuename, string const value, bool const create)
{
	long valueID = FindValue( unsigned(keyID), valuename);
	if ( valueID == noID)
	{
		if ( !create) return false;

		keys[keyID].names.resize( keys[keyID].names.size() + 1, valuename);
		keys[keyID].values.resize( keys[keyID].values.size() + 1, value);
	}
	else
		keys[keyID].values[valueID] = value;

	return true;
}

bool JFCIniFile::SetValue( string const keyname, string const valuename, string const value, bool const create)
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
	{
		if ( create)
			keyID = long( AddKeyName( keyname));
		else
			return false;
	}

	long valueID = FindValue( unsigned(keyID), valuename);
	if ( valueID == noID)
	{
		if ( !create) return false;

		keys[keyID].names.resize( keys[keyID].names.size() + 1, valuename);
		keys[keyID].values.resize( keys[keyID].values.size() + 1, value);
	}
	else
		keys[keyID].values[valueID] = value;

	return true;
}

bool JFCIniFile::SetValueI( string const keyname, string const valuename, int const value, bool const create)
{
	char svalue[MAX_VALUEDATA];

	sprintf_s( svalue, "%d", value);

	return SetValue( keyname, valuename, svalue);
}

bool JFCIniFile::SetValueF( string const keyname, string const valuename, double const value, bool const create)
{
	char svalue[MAX_VALUEDATA];

	sprintf_s( svalue, "%f", value);

	return SetValue( keyname, valuename, svalue);
}

bool JFCIniFile::SetValueV( string const keyname, string const valuename, char *format, ...)
{
	va_list args;
	char value[MAX_VALUEDATA];

	va_start( args, format);
	vsprintf_s( value, format, args);
	va_end( args);

	return SetValue( keyname, valuename, value);
}

string JFCIniFile::GetValue( unsigned const keyID, unsigned const valueID, string const defValue) const
{
	if ( keyID < keys.size() && valueID < keys[keyID].names.size())
		return keys[keyID].values[valueID];

	return defValue;
}

string JFCIniFile::GetValue( unsigned const keyID, string const valuename, string const defValue) const
{
	long valueID = FindValue( unsigned(keyID), valuename);
	if ( valueID == noID)
		return defValue;

	return keys[keyID].values[valueID];
}


string JFCIniFile::GetValue( string const keyname, string const valuename, string const defValue) const
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return defValue;

	long valueID = FindValue( unsigned(keyID), valuename);
	if ( valueID == noID)
		return defValue;

	return keys[keyID].values[valueID];
}

int JFCIniFile::GetValueI(string const keyname, string const valuename, int const defValue) const
{
	char svalue[MAX_VALUEDATA];

	sprintf_s( svalue, "%d", defValue);

	return atoi( GetValue( keyname, valuename, svalue).c_str()); 
}

int JFCIniFile::GetValueI(unsigned const keyID, string const valuename, int const defValue) const
{
	long valueID = FindValue( unsigned(keyID), valuename);
	if ( valueID == noID)
		return defValue;

	return atoi( keys[keyID].values[valueID].c_str());
}

bool JFCIniFile::GetValueB(string const keyname, string const valuename, bool const defValue) const
{
	return (GetValueI( keyname, valuename, int( defValue))?true:false);
}


bool JFCIniFile::GetValueB(unsigned const keyID, string const valuename, bool const defValue) const
{
	long valueID = FindValue( unsigned(keyID), valuename);
	if ( valueID == noID)
		return defValue;

	int bvalue = atoi( keys[keyID].values[valueID].c_str());
	return (bvalue != 0)?true:false;
}


double JFCIniFile::GetValueF(string const keyname, string const valuename, double const defValue) const
{
	char svalue[MAX_VALUEDATA];

	sprintf_s( svalue, "%f", defValue);

	return atof( GetValue( keyname, valuename, svalue).c_str()); 
}

unsigned JFCIniFile::GetValueV( string const keyname, string const valuename, char *format,
							   void *v1, void *v2, void *v3, void *v4,
							   void *v5, void *v6, void *v7, void *v8,
							   void *v9, void *v10, void *v11, void *v12,
							   void *v13, void *v14, void *v15, void *v16)
{
	string   value;
	unsigned nVals;

	value = GetValue( keyname, valuename);
	if ( !value.length())

		nVals = sscanf_s( value.c_str(), format,
		v1, v2, v3, v4, v5, v6, v7, v8,
		v9, v10, v11, v12, v13, v14, v15, v16);

	return nVals;
}

bool JFCIniFile::DeleteValue( string const keyname, string const valuename)
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return false;

	long valueID = FindValue( unsigned(keyID), valuename);
	if ( valueID == noID)
		return false;

	vector<string>::iterator npos = keys[keyID].names.begin() + valueID;
	vector<string>::iterator vpos = keys[keyID].values.begin() + valueID;
	keys[keyID].names.erase( npos, npos + 1);
	keys[keyID].values.erase( vpos, vpos + 1);

	return true;
}

bool JFCIniFile::DeleteKey( string const keyname)
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return false;

	vector<string>::iterator npos = names.begin() + keyID;
	vector<key>::iterator    kpos = keys.begin() + keyID;
	names.erase( npos, npos + 1);
	keys.erase( kpos, kpos + 1);

	return true;
}

void JFCIniFile::Erase()
{
	names.clear();
	keys.clear();
	comments.clear();
}

void JFCIniFile::HeaderComment( string const comment)
{
	comments.resize( comments.size() + 1, comment);
}

string JFCIniFile::HeaderComment( unsigned const commentID) const
{
	if ( commentID < comments.size())
		return comments[commentID];

	return "";
}

bool JFCIniFile::DeleteHeaderComment( unsigned commentID)
{
	if ( commentID < comments.size())
	{
		vector<string>::iterator cpos = comments.begin() + commentID;
		comments.erase( cpos, cpos + 1);
		return true;
	}

	return false;
}

unsigned int JFCIniFile::NumKeyComments( unsigned const keyID) const
{
	if ( keyID < keys.size())
		return (unsigned int)keys[keyID].comments.size();

	return 0;
}

unsigned int JFCIniFile::NumKeyComments( string const keyname) const
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return 0;

	return (unsigned int)keys[keyID].comments.size();
}

bool JFCIniFile::KeyComment( unsigned const keyID, string const comment)
{
	if ( keyID < keys.size())
	{
		keys[keyID].comments.resize( keys[keyID].comments.size() + 1, comment);
		return true;
	}

	return false;
}

bool JFCIniFile::KeyComment( string const keyname, string const comment)
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return false;

	return KeyComment( unsigned(keyID), comment);
}

string JFCIniFile::KeyComment( unsigned const keyID, unsigned const commentID) const
{
	if ( keyID < keys.size() && commentID < keys[keyID].comments.size())
		return keys[keyID].comments[commentID];

	return "";
}

string JFCIniFile::KeyComment( string const keyname, unsigned const commentID) const
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return "";

	return KeyComment( unsigned(keyID), commentID);
}

bool JFCIniFile::DeleteKeyComment( unsigned const keyID, unsigned const commentID)
{
	if ( keyID < keys.size() && commentID < keys[keyID].comments.size())
	{
		vector<string>::iterator cpos = keys[keyID].comments.begin() + commentID;
		keys[keyID].comments.erase( cpos, cpos + 1);
		return true;
	}

	return false;
}

bool JFCIniFile::DeleteKeyComment( string const keyname, unsigned const commentID)
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return false;

	return DeleteKeyComment( unsigned(keyID), commentID);
}

bool JFCIniFile::DeleteKeyComments( unsigned const keyID)
{
	if ( keyID < keys.size())
	{
		keys[keyID].comments.clear();
		return true;
	}

	return false;
}

bool JFCIniFile::DeleteKeyComments( string const keyname)
{
	long keyID = FindKey( keyname);
	if ( keyID == noID)
		return false;

	return DeleteKeyComments( unsigned(keyID));
}

string JFCIniFile::CheckCase( string s) const
{
	if ( caseInsensitive)
		for ( string::size_type i = 0; i < s.length(); ++i)
			s[i] = tolower(s[i]);

	return s;
}
