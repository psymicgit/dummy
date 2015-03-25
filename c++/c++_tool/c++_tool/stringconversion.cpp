//#include "StdAfx.h"
#include "stringconversion.h"
#ifdef WIN32
#include <Windows.h>
#endif

StringConversion::StringConversion() {

}

StringConversion::~StringConversion() {

}

#ifdef __LINUX__
unsigned int UniCharToUTF8(wchar_t UniChar, char *OutUTFString)
{

    unsigned int UTF8CharLength = 0;

    if (UniChar < 0x80)
    {  
        if ( OutUTFString )
            OutUTFString[UTF8CharLength++] = (char)UniChar;
        else
            UTF8CharLength++;
    }
    else if(UniChar < 0x800)
    {
        if ( OutUTFString )
        {
            OutUTFString[UTF8CharLength++] = 0xc0 | ( UniChar >> 6 );
            OutUTFString[UTF8CharLength++] = 0x80 | ( UniChar & 0x3f );
        }
        else
        {
            UTF8CharLength += 2;
        }
    }
    else if(UniChar < 0x10000 )
    {
        if ( OutUTFString )
        {
            OutUTFString[UTF8CharLength++] = 0xe0 | ( UniChar >> 12 );
            OutUTFString[UTF8CharLength++] = 0x80 | ( (UniChar >> 6) & 0x3f );
            OutUTFString[UTF8CharLength++] = 0x80 | ( UniChar & 0x3f );
        }
        else
        {
            UTF8CharLength += 3;
        }
    }
    else if( UniChar < 0x200000 ) 
    {
        if ( OutUTFString )
        {
            OutUTFString[UTF8CharLength++] = 0xf0 | ( (int)UniChar >> 18 );
            OutUTFString[UTF8CharLength++] = 0x80 | ( (UniChar >> 12) & 0x3f );
            OutUTFString[UTF8CharLength++] = 0x80 | ( (UniChar >> 6) & 0x3f );
            OutUTFString[UTF8CharLength++] = 0x80 | ( UniChar & 0x3f );
        }
        else
        {
            UTF8CharLength += 4;
        }

    }

    return UTF8CharLength;
}

unsigned int  UTF8StrToUnicode( const char* UTF8String, unsigned int UTF8StringLength, wchar_t* OutUnicodeString, unsigned int UnicodeStringBufferSize )
{
    unsigned int UTF8Index = 0;
    unsigned int UniIndex = 0;

    while ( UTF8Index < UTF8StringLength )
    {
        unsigned char UTF8Char = UTF8String[UTF8Index];

        if ( UnicodeStringBufferSize != 0 && UniIndex >= UnicodeStringBufferSize )
            break;

        if ((UTF8Char & 0x80) == 0) 
        {
            const unsigned int cUTF8CharRequire = 1;

            // UTF8×ÖÂë²»×ã
            if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
                break;

            if ( OutUnicodeString )
            {
                wchar_t& WideChar = OutUnicodeString[UniIndex]; 

                WideChar = UTF8Char;
            }

            UTF8Index++;
            
        } 
        else if((UTF8Char & 0xE0) == 0xC0)  ///< 110x-xxxx 10xx-xxxx
        {
            const unsigned int cUTF8CharRequire = 2;

            // UTF8×ÖÂë²»×ã
            if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
                break;

            if ( OutUnicodeString )
            {
                wchar_t& WideChar = OutUnicodeString[UniIndex]; 
                WideChar  = (UTF8String[UTF8Index + 0] & 0x3F) << 6;
                WideChar |= (UTF8String[UTF8Index + 1] & 0x3F);
            }
            
            UTF8Index += cUTF8CharRequire;
        }
        else if((UTF8Char & 0xF0) == 0xE0)  ///< 1110-xxxx 10xx-xxxx 10xx-xxxx
        {
            const unsigned int cUTF8CharRequire = 3;

            // UTF8×ÖÂë²»×ã
            if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
                break;

            if ( OutUnicodeString )
            {
                wchar_t& WideChar = OutUnicodeString[UniIndex]; 

                WideChar  = (UTF8String[UTF8Index + 0] & 0x1F) << 12;
                WideChar |= (UTF8String[UTF8Index + 1] & 0x3F) << 6;
                WideChar |= (UTF8String[UTF8Index + 2] & 0x3F);
            }
            

            UTF8Index += cUTF8CharRequire;
        } 
        else if((UTF8Char & 0xF8) == 0xF0)  ///< 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
        {
            const unsigned int cUTF8CharRequire = 4;

            // UTF8×ÖÂë²»×ã
            if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
                break;

            if ( OutUnicodeString )
            {
                wchar_t& WideChar = OutUnicodeString[UniIndex]; 

                WideChar  = (UTF8String[UTF8Index + 0] & 0x0F) << 18;
                WideChar  = (UTF8String[UTF8Index + 1] & 0x3F) << 12;
                WideChar |= (UTF8String[UTF8Index + 2] & 0x3F) << 6;
                WideChar |= (UTF8String[UTF8Index + 3] & 0x3F);
            }

            UTF8Index += cUTF8CharRequire;
        } 
        else ///< 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx 
        {
            const unsigned int cUTF8CharRequire = 5;

            // UTF8×ÖÂë²»×ã
            if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
                break;

            if ( OutUnicodeString )
            {
                wchar_t& WideChar = OutUnicodeString[UniIndex]; 

                WideChar  = (UTF8String[UTF8Index + 0] & 0x07) << 24;
                WideChar  = (UTF8String[UTF8Index + 1] & 0x3F) << 18;
                WideChar  = (UTF8String[UTF8Index + 2] & 0x3F) << 12;
                WideChar |= (UTF8String[UTF8Index + 3] & 0x3F) << 6;
                WideChar |= (UTF8String[UTF8Index + 4] & 0x3F);
            }

            UTF8Index += cUTF8CharRequire;
        }


        UniIndex++;
    }
	OutUnicodeString[UniIndex] = 0;
    return UniIndex;
}

