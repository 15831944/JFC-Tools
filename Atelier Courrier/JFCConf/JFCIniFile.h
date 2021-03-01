#ifndef JFCINIFILE_H
#define JFCINIFILE_H

// C++ Includes
#include <string>
#include <vector>
#include <sstream>
#include <iterator>

// C Includes
#include <stdlib.h>


#define MAX_KEYNAME    128
#define MAX_VALUENAME  128
#define MAX_VALUEDATA 2048

using namespace std;

class AFX_EXT_CLASS JFCIniFile  
{
protected:

	string path;
	bool   caseInsensitive;

	struct key
	{
		vector<string> names;
		vector<string> values; 
		vector<string> comments;
	};

	vector<key>    keys; 
	vector<string> names; 
	vector<string> comments;

	string CheckCase( string s) const;


public:

	typedef  enum errors {noID = -1};

	JFCIniFile( string const iniPath = "");

	virtual ~JFCIniFile()                            {}

	////////////////////////////////////////////////////////////////////////
	// Sets whether or not keynames and valuenames should be case sensitive.
	// The default is case insensitive.
	void CaseSensitive()                           {caseInsensitive = false;}

	void CaseInsensitive()                         {caseInsensitive = true;}


	////////////////////////////////////////////////
	// Sets path of ini file to read and write from.
	void Path(string const newPath)                {path = newPath;}

	string Path() const                            {return path;}

	void SetPath(string const newPath)             {Path( newPath);}


	///////////////////////////////////////////////
	// Reads ini file specified using path.
	// Returns true if successful, false otherwise.
	virtual bool ReadFile();


	///////////////////////////////////////////
	// Writes data stored in class to ini file.
	virtual bool WriteFile(); 


	///////////////////////////////
	// Deletes all stored ini data.
	void Erase();

	void Clear()                                   {Erase();}

	void Reset()                                   {Erase();}


	////////////////////////////////////////////////////////
	// Returns index of specified key, or noID if not found.
	long FindKey( string const keyname) const;


	////////////////////////////////////////////////////////////////////////////////
	// Returns index of specified value, in the specified key, or noID if not found.
	long FindValue( unsigned const keyID, string const valuename) const;


	///////////////////////////////////////////////
	// Returns number of keys currently in the ini.
	unsigned int NumKeys() const                       {return (unsigned int)names.size();}

	unsigned int GetNumKeys() const                    {return (unsigned int)NumKeys();}


	//////////////////
	// Add a key name.
	unsigned int AddKeyName( string const keyname);


	//////////////////////////////
	// Returns key names by index.
	string KeyName( unsigned const keyID) const;

	string GetKeyName( unsigned const keyID) const {return KeyName(keyID);}


	/////////////////////////////////////////////////////
	// Returns number of values stored for specified key.
	unsigned int NumValues( unsigned const keyID);

	unsigned int GetNumValues( unsigned const keyID)   {return NumValues( keyID);}

	unsigned int NumValues( string const keyname);

	unsigned int GetNumValues( string const keyname)   {return NumValues( keyname);}


	////////////////////////////////////////////////////////////
	// Returns value name by index for a given keyname or keyID.
	string ValueName( unsigned const keyID, unsigned const valueID) const;

	string GetValueName( unsigned const keyID, unsigned const valueID) const
	{
		return ValueName( keyID, valueID);
	}

	string ValueName( string const keyname, unsigned const valueID) const;

	string GetValueName( string const keyname, unsigned const valueID) const
	{
		return ValueName( keyname, valueID);
	}

	////////////////////////////////////////////////
	// Gets value of [keyname] valuename =.
	// Overloaded to return string, int, and double.
	// Returns defValue if key/value not found.
	string	GetValue( unsigned const keyID, unsigned const valueID, string const defValue = "") const;
	string	GetValue( unsigned const keyID, string const valuename, string const defValue = "") const;
	string	GetValue(string const keyname, string const valuename, string const defValue = "") const; 

	int		GetValueI(string const keyname, string const valuename, int const defValue = 0) const;
	int		GetValueI(unsigned const keyID, string const valuename, int const defValue = 0) const;

	bool	GetValueB(string const keyname, string const valuename, bool const defValue = false) const;
	bool	GetValueB(unsigned const keyID, string const valuename, bool const defValue = false) const;

	double	GetValueF(string const keyname, string const valuename, double const defValue = 0.0) const;

	////////////////////////////////////////////////
	// Gets a list of value (values must have been written with SetValueList or of the form : val1 val2 val3)of [keyname] valuename =.
	// return true if reading has been successful
	template<typename _Cont>
	bool GetValues(string const keyname, string const valuename, _Cont & values) const
	{
		try
		{
			// create an iterator 
			insert_iterator<typename _Cont> iter(values, values.begin());

			// prepare the streaming
			stringstream ss;
			values.clear();

			_Cont::value_type elem;

			// read the values
			ss.str(GetValue(keyname, valuename));
			ss.clear();
			while(ss >> elem)
				*iter++ = elem;

			return true;
		}
		catch (...)
		{
			values.clear();
			return false;
		}
	}

