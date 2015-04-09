
#ifndef FEEDBACK_H
#define FEEDBACK_H

#include "sdtype.h"
#include <sdnetutils.h>
#include <sdutil.h>
#include <sdframework.h>

#include "sslcomm.h"
#include "defapns.h"
#include "iapns.h"

#define MAX_FEED_BACK_DATA 2048



class CFeedBack : public IAPNS
{
public:
	CFeedBack();
	~CFeedBack();

private:
	virtual void Read();
	virtual void Write();

	void HandleData(const char * szData, const int nLen);

private:
	int	m_nProtelLen;
	char	m_szProtelData[MAX_FEED_BACK_DATA * 2];
};


#endif