#endif // __LINUX__

const wchar_t* StringConversion::UTF8ToUnicode(const char *buffer) {
	static wchar_t Converwbuffer[4096];

#if defined WIN32
	MultiByteToWideChar(CP_UTF8 , 0, buffer, -1, Converwbuffer, 4095);
	Converwbuffer[4095] = '\0';
#elif defined __LINUX__
	static wchar_t wcharBuff[4096];	
	if (0 < UTF8StrToUnicode(buffer, strlen(buffer), wcharBuff, 4096)) {
		wcs_cpy_to_ucs2(Converwbuffer, wcharBuff);
	}
#endif // WIN32  
	return Converwbuffer;
}

const char* StringConversion::unicodeToUTF8(const wchar_t *buffer) {
	static char Converbuffer[4096];

#if defined WIN32
	WideCharToMultiByte(CP_UTF8 , 0 , buffer, -1, Converbuffer, 4095 , 0 , 0);
	Converbuffer[4095] = '\0';
#elif defined __LINUX__
	static wchar_t wcharBuff[4096];
	ucs2_cpy_to_wcs(wcharBuff, buffer);
	unsigned int uiLen = 0;
	unsigned int uiOutLen = 0;
	while (wcharBuff[uiLen] != 0 && uiLen < 4096) {
		uiOutLen += UniCharToUTF8(wcharBuff[uiLen], &Converbuffer[uiOutLen]);
		++uiLen;
	}	
	Converbuffer[uiOutLen] = 0;
#endif // WIN32

	return Converbuffer;
}
#ifdef WIN32

const char* StringConversion::UTF8ToANSI(const char *buffer) {
	const wchar_t *pUnicodeBuffer = UTF8ToUnicode(buffer);
	return unicodeToANSI(pUnicodeBuffer);
}

const char* StringConversion::unicodeToANSI(const wchar_t *buffer) {
	static char Converbuffer[4096];

	WideCharToMultiByte(CP_ACP , 0 , buffer, -1, Converbuffer, 4095 , 0 , 0);
	Converbuffer[4095] = '\0';


	return Converbuffer;	
}

const char* StringConversion::ANSIToUTF8(const char *buffer) {
	const wchar_t *pUnicodeBuffer = ANSIToUnicode(buffer);
	return unicodeToUTF8(pUnicodeBuffer);
}

const wchar_t* StringConversion::ANSIToUnicode(const char *buffer) {
	static wchar_t Converwbuffer[4096];

	MultiByteToWideChar(CP_ACP , 0, buffer, -1, Converwbuffer, 4095);
	Converwbuffer[4095] = '\0';


	return Converwbuffer;
}
#endif // WIN32

void StringConversion::stringToLower(char* pStr) {
	while(NULL != pStr && '\0' != *pStr) {
		*pStr = tolower(*pStr);
		++pStr;
	}
}
