#pragma once

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
class LookupID
{
public:
	LookupID(void);
	virtual ~LookupID(void);

	bool operator < (const LookupID &rhs) const;
	bool operator > (const LookupID &rhs) const;

	JInt32	m_ID1;
	JInt32	m_ID2;
};
