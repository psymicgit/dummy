
#ifndef SDMSG_H_2010_04_23
#define SDMSG_H_2010_04_23

#include "sdbase.h"

namespace SGDP{

	enum ENMsgErr
	{
		SDMSG_SUCCESS = 0,
		SDMSG_GET_DEST_INSUFFICIENT = -1,
		SDMSG_GET_NO_CONTENT = -2,

		SDMSG_COPY_SRC_ERROR = -10,
		SDMSG_COPY_SYSTEM_ERROR = -11,
		SDMSG_COPY_PUSH_FAIL = -12,
	};

	class ISDMsg
	{
	public:
		virtual ~ISDMsg() {}

		/*
			Name:		PushValue
			Function:	The key value will be stored in ISDMsg.
			Param:		
				[in]		pszKey:		Key of value(must be string).
				[in]		pData:		Value.
				[in]		nLen:		Length of value.
			Return:
				true:		Push successfully.
				false:		Push failed.
		*/
		virtual BOOL SDAPI PushValue(const char* pszKey, const UINT8* pData, UINT32 nLen) = 0;

		/*
			Name:		GetValue
			Function:	Get value through the specified key.
			Param:		
				[in]		pszKey:		Key of value(must be string).
				[in|out]	pszValue:	Value buffer.
				[in|out]	nLen:		Length of value.
			Return:
				0:			GetValue successfully.
				-1:			pDestBuf is not sufficient.
				-2:			No content found.
		*/
		virtual INT32 SDAPI GetValue(const CHAR* pszKey, UINT8* pValue, UINT32 &nLen) = 0;

		/*
			Name:		PushBuffer
			Function:	Push buffer with specified content.
			Param:		
				[in]		pBuf:		Buffer to be pushed.
				[in]		nLen:		Length of pushed buffer.
			Return:
				true:		Push successfully.
				false:		Push failed.
		*/
		virtual BOOL SDAPI PushBuffer(const UINT8* pBuf, UINT32 nLen) = 0;

		/*
			Name:		GetBuffer
			Function:	After pushed value into this message, 
						the key value buffer can be got through this function.
			Param:		
				[in|out]		pDestBuf:	Buffer for storing got key value.
				[in|out]		nLen:		Length of pDestBuf. 
											It would be set with the real buffer size if get success, 
											or needed buffer size if the pDestBuf is not sufficient.
			Return:
				0:			GetBuffer successfully.
				-1:			pDestBuf is not sufficient.
				-2:			No content found.
		*/
		virtual INT32 SDAPI GetBuffer(UINT8* pDestBuf, UINT32 &nLen) = 0;

		/*
			Name:		CleanData
			Function:	RClean all data.
		*/
		virtual VOID SDAPI CleanData() = 0;

		/*
			Name:		CopyData
			Function:	Copy data from other ISDMsg.
			Param:
				[in]			pSrcMsg:	Where data from.
			Return:
				0:			GetBuffer successfully.
				-10:		Source error.
				-11:		new memory failed.
				-12:		Pushing buffer failed.
		*/
		virtual INT32 SDAPI CopyData(ISDMsg* pSrcMsg) = 0;

		/*
			Name:		Release
			Function:	Release this message.
		*/
		virtual VOID SDAPI Release() = 0;
	};

	ISDMsg* SDAPI NewSDMsg();
}
#endif/*SDMSG_H_2010_04_23*/

