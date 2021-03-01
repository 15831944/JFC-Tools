
#pragma once

#pragma warning( disable : 4786 )


/**
 * \ingroup JFCSOURCE
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
class CRepereAbsolu  
{
public:

	CRepereAbsolu(JInt32 ItemNatif);
	virtual ~CRepereAbsolu();

	JArchive&	Recv(JArchive& lhs, JInt32 Version);

	JInt32	GetRepereID() const
				{ return m_lRepereID; }

private:

	/// Repere ID
	JInt32				m_lRepereID;
	/// Item natif index
	JInt32				m_lItemNatif;

private:

	/// assignment operator
	CRepereAbsolu & operator =(const CRepereAbsolu & rhs);
	/// copy constructor
	CRepereAbsolu(const CRepereAbsolu & rhs);
};

