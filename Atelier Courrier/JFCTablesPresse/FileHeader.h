#if !defined(FILEHEADER_H__INCLUDED_)
#define FILEHEADER_H__INCLUDED_

#pragma once

#pragma warning( disable : 4786 )

/**
 * \ingroup JFCSOURCE EXTERNAL
 *
 *
 * \par requirements
 * win98 or later\n
 * win2k or later\n
 * MFC\n
 *
 * \version 1.0
 * first version
 *
 * \date 07-09-2003
 *
 * \author eddie
 *
 * \par license
 * This code is the property of JFC Informatique & Media\n
 * Unauthorized copying or use is strictly forbiden.
 * 
 * \todo 
 *
 * \bug 
 *
 */
class AFX_EXT_CLASS CFileHeader  
{
public:
	CFileHeader();
	virtual ~CFileHeader();
		/// assignment operator
	CFileHeader & operator =(const CFileHeader & rhs);

	JArchive&	Recv( JArchive& lhs);
	JArchive&	Send( JArchive& lhs);

	JUnt32		GetCompagnie()	const		{ return m_lCompagnie;}
	void		SetCompagnie(JUnt32 t)		{ m_lCompagnie = t;}
	JUnt32		GetMagicNumber() const		{ return m_lMagicNumber;}
	void		SetMagicNumber(JUnt32 t)	{ m_lMagicNumber = t;}
	JUnt32		GetCompression() const		{ return m_lCompression;}
	void		SetCompression(JUnt32 t)	{ m_lCompression = t;}
	JUnt32		GetCryptage() const			{ return m_lCryptage;}
	void		SetCryptage(JUnt32 t)		{ m_lCryptage = t;}
	JUnt32		GetVersion() const			{ return m_lVersion;}
	void		SetVersion(JUnt32 t)		{ m_lVersion = t;}

protected:
	/// File Header
	JUnt32				m_lCompagnie;
	JUnt32				m_lMagicNumber;
	JUnt32				m_lCompression;
	JUnt32				m_lCryptage;
	JUnt32				m_lVersion;

private:

	/// copy constructor
	CFileHeader(const CFileHeader & rhs);

};

#endif // !defined(FILEHEADER_H__INCLUDED_)
