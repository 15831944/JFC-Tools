#include "StdAfx.h"
#include "JFCZipIniFile.h"
#include "ZLib/Include/zlib.h"

using namespace std;

///////////////////////////////////////////////
// le constructeur

JFCZipIniFile::JFCZipIniFile ()
{

}



///////////////////////////////////////////////
// le destructeur virtuel

JFCZipIniFile::~JFCZipIniFile ()
{

}

///////////////////////////////////////////////
// d�compresse et lit le fichier ini compress�
bool JFCZipIniFile::ReadFile ()
{
	gzFile fp = 0;

	// on tente d'ouvrir le fichier compress� par 'zlib'
	fp = ::gzopen(path.c_str(), "rb");
	
	// echec : on quitte
	if (fp == 0) return false;

	string sFile;
	int count = 0;
	const unsigned int buflen = 256;
	char * buf = new char[buflen];

	// on lit tout le fichier et on le traite apr�s comme une string
	while (count = ::gzread(fp, buf, buflen-1))
	{
		buf[count] = '\0';
		sFile.append(buf);
	}

	// on ferme le fichier compress�
	::gzclose(fp); fp = 0;

	string   line;
	string   keyname, valuename, value;
	string::size_type pLeft, pRight;
	size_t next_slash = 0;
	
	//line = sFile.substr(sFile.find('\n'));

	// on traite la chaine ligne par ligne (s�parateur '\n')
	while( (next_slash = sFile.find('\n')) != string::npos)
	{
		line = sFile.substr(0, next_slash);
	
		// on oublie cette ligne si juste un saut de ligne
		if ( (line.length() > 0) && (line[line.length() - 1] == '\r'))
			line = line.substr( 0, line.length() - 1);

		// la ligne est utile
		if ( line.length())
		{
			// on v�rifie que l'on a bien � faire � du texte et non du binaire
			if ( !isprint( line[0]))
			{
				// on lib�re le buffer
				if (buf) delete[] buf;
				buf = 0;
				return false;
			}

			// la ligne contient au moins un 'caract�re-cl�'
			if (( pLeft = line.find_first_of(";#[=")) != string::npos)
			{
				switch ( line[pLeft])
				{
					// cette ligne d�finit une cl�
					case '[':
						if ((pRight = line.find_last_of("]")) != string::npos && pRight > pLeft)
						{
							keyname = line.substr( pLeft + 1, pRight - pLeft - 1);
							AddKeyName( keyname);
						} 
					break;

					// cette ligne d�finit une valeur
					case '=':
						valuename = line.substr( 0, pLeft);
						value = line.substr( pLeft + 1);
						SetValue( keyname, valuename, value);
					break;

					// cette ligne est un commentaire
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
		// on supprime la partie de la chaine que l'on vient de traiter
		sFile.erase(0, next_slash + 1);
	}
	// on lib�re le buffer
	if (buf) delete[] buf;
	buf = 0;

	// succ�s ssi on a bien des donn�es de configuration (on ne peut avoir de fichier de config vide)
	if ( names.size())	return true;
	else				return false;
}

///////////////////////////////////////////
// �crit les donn�es de config dans un fichier ini compress�
bool JFCZipIniFile::WriteFile ()
{
	unsigned commentID, keyID, valueID;

	string sEOL("\n");

    char mode[4] = "wb\0";
	int level = Z_DEFAULT_COMPRESSION;
    if (level != Z_DEFAULT_COMPRESSION) mode[2] = '0'+Z_DEFAULT_COMPRESSION;
	
	// on tente d'ouvrir le fichier compress� par 'zlib'
	gzFile fp = ::gzopen(this->path.c_str(), mode);

	// echec : on quitte
	if (fp == 0) return false;	
	
	// on �crit les commentaires
	for ( commentID = 0; commentID < comments.size(); ++commentID)
	{
		string s = ';' + comments[commentID] + '\n';
		::gzwrite(fp, (voidp) s.c_str(), (unsigned int)s.length());
	}

	// on saute une ligne apr�s les commentaires
	if ( comments.size()) ::gzwrite(fp, (voidp) sEOL.c_str(), (unsigned int)sEOL.length());

	// on �crit les cl�s et les valeurs
	for ( keyID = 0; keyID < keys.size(); ++keyID)
	{
		string s = "[" + names[keyID] + "]\n";
		::gzwrite(fp, (voidp) s.c_str(), (unsigned int)s.length());

		// commentaires
		for ( commentID = 0; commentID < keys[keyID].comments.size(); ++commentID)
		{
			s =  ";" + keys[keyID].comments[commentID] + '\n';
			::gzwrite(fp, (voidp) s.c_str(), (unsigned int)s.length());
		}

		// valeurs
		for ( valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
		{
			s = keys[keyID].names[valueID] + '=' + keys[keyID].values[valueID] + '\n';
			::gzwrite(fp, (voidp) s.c_str(), (unsigned int)s.length());
		}
			
		// on saute une ligne
		::gzwrite(fp, (voidp) sEOL.c_str(), (unsigned int)sEOL.length());
	}

	// on ferme le fichier compress�
	::gzclose(fp); fp = 0;
	return true;
}