	/////////////////////////////////////////////////////////////////////////
	// This is a variable length formatted GetValue routine. All these voids
	// are required because there is no vsscanf() like there is a vsprintf().
	// Only a maximum of 8 variable can be read.
	unsigned int GetValueV( string const keyname, string const valuename, char *format,
		void *v1 = 0, void *v2 = 0, void *v3 = 0, void *v4 = 0,
		void *v5 = 0, void *v6 = 0, void *v7 = 0, void *v8 = 0,
		void *v9 = 0, void *v10 = 0, void *v11 = 0, void *v12 = 0,
		void *v13 = 0, void *v14 = 0, void *v15 = 0, void *v16 = 0);


	/////////////////////////////////////////////////////////////////////////////
	// Sets value of [keyname] valuename =.
	// Specify the optional paramter as false (0) if you do not want it to create
	// the key if it doesn't exist. Returns true if data entered, false otherwise.
	// Overloaded to accept string, int, and double.
	bool SetValue( unsigned const keyID, unsigned const valueID, string const value);

	bool SetValue( unsigned const keyID, string const valuename, string const value, bool const create = true);

	bool SetValue( string const keyname, string const valuename, string const value, bool const create = true);

	bool SetValueI( string const keyname, string const valuename, int const value, bool const create = true);

	bool SetValueB( string const keyname, string const valuename, bool const value, bool const create = true)
	{
		return SetValueI( keyname, valuename, int(value), create);
	}

	bool SetValueF( string const keyname, string const valuename, double const value, bool const create = true);

	bool SetValueV( string const keyname, string const valuename, char *format, ...);


	////////////////////////////////////////////////
	// Gets a list of value (values must have been written with SetValueList or of the form : val1 val2 val3)of [keyname] valuename =.
	// return true if reading has been successful
	template<typename _Cont>
	bool SetValues(string const keyname, string const valuename, const _Cont & values)
	{
		try
		{
			// on crée la chaine de valeurs à partir du container
			stringstream ss;

			// on convertit les valeurs du container, on les place dans le stream
			copy(values.begin(), values.end(), ostream_iterator<_Cont::value_type>(ss, " "));

			// on ajoute la valeur chaine
			SetValue(keyname, valuename, ss.str(), true);

			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	//////////////////////////////////////////////////////////////
	// Deletes specified value.
	// Returns true if value existed and deleted, false otherwise.
	bool DeleteValue( string const keyname, string const valuename);


	////////////////////////////////////////////////////////////
	// Deletes specified key and all values contained within.
	// Returns true if key existed and deleted, false otherwise.
	bool DeleteKey(string keyname);


	///////////////////////////////////////////////////////////
	// Header comment functions.
	// Header comments are those comments before the first key.
	//
	// Number of header comments.
	unsigned int NumHeaderComments()                  {return (unsigned int)comments.size();}


	////////////////////////
	// Add a header comment.
	void     HeaderComment( string const comment);


	///////////////////////////
	// Return a header comment.
	string   HeaderComment( unsigned const commentID) const;


	///////////////////////////
	// Delete a header comment.
	bool     DeleteHeaderComment( unsigned commentID);


	//////////////////////////////
	// Delete all header comments.
	void     DeleteHeaderComments()               {comments.clear();}


	///////////////////////////////////////////////////////////////////////
	// Key comment functions.
	// Key comments are those comments within a key. Any comments
	// defined within value names will be added to this list. Therefore,
	// these comments will be moved to the top of the key definition when
	// the JFCIniFile::WriteFile() is called.
	//
	// Number of key comments.
	unsigned int NumKeyComments( unsigned const keyID) const;
	unsigned int NumKeyComments( string const keyname) const;


	/////////////////////
	// Add a key comment.
	bool     KeyComment( unsigned const keyID, string const comment);
	bool     KeyComment( string const keyname, string const comment);


	////////////////////////
	// Return a key comment.
	string   KeyComment( unsigned const keyID, unsigned const commentID) const;
	string   KeyComment( string const keyname, unsigned const commentID) const;

	////////////////////////
	// Delete a key comment.
	bool     DeleteKeyComment( unsigned const keyID, unsigned const commentID);
	bool     DeleteKeyComment( string const keyname, unsigned const commentID);


	/////////////////////////////////
	// Delete all comments for a key.
	bool     DeleteKeyComments( unsigned const keyID);
	bool     DeleteKeyComments( string const keyname);
};

#endif // JFCINIFILE_H